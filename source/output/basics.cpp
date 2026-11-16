#include "pch.h"

#include "basics.h"

#include "kmc/simulation.h"
#include "utility/file.h"

using namespace utility;
using enum file::Type;

using namespace output;

namespace global { std::string settings(); }

bool control::StepLimit::stop_requested() const
{
    return simulation_ and simulation_->step() >= limit_;
}

bool control::DurationLimit::stop_requested() const
{
    return simulation_ and simulation_->run_duration() >= limit_;
}

void output::Config::write_init(kmc::Simulation const& kmc)
{
    file::write<txt>(root_path() / "model", std::format("{}", kmc.model()->output_text()));
    file::write<txt>(root_path() / "lattice", std::format("{}", kmc.lattice()));
    file::write<txt>(root_path() / "seed", std::format("{}", kmc.rng()));
    file::write<txt>(root_path() / "settings", global::settings());
    file::write<txt>(root_path() / "rules", kmc.output_settings());
}

void output::Time::write_init(kmc::Simulation const&)
{
    file::write<csv>(writer_path(), std::format("{}\n", default_header));
}

void output::Time::write_snapshot(kmc::Simulation const& kmc)
{
    file::append<csv>(writer_path(), std::format("{},{:v:}\n", kmc.step(), kmc.time()));
}

void output::Performance::write_init(kmc::Simulation const&)
{
    file::write<csv>(writer_path(), "key:step,runtime/s,per_step/ns\n");
}

void output::Performance::write_snapshot(kmc::Simulation const& kmc)
{
    if (not kmc.step())
        return;

    file::append<csv>(
        writer_path(),
        std::format(
            "{},{:v:},{:v:ns}\n",
            kmc.step(),
            kmc.run_duration(),
            kmc.run_duration() / kmc.step()));
}

void output::Lattice::write_init(kmc::Simulation const&)
{
    std::filesystem::create_directories(writer_path());
}

void output::Lattice::write_snapshot(kmc::Simulation const& kmc)
{
    file::write<xyz>(
        writer_path() / snapshot_name(kmc.step()),
        kmc.lattice().to_xyz_file(save_bulk_));
}

void output::Lattice::write_final(kmc::Simulation const& kmc)
{
    if (not save_bulk_)
        file::write<xyz>(writer_path() / "final", kmc.lattice().to_xyz_file(true));
}

void output::Rates::write_final(kmc::Simulation const& kmc)
{
    std::string text{ kmc::ClassInfo::header };
    text += ",rate/Hz\n";

    for (auto const& [info, rate] : kmc.class_rates())
        std::format_to(std::back_inserter(text), "{},{:v:Hz}\n", info, rate);

    file::write<csv>(writer_path(), text);
}

template <std::ranges::input_range Range>
std::string format_range(Range const& range)
{
    std::string result{ "[" };
    bool first{ true };

    for (auto const& value : range)
    {
        if (not first)
            result += ", ";

        std::format_to(std::back_inserter(result), "{}", value);
        first = false;
    }

    result += ']';
    return result;
}

std::string output::Composition::settings() const
{
    return std::format("elements = {}", format_range(active_elements));
}

void output::Composition::capture_init(kmc::Simulation const& kmc)
{
    element_counts_.clear();

    for (auto& site : kmc.lattice().sites())
        if (site.is_occupied())
            ++element_counts_[site.element()];
}

void output::Composition::observe_selected(kmc::event::Record const& record)
{
    if (record.outcome != kmc::event::Outcome::accepted)
        return;

    auto const delta{ kmc::event::solid_change(record.type) };
    if (delta != 0)
        element_counts_[record.location().source().element()] += delta;
}

void output::Composition::write_init(kmc::Simulation const&)
{
    std::string header{ default_header };

    for (auto element : active_elements)
        std::format_to(std::back_inserter(header), ",{}_fraction", element);

    file::write<csv>(writer_path(), header += '\n');
}

void output::Composition::write_snapshot(kmc::Simulation const& kmc)
{
    auto const occupied_sites{ utility::sum(element_counts_ | std::views::values) };

    std::string line{ std::format("{},{:v:}", kmc.step(), kmc.time()) };

    for (auto element : active_elements)
    {
        auto const fraction{ occupied_sites == 0
                                 ? std::numeric_limits<double>::quiet_NaN()
                                 : static_cast<double>(element_counts_[element]) / occupied_sites };

        std::format_to(std::back_inserter(line), ",{}", fraction);
    }

    file::append<csv>(writer_path(), line += '\n');
}

std::string global::settings()
{
    std::string settings;

    settings += std::format(
        "Lattice Settings\n"
        "active_elements = {}\n"
        "element_specific_neighbors = {}\n"
        "use_layers = {}\n"
        "epsilon::length = {:fm}\n"
        "epsilon::coordinate = {}\n",
        format_range(lattice::active_elements),
        lattice::element_specific_neighbors,
        lattice::use_layers,
        lattice::epsilon::length,
        lattice::epsilon::coordinate);

    return settings;
}
