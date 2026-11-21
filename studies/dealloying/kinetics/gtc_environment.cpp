#include "pch.h"
#include "gtc_environment.h"

#include "lattice/lattice.h"
#include "lattice/site.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

using studies::dealloying::kinetics::gtc::FccEnvironment;

namespace
{
    struct Offset
    {
        int x{}, y{}, z{};
        auto operator<=>(Offset const&) const = default;
    };

    constexpr Offset operator+(Offset a, Offset b)
    {
        return {a.x + b.x, a.y + b.y, a.z + b.z};
    }

    constexpr Offset operator-(Offset a, Offset b)
    {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }

    // FCC nearest-neighbor vectors in half-cubic-lattice-constant units.
    constexpr std::array<Offset, 12> fcc_directions{{
        {-1, -1, 0}, {-1, 0, -1}, {-1, 0, 1}, {-1, 1, 0},
        {0, -1, -1}, {0, -1, 1}, {0, 1, -1}, {0, 1, 1},
        {1, -1, 0}, {1, 0, -1}, {1, 0, 1}, {1, 1, 0}
    }};

    constexpr Offset reference_hop{1, 1, 0};

    std::uint8_t index_of(Offset direction)
    {
        for (std::size_t i{}; i < fcc_directions.size(); ++i)
            if (fcc_directions[i] == direction)
                return static_cast<std::uint8_t>(i);
        throw std::invalid_argument{"Not an FCC nearest-neighbor direction."};
    }

    // The union of source and target NN positions, excluding both endpoints.
    std::array<Offset, 18> spectators(Offset target)
    {
        std::vector<Offset> sites;
        sites.reserve(18);
        auto insert = [&](Offset offset) {
            if (offset == Offset{} or offset == target)
                return;
            if (std::ranges::find(sites, offset) == sites.end())
                sites.push_back(offset);
        };

        for (auto d : fcc_directions)
        {
            insert(d);
            insert(target + d);
        }

        if (sites.size() != 18)
            throw std::logic_error{"FCC hop does not have eighteen distinct spectators."};

        std::ranges::sort(sites);
        std::array<Offset, 18> result{};
        std::ranges::copy(sites, result.begin());
        return result;
    }

    // Proper cubic rotations (24 signed axis permutations). The first
    // matching rotation establishes a deterministic convention per hop.
    struct Rotation
    {
        std::array<int, 3> permutation{};
        std::array<int, 3> signs{};

        Offset apply(Offset v) const
        {
            std::array<int, 3> const coordinates{v.x, v.y, v.z};
            return {
                signs[0] * coordinates[permutation[0]],
                signs[1] * coordinates[permutation[1]],
                signs[2] * coordinates[permutation[2]]
            };
        }
    };

    Rotation to_reference(Offset hop)
    {
        std::array<int, 3> permutation{0, 1, 2};
        do
        {
            int inversions{};
            for (int i{}; i < 3; ++i)
                for (int j{i + 1}; j < 3; ++j)
                    inversions += permutation[i] > permutation[j];
            const int parity{inversions % 2 ? -1 : 1};

            for (int sx : {-1, 1})
                for (int sy : {-1, 1})
                    for (int sz : {-1, 1})
                    {
                        if (parity * sx * sy * sz != 1)
                            continue;
                        Rotation const rotation{permutation, {sx, sy, sz}};
                        if (rotation.apply(hop) == reference_hop)
                            return rotation;
                    }
        }
        while (std::next_permutation(permutation.begin(), permutation.end()));

        throw std::logic_error{"Cannot orient FCC hop to its reference direction."};
    }

    using Vector = std::array<double, 3>;

    Vector coordinates(lattice::Position const& position)
    {
        return {position.x().value(), position.y().value(), position.z().value()};
    }

    Vector operator+(Vector const& a, Vector const& b)
    {
        return {a[0] + b[0], a[1] + b[1], a[2] + b[2]};
    }

    Vector operator-(Vector const& a, Vector const& b)
    {
        return {a[0] - b[0], a[1] - b[1], a[2] - b[2]};
    }

    Vector operator/(Vector const& v, double scalar)
    {
        return {v[0] / scalar, v[1] / scalar, v[2] / scalar};
    }

    double dot(Vector const& a, Vector const& b)
    {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }

    double norm(Vector const& a)
    {
        return std::sqrt(dot(a, a));
    }

    Vector cross(Vector const& a, Vector const& b)
    {
        return {
            a[1] * b[2] - a[2] * b[1],
            a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0]
        };
    }
}

