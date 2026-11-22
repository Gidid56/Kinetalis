#pragma once
#include "lattice/types.h"
#include <array>
#include <span>
#include <vector>

namespace studies::dealloying::kinetics::gtc
{
    inline constexpr double default_distance_width{1.0 / 6.0};

    // Thesis Eq. (1.44). Positions must share one coordinate frame.
    double distance_weight(lattice::Distance separation, lattice::Distance nearest_neighbor_distance,
                           double width = default_distance_width);
    double effective_coordination(lattice::Position const& position,
                                  std::span<lattice::Position const> occupied_supports,
                                  lattice::Distance nearest_neighbor_distance,
                                  double width = default_distance_width);

    // Current geometry prototype: isolated segment midpoints, not complete paths
    // or validated saddle points. For visualization only until path grouping is solved.
    std::vector<lattice::Position> candidate_positions(
        std::array<lattice::Site*, 2> const& cluster, lattice::Lattice const& lattice);
}
