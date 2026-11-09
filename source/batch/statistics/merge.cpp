#include "pch.h"

#include "batch/campaign.h"
#include "statistics.h"

using namespace batch;
using namespace batch::statistics;
using namespace utility;

void campaign::merge(std::filesystem::path const& campaign_path, std::string_view dataset)
{
    Merger{ campaign_path, dataset }.run();
}

void campaign::summarize(std::filesystem::path const& campaign_path, std::string_view dataset)
{
    auto const descriptions{ Merger{ campaign_path, dataset }.setup_descriptions() };

    std::map<std::string, std::map<std::size_t, std::size_t>> counts;

    for (auto const& path : file::recursive_files(campaign_path / dataset, file::Type::csv))
    {
        auto const condition{ path.stem().string() };

        csv::Reader reader{ path };
        auto const table{ reader.read() };

        auto const& header{ table.front() };

        auto const setup_column{ static_cast<std::size_t>(
            std::ranges::find(header, "key:setup_index") - header.begin()) };

        auto const samples_column{ static_cast<std::size_t>(
            std::ranges::find(header, "samples") - header.begin()) };

        for (auto const& row : table | std::views::drop(1))
        {
            auto const setup{ csv::parse<std::size_t>(row[setup_column]).value() };
            auto const samples{ csv::parse<std::size_t>(row[samples_column]).value() };

            counts[condition][setup] = samples;
        }
    }

    csv::Table::Data output;

    csv::Table::Row header{
        "key:setup_index",
        "description",
    };

    for (auto const& condition : counts | std::views::keys)
        header.push_back(condition);

    output.push_back(std::move(header));

    for (std::size_t setup{}; setup < descriptions.size(); ++setup)
    {
        csv::Table::Row row{
            std::format("{}", setup),
            descriptions[setup],
        };

        for (auto const& condition : counts | std::views::keys)
            row.push_back(std::format("{}", counts[condition][setup]));

        output.push_back(std::move(row));
    }

    csv::Table{ std::move(output) }.write(campaign_path / dataset);
}

void Merger::run()
{
    auto const descriptions{ setup_descriptions() };
    std::vector<std::filesystem::path> paths(descriptions.size());

    for (auto const& path : file::directories(campaign_path_ / "setups"))
    {
        auto const index{ csv::parse<std::size_t>(path.filename().string()) };

        if (not index)
            throw std::runtime_error{ std::format("Invalid setup path '{}'.", path.string()) };

        if (*index >= paths.size())
            throw std::runtime_error{ std::format("Setup {} has no entry in setups.csv.", *index) };

        if (not paths[*index].empty())
            throw std::runtime_error{
                std::format("Duplicate setup index {} in setup directories.", *index)
            };

        paths[*index] = path;
    }

    for (std::size_t index{}; index < paths.size(); ++index)
    {
        if (paths[index].empty())
            throw std::runtime_error{ std::format("Setup {} has no directory.", index) };

        add(paths[index], index, descriptions[index]);
    }

    write();
}

bool Merger::valid_file_name(std::string_view name)
{
    return not name.empty() and name != "." and name != ".." and
           name.find_first_of("<>:\"/\\|?*") == std::string_view::npos;
}

std::vector<std::string> Merger::setup_descriptions() const
{
    csv::Reader reader{ campaign_path_ / "setups" };
    auto const table{ reader.read() };

    if (table.empty())
        reader.fail("CSV table is empty");

    if (not table.rows_match_header())
        reader.fail("CSV row sizes do not match the header");

    if (table.front() != csv::Table::Row{ "setup", "description" })
        reader.fail("Expected header 'setup,description'");

    std::vector<std::string> descriptions;
    descriptions.reserve(table.size() - 1);

    for (std::size_t row_index{ 1 }; row_index < table.size(); ++row_index)
    {
        auto const& row{ table[row_index] };
        auto const index{ csv::parse<std::size_t>(row[0]) };
        auto const expected{ descriptions.size() };

        if (not index or *index != expected)
            reader.fail(
                std::format(
                    "Expected setup index {}, found '{}' in row {}",
                    expected,
                    row[0],
                    row_index + 1));

        descriptions.push_back(row[1]);
    }

    return descriptions;
}

std::optional<std::string> Merger::initialize(csv::Table const& table)
{
    if (table.empty())
        return "CSV table is empty";

    if (not table.rows_match_header())
        return "CSV row sizes do not match the header";

    auto const& header{ table.front() };

    if (header.empty() or not header.front().starts_with("key:"))
        return "Dataset must begin with a key column";

    if (std::ranges::any_of(header | std::views::drop(1), [](auto const& field) {
            return field.starts_with("key:");
        }))
        return "Dataset must contain exactly one key column";

    if (source_header_.empty())
    {
        source_header_ = header;
        output_header_.reserve(header.size() + 1);
        output_header_.emplace_back("key:setup_index");
        output_header_.emplace_back("description");
        output_header_.insert(output_header_.end(), header.begin() + 1, header.end());
    }
    else if (header != source_header_)
    {
        return "Dataset has an inconsistent header";
    }

    return std::nullopt;
}

void Merger::add(
    std::filesystem::path const& setup_path,
    std::size_t setup_index,
    std::string_view description)
{
    csv::Reader reader{ setup_path / file_name_ };
    auto const table{ reader.read() };

    if (auto error{ initialize(table) })
        reader.fail(*error);

    std::set<std::string> encountered;

    for (std::size_t row_index{ 1 }; row_index < table.size(); ++row_index)
    {
        auto const& row{ table[row_index] };
        auto const& key{ row.front() };

        if (not valid_file_name(key))
            reader.fail(std::format("Key '{}' cannot be used as a file name", key));

        if (not encountered.emplace(key).second)
            reader.fail(std::format("Duplicate key '{}' in row {}", key, row_index + 1));

        csv::Table::Row output_row;
        output_row.reserve(row.size() + 1);
        output_row.push_back(std::format("{}", setup_index));
        output_row.emplace_back(description);
        output_row.insert(output_row.end(), row.begin() + 1, row.end());
        outputs_[key].push_back(std::move(output_row));
    }
}

void Merger::write()
{
    auto const output_path{ campaign_path_ / file_name_ };
    std::filesystem::remove_all(output_path);
    std::filesystem::create_directories(output_path);

    for (auto& [key, rows] : outputs_)
    {
        csv::Table::Data data;
        data.reserve(rows.size() + 1);
        data.push_back(output_header_);
        std::ranges::move(rows, std::back_inserter(data));
        csv::Table{ std::move(data) }.write(output_path / key);
    }
}
