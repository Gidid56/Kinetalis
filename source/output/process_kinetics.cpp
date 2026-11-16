#include "pch.h"

#include "process_kinetics.h"

#include "kmc/simulation.h"
#include "utility/file.h"

using namespace utility;
using enum file::Type;

using namespace output;

namespace
{
    using SiteKey = output::process::Kinetics::Data::SiteKey;

    SiteKey site_key(kmc::ClassInfo const& info)
    {
        return { info.element(), info.coordination() };
    }

    std::map<SiteKey, std::size_t> site_counts(kmc::Simulation const& simulation)
    {
        std::map<SiteKey, std::size_t> counts;
        std::set<SiteKey> direct_counts;

        auto const& process_counts{ simulation.process_counts() };
        auto const& process_rates{ simulation.process_rates() };

        for (std::size_t key{ 1 }; key < process_counts.size(); ++key)
        {
            auto const process_count{ process_counts[key] };

            if (process_count == 0)
                continue;

            auto const& process{ process_rates[key].first };

            if (process.event() == kmc::event::Type::none or process.element() == lattice::Element::none)
            {
                continue;
            }

            auto const site{ site_key(process) };

            if (process.event() != kmc::event::Type::diffusion)
            {
                if constexpr (utility::is_debug)
                {
                    if (direct_counts.contains(site) and counts[site] != process_count)
                        throw std::logic_error{
                            "Multiple direct process counts disagree for one site class."
                        };
                }

                counts[site] = process_count;
                direct_counts.insert(site);
                continue;
            }

            if (direct_counts.contains(site))
                continue;

            auto const paths_per_site{ static_cast<std::size_t>(lattice::max_coordination) -
                                       static_cast<std::size_t>(process.coordination()) };

            if (paths_per_site == 0)
                continue;

            if constexpr (utility::is_debug)
            {
                if (process_count % paths_per_site != 0)
                    throw std::logic_error{
                        "Diffusion process count is inconsistent with site multiplicity."
                    };
            }

            counts[site] = process_count / paths_per_site;
        }

        return counts;
    }

    // The process key, rather than the set of already observed processes,
    // determines the column position for the entire simulation.
    std::string process_header(kmc::Simulation const& simulation)
    {
        std::string header{ output::Writer::default_header };
        auto const& rates{ simulation.process_rates() };

        for (std::size_t key{ 1 }; key < rates.size(); ++key)
        {
            auto const& process{ rates[key].first };
            if (process.is_active())
                std::format_to(std::back_inserter(header), ",{}", process.label());
            else
                std::format_to(std::back_inserter(header), ",not_encountered_{}", key);
        }
        return header;
    }

    // Changing a header's length cannot be done safely in-place. On discovery
    // of new classes, stream the old rows through a replacement file, padding
    // them when dynamic keys have added new columns.
    void rewrite_header(std::filesystem::path path, std::string_view header,
        std::size_t extra_columns)
    {
        path.replace_extension(enum_name(csv));
        auto temporary{ path };
        temporary += ".tmp";

        {
            auto input{ file::wait_for_read(path) };
            std::string old_header;
            if (not std::getline(input, old_header))
                throw std::runtime_error{ "Cannot replace an empty CSV header." };

            auto output{ file::wait_for_open<std::ofstream>(
                temporary, std::ios_base::out | std::ios_base::trunc) };

            output << header << '\n';
            for (std::string line; std::getline(input, line);)
            {
                output << line;
                for (std::size_t i{}; i < extra_columns; ++i)
                    output.put(',');
                output.put('\n');
            }

            if (input.bad() or not output)
                throw std::runtime_error{ "Failed to rewrite process kinetics CSV header." };
        }

        std::filesystem::rename(temporary, path);
    }
}

void process::Kinetics::capture_init(kmc::Simulation const& simulation)
{
    simulation_ = &simulation;
    last_time_ = simulation.time();
    last_data_ = {};
    accumulation_ = {};
    process_header_.clear();
    process_columns_ = {};
}

void process::Kinetics::observe_selected(kmc::process::Record const& record)
{
    if (record.outcome != kmc::process::Outcome::accepted)
        return;

    auto const& simulation{ *simulation_ };
    auto const time_delta{ simulation.time() - last_time_ };

    last_time_ = simulation.time();

    auto const& process_counts{ simulation.process_counts() };
    auto const& process_rates{ simulation.process_rates() };

    kmc::Rate total_activity{};

    for (std::size_t key{ 1 }; key < process_counts.size(); ++key)
    {
        auto const& [process, rate]{ process_rates[key] };

        if (process.event() != kmc::event::Type::none and process.element() != lattice::Element::none)
        {
            total_activity += process_counts[key] * rate;
        }
    }

    if (total_activity <= kmc::Rate{})
        return;

    for (std::size_t key{ 1 }; key < process_counts.size(); ++key)
    {
        auto const count{ process_counts[key] };
        auto const& [process, rate]{ process_rates[key] };

        if (count == 0 or process.event() == kmc::event::Type::none or
            process.element() == lattice::Element::none)
        {
            continue;
        }

        auto const activity{ count * rate / total_activity };
        auto& accumulation{ accumulation_.processes[static_cast<kmc::process::Key>(key)] };

        accumulation.activity_fraction += activity;
        accumulation.activity_time += activity * time_delta;
    }

    auto const counts{ site_counts(simulation) };

    std::size_t total_site_count{};
    for (auto const count : counts | std::views::values)
        total_site_count += count;

    if (total_site_count == 0)
        return;

    for (auto const& [site, count] : counts)
    {
        auto const population{ static_cast<double>(count) / static_cast<double>(total_site_count) };
        auto& accumulation{ accumulation_.sites[site] };

        accumulation.count += static_cast<double>(count);
        accumulation.population_fraction += population;
        accumulation.count_time += static_cast<double>(count) * time_delta;
        accumulation.population_time += population * time_delta;
    }

    accumulation_.residence_time += time_delta;
    ++accumulation_.samples;
}

