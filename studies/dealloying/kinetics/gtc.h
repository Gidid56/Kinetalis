#pragma once
#include "power_law_bond_energy.h"
#include <map>
#include <optional>
#include <vector>

namespace studies::dealloying::kinetics
{
    // Power-law binding energies, with an extensible transition-state descriptor.
    // Until complete paths and saddle selection are implemented, the KMC classifier
    // uses the power-law baseline (n_TS=2); no geometry-dependent rates are claimed.
    class GTC : public PowerLawBondEnergy
    {
    public:
        static constexpr std::size_t invalidation_hops{2};

        struct Descriptor
        {
            kmc::event::Type event{};
            chemistry::Element element{};
            lattice::Coordination coordination{};
            // Present only after a saddle has been explicitly characterized.
            std::optional<double> transition_state_coordination{};
            auto operator<=>(Descriptor const&) const = default;
        };

        Descriptor classify(kmc::event::Type, kmc::event::Location const&,
                            lattice::Lattice const&) const;
        kmc::RateClassKey key(Descriptor const&);
        std::size_t number_keys() const noexcept { return descriptors_.size(); }
        Descriptor const& descriptor(kmc::RateClassKey key) const { return descriptors_.at(key); }
        kmc::Rate rate(Descriptor const&) const;
        std::string output_text() const;

    private:
        std::map<Descriptor, kmc::RateClassKey> keys_;
        std::vector<Descriptor> descriptors_{Descriptor{}};
    };
}
