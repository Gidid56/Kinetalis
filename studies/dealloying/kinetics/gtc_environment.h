#pragma once
#include "lattice/types.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace studies::dealloying::kinetics::gtc
{
    // Fixed FCC first-neighbor environment of a directed source -> target hop:
    // seven source-only, four common and seven target-only spectator sites.
    // Bit indices are crystallographic, independent of site and neighbor storage order.
    class FccEnvironment
    {
    public:
        using Mask = std::uint32_t;
        static constexpr std::size_t site_count{18};
        static constexpr Mask full_mask{(Mask{1} << site_count) - 1};

        explicit FccEnvironment(lattice::Lattice const& lattice);

        Mask classify(lattice::Site const& source, lattice::Site const& target) const;
        bool belongs_to(lattice::Lattice const& lattice) const noexcept
        {
            return lattice_ == &lattice;
        }

    private:
        static constexpr std::size_t directions{12};
        static constexpr std::uint8_t missing_slot{0xff};

        struct SiteReference
        {
            std::uint8_t endpoint{};  // 0 = source, 1 = target
            std::uint8_t direction{};
        };

        using Slots = std::array<std::uint8_t, directions>;
        using Mapping = std::array<SiteReference, site_count>;

        std::uint8_t direction_of(lattice::Position const& displacement) const;

        lattice::Lattice const* lattice_{};
        double nearest_neighbor_distance_{};
        // Cartesian unit vectors of a consistent cubic frame for this lattice.
        std::array<std::array<double, 3>, 3> cubic_axes_{};
        // One 12-byte neighbor-list permutation per lattice site.
        std::vector<Slots> neighbor_slots_;
        // Map each canonical bit to an endpoint and a crystallographic direction.
        std::array<Mapping, directions> hop_mappings_{};
    };
}
