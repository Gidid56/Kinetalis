#pragma once
#include "chemistry/chemistry.h"
#include "math/math.h"
#include "physics/physics.h"
#include "utility/enums.h"
#include "utility/storage.h"

#include <functional>
#include <map>
#include <vector>

namespace simulation { class Engine; }

namespace lattice
{
    using chemistry::Composition;
    using chemistry::Element;

    // Add new elements here if desired
    inline constexpr std::array activeElements{
        Element::Ag,
        Element::Au,
        //Element::Cu,
    };

    inline constexpr bool elementSpecificNeighbors{ false }, useLayers{ false };
}

namespace lattice
{
    class Site;
    using Sites = std::vector<Site>;

    using Coord = math::Vector<4, std::int16_t>;

    class Path;
    using Paths = std::vector<Path>;

    using Coordination = std::uint8_t;
    // Implemented for static performance optimizations,
    // adjust for crystal system
    static constexpr Coordination max_coordination{ 12 };

    using ElementNeighbors =
        utility::ArrayStorage<Coordination, std::size(activeElements), elementSpecificNeighbors>;

    using Index = std::uint32_t;
    using Indices = math::Vector<3, int>;

    using Neighbors = std::vector<Index>;
    
    using physics::quantity::Distance;
    using physics::quantity::Displacement;
    using physics::quantity::Diameter;
    using physics::quantity::Area;
    using physics::quantity::Volume;

    using Layer = std::uint16_t;
    using Layers = std::map<physics::quantity::Displacement, Neighbors>;

    using Coordinate = math::Vector<3, physics::SignedNumber>;
    using CoordinateBasis = math::Matrix<3, 3, physics::SignedNumber>;

    using Angle = physics::quantity::PlaneAngle;
    using Angles = math::Vector<3, Angle>;

    using Parameter = Distance;
    using Parameters = math::Vector<3, Parameter>;

    using BasisVector = math::Vector<3, Displacement>;
    using Basis = math::Matrix<3, 3, Displacement>;

    using Position = math::Vector<3, physics::quantity::Displacement>;
    using Positions = std::vector<Position>;

    using ReciprocalVector = math::Vector<3, physics::Quantity<physics::dimension::Wavenumber, physics::Reference::difference>>;
    using ReciprocalBasis = math::Matrix<3, 3, physics::Quantity<physics::dimension::Wavenumber, physics::Reference::difference>>;

    using SurfaceVector = math::Vector<3, physics::Quantity<physics::dimension::Area>>;
    using SurfaceBasis = math::Matrix<3, 3, physics::Quantity<physics::dimension::Area>>;

    namespace shape
    {
        class Shape;
        class Parallelepiped;
        class Ellipsoid;
        class Cylindroid;
        class EllipticCone;
    }
    using shape::Shape;

    namespace unitcell
    {
        struct Site
        {
            Coordinate coordinate;
            chemistry::Atom atom;
        };
        using Sites = std::vector<Site>;

        class UnitCell;
    }
    using unitcell::UnitCell;

    class Cells;
    class Lattice;

    using FillFunction = std::function<void(Lattice&, simulation::Engine&)>;
    using Generator = std::function<Lattice()>;
}
using lattice::Lattice;
using lattice::UnitCell;

// Avoids issues with floating point arithmetics errors,
// shouldn't need adjustment
namespace lattice::epsilon
{
    using namespace physics::unit::literals;
    inline constexpr physics::quantity::Distance length{ 10_fm };
    inline constexpr Difference coordinate{ 1e-6 };
}

// Automatically managed activeElements. Enabled by elementSpecificNeighbors
namespace lattice
{
    inline constexpr std::size_t numberActiveElements{ elementSpecificNeighbors *
                                                       std::size(lattice::activeElements) };
    inline constexpr auto elementKey{ &utility::array_key<activeElements> };
}
