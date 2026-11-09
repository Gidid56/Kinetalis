#include "pch.h"

#include "surface.h"

#include "kmc/simulation.h"
#include "lattice/lattice.h"
#include "utility/file.h"

using namespace utility::file;
using enum utility::file::Type;

using namespace output;

void Surface::capture_init(kmc::Simulation const& kmc)
{
    lattice_ = &kmc.lattice();

    sites_ = kmc.lattice().surface_atoms();
    bonds_ = kmc.lattice().broken_bonds();
}

void Surface::update(lattice::Site const& site, int change)
{
    auto const bulk_neighbors{ std::ranges::count_if(
        lattice_->neighbors(site),
        [](auto const neighbor) { return neighbor->is_occupied() and neighbor->is_bulk(); }) };

    sites_ -= change * (bulk_neighbors - 1);
    bonds_ += change * (site.vacant_neighbors() - site.coordination());
}

void Surface::observe_selected(kmc::process::Record const& process)
{
    if (process.outcome != kmc::process::Outcome::accepted)
        return;

    update(process.path.front(), process.process.neighbor_change(true));
}

void Surface::observe_processed(kmc::process::Record const& process)
{
    if (process.process.event() != kmc::Event::diffusion)
        return;

    update(process.path.back(), process.process.neighbor_change(false));

    if constexpr (utility::is_debug)
        if (sites_ != lattice_->surface_atoms())
            throw std::runtime_error{ "Surface site count mismatch: " + std::to_string(sites_) +
                                      " vs. " + std::to_string(lattice_->surface_atoms()) };
}

void Surface::write_init(kmc::Simulation const&)
{
    write<csv>(writer_path(), std::string{ default_header } + ",surface_sites,broken_bonds\n");
}

void Surface::write_snapshot(kmc::Simulation const& kmc)
{
    append<csv>(
        writer_path(),
        std::format("{},{:v:},{},{}\n", kmc.step(), kmc.time(), sites_, bonds_));
}
