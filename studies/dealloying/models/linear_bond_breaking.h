#pragma once
#include "electrochemical_interface.h"
#include <array>
#include <optional>

namespace studies::dealloying::models
{
    class LinearBondBreaking : public ElectrochemicalInterface
    {
    public:
        struct Descriptor
        {
            kmc::event::Type event{};
            kmc::Element element{};
            lattice::Coordination coordination{};
            auto operator<=>(Descriptor const&) const = default;
        };

        struct Modulation { Descriptor descriptor; Ratio ratio; };
        struct Constants { Energy bond_energy{ 0.15_eV }; };

        static void ensure_range(Energy);

        auto& set_bond_energy(this auto& self, Energy bond_energy)
        {
            ensure_range(bond_energy);
            self.constants_.bond_energy = bond_energy;
            return self;
        }

        auto& set_modulation(this auto& self, Modulation const& modulation)
        {
            self.modulation_ = modulation;
            return self;
        }

        auto constants() const { return constants_; }

        kmc::RateClassKey classify(kmc::event::Type, kmc::event::Location const&, lattice::Lattice const&) override;
        std::size_t number_keys() const noexcept override;
        kmc::event::Type class_event(kmc::RateClassKey) const override;
        std::string output_text() const override;
        static kmc::RateClassKey static_key(Descriptor const&);
        Descriptor const& descriptor(kmc::RateClassKey) const;

    protected:
        kmc::Element class_element(kmc::RateClassKey) const override;
        Energy lattice_energy_barrier(kmc::RateClassKey) const override;
        Ratio modulation_ratio(kmc::RateClassKey) const override;

    private:
        static constexpr std::size_t key_count{
            1 + lattice::max_coordination * (std::size(lattice::active_elements) + 2)
        };
        std::array<std::optional<Descriptor>, key_count> descriptors_{};
        std::optional<Modulation> modulation_{};
        Constants constants_{};
    };
}
