#include "pch.h"

#include "linear_bond_breaking.h"

#include "kmc/process.h"

using namespace kmc;

void models::LinearBondBreaking::ensure_range(Energy bond_energy)
{
    if (not isfinite(bond_energy))
        throw std::invalid_argument{ "Bond energy must be finite." };
}

Energy models::LinearBondBreaking::lattice_energy_barrier(Process const& process) const
{
    return constants_.bond_energy * process.coordination();
}

std::string models::LinearBondBreaking::output_text() const
{
    return std::format(
        "Model = Linear bond breaking\n"
        "Bond energy = {:meV}\n"
        "{}",
        constants_.bond_energy,
        ElectrochemicalInterface::output_text());
}
