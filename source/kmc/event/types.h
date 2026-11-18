#pragma once
#include <cstdint>

namespace kmc::event
{
    enum class Type : std::uint8_t
    {
        none,
        diffusion,
        dissolution,
        deposition,
    };

    enum class Outcome : std::uint8_t
    {
        active,
        accepted,
        rejected,
    };

    // Lattice occupancy changes associated with an event type.
    constexpr int solid_change(Type type) noexcept
    {
        switch (type)
        {
            case Type::dissolution: return -1;
            case Type::deposition: return 1;
            default: return 0;
        }
    }

    constexpr int neighbor_change(Type type, bool source) noexcept
    {
        switch (type)
        {
            case Type::dissolution: return source ? -1 : 0;
            case Type::deposition: return source ? 1 : 0;
            case Type::diffusion: return source ? -1 : 1;
            default: return 0;
        }
    }
}
