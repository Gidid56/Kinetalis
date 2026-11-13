#include "pch.h"

#include "models.h"

#include "kmc/process.h"
#include "lattice/path.h"
#include "lattice/site.h"

using namespace studies::dealloying::rules;
using lattice::Element;

bool Dissolution::active(lattice::Site const& site) const
{
    return site.element() == Element::Ag and not site.is_bulk();
}

bool Dissolution::allowed(lattice::Site const& site) const { return active(site); }

bool Dissolution::supports(kmc::Process const& process) const
{
    return process.event() == event and process.element() == Element::Ag;
}

bool Diffusion::source_active(lattice::Site const& site) const
{
    return site.is_occupied() and site.vacant_neighbors() >= 3;
}

bool Diffusion::active(lattice::Path const& path) const
{
    return source_active(path.front()) and path.back().is_vacant();
}

bool Diffusion::allowed(lattice::Path const& path) const
{
    return active(path) and (path.back().coordination() >= 3 or path.front().coordination() == 0);
}

bool Diffusion::supports(kmc::Process const& process) const
{
    return process.event() == event and process.element() != Element::none;
}
