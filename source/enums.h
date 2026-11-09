#pragma once

namespace kmc
{
    // Add new events here if desired
    enum class Event : std::uint8_t
    {
        none,
        diffusion,
        dissolution,
        deposition,
    };

    namespace process
    {
        enum class Outcome : std::uint8_t
        {
            active,
            accepted,
            rejected,
        };
    }
}
