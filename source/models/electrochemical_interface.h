#pragma once
#include "kmc/model.h"
#include "types.h"

class kmc::models::ElectrochemicalInterface : public Model
{
    using MolarConcentration = physics::quantity::MolarConcentration;

public:
    static constexpr auto standard_concentration{ 1_mol / 1_l };

    struct Constants
    {
        ElectricPotential reference_potential{ 0.5236_V };
        //ElectricPotential reference_potential{ 0.535737863990985_V };
        Energy solvation_energy{ 0.75_eV };
        double charge_transfer_coefficient{ 1.0 };
    };

    static void ensure_range(MolarConcentration concentration);
    static void ensure_range(Constants const& constants);

    auto& set_potential(this auto& self, ElectricPotential potential)
    {
        //physics::quantity::ensure_range(potential);
        self.potential_ = potential;
        return self;
    }

    auto& set_concentration(this auto& self, MolarConcentration concentration)
    {
        ensure_range(concentration);
        self.concentration_ = concentration;
        return self;
    }

    auto& set_constants(this auto& self, Constants constants)
    {
        ensure_range(constants);
        self.constants_ = constants;
        return self;
    }

    std::string output_text() const override;

    auto potential() const { return potential_; }
    auto concentration() const { return concentration_; }
    auto& constants() const { return constants_; }

protected:
    virtual Energy lattice_energy_barrier(Process const&) const = 0;
    Ratio transmission_coefficient(Process const&) const override;

private:
    Energy activation_gibbs_energy(Process const&) const override final;

    ElectricPotential potential_{ Constants{}.reference_potential };
    MolarConcentration concentration_{ standard_concentration };
    Constants constants_{};
};
