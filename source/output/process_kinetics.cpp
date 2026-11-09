#include "pch.h"

#include "process_kinetics.h"

#include "kmc/simulation.h"
#include "utility/file.h"

using namespace utility;
using enum file::Type;

using namespace output;

namespace
{
    kmc::Process site_key(kmc::Process const& process)
    {
        return kmc::Process{ kmc::Event::none, process.element(), process.coordination() };
    }

    std::map<kmc::Process, std::size_t> site_counts(kmc::Simulation const& simulation)
    {
        std::map<kmc::Process, std::size_t> counts;
        std::set<kmc::Process> direct_counts;

        auto const& process_counts{ simulation.process_counts() };
        auto const& process_rates{ simulation.process_rates() };

        for (kmc::process::Key key{ 1 }; key < process_counts.size(); ++key)
        {
            auto const process_count{ process_counts[key] };

            if (process_count == 0)
                continue;

            auto const& process{ process_rates[key].first };

            if (process.event() == kmc::Event::none or process.element() == lattice::Element::none)
            {
                continue;
            }

            auto const site{ site_key(process) };

            if (process.event() != kmc::Event::diffusion)
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

    std::set<kmc::Process> site_classes(kmc::Simulation const& simulation)
    {
        std::set<kmc::Process> classes;

        for (auto const& [process, _] : simulation.process_rates())
            if (process.event() != kmc::Event::none and process.element() != lattice::Element::none)
            {
                classes.insert(site_key(process));
            }

        return classes;
    }
}

void process::Kinetics::capture_init(kmc::Simulation const& simulation)
{
    simulation_ = &simulation;
    last_time_ = simulation.time();
    last_data_ = {};
    accumulation_ = {};
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

    for (kmc::process::Key key{ 1 }; key < process_counts.size(); ++key)
    {
        auto const& [process, rate]{ process_rates[key] };

        if (process.event() != kmc::Event::none and process.element() != lattice::Element::none)
        {
            total_activity += process_counts[key] * rate;
        }
    }

    if (total_activity <= kmc::Rate{})
        return;

    for (kmc::process::Key key{ 1 }; key < process_counts.size(); ++key)
    {
        auto const count{ process_counts[key] };
        auto const& [process, rate]{ process_rates[key] };

        if (count == 0 or process.event() == kmc::Event::none or
            process.element() == lattice::Element::none)
        {
            continue;
        }

        auto const activity{ count * rate / total_activity };
        auto& accumulation{ accumulation_.processes[process] };

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
    std::string site_header{ Writer::default_header };

    for (auto const& site : site_classes(simulation))
        std::format_to(
            std::back_inserter(site_header),
            ",{}_{}",
            enum_name(site.element()),
            static_cast<unsigned>(site.coordination()));

    site_header += '\n';

    std::string process_header{ Writer::default_header };

    for (auto const& [process, _] : simulation.process_rates())
        if (process.event() != kmc::Event::none and process.element() != lattice::Element::none)
        {
            std::format_to(std::back_inserter(process_header), ",{:_}", process);
        }

    process_header += '\n';

    file::write<csv>(writer_path() / "site_count", site_header);
    file::write<csv>(writer_path() / "population", site_header);
    file::write<csv>(writer_path() / "time_weighted_site_count", site_header);
    file::write<csv>(writer_path() / "time_weighted_population", site_header);
    file::write<csv>(writer_path() / "activity", process_header);
    file::write<csv>(writer_path() / "time_weighted_activity", process_header);
}

void process::Kinetics::write_snapshot(kmc::Simulation const& simulation)
{
    if (last_data_.samples == 0 or last_data_.residence_time <= physics::quantity::TimeDifference{})
    {
        return;
    }

    auto const prefix{ std::format("{},{:v:}", simulation.step(), simulation.time()) };
    auto const samples{ static_cast<double>(last_data_.samples) };

    std::string site_count{ prefix };
    std::string population{ prefix };
    std::string time_weighted_site_count{ prefix };
    std::string time_weighted_population{ prefix };

    for (auto const& site : site_classes(simulation))
    {
        SiteAccumulation accumulation{};

        if (auto const iterator{ last_data_.sites.find(site) }; iterator != last_data_.sites.end())
        {
            accumulation = iterator->second;
        }

        std::format_to(std::back_inserter(site_count), ",{}", accumulation.count / samples);
        std::format_to(
            std::back_inserter(population),
            ",{}",
            accumulation.population_fraction / samples);
        std::format_to(
            std::back_inserter(time_weighted_site_count),
            ",{}",
            accumulation.count_time / last_data_.residence_time);
        std::format_to(
            std::back_inserter(time_weighted_population),
            ",{}",
            accumulation.population_time / last_data_.residence_time);
    }

    std::string activity{ prefix };
    std::string time_weighted_activity{ prefix };

    for (auto const& [process, _] : simulation.process_rates())
    {
        if (process.event() == kmc::Event::none or process.element() == lattice::Element::none)
        {
            continue;
        }

        ProcessAccumulation accumulation{};

        if (auto const iterator{ last_data_.processes.find(process) };
            iterator != last_data_.processes.end())
        {
            accumulation = iterator->second;
        }

        std::format_to(
            std::back_inserter(activity),
            ",{}",
            accumulation.activity_fraction / samples);
        std::format_to(
            std::back_inserter(time_weighted_activity),
            ",{}",
            accumulation.activity_time / last_data_.residence_time);
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
