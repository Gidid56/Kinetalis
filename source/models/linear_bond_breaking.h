#pragma once
#include "electrochemical_interface.h"
#include "kmc/process.h"

class kmc::models::LinearBondBreaking : public ElectrochemicalInterface
{
public:
    struct Constants
    {
        Energy bond_energy{ 0.15_eV };
    };

    static void ensure_range(Energy bond_energy);

    auto& set_bond_energy(this auto& self, Energy bond_energy)
    {
        ensure_range(bond_energy);
        self.constants_ = bond_energy;
        return self;
    }

    auto constants() const { return constants_; }

    std::string output_text() const override;

private:
    virtual Energy lattice_energy_barrier(Process const&) const override;

    Constants constants_{};
};
