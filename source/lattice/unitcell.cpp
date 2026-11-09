#include "pch.h"

#include "unitcell.h"

using namespace lattice;
using namespace lattice::unitcell;

/**
 * Return the parallelepiped shape of the unit cell when its x-y-plane is rotated to
 * be parallel to the miller plane defined by the given miller indices.
 * Calculation is analogous to a look-at matrix.
 * Works only for rectangular unit vectors (at least orthorombic crystal system),
 * because the unit vectors need to be parallel to reciprocal vectors (wave vectors)
 * 
 * @param newZ The miller indices of the plane that will become the new z-axis of the rotated
 *             parallelepiped.
 *
 * @throws std::runtime_error If the unit cell vectors are not rectangular.
 *
 * @returns A parallelepiped shape for the unit cell with the rotated x-y-plane.
 */
shape::Parallelepiped
UnitCell::oriented_periodic_supercell(Indices const& new_z, Indices const& old_z) const
{
    if (new_z.is_zero())
        throw std::invalid_argument{ "Miller indices must not all be zero." };

    const auto divisor{
        std::gcd(std::gcd(std::abs(new_z.x()), std::abs(new_z.y())), std::abs(new_z.z()))
    };
    const Indices direction{ new_z / divisor };
    const Indices new_x{ old_z.cross(direction) };

    if (new_x.is_zero())
        return shape();

    if (system() != System::cubic)
        throw std::domain_error{
            "Rotated periodic Miller supercells currently require a cubic unit cell."
        };

    Indices new_y{ direction.cross(new_x) };
    math::Matrix<3, 3, int> new_vectors{ new_x, new_y, direction };

    return shape::Parallelepiped{ new_vectors * parameters() };
}

// The shape is first expanded into an outer cuboid using the shape's bounding box.
// The outer cuboid is then transformed into reciprocal space by multiplying it
// with the inverse of the wave vectors. Finally, the magnitudes
// of the reciprocal space vectors are taken and rounded up to the nearest integer.
// These integer values give the minimum number of unit cell multiples
// in each reciprocal lattice direction that are required to fully cover the given shape.
Coordinate UnitCell::multiples_to_cover(Shape const& shape) const
{
    auto outer_cuboid{ reciprocal_basis_.transposed() * shape.outer_cuboid() };

    return ceil(abs(outer_cuboid)).sum_columns();
}

Distance UnitCell::next_neighbor_distance() const
{
    if (sites_.empty())
        throw std::logic_error{ "Cannot determine the neighbor distance of an empty unit cell." };

    constexpr auto shifts{ Coordinate{ -1, 0, 1 }.cartesian_product() };
    std::optional<Distance> min_distance;

    for (auto& left : sites_)
        for (auto& right : sites_)
            for (auto const& shift : shifts)
            {
                if (&left == &right and shift.is_zero())
                    continue;

                const auto displacement{ vectors_.transpose_dot(
                    right.coordinate - left.coordinate + shift) };
                const auto distance{ displacement.norm() };

                if (not min_distance or distance < *min_distance)
                    min_distance = distance;
            }

    if (not min_distance)
        throw std::logic_error{ "Cannot determine a neighbor distance." };

    return *min_distance;
}

CoordinateBasis vectors::conventional::rhombohedral(Angle angle)
{
    const auto bx = cos(angle / 2);
    const auto by = sin(angle / 2);

    const auto cx = (2 * math::square(bx) - 1) / bx;
    const auto cz = sqrt(1 - math::square(cx));

    return {
        Coordinate{ bx, -by, .0 },
        Coordinate{ bx, by, .0 },
        Coordinate{ cx, .0, cz },
    };
}

CoordinateBasis vectors::conventional::monoclinic(Angle angle)
{
    const auto cy = cos(angle);
    const auto cz = sin(angle);

    return { unit::x, unit::y, Coordinate{ cy, .0, cz } };
}

CoordinateBasis vectors::conventional::triclinic(Angles const& angles)
{
    const auto bx = cos(angles.z());
    const auto by = sin(angles.z());

    const auto cx = cos(angles.y());
    const auto cy = (cos(angles.x()) - cx * bx) / by;
    const auto cz = sqrt(1 - math::square(cx) - math::square(cy));

    return { unit::x, Coordinate{ bx, by, .0 }, Coordinate{ cx, cy, cz } };
}

std::format_context::iterator
std::formatter<UnitCell>::format(UnitCell const& unitcell, std::format_context& context) const
{
    auto out{ context.out() };
    out = std::format_to(out, "lattice vectors\n");

    context.advance_to(out);
    out = std::formatter<lattice::Basis>::format(unitcell.vectors(), context);

    out = std::format_to(out, "\n\tbasis sites\n");

    for (auto const& [site, atom] : unitcell.sites())
        out = std::format_to(out, "{:>4} {:;5.3g}\n", std::format("{}", atom), site);

    out =
        std::format_to(out, "\nNext neighbor distance {:nm}\n", unitcell.next_neighbor_distance());

    return out;
}
