#pragma once
#include "electrochemical_interface.h"
#include <array>
#include <optional>

namespace studies::dealloying::kinetics
{
    class LinearBondBreaking : public ElectrochemicalInterface
    {
    public:
        struct Descriptor
        {
            kmc::event::Type event{};
            chemistry::Element element{};
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

        Descriptor classify(kmc::event::Type, kmc::event::Location const&, lattice::Lattice const&) const;
        kmc::RateClassKey key(Descriptor const& desc) const { return static_key(desc); }
        std::size_t number_keys() const noexcept;
        std::string output_text() const;
        kmc::Rate rate(Descriptor const&) const;
        static kmc::RateClassKey static_key(Descriptor const&);
        Descriptor descriptor(kmc::RateClassKey key) const;

    protected:
        Energy lattice_energy_barrier(Descriptor const&) const;
        Ratio modulation_ratio(Descriptor const&) const;

    private:
        static constexpr std::size_t key_count{
            1 + 3 * lattice::max_coordination * std::size(lattice::active_elements)
        };
        std::optional<Modulation> modulation_{};
        Constants constants_{};
    };
}
