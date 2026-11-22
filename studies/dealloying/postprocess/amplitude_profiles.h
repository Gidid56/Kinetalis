#pragma once

#include "utility/csv.h"
#include "utility/file.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <format>
#include <iterator>
#include <limits>
#include <ranges>
#include <map>
#include <numeric>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// Campaign-level postprocessor for the KMC dealloying amplitude profiles.
// Reads the individual phases.csv files; never averages at equal KMC steps.
// Geometric convention: fcc lattice with four sites per conventional cell,
// R^V=(3*pore_volume/(4*pi))^(1/3), R^A=3*l_pore/4.
namespace studies::dealloying::postprocess
{
    namespace amplitude_detail
    {
        using utility::csv::Table;

        inline std::size_t column(Table::Row const& header, std::string_view name)
        {
            auto const it = std::ranges::find(header, name);
            if (it == header.end())
                throw std::runtime_error(std::format("Missing CSV column '{}'", name));
            return static_cast<std::size_t>(it - header.begin());
        }

        inline double number(Table::Row const& row, std::size_t i)
        {
            if (i >= row.size())
                throw std::runtime_error("Missing numeric CSV field");
            auto const value = utility::csv::parse<double>(row[i]);
            if (not value or not std::isfinite(*value))
                throw std::runtime_error(std::format("Invalid numeric CSV field '{}'", row[i]));
            return *value;
        }

        inline double median(std::vector<double> values)
        {
            if (values.empty())
                throw std::runtime_error("Median of empty data");
            std::ranges::sort(values);
            auto const m = values.size() / 2;
            return values.size() % 2 ? values[m] : (values[m - 1] + values[m]) / 2;
        }

        inline double mean(std::vector<double> const& values)
        {
            return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
        }

        inline double stddev(std::vector<double> const& values)
        {
            if (values.size() < 2)
                return 0.0;
            auto const avg = mean(values);
            double ss{};
            for (auto v : values)
                ss += (v - avg) * (v - avg);
            return std::sqrt(ss / (values.size() - 1));
        }

        inline double get_regex_double(std::string const& text, std::regex const& expression,
                                       std::string_view what)
        {
            std::smatch match;
            if (not std::regex_search(text, match, expression))
                throw std::runtime_error(std::format("Cannot read {} from lattice.txt", what));
            return std::stod(match[1].str());
        }

        inline std::string load_text(std::filesystem::path const& path)
        {
            std::ifstream input(path);
            if (not input)
                throw std::runtime_error(std::format("Cannot open '{}'", path.string()));
            return { std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>() };
        }

        struct LatticeMetadata
        {
            double volume_nm3{};
            double dnn_nm{};
        };

        inline LatticeMetadata lattice_metadata(std::filesystem::path const& sample_path)
        {
            auto const text = load_text(sample_path / "lattice.txt");
            auto const sites = get_regex_double(text, std::regex(R"((\d+)\s+sites\b)"), "site count");
            auto const dnn = get_regex_double(
                text, std::regex(R"(Next neighbor distance\s+([\d.eE+-]+)\s+nm)"),
                "nearest-neighbor distance");
            if (sites <= 0 or dnn <= 0)
                throw std::runtime_error("Invalid lattice geometry");
            // FCC: a = sqrt(2) * d_nn, V_site = a^3 / 4 = d_nn^3/sqrt(2).
            return { sites * dnn * dnn * dnn / std::sqrt(2.0), dnn };
        }

        inline std::string outcome(std::filesystem::path const& sample)
        {
            auto const path = sample / "regimes.csv";
            if (not std::filesystem::exists(path))
                return "unknown";
            auto const data = utility::csv::Reader(path).read();
            if (data.size() < 2)
                return "unknown";
            std::string conditions;
            for (std::size_t i = 1; i < data.size(); ++i)
            {
                auto const& label = data[i].front();
                if (label == "none" or label == "final")
                    continue;
                if (not conditions.empty())
                    conditions += ";";
                conditions += label;
            }
            return conditions.empty() ? "none" : conditions;
        }

        struct Point
        {
            double radius_nm{};
            double reduced_radius{};
            double excess{};
            double step{};
        };

