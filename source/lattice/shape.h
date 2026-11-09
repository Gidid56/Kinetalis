#pragma once
#pragma once
#include "types.h"

// Local coordiantes inside the shape range from [0, 0, 0] to (1, 1, 1)

// Shape is the abstract base class of all 3D geometric shapes in this library.
class lattice::shape::Shape
{
public:
    constexpr explicit Shape(lattice::Basis const& vectors, Position const& center = {})
        : center_{ center },
          vectors_{ vectors },
          parameters_{ vectors.norms() },
          reciprocal_basis_{ vectors.transpose_invert() }
    { }
    constexpr explicit Shape(Parameters const& parameters, Position const& center = {})
        : Shape{ math::Matrix<3>::identity() * parameters, center }
    { }
    constexpr virtual ~Shape() = default;

    Shape& move_by(math::Vector<3, Displacement> const& translation);
    Shape& move_by(math::Axis, Displacement);

    // Counterclockwise rotation around the axis
    Shape& rotate(math::Axis, Angle);

    // 3D rotation angles are alpha, beta, gamma (yaw, pitch, and roll), about axes z, y, x, respectively
    Shape& rotate(Angles const&);

    // Transfroms world coordinates [m] to dimensionless local coordinates
    Coordinate coordinate(Position const&) const;

    // Transfroms dimensionless local coordinates to world coordinates [m]
    Position position(Coordinate const&) const;

    // Checks if the shape contains the position in world coordinates [m]
    bool contains(Position const&) const;

    // Checks if the shape contains the position in dimensionless local coordinates
    virtual bool contains(Coordinate const&) const = 0;

    // Returns the dimensions [x,y,z] of the smallest cuboid containing the entire shape
    Parameters outer_cuboid() const;

    virtual Volume volume() const = 0;

    constexpr void set_origin(Position const& center) { center_ = center; }
    constexpr void set_vectors(Basis const& vectors)
    {
        vectors_ = vectors;
        parameters_ = vectors.norms();
        reciprocal_basis_ = vectors.transpose_invert();
    }

    constexpr auto& center() const { return center_; }
    constexpr auto& vectors() const { return vectors_; }
    constexpr auto& parameters() const { return parameters_; }
    constexpr auto& reciprocal_basis() const { return reciprocal_basis_; }

protected:
    Position center_;
    Basis vectors_;
    // Cached for performance
    Parameters parameters_;   // Norms of the lattice vectors
    ReciprocalBasis reciprocal_basis_; // Inverse of the lattice vectors
};

// A Parallelepiped is a three-dimensional parallelogram defined by three
// edge vectors, that represent its width, height, and depth. The edges
// are arbitrary vectors in three-dimensional space, and their orientation
// defines the orientation of the shape.
class lattice::shape::Parallelepiped : public Shape
{
public:
    using Shape::contains;
    using Shape::Shape;
    bool contains(Coordinate const&) const override;
    Volume volume() const override;

    Parallelepiped& scale(math::Vector<3> const& scale);
    Parallelepiped& scale(Parameters const& min);

    SurfaceBasis surface_basis() const;
    Area surface_area(math::Axis) const;
    Area surface_area() const;

    // Returns the ratio of the volume of the parallelepiped to the product of its axes' lengths.
    Ratio relative_volume() const;

    bool is_cube() const;
    bool is_rectangular() const;
};

// Ellipsoid is an ellipsoidal shape defined by three arbitrarily oriented diameters.
// It can be considered a parallelepiped with rounded surfaces.
class lattice::shape::Ellipsoid : public Shape
{
public:
    using Shape::contains;
    using Shape::Shape;
    bool contains(Coordinate const&) const override;
    Volume volume() const override;

    Area surface_area() const;
};

// Cylindroid is a cylindrical shape defined by three parameters:
// the two diameters of the elliptic base and the height of the cylinder.
class lattice::shape::Cylindroid : public Shape
{
public:
    using Shape::contains;
    using Shape::Shape;
    bool contains(Coordinate const&) const override;
    Volume volume() const override;
};

// EllipticCone is a shape defined by three parameters:
// the two diameters of the elliptic base and the height of the cone.
// It is an elliptical cone that is symmetric around the z-axis.
class lattice::shape::EllipticCone : public Shape
{
public:
    using Shape::contains;
    using Shape::Shape;
    bool contains(Coordinate const&) const override;
    Volume volume() const override;
};

namespace lattice::shape
{
    constexpr Parallelepiped cube(Length side) { return Parallelepiped{ Parameters::all(side) }; }
    constexpr Parallelepiped cuboid(Length x, Length y, Length z)
    {
        return Parallelepiped{ Parameters{ x, y, z } };
    }
    constexpr Parallelepiped sheet(Length height)
    {
        return Parallelepiped{ Parameters{ 1_Qm, 1_Qm, height } };
    }
    constexpr Ellipsoid sphere(Length radius) { return Ellipsoid{ Parameters::all(radius * 2) }; }
    constexpr Cylindroid cylinder(Length radius, Length height = 1_Qm)
    {
        return Cylindroid{ Parameters{ radius * 2, radius * 2, height } };
    }
    constexpr EllipticCone cone(Length radius, Length height)
    {
        return EllipticCone{ Parameters{ radius * 2, radius * 2, height } };
    }
}

template <>
struct std::formatter<lattice::shape::Shape> : public std::formatter<lattice::Basis>
{
    std::format_context::iterator format(lattice::shape::Shape const&, std::format_context&) const;
};

template <>
struct std::formatter<lattice::shape::Parallelepiped> : public std::formatter<lattice::Shape>
{
};
template <>
struct std::formatter<lattice::shape::Ellipsoid> : public std::formatter<lattice::Shape>
{
};
template <>
struct std::formatter<lattice::shape::Cylindroid> : public std::formatter<lattice::Shape>
{
};
template <>
struct std::formatter<lattice::shape::EllipticCone> : public std::formatter<lattice::Shape>
{
};
