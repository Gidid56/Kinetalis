#include "pch.h"

#include "shape.h"

#include "bravais.h"

using namespace lattice;
using namespace lattice::shape;

namespace
{
    static constexpr auto is_inside = [](auto x) {
        return x >= 0 and x < 1;
    };
}

Shape& Shape::move_by(math::Vector<3, Displacement> const& translation)
{
    center_ += translation;
    return *this;
}

Shape& Shape::move_by(math::Axis axis, Displacement displacement)
{
    center_ += vectors_[axis].normalized() * displacement;
    return *this;
}

CoordinateBasis rotation(math::Axis axis, Angle angle)
{
    auto c{ cos(angle) }, s{ sin(angle) };

    switch (axis)
    {
        case math::Axis::x:
            return {
                unit::x,
                Coordinate{ 0., c, -s },
                Coordinate{ 0., s, c },
            };

        case math::Axis::y:
            return {
                Coordinate{ c, 0., s },
                unit::y,
                Coordinate{ -s, 0., c },
            };

        case math::Axis::z:
            return {
                Coordinate{ c, -s, 0. },
                Coordinate{ s, c, 0. },
                unit::z,
            };

        default: throw std::logic_error{ "invalid axis of rotation" };
    }
};

Shape& Shape::rotate(math::Axis axis, Angle angle)
{
    set_vectors(vectors_.dot(rotation(axis, angle)));
    return *this;
}

Shape& Shape::rotate(Angles const& angles)
{
    set_vectors(vectors_.dot(rotation(math::Axis::z, angles[math::Axis::z]))
            .dot(rotation(math::Axis::y, angles[math::Axis::y]))
            .dot(rotation(math::Axis::x, angles[math::Axis::x])));
    return *this;
}

Coordinate Shape::coordinate(Position const& position) const
{
    return reciprocal_basis_.dot(position - center_) + site::center;
}

Position Shape::position(Coordinate const& coordinate) const
{
    return center_ + vectors_.transpose_dot(coordinate - site::center);
}

bool Shape::contains(Position const& position) const
{
    return contains(coordinate(position) - epsilon::coordinate);
}

Parameters Shape::outer_cuboid() const
{
    constexpr math::Matrix vector_shifts{ math::Vector<2, int>{ 0, 1 }.cartesian_product() };

    auto corners{ vector_shifts.dot(vectors_) };

    return {
        physics::ensure_non_negative(corners.column(0).max() - corners.column(0).min()),
        physics::ensure_non_negative(corners.column(1).max() - corners.column(1).min()),
        physics::ensure_non_negative(corners.column(2).max() - corners.column(2).min()),
    };
}

Parallelepiped& Parallelepiped::scale(math::Vector<3> const& scale)
{
    vectors_ *= scale;
    parameters_ *= scale;
    reciprocal_basis_ /= scale;
    return *this;
}

Parallelepiped& Parallelepiped::scale(Parameters const& min)
{
    return scale(ceil(min / parameters_).to<double>());
}

bool Parallelepiped::contains(Coordinate const& coordinate) const
{
    return std::ranges::all_of(coordinate, is_inside);
}

SurfaceBasis Parallelepiped::surface_basis() const { return reciprocal_basis_ * volume(); }

Area Parallelepiped::surface_area(math::Axis axis) const { return surface_basis()[axis].norm(); }

Area Parallelepiped::surface_area() const { return 2 * surface_basis().norms().sum(); }

Volume Parallelepiped::volume() const
{
    // Equivalent to the scalar triple product
    return Volume{} + abs(vectors_.determinant());
}

Ratio Parallelepiped::relative_volume() const { return volume() / parameters_.product(); }

bool Parallelepiped::is_cube() const
{
    return parameters_.x() == parameters_.y() and parameters_.y() == parameters_.z() and
           is_rectangular();
}

bool Parallelepiped::is_rectangular() const
{
    return vectors_.dot(vectors_.transposed()).is_diagonal();
}

bool Ellipsoid::contains(Coordinate const& coordinate) const
{
    return (coordinate - site::center).r_squared<3>() < math::square(0.5);
}

Volume Ellipsoid::volume() const { return std::numbers::pi / 6 * parameters_.product(); }

// Approximation for nearly spherical ellipsoids
// with a relative error of at most 1.178%
// Based on: Simple approximation for surface area of an ellipsoid
// https://www.johndcook.com/blog/2021/03/24/surface-area-ellipsoid/
Area Ellipsoid::surface_area() const
{
    constexpr auto p{ 1.6 };
    const math::Vector<3> l{ (parameters_ / 2) / 1_m };

    return 4 * std::numbers::pi * math::square(1_m) *
           pow((pow(l.x() * l.y(), p) + pow(l.y() * l.z(), p) + pow(l.z() * l.x(), p)) / 3, 1 / p);
}

bool Cylindroid::contains(Coordinate const& coordinate) const
{
    return is_inside(coordinate.z()) and
           (coordinate - site::center).r_squared<2>() < math::square(0.5);
}

Volume Cylindroid::volume() const { return std::numbers::pi / 4 * parameters_.product(); }

bool EllipticCone::contains(Coordinate const& coordinate) const
{
    return is_inside(coordinate.z()) and
           (coordinate - site::center).r_squared<2>() < math::square(0.5 * (1 - coordinate.z()));
}

Volume EllipticCone::volume() const { return std::numbers::pi / 12 * parameters_.product(); }

std::format_context::iterator
std::formatter<Shape>::format(Shape const& shape, std::format_context& context) const
{
    auto out{ context.out() };
    out = std::format_to(out, "\tvectors\n");
    context.advance_to(out);
    return std::formatter<lattice::Basis>::format(shape.vectors(), context);
}
