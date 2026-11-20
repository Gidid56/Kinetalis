#pragma once
#include "lattice/types.h"

namespace studies::dealloying::gtc
{
    // Positions of geometric transition-state contact sites for FCC diffusion.
    // The result is intended for visualization; the event rate model does not use it.
    std::vector<lattice::Position> transition_states(
        std::array<lattice::Site*, 2> const& cluster,
        lattice::Lattice const& lattice);
}
