#include "pch.h"

#include "batch/campaign.h"
#include "statistics.h"

using namespace batch::statistics;
using namespace utility;

void batch::campaign::aggregate(std::filesystem::path const& campaign_path)
{
    Aggregator{ campaign_path }.run();
}

void Aggregator::run() const
{
    for (auto const& setup_path : file::directories(campaign_path_ / "setups"))
        aggregate_setup(setup_path);
}

void Aggregator::aggregate_setup(std::filesystem::path const& setup_path) const
{
    auto const sample_paths{ file::directories(setup_path / "samples") };

    if (sample_paths.empty())
        return;

    copy_files(sample_paths.front(), setup_path, file::Type::txt);

    std::map<std::filesystem::path, Dataset> datasets;

    for (auto const& sample_path : sample_paths)
        for (auto const& path : file::recursive_files(sample_path, file::Type::csv))
            add_sample(datasets[path.lexically_relative(sample_path)], path);

    for (auto const& [relative, dataset] : datasets)
        write_dataset(dataset, setup_path / relative);
}

void Aggregator::add_sample(Dataset& dataset, std::filesystem::path const& path)
{
    csv::Reader reader{ path };
    auto const table{ reader.read() };

    if (table.empty())
        reader.fail("CSV table is empty");

    if (not table.rows_match_header())
        reader.fail("CSV row sizes do not match the header");

    if (dataset.header.empty())
    {
        dataset.header = table.front();

        auto const first_value{ std::ranges::find_if_not(dataset.header, [](auto const& field) {
            return field.starts_with("key:");
        }) };

        dataset.key_count = static_cast<std::size_t>(first_value - dataset.header.begin());

        if (dataset.key_count == dataset.header.size())
            reader.fail("Dataset contains no value columns");
    }
    else if (table.front() != dataset.header)
    {
        reader.fail("Dataset has an inconsistent header");
    }

    std::set<csv::Table::Row> encountered;

    for (std::size_t row_index{ 1 }; row_index < table.size(); ++row_index)
    {
        auto const& fields{ table[row_index] };
        csv::Table::Row key{ fields.begin(), fields.begin() + dataset.key_count };

        if (not encountered.emplace(key).second)
            reader.fail(std::format("Duplicate key in row {}", row_index + 1));

        auto& row{ dataset.rows[find_or_add(dataset, std::move(key))] };
        ++row.samples;

        for (std::size_t column{ dataset.key_count }; column < fields.size(); ++column)
        {
            auto const value{ csv::parse<double>(fields[column]) };

            if (not value)
                reader.fail(
                    std::format(
                        "Invalid numeric value '{}' in row {}, column {}",
                        fields[column],
                        row_index + 1,
                        column + 1));

            row.values[column - dataset.key_count].add(*value);
        }
    }
}

void Aggregator::write_dataset(Dataset const& dataset, std::filesystem::path const& path)
{
    csv::Table::Row output_header;
    output_header.reserve(dataset.key_count + 1 + 3 * (dataset.header.size() - dataset.key_count));
    output_header.insert(
        output_header.end(),
        dataset.header.begin(),
        dataset.header.begin() + dataset.key_count);
    output_header.emplace_back("samples");

    for (std::size_t column{ dataset.key_count }; column < dataset.header.size(); ++column)
    {
        output_header.push_back(header_name(dataset.header[column], "mean"));
        output_header.push_back(header_name(dataset.header[column], "median"));
        output_header.push_back(header_name(dataset.header[column], "std_dev"));
    }

    auto const field_count{ output_header.size() };
    csv::Table::Data data;
    data.reserve(dataset.rows.size() + 1);
    data.push_back(std::move(output_header));

    for (auto const& row : dataset.rows)
    {
        csv::Table::Row fields{ row.key };
        fields.reserve(field_count);
        fields.push_back(std::format("{}", row.samples));

        for (auto const& samples : row.values)
        {
            fields.push_back(std::format("{:.{}g}", samples.mean(), resolution));
            fields.push_back(std::format("{:.{}g}", samples.median(), resolution));
            fields.push_back(std::format("{:.{}g}", samples.standard_deviation(), resolution));
        }

        data.push_back(std::move(fields));
    }

    csv::Table{ std::move(data) }.write(path);
}

void Aggregator::copy_files(
    std::filesystem::path const& source_root,
    std::filesystem::path const& destination_root,
    file::Type type)
{
    for (auto const& source : file::recursive_files(source_root, type))
    {
        auto const destination{ destination_root / source.lexically_relative(source_root) };
        std::filesystem::create_directories(destination.parent_path());
        std::filesystem::copy_file(
            source,
            destination,
            std::filesystem::copy_options::overwrite_existing);
    }
}

std::size_t Aggregator::find_or_add(Dataset& dataset, csv::Table::Row key)
{
    if (auto const position{ dataset.row_indices.find(key) }; position != dataset.row_indices.end())
        return position->second;

    auto const index{ dataset.rows.size() };
    dataset.rows.push_back(
        {
            .key = std::move(key),
            .values = std::vector<Samples>(dataset.header.size() - dataset.key_count),
        });
    dataset.row_indices.emplace(dataset.rows.back().key, index);
    return index;
}

std::string Aggregator::header_name(std::string_view source_name, std::string_view suffix)
{
    auto const separator{ source_name.find('/') };
    auto result{ std::format("{}_{}", source_name.substr(0, separator), suffix) };

    if (separator != std::string_view::npos)
        result += source_name.substr(separator);

    return result;
}
