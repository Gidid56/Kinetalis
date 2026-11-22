#pragma once
#include <cstdint>

namespace studies::dealloying::kinetics::gtc
{
    // Effective coordination at the highest-energy point, including the
    // final lattice site. Choose the arc with the largest bottleneck
    // coordination; if no arc is admissible, use the direct-hop midpoint.
    // Calculations use the 18 nearest-neighbor spectator sites, cached lazily.
    double saddle_coordination(std::uint32_t occupancy);

    // Target NN coordination AFTER source vacates; source is excluded.
    unsigned final_coordination(std::uint32_t occupancy);
}