void process::Kinetics::capture_snapshot(kmc::Simulation const&)
{
    last_data_ = std::exchange(accumulation_, {});
}

void process::Kinetics::write_init(kmc::Simulation const& simulation)
{
    // Site columns are independent of which process classes were encountered.
    std::string site_header{ Writer::default_header };
    for (auto element : lattice::active_elements)
        for (lattice::Coordination coordination{}; coordination < lattice::max_coordination;
            ++coordination)
            std::format_to(std::back_inserter(site_header), ",{}_{}", enum_name(element),
                static_cast<unsigned>(coordination));

    site_header += '\n';

    process_header_ = process_header(simulation);
    process_columns_ = simulation.process_rates().size() - 1;

    file::write<csv>(writer_path() / "site_count", site_header);
    file::write<csv>(writer_path() / "population", site_header);
    file::write<csv>(writer_path() / "time_weighted_site_count", site_header);
    file::write<csv>(writer_path() / "time_weighted_population", site_header);
    file::write<csv>(writer_path() / "activity", process_header_ + '\n');
    file::write<csv>(writer_path() / "time_weighted_activity", process_header_ + '\n');
}

void process::Kinetics::write_snapshot(kmc::Simulation const& simulation)
{
    if (last_data_.samples == 0 or last_data_.residence_time <= physics::quantity::TimeDifference{})
        return;

    auto const new_header{ process_header(simulation) };
    if (new_header != process_header_)
    {
        const auto new_columns{ simulation.process_rates().size() - 1 };
        if (new_columns < process_columns_)
            throw std::logic_error{ "Process key space shrank during CSV output." };

        const auto extra_columns{ new_columns - process_columns_ };
        rewrite_header(writer_path() / "activity", new_header, extra_columns);
        rewrite_header(writer_path() / "time_weighted_activity", new_header, extra_columns);
        process_header_ = new_header;
        process_columns_ = new_columns;
    }

    auto const prefix{ std::format("{},{:v:}", simulation.step(), simulation.time()) };
    auto const samples{ static_cast<double>(last_data_.samples) };

    std::string site_count{ prefix };
    std::string population{ prefix };
    std::string time_weighted_site_count{ prefix };
    std::string time_weighted_population{ prefix };

    for (auto element : lattice::active_elements)
        for (lattice::Coordination coordination{}; coordination < lattice::max_coordination;
            ++coordination)
        {
            const SiteKey site{ element, coordination };
            SiteAccumulation data{};

            if (auto it{ last_data_.sites.find(site) }; it != last_data_.sites.end())
                data = it->second;

            std::format_to(std::back_inserter(site_count), ",{}", data.count / samples);
            std::format_to(std::back_inserter(population), ",{}", data.population_fraction / samples);
            std::format_to(std::back_inserter(time_weighted_site_count), ",{}",
                data.count_time / last_data_.residence_time);
            std::format_to(std::back_inserter(time_weighted_population), ",{}",
                data.population_time / last_data_.residence_time);
        }

    std::string activity{ prefix };
    std::string time_weighted_activity{ prefix };

    for (std::size_t key{ 1 }; key < simulation.process_rates().size(); ++key)
    {
        auto const& process{ simulation.process_rates()[key].first };
        if (not process.is_active())
        {
            activity += ',';
            time_weighted_activity += ',';
            continue;
        }

        ProcessAccumulation data{};
        if (auto it{ last_data_.processes.find(static_cast<kmc::process::Key>(key)) };
            it != last_data_.processes.end())
            data = it->second;

        std::format_to(std::back_inserter(activity), ",{}", data.activity_fraction / samples);
        std::format_to(std::back_inserter(time_weighted_activity), ",{}",
            data.activity_time / last_data_.residence_time);
    }

    site_count += '\n';
    population += '\n';
    time_weighted_site_count += '\n';
    time_weighted_population += '\n';
    activity += '\n';
    time_weighted_activity += '\n';

    file::append<csv>(writer_path() / "site_count", site_count);
    file::append<csv>(writer_path() / "population", population);
    file::append<csv>(writer_path() / "time_weighted_site_count", time_weighted_site_count);
    file::append<csv>(writer_path() / "time_weighted_population", time_weighted_population);
    file::append<csv>(writer_path() / "activity", activity);
    file::append<csv>(writer_path() / "time_weighted_activity", time_weighted_activity);
}
