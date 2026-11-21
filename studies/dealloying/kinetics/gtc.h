#pragma once
#include "power_law_bond_energy.h"
#include "gtc_environment.h"
#include <cstdint>
#include <memory>
#include <map>
#include <optional>
#include <vector>

namespace studies::dealloying::kinetics
{
    // Power-law binding energies with a geometry-dependent diffusion saddle.
    // FCC paths and Gaussian weights are precomputed and cached by the canonical
    // 18-site occupancy mask. No-path fallback to n_TS=2 remains provisional.
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
            // Canonical 18-bit FCC spectator occupation; zero for dissolution.
            std::uint32_t environment{};
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
        mutable std::shared_ptr<gtc::FccEnvironment const> environment_;
    };
}
