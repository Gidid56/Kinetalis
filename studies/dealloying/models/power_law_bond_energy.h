#pragma once
#include "linear_bond_breaking.h"
namespace studies::dealloying::models
{
    class PowerLawBondEnergy : public LinearBondBreaking
    {
    public:
        Energy lattice_energy_barrier(kmc::process::Key) const override;
        std::string output_text() const override;
    };
}
