#pragma once
#include "batch/types.h"
#include "utility/csv.h"
#include "utility/file.h"

namespace batch::statistics
{
    namespace csv = utility::csv;
    namespace file = utility::file;
}

class batch::statistics::Samples
{
public:
    void add(double value) { values_.push_back(value); }

    double mean() const
    {
        return std::ranges::fold_left(values_, 0.0, std::plus{}) /
               static_cast<double>(values_.size());
    }

    double median() const
    {
        auto values{ values_ };
        auto const middle{ values.begin() + values.size() / 2 };

        std::nth_element(values.begin(), middle, values.end());

        if (values.size() % 2)
            return *middle;

        return (*std::max_element(values.begin(), middle) + *middle) / 2.0;
    }

    double standard_deviation() const
    {
        if (values_.size() < 2)
            return std::numeric_limits<double>::quiet_NaN();

        auto const sum_squared_deviation{ std::ranges::fold_left(
            values_ | std::views::transform([mean = mean()](double value) {
                auto const deviation{ value - mean };
                return deviation * deviation;
            }),
            0.0,
            std::plus{}) };

        return std::sqrt(sum_squared_deviation / static_cast<double>(values_.size() - 1));
    }

private:
    std::vector<double> values_;
};

class batch::statistics::Aggregator
{
public:
    explicit Aggregator(std::filesystem::path campaign_path)
        : campaign_path_{ std::move(campaign_path) }
    { }

    void run() const;

private:
    struct Row
    {
        csv::Table::Row key;
        std::size_t samples{};
        std::vector<Samples> values;
    };

    struct Dataset
    {
        csv::Table::Row header;
        std::size_t key_count{};
        std::vector<Row> rows;
        std::map<csv::Table::Row, std::size_t> row_indices;
    };

    void aggregate_setup(std::filesystem::path const&) const;

    static void add_sample(Dataset&, std::filesystem::path const&);
    static void write_dataset(Dataset const&, std::filesystem::path const&);
    static void copy_files(std::filesystem::path const&, std::filesystem::path const&, file::Type);

    static std::size_t find_or_add(Dataset&, csv::Table::Row);

    static std::string header_name(std::string_view source_name, std::string_view suffix);

    static constexpr auto resolution{ std::numeric_limits<double>::max_digits10 };

    std::filesystem::path campaign_path_;
};

class batch::statistics::Merger
{
public:
    explicit Merger(std::filesystem::path campaign_path, std::string_view file_name)
        : campaign_path_{ std::move(campaign_path) },
          file_name_{ file_name }
    { }

    void run();

    std::vector<std::string> setup_descriptions() const;

    static bool valid_file_name(std::string_view);

private:
    std::optional<std::string> initialize(csv::Table const&);
    void add(std::filesystem::path const&, std::size_t, std::string_view);
    void write();

    std::filesystem::path campaign_path_;
    std::string file_name_;

    csv::Table::Row source_header_, output_header_;
    std::map<std::string, csv::Table::Data> outputs_;
};
