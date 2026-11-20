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

