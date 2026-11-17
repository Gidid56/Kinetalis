#pragma once
#include "linear_bond_breaking.h"
namespace studies::dealloying::models
{
    class PowerLawBondEnergy : public LinearBondBreaking
    {
    public:
        Energy lattice_energy_barrier(kmc::RateClassKey) const;
        kmc::Rate rate(kmc::RateClassKey) const;
        std::string output_text() const;
    };
}
