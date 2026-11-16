#pragma once
#include "kmc/model.h"

namespace studies::dealloying::models
{
    using namespace kmc;
    using namespace physics::unit::literals;

    class ElectrochemicalInterface : public kmc::Model
    {
        using MolarConcentration = physics::quantity::MolarConcentration;

    public:
        static constexpr auto standard_concentration{ 1_mol / 1_l };
        struct Constants
        {
            ElectricPotential reference_potential{ 0.535737863990985_V };
            Energy solvation_energy{ 0.75_eV };
            double charge_transfer_coefficient{ 1.0 };
        };

        static void ensure_range(MolarConcentration);
        static void ensure_range(Constants const&);

        auto& set_potential(this auto& self, ElectricPotential potential)
        {
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

        kmc::Rate rate(kmc::RateClassKey) const override;
        void refresh_rates() override {}
        std::string output_text() const override;

        auto potential() const { return potential_; }
        auto concentration() const { return concentration_; }
        auto& constants() const { return constants_; }

    protected:
        virtual kmc::Element class_element(kmc::RateClassKey) const = 0;
        virtual Energy lattice_energy_barrier(kmc::RateClassKey) const = 0;
        virtual Ratio modulation_ratio(kmc::RateClassKey) const { return 1; }

    private:
        Ratio transmission_coefficient(kmc::RateClassKey) const;
        Energy activation_gibbs_energy(kmc::RateClassKey) const;

        ElectricPotential potential_{ Constants{}.reference_potential };
        MolarConcentration concentration_{ standard_concentration };
        Constants constants_{};
    };
}
