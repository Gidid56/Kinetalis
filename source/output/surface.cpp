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

void Surface::observe_selected(kmc::event::Record const& record)
{
    if (record.outcome != kmc::event::Outcome::accepted)
        return;

    update(*record.sites()[0], kmc::event::neighbor_change(record.type, true));
}

void Surface::observe_processed(kmc::event::Record const& record)
{
    if (record.type != kmc::event::Type::diffusion)
        return;

    update(*record.sites()[1], kmc::event::neighbor_change(record.type, false));
}

void Surface::write_init(kmc::Simulation const& kmc)
{
    csv_.initialize(writer_path(),
        [] { return std::string{ default_header } + ",surface_sites,broken_bonds"; },
        [this, &kmc] {
            return std::format("{},{:v:},{},{}", kmc.step(), kmc.time(), sites_, bonds_);
        });
}

void Surface::write_snapshot(kmc::Simulation const&) { csv_.append(2); }
void Surface::write_final(kmc::Simulation const&) { csv_.finalize(); }
