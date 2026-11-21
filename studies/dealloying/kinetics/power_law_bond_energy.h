#pragma once
#include "linear_bond_breaking.h"
namespace studies::dealloying::kinetics
{
    class PowerLawBondEnergy : public LinearBondBreaking
    {
    public:
        static constexpr double diffusion_transition_state_coordination{ 2.0 };

        PowerLawBondEnergy();
        Energy lattice_energy_barrier(Descriptor const&) const;
        kmc::Rate rate(Descriptor const&) const;
        std::string output_text() const;
    };
}
