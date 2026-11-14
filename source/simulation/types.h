#pragma once

namespace simulation
{
    enum class Status
    {
        created,
        paused,
        running,
        exhausted,
        analysed,
    };

    class Base;
    class Engine;

    constexpr std::size_t seed_words{ 8 };
    using Seed = std::array<std::uint32_t, seed_words>; // 256-bit seed
}
