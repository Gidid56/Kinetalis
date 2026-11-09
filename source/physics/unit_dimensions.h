#pragma once
#include "dimensions.h"
#include "units.h"

#include <stdexcept>

namespace physics::unit::dimensions
{
    template <Unit unit>
    consteval auto get()
    {
        if constexpr (unit == Unit::none)
            return Dimensionless{};

        // Base units
        else if constexpr (unit == Unit::second)
            return dimension::Time{};
        else if constexpr (unit == Unit::meter)
            return dimension::Length{};
        else if constexpr (unit == Unit::gram)
            return dimension::Mass{};
        else if constexpr (unit == Unit::ampere)
            return dimension::ElectricCurrent{};
        else if constexpr (unit == Unit::kelvin)
            return dimension::Temperature{};
        else if constexpr (unit == Unit::mole)
            return dimension::AmountOfSubstance{};
        else if constexpr (unit == Unit::candela)
            return dimension::LuminousIntensity{};
        else if constexpr (unit == Unit::radian)
            return dimension::Angle{};

        // Derived SI units
        else if constexpr (unit == Unit::steradian)
            return dimension::SolidAngle{};
        else if constexpr (unit == Unit::hertz)
            return dimension::Frequency{};
        else if constexpr (unit == Unit::newton)
            return dimension::Force{};
        else if constexpr (unit == Unit::pascal)
            return dimension::Pressure{};
        else if constexpr (unit == Unit::joule)
            return dimension::Energy{};
        else if constexpr (unit == Unit::watt)
            return dimension::Power{};
        else if constexpr (unit == Unit::coulomb)
            return dimension::ElectricCharge{};
        else if constexpr (unit == Unit::volt)
            return dimension::ElectricPotential{};
        else if constexpr (unit == Unit::farad)
            return dimension::Capacitance{};
        else if constexpr (unit == Unit::ohm)
            return dimension::ElectricalResistance{};
        else if constexpr (unit == Unit::siemens)
            return dimension::ElectricalConductance{};
        else if constexpr (unit == Unit::weber)
            return dimension::MagneticFlux{};
        else if constexpr (unit == Unit::tesla)
            return dimension::MagneticFluxDensity{};
        else if constexpr (unit == Unit::henry)
            return dimension::Inductance{};
        else if constexpr (unit == Unit::degree_celsius)
            return dimension::Temperature{};
        else if constexpr (unit == Unit::lumen)
            return dimension::LuminousFlux{};
        else if constexpr (unit == Unit::lux)
            return dimension::Illuminance{};
        else if constexpr (unit == Unit::becquerel)
            return dimension::Frequency{};
        else if constexpr (unit == Unit::gray)
            return dimension::SpecificEnergy{};
        else if constexpr (unit == Unit::sievert)
            return dimension::SpecificEnergy{};
        else if constexpr (unit == Unit::katal)
            return dimension::CatalyticActivity{};

        // Time
        else if constexpr (unit == Unit::minute)
            return dimension::Time{};
        else if constexpr (unit == Unit::hour)
            return dimension::Time{};
        else if constexpr (unit == Unit::day)
            return dimension::Time{};

        // Angle
        else if constexpr (unit == Unit::degree)
            return dimension::Angle{};
        else if constexpr (unit == Unit::turn)
            return dimension::Angle{};
        else if constexpr (unit == Unit::arcminute)
            return dimension::Angle{};
        else if constexpr (unit == Unit::arcsecond)
            return dimension::Angle{};

        // Length
        else if constexpr (unit == Unit::angstrom)
            return dimension::Length{};
        else if constexpr (unit == Unit::astronomical_unit)
            return dimension::Length{};
        else if constexpr (unit == Unit::nautical_mile)
            return dimension::Length{};

        // Area
        else if constexpr (unit == Unit::are)
            return dimension::Area{};
        else if constexpr (unit == Unit::hectare)
            return dimension::Area{};
        else if constexpr (unit == Unit::barn)
            return dimension::Area{};

        // Volume
        else if constexpr (unit == Unit::liter)
            return dimension::Volume{};

        // Mass
        else if constexpr (unit == Unit::tonne)
            return dimension::Mass{};
        else if constexpr (unit == Unit::dalton)
            return dimension::Mass{};

        // Velocity and acceleration
        else if constexpr (unit == Unit::knot)
            return dimension::Velocity{};
        else if constexpr (unit == Unit::gal)
            return dimension::Acceleration{};

        // Mechanical
        else if constexpr (unit == Unit::bar)
            return dimension::Pressure{};
        else if constexpr (unit == Unit::calorie)
            return dimension::Energy{};
        else if constexpr (unit == Unit::electronvolt)
            return dimension::Energy{};
        else if constexpr (unit == Unit::var)
            return dimension::Power{};

        // Dimensionless
        else if constexpr (unit == Unit::neper)
            return Dimensionless{};
        else if constexpr (unit == Unit::bel)
            return Dimensionless{};
        else if constexpr (unit == Unit::percent)
            return Dimensionless{};
        else if constexpr (unit == Unit::ppm)
            return Dimensionless{};

        else
            static_assert(false, "invalid unit");
    }
    constexpr Dimensions<>::Representation representation(Unit unit)
    {
        switch (unit)
        {
            case Unit::none: return get<Unit::none>().representation();

            case Unit::second: return get<Unit::second>().representation();
            case Unit::meter: return get<Unit::meter>().representation();
            case Unit::gram: return get<Unit::gram>().representation();
            case Unit::ampere: return get<Unit::ampere>().representation();
            case Unit::mole: return get<Unit::mole>().representation();
            case Unit::kelvin: return get<Unit::kelvin>().representation();
            case Unit::candela: return get<Unit::candela>().representation();

            case Unit::radian: return get<Unit::radian>().representation();
            case Unit::steradian: return get<Unit::steradian>().representation();

            case Unit::hertz: return get<Unit::hertz>().representation();
            case Unit::newton: return get<Unit::newton>().representation();
            case Unit::pascal: return get<Unit::pascal>().representation();
            case Unit::joule: return get<Unit::joule>().representation();
            case Unit::watt: return get<Unit::watt>().representation();
            case Unit::coulomb: return get<Unit::coulomb>().representation();
            case Unit::volt: return get<Unit::volt>().representation();
            case Unit::farad: return get<Unit::farad>().representation();
            case Unit::ohm: return get<Unit::ohm>().representation();
            case Unit::siemens: return get<Unit::siemens>().representation();
            case Unit::weber: return get<Unit::weber>().representation();
            case Unit::tesla: return get<Unit::tesla>().representation();
            case Unit::henry: return get<Unit::henry>().representation();
            case Unit::degree_celsius: return get<Unit::degree_celsius>().representation();
            case Unit::lumen: return get<Unit::lumen>().representation();
            case Unit::lux: return get<Unit::lux>().representation();
            case Unit::becquerel: return get<Unit::becquerel>().representation();
            case Unit::gray: return get<Unit::gray>().representation();
            case Unit::sievert: return get<Unit::sievert>().representation();
            case Unit::katal: return get<Unit::katal>().representation();

            case Unit::minute: return get<Unit::minute>().representation();
            case Unit::hour: return get<Unit::hour>().representation();
            case Unit::day: return get<Unit::day>().representation();

            case Unit::turn: return get<Unit::turn>().representation();
            case Unit::degree: return get<Unit::degree>().representation();
            case Unit::arcminute: return get<Unit::arcminute>().representation();
            case Unit::arcsecond: return get<Unit::arcsecond>().representation();

            case Unit::are: return get<Unit::are>().representation();
            case Unit::hectare: return get<Unit::hectare>().representation();
            case Unit::barn: return get<Unit::barn>().representation();

            case Unit::liter: return get<Unit::liter>().representation();
            case Unit::tonne: return get<Unit::tonne>().representation();
            case Unit::gal: return get<Unit::gal>().representation();
            case Unit::bar: return get<Unit::bar>().representation();

            case Unit::var: return get<Unit::var>().representation();
            case Unit::calorie: return get<Unit::calorie>().representation();

            case Unit::angstrom: return get<Unit::angstrom>().representation();
            case Unit::dalton: return get<Unit::dalton>().representation();
            case Unit::electronvolt: return get<Unit::electronvolt>().representation();

            case Unit::astronomical_unit: return get<Unit::astronomical_unit>().representation();
            case Unit::nautical_mile: return get<Unit::nautical_mile>().representation();
            case Unit::knot: return get<Unit::knot>().representation();

            case Unit::neper: return get<Unit::neper>().representation();
            case Unit::bel: return get<Unit::bel>().representation();

            case Unit::percent: return get<Unit::percent>().representation();
            case Unit::ppm: return get<Unit::ppm>().representation();

            default: throw std::runtime_error{ "invalid unit" };
        }
    }

