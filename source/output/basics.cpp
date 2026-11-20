#include "pch.h"

#include "basics.h"

#include "kmc/simulation.h"
#include "utility/file.h"

using namespace utility;
using enum file::Type;

using namespace output;

bool control::StepLimit::stop_requested(kmc::Simulation const& simulation) const
{
    return simulation.step() >= limit_;
}

bool control::DurationLimit::stop_requested(kmc::Simulation const& simulation) const
{
    return simulation.run_duration() >= limit_;
}

void output::Config::write_init(kmc::Simulation const& kmc)
{
    file::write<txt>(root_path() / "kinetics", kmc.kinetics_settings());
    file::write<txt>(root_path() / "lattice", std::format("{}", kmc.lattice()));
    file::write<txt>(root_path() / "seed", std::format("{}", kmc.rng()));
    file::write<txt>(root_path() / "processes", kmc.process_settings());
}

void output::Time::write_init(kmc::Simulation const& kmc)
{
    csv_.initialize(writer_path(), [] { return std::string{ default_header }; },
        [&kmc] { return std::format("{},{:v:}", kmc.step(), kmc.time()); });
}

void output::Time::write_snapshot(kmc::Simulation const&) { csv_.append(0); }
void output::Time::write_final(kmc::Simulation const&) { csv_.finalize(); }

void output::Performance::write_init(kmc::Simulation const& kmc)
{
    csv_.initialize(writer_path(),
        [] { return std::string{ "key:step,runtime/s,per_step/ns" }; },
        [&kmc] {
            return std::format("{},{:v:},{:v:ns}",
                kmc.step(), kmc.run_duration(), kmc.run_duration() / kmc.step());
        });
}

void output::Performance::write_snapshot(kmc::Simulation const& kmc)
{
    if (kmc.step())
        csv_.append(2);
}
void output::Performance::write_final(kmc::Simulation const&) { csv_.finalize(); }

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

std::string output::Composition::settings() const
{
    return "elements = dynamically encountered";
}

void output::Composition::capture_init(kmc::Simulation const& kmc)
{
    element_counts_.clear();
    columns_.clear();
    for (auto& site : kmc.lattice().sites())
        if (site.is_occupied())
        {
            auto const element{ site.element() };
            if (not element_counts_.contains(element))
                columns_.push_back(element);
            ++element_counts_[element];
        }
}

void output::Composition::observe_selected(kmc::event::Record const& record)
{
    if (record.outcome != kmc::event::Outcome::accepted)
        return;

    auto const delta{ kmc::event::solid_change(record.type) };
    if (delta != 0)
    {
        auto const element{ record.location().source().element() };
        if (not element_counts_.contains(element))
            columns_.push_back(element);
        element_counts_[element] += delta;
    }
}

void output::Composition::write_init(kmc::Simulation const& kmc)
{
    auto header = [this] {
        std::string text{ default_header };
        for (auto element : columns_)
            std::format_to(std::back_inserter(text), ",{}_fraction", element);
        return text;
    };

    auto row = [this, &kmc] {
        auto const total{ utility::sum(element_counts_ | std::views::values) };
        std::string text{ std::format("{},{:v:}", kmc.step(), kmc.time()) };
        for (auto element : columns_)
        {
            auto const fraction{ total == 0
                ? std::numeric_limits<double>::quiet_NaN()
                : static_cast<double>(element_counts_.at(element)) / total };
            std::format_to(std::back_inserter(text), ",{}", fraction);
        }
        return text;
    };
    csv_.initialize(writer_path(), std::move(header), std::move(row));
}

void output::Composition::write_snapshot(kmc::Simulation const&)
{
    csv_.append(columns_.size());
}

void output::Composition::write_final(kmc::Simulation const&)
{
    csv_.finalize();
}
