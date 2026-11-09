#pragma once

/*
   This header file defines a namespace 'lattice' that encapsulates various enums,
   constants, and constexpr functions related to crystal lattice structures.

   - 'Bravais' enum lists different Bravais lattice types.
   - 'System' enum defines various crystal systems.
   - 'Centering' enum represents different types of centering within a lattice.

   The 'centering' function determines centering based on a given Bravais type.
   The 'system' function maps Bravais types to crystal systems.
   The 'orthogonal' function checks if a crystal system is orthogonal.

   Namespaces 'unit' and 'site' contain constants for unit cell and site coordinates, respectively.
   The 'vectors' namespace includes constants and functions related to lattice vectors.
   - 'conventional' sub-namespace generates conventional lattice vectors for different types.
   - 'to' sub-namespace converts vectors to primitive lattice vectors based on centering.

   The 'parameters' namespace provides constants for specific lattice parameters based on centering.

   Overall, this header organizes essential lattice-related data structures, calculations, and
   conversions required for generating crystal lattice structures.
*/

namespace lattice
{
    // NOLINTBEGIN(readability-identifier-naming)
    enum class Bravais
    {
        aP, // Triclinic primitive
        mP, // Monoclinic primitive
        mS, // Monoclinic base-centered
        oP, // Orthorhombic primitive
        oS, // Orthorhombic base-centered
        oI, // Orthorhombic body-centered
        oF, // Orthorhombic face-centered
        tP, // Tetragonal primitive
        tI, // Tetragonal body-centered
        hR, // Rhombohedral
        hP, // Hexagonal primitive
        cP, // Cubic primitive
        cI, // Cubic body-centered
        cF, // Cubic face-centered
    };
    // NOLINTEND(readability-identifier-naming)

    enum class System
    {
        triclinic,
        monoclinic,
        orthorhombic,
        tetragonal,
        rhombohedral,
        hexagonal,
        cubic,
    };

    enum class Centering
    {
        none,
        base,
        body,
        face,
    };

    constexpr Centering centering(Bravais bravais)
    {
        switch (bravais)
        {
            case Bravais::mS:
            case Bravais::oS: return Centering::base;

            case Bravais::oI:
            case Bravais::tI:
            case Bravais::cI: return Centering::body;

            case Bravais::oF:
            case Bravais::cF: return Centering::face;

            default: return Centering::none;
        }
    }

    constexpr System system(Bravais bravais)
    {
        switch (bravais)
        {
            default:
            case Bravais::aP: return System::triclinic;
            case Bravais::mP:
            case Bravais::mS: return System::monoclinic;
            case Bravais::oP:
            case Bravais::oS:
            case Bravais::oI:
            case Bravais::oF: return System::orthorhombic;
            case Bravais::tP:
            case Bravais::tI: return System::tetragonal;
            case Bravais::hR: return System::rhombohedral;
            case Bravais::hP: return System::hexagonal;
            case Bravais::cP:
            case Bravais::cI:
            case Bravais::cF: return System::cubic;
        }
    }

    constexpr bool orthogonal(System system)
    {
        switch (system)
        {
            case System::orthorhombic:
            case System::tetragonal:
            case System::cubic: return true;

            default: return false;
        }
    }

    namespace unit
    {
        constexpr Coordinate x{ 1., 0., 0. };
        constexpr Coordinate y{ 0., 1., 0. };
        constexpr Coordinate z{ 0., 0., 1. };
    }

    namespace site
    {
        constexpr Coordinate origin{ .0, .0, .0 };
        constexpr Coordinate side{ .0, .5, .5 };
        constexpr Coordinate front{ .5, .0, .5 };
        constexpr Coordinate base{ .5, .5, .0 };
        constexpr Coordinate center{ .5, .5, .5 };
        constexpr Coordinate hcp{ 1. / 3., 2. / 3., .5 };
    }

    namespace vectors
    {
        constexpr CoordinateBasis basis{ unit::x, unit::y, unit::z };

        namespace conventional
        {
            //template<Bravais bravais, Angles const&>
            CoordinateBasis triclinic(Angles const&);

            //template<Bravais bravais, Angle>
            CoordinateBasis monoclinic(Angle);

            //template<Bravais bravais, Angle>
            CoordinateBasis rhombohedral(Angle);

            constexpr CoordinateBasis hexagonal{
                Coordinate{ .5, -.5 * std::numbers::sqrt3, .0 },
                Coordinate{ .5, .5 * std::numbers::sqrt3, .0 },
                unit::z,
            };
        }

        namespace to
        {
            constexpr CoordinateBasis primitive(Centering centering)
            {
                switch (centering)
                {
                    default:
                    case Centering::none: return basis;

                    case Centering::base:
                        return {
                            Coordinate{ .5, -.5, .0 },
                            Coordinate{ .5, .5, .0 },
                            unit::z,
                        };
                    case Centering::body:
                        return {
                            Coordinate{ -.5, .5, .5 },
                            Coordinate{ .5, -.5, .5 },
                            Coordinate{ .5, .5, -.5 },
                        };
                    case Centering::face:
                        return {
                            site::side,
                            site::front,
                            site::base,
                        };
                }
            }
        }
    }

    namespace parameters
    {
        constexpr double smallest(Centering centering)
        {
            switch (centering)
            {
                case Centering::body: return 2 * std::numbers::inv_sqrt3;
                case Centering::face: return std::numbers::sqrt2;
                default: return 1.;
            }
        }

        constexpr Coordinate hcp{ 1., 1., 2 * std::numbers::sqrt2* std::numbers::inv_sqrt3 };
    }
}