    constexpr bool contains(
        Dimensions<>::Representation const& representation,
        Dimensions<>::Representation const& required)
    {
        for (std::size_t i{}; i < representation.size(); ++i)
        {
            if (required[i] > 0 and required[i] > representation[i])
                return false;

            if (required[i] < 0 and required[i] < representation[i])
                return false;
        }

        return true;
    }

    constexpr bool contains(Dimensions<>::Representation const& representation, Unit unit)
    {
        switch (unit)
        {
            case Unit::none: return false;

            // Dimensionless units must match exactly; otherwise their zero
            // dimensions would be "contained" in every representation.
            case Unit::neper:
            case Unit::bel:
            case Unit::percent:
            case Unit::ppm: return representation == Dimensionless::representation();

            // Mechanical units may be useful even when another dimension
            // must subsequently compensate part of their representation.
            case Unit::newton:
            case Unit::pascal:
            case Unit::bar:
            case Unit::watt:
            case Unit::var: return contains(representation, dimension::Mass::representation());

            case Unit::joule:
            case Unit::electronvolt:
            case Unit::calorie:
                return contains(representation, dimension::Mass::representation()) or
                       contains(representation, dimension::SpecificEnergy::representation());

            // Electrical units use the characteristic current exponent to
            // permit compound representations such as V/m and F/m.
            case Unit::volt:
                return contains(
                    representation,
                    dimension::ElectricCurrent{}.power<-1>().representation());

            case Unit::farad:
            case Unit::siemens:
                return contains(
                    representation,
                    dimension::ElectricCurrent{}.power<2>().representation());

            case Unit::ohm:
            case Unit::henry:
                return contains(
                    representation,
                    dimension::ElectricCurrent{}.power<-2>().representation());

            default: return contains(representation, dimensions::representation(unit));
        }
    }
}
