#include "pch.h"

#include "csv.h"

#include "file.h"

using namespace utility;


std::filesystem::path csv::Writer::path(std::string_view suffix) const
{
    auto result{ base_path_ };
    if (not suffix.empty())
        result += std::format(".{}", suffix);
    result += std::format(".{}", enum_name(file::Type::csv));
    return result;
}

void csv::Writer::initialize(
    std::filesystem::path base_path, Formatter header, Formatter row)
{
    base_path_ = std::move(base_path);
    header_ = std::move(header);
    row_ = std::move(row);
    rows_ = {};
    previous_columns_.reset();

    // A new output session replaces any completed CSV at this path.
    std::filesystem::remove(path());
    file::write<file::Type::csv>(path("data"), "");
    file::write<file::Type::csv>(path("schema"), "");
}

void csv::Writer::append(std::size_t data_columns)
{
    if (not header_ or not row_)
        throw std::logic_error{ "CSV writer is not initialized." };

    if (previous_columns_ and data_columns < *previous_columns_)
        throw std::logic_error{ "CSV data-column count decreased." };

    if (not previous_columns_ or data_columns != *previous_columns_)
    {
        file::append<file::Type::csv>(
            path("schema"),
            std::format("{},{},{}\n",
                rows_, data_columns, Table::encode_field(header_())));
        previous_columns_ = data_columns;
    }

    file::append<file::Type::csv>(path("data"), row_() + '\n');
    ++rows_;
}

void csv::Writer::finalize()
{
    if (not header_ or not row_)
        throw std::logic_error{ "CSV writer is not initialized." };

    struct Change
    {
        std::size_t first_row;
        std::size_t columns;
    };

    std::vector<Change> changes;
    Table schema;
    schema.read(path("schema"));
    for (auto const& fields : schema.data())
        changes.push_back({ std::stoull(fields[0]), std::stoull(fields[1]) });

    // Labels can become available without changing the number of kinetic classes.
    auto const final_header{ header_() };

    auto const final_columns{ changes.empty() ? std::size_t{} : changes.back().columns };
    auto completed_path{ path() };
    auto temporary_path{ completed_path };
    temporary_path += ".tmp";

    {
        auto input{ file::wait_for_read(path("data")) };
        auto output{ file::wait_for_open<std::ofstream>(
            temporary_path, std::ios::out | std::ios::binary | std::ios::trunc) };

        output << final_header << '\n';

        std::size_t current_columns{};
        std::size_t next_change{};
        std::size_t row_index{};

        for (std::string line; std::getline(input, line); ++row_index)
        {
            while (next_change < changes.size() and changes[next_change].first_row <= row_index)
            {
                current_columns = changes[next_change].columns;
                ++next_change;
            }

            if (line.ends_with('\r'))
                line.pop_back();
            output << line;

            for (auto column{ current_columns }; column < final_columns; ++column)
            {
                output.put(',');
                if (padding_ == Padding::zero)
                    output.put('0');
            }
            output.put('\n');
        }

        if (input.bad() or not output)
            throw std::runtime_error{ "Failed to combine CSV data and schema." };

        output.close();
        if (not output)
            throw std::runtime_error{ "Failed to close combined CSV." };
    }

    std::filesystem::rename(temporary_path, completed_path);
    std::filesystem::remove(path("data"));
    std::filesystem::remove(path("schema"));

    header_ = {};
    row_ = {};
    previous_columns_.reset();
    rows_ = {};
}

csv::Table& csv::Table::read(std::filesystem::path path)
{
    *this = Reader{ std::move(path) }.read();
    return *this;
}

csv::Table& csv::Table::write(std::filesystem::path path)
{
    file::write<file::Type::csv>(path, text());
    return *this;
}

std::string csv::Table::encode_field(std::string_view field)
{
    if (field.find_first_of(",\"\r\n") == std::string_view::npos)
        return std::string{ field };

    std::string encoded{ "\"" };

    for (auto const character : field)
    {
        if (character == '"')
            encoded += '"';

        encoded += character;
    }

    encoded += '"';
    return encoded;
}

std::string csv::Table::text() const
{
    std::string text;

    for (auto const& row : data_)
    {
        for (std::size_t column{}; column < row.size(); ++column)
        {
            if (column)
                text += ',';

            text += encode_field(row[column]);
        }

        text += '\n';
    }

    return text;
}

bool csv::Table::rows_match_header() const
{
    if (data_.empty())
        return true;

    auto const size{ data_.front().size() };
    return std::ranges::all_of(data_ | std::views::drop(1), [size](auto const& row) {
        return row.size() == size;
    });
}

csv::Reader::Reader(std::filesystem::path path) : path_{ std::move(path) }
{
    path_.replace_extension(".csv");
    file_ = file::wait_for_read(path_);
}

csv::Table csv::Reader::read()
{
    Table::Data data;

    while (auto row{ read_record() })
        data.push_back(std::move(*row));

    return Table{ std::move(data) };
}

std::optional<csv::Table::Row> csv::Reader::read_record()
{
    Table::Row row;
    std::string field, line;
    state_ = State::awaiting_field;

    while (std::getline(file_, line))
    {
        if (line.ends_with('\r'))
            line.pop_back();

        field.reserve(field.size() + line.size());

        for (auto const character : line)
        {
            parse_character(character, field);

            if (state_ == State::field_complete)
            {
                row.push_back(std::move(field));
                field.clear();
                state_ = State::awaiting_field;
            }
        }

        if (state_ == State::reading_quoted)
        {
            field += '\n';
            continue;
        }

        row.push_back(std::move(field));
        return row;
    }

    if (state_ == State::reading_quoted)
        fail("Unterminated quoted field");

    return std::nullopt;
}

void csv::Reader::parse_character(char character, std::string& field)
{
    switch (state_)
    {
        case State::awaiting_field: parse_awaiting_field(character, field); break;

        case State::reading_unquoted: parse_unquoted(character, field); break;

        case State::reading_quoted: parse_quoted(character, field); break;

        case State::after_quote: parse_after_quote(character, field); break;

        case State::field_complete: std::unreachable();
    }
}

void csv::Reader::parse_awaiting_field(char character, std::string& field)
{
    if (character == ',')
        state_ = State::field_complete;
    else if (character == '"')
        state_ = State::reading_quoted;
    else
    {
        field += character;
        state_ = State::reading_unquoted;
    }
}

void csv::Reader::parse_unquoted(char character, std::string& field)
{
    if (character == ',')
        state_ = State::field_complete;
    else if (character == '"')
        fail("Invalid quote in unquoted field");
    else
        field += character;
}

void csv::Reader::parse_quoted(char character, std::string& field)
{
    if (character == '"')
        state_ = State::after_quote;
    else
        field += character;
}

void csv::Reader::parse_after_quote(char character, std::string& field)
{
    if (character == '"')
    {
        field += '"';
        state_ = State::reading_quoted;
    }
    else if (character == ',')
        state_ = State::field_complete;
    else
        fail(std::format("Invalid character '{}' after quoted field", character));
}

[[noreturn]] void csv::Reader::fail(std::string message) const
{
    throw std::runtime_error{ std::format("{} in '{}'.", message, path_.string()) };
}
