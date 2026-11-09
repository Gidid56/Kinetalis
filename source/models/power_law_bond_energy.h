#pragma once
#include "linear_bond_breaking.h"

class kmc::models::PowerLawBondEnergy : public LinearBondBreaking
{
public:
    Energy lattice_energy_barrier(Process const&) const override;

    std::string output_text() const override;
};