        inline std::vector<Point> read_profile(std::filesystem::path const& sample,
                                               double& r_pas_nm)
        {
            auto const lattice = lattice_metadata(sample);
            auto const csv = utility::csv::Reader(sample / "phases.csv").read();
            if (csv.size() < 2 or not csv.rows_match_header())
                throw std::runtime_error(std::format("Invalid phases.csv in '{}'", sample.string()));

            auto const& header = csv.front();
            auto const step_idx = column(header, "key:step");
            auto const pore_idx = column(header, "pore_fraction");
            auto const length_idx = column(header, "pore_correlation_length/nm");

            // Read composition from per-run initial lattice metadata.
            auto const lattice_text = load_text(sample / "lattice.txt");
            auto const au_percent = get_regex_double(
                lattice_text, std::regex(R"(\bAu\s+([\d.]+)\s+%)"), "Au composition");
            auto const x0 = au_percent / 100.0;
            if (x0 <= 0 or x0 >= 1)
                throw std::runtime_error("Invalid Au composition in lattice.txt");
            // Inventory-based infinite-radius estimate. d ~= 0.91 d_nn.
            r_pas_nm = 3.0 * (0.91 * lattice.dnn_nm) * (1 - x0) / x0;

            constexpr double pi = 3.14159265358979323846;
            std::vector<Point> points;
            points.reserve(csv.size() - 1);
            for (std::size_t i = 1; i < csv.size(); ++i)
            {
                auto const& row = csv[i];
                auto const fraction = number(row, pore_idx);
                auto const pore_l = number(row, length_idx);
                if (fraction <= 0 or pore_l <= 0)
                    continue;
                auto const rv = std::cbrt(3.0 * lattice.volume_nm3 * fraction / (4.0 * pi));
                auto const ra = 0.75 * pore_l;
                points.push_back({ rv, rv / r_pas_nm, rv / ra - 1.0,
                                   number(row, step_idx) });
            }
            if (points.empty())
                throw std::runtime_error(std::format("No valid profile points in '{}'", sample.string()));
            return points;
        }
    }

    // Execute after batch aggregation. Fits and baseline corrections are deliberately
    // left to downstream analysis: here all A values remain raw, including negatives.
    inline void generate_amplitude_profiles(std::filesystem::path const& campaign_path,
                                            double bin_width = 0.01)
    {
        using namespace amplitude_detail;
        if (bin_width <= 0 or not std::isfinite(bin_width))
            throw std::invalid_argument("Reduced-radius bin width must be positive");

        auto const output_path = campaign_path / "amplitude_profiles";
        std::filesystem::create_directories(output_path);

        Table::Data sample_profiles{
            { "key:setup", "sample", "r_over_Rpas_bin", "r_pas/nm",
              "rV_median/nm", "A_mean", "A_median", "snapshot_count" }
        };
        Table::Data ensemble_profiles{
            { "key:setup", "r_over_Rpas_bin", "samples", "A_mean", "A_median", "A_std_dev" }
        };
        Table::Data peaks{
            { "key:setup", "sample", "outcome", "r_pas/nm", "rV_peak/nm",
              "r_over_Rpas_peak", "A_max_raw", "step_peak", "peak_at_last_snapshot" }
        };

        // Each setup/bin gets ONE value per independent replicate.
        std::map<std::pair<std::string, long long>, std::vector<double>> ensemble_bins;
        for (auto const& setup_path : utility::file::directories(campaign_path / "setups"))
        {
            auto const setup = setup_path.filename().string();
            auto replicates = utility::file::directories(setup_path / "samples");
            if (replicates.empty())
                replicates.push_back(setup_path);  // single-run campaign

            for (auto const& sample_path : replicates)
            {
                if (not std::filesystem::exists(sample_path / "phases.csv"))
                    continue;
                auto const sample = sample_path == setup_path
                                  ? std::string{ "0" } : sample_path.filename().string();
                double r_pas_nm{};
                auto const points = read_profile(sample_path, r_pas_nm);

                auto const peak = std::ranges::max_element(points, {}, &Point::excess);
                peaks.push_back({ setup, sample, outcome(sample_path),
                    std::format("{:.12g}", r_pas_nm),
                    std::format("{:.12g}", peak->radius_nm),
                    std::format("{:.12g}", peak->reduced_radius),
                    std::format("{:.12g}", peak->excess),
                    std::format("{:.0f}", peak->step),
                    peak == std::prev(points.end()) ? "1" : "0" });

                std::map<long long, std::vector<double>> amplitude_bins;
                std::map<long long, std::vector<double>> radius_bins;
                for (auto const& point : points)
                {
                    auto const bin = static_cast<long long>(std::floor(point.reduced_radius / bin_width));
                    amplitude_bins[bin].push_back(point.excess);
                    radius_bins[bin].push_back(point.radius_nm);
                }
                for (auto const& [bin, values] : amplitude_bins)
                {
                    auto const representative = median(values);
                    ensemble_bins[{ setup, bin }].push_back(representative);
                    sample_profiles.push_back({
                        setup, sample,
                        std::format("{:.12g}", (bin + 0.5) * bin_width),
                        std::format("{:.12g}", r_pas_nm),
                        std::format("{:.12g}", median(radius_bins.at(bin))),
                        std::format("{:.12g}", mean(values)),
                        std::format("{:.12g}", representative),
                        std::format("{}", values.size()) });
                }
            }
        }

        for (auto const& [key, values] : ensemble_bins)
        {
            auto const& [setup, bin] = key;
            ensemble_profiles.push_back({
                setup, std::format("{:.12g}", (bin + 0.5) * bin_width),
                std::format("{}", values.size()),
                std::format("{:.12g}", mean(values)),
                std::format("{:.12g}", median(values)),
                std::format("{:.12g}", stddev(values)) });
        }

        Table{ std::move(sample_profiles) }.write(output_path / "samples");
        Table{ std::move(ensemble_profiles) }.write(output_path / "ensemble");
        Table{ std::move(peaks) }.write(output_path / "peaks");
    }
}
