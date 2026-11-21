#pragma once
#include <cstdint>
#include <optional>

namespace studies::dealloying::kinetics::gtc
{
    // Returns the largest minimum effective coordination among admissible
    // one- or two-segment FCC paths. Geometry and weights are precomputed
    // once; 18-bit occupancy results are cached lazily.
    //
    // Only the union of source and destination NN shells contributes to
    // the transition-state energy (first GTC approximation).
    // No path: nullopt; the kinetics layer decides how to handle it.
    std::optional<double> saddle_coordination(std::uint32_t occupancy);
}