FccEnvironment::FccEnvironment(lattice::Lattice const& lattice)
    : lattice_{&lattice},
      nearest_neighbor_distance_{lattice.unit_cell().next_neighbor_distance().value()}
{
    if (lattice.unit_cell().bravais() != lattice::Bravais::cF)
        throw std::invalid_argument{"GTC requires an FCC lattice."};
    if (not (nearest_neighbor_distance_ > 0.0))
        throw std::invalid_argument{"GTC requires a positive nearest-neighbor distance."};

    // Infer the cubic coordinate frame from the actual site positions.
    // This also handles the rotation performed by Lattice::rotate_up().
    // Never use Hilbert indices or neighbor-list offsets as directions.
    std::vector<Vector> nearest_displacements;
    nearest_displacements.reserve(12);
    for (auto const& site : lattice.sites())
    {
        for (auto const* neighbor : lattice.neighbors(site))
        {
            const Vector v{coordinates(lattice.displacement(site, *neighbor)) /
                           nearest_neighbor_distance_};
            if (std::abs(norm(v) - 1.0) > 1e-3)
                continue;
            if (std::ranges::none_of(nearest_displacements,
                [&](Vector const& previous) { return norm(v - previous) < 1e-3; }))
                nearest_displacements.push_back(v);
        }
        if (nearest_displacements.size() >= fcc_directions.size())
            break;
    }

    if (nearest_displacements.size() != fcc_directions.size())
        throw std::invalid_argument{"Incomplete FCC directions: cannot establish the cubic frame."};

    std::ranges::sort(nearest_displacements);
    const Vector u{nearest_displacements.front()};
    auto const v_it{std::ranges::find_if(nearest_displacements,
        [&](Vector const& v) { return std::abs(dot(u, v)) < 1e-3; })};

    if (v_it == nearest_displacements.end())
        throw std::logic_error{"Cannot construct an orthogonal FCC direction pair."};

    constexpr double root_two{std::numbers::sqrt2};
    cubic_axes_[0] = (u + *v_it) / root_two;
    cubic_axes_[1] = (u - *v_it) / root_two;
    cubic_axes_[2] = cross(cubic_axes_[0], cubic_axes_[1]);

    // Map every actual neighbor-list slot to a crystallographic direction.
    // This is performed ONCE; classification thereafter only reads 18 bits.
    neighbor_slots_.resize(lattice.sites().size());
    for (auto const& site : lattice.sites())
    {
        auto& slots{neighbor_slots_.at(site.index())};
        slots.fill(missing_slot);
        std::size_t slot{};
        for (auto const* neighbor : lattice.neighbors(site))
        {
            auto const displacement{lattice.displacement(site, *neighbor)};
            const Vector v{coordinates(displacement)};
            if (std::abs(norm(v) / nearest_neighbor_distance_ - 1.0) < 1e-3)
            {
                if (slot >= missing_slot)
                    throw std::overflow_error{"FCC neighbor slot exceeds uint8_t range."};

                const auto index{direction_of(displacement)};
                if (slots[index] != missing_slot)
                    throw std::logic_error{"Duplicate crystallographic NN direction."};
                slots[index] = static_cast<std::uint8_t>(slot);
            }
            ++slot;
        }
    }

    // Precompute the mapping for each of the 12 possible directed hops.
    // A shared spectator is always read from the source neighbor list.
    auto const reference{spectators(reference_hop)};
    for (std::size_t d{}; d < fcc_directions.size(); ++d)
    {
        const Offset hop{fcc_directions[d]};
        const Rotation rotation{to_reference(hop)};
        const auto actual{spectators(hop)};

        for (std::size_t bit{}; bit < site_count; ++bit)
        {
            const auto it{std::ranges::find_if(actual, [&](Offset position) {
                return rotation.apply(position) == reference[bit];
            })};
            if (it == actual.end())
                throw std::logic_error{"Incomplete FCC orientation mapping."};

            const Offset position{*it};
            const auto source_direction{std::ranges::find(fcc_directions, position)};
            hop_mappings_[d][bit] = source_direction != fcc_directions.end()
                ? SiteReference{0, index_of(position)}
                : SiteReference{1, index_of(position - hop)};
        }
    }
}

std::uint8_t FccEnvironment::direction_of(lattice::Position const& displacement) const
{
    const Vector v{coordinates(displacement)};
    std::array<int, 3> coordinate{};
    for (std::size_t axis{}; axis < 3; ++axis)
    {
        const double projected{
            dot(v, cubic_axes_[axis]) * std::numbers::sqrt2 / nearest_neighbor_distance_
        };
        coordinate[axis] = static_cast<int>(std::lround(projected));
        if (std::abs(projected - coordinate[axis]) > 1e-3)
            throw std::invalid_argument{"Neighbor displacement is not aligned with FCC axes."};
    }
    return index_of({coordinate[0], coordinate[1], coordinate[2]});
}

FccEnvironment::Mask FccEnvironment::classify(
    lattice::Site const& source, lattice::Site const& target) const
{
    auto const direction{direction_of(lattice_->displacement(source, target))};
    auto const& mapping{hop_mappings_[direction]};

    Mask mask{};
    for (std::size_t bit{}; bit < site_count; ++bit)
    {
        auto const& entry{mapping[bit]};
        auto const& endpoint{entry.endpoint ? target : source};
        auto const slot{neighbor_slots_.at(endpoint.index())[entry.direction]};
        if (slot == missing_slot)
            continue; // A missing boundary site is equivalent to an empty site.

        const auto index{lattice_->neighbors().at(endpoint.neighbor_index() + slot)};
        if (lattice_->sites().at(index).is_occupied())
            mask |= Mask{1} << bit;
    }
    return mask;
}
