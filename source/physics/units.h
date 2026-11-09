#pragma once
#include "config.h"
#include "constraints.h"

#include <cstdint>
#include <numbers>
#include <string_view>
#include <utility>

namespace physics
{
    enum class Unit
    {
        none, // No unit.

        second,  // SI base unit of time.
        meter,   // SI base unit of length.
        gram,    // Unit of mass; 1 g = 1e-3 kg.
        ampere,  // SI base unit of electric current.
        kelvin,  // SI base unit of thermodynamic temperature.
        mole,    // SI base unit of amount of substance.
        candela, // SI base unit of luminous intensity.
        radian,  // Plane angle; arc length divided by radius.

        steradian, // Solid angle; subtended area divided by radius squared.

        hertz,          // Frequency; 1 Hz = 1 s^-1.
        newton,         // Force; 1 N = 1 kg*m*s^-2.
        pascal,         // Pressure and stress; 1 Pa = 1 N/m^2.
        joule,          // Energy, work, and heat; 1 J = 1 N*m.
        watt,           // Power and radiant flux; 1 W = 1 J/s.
        coulomb,        // Electric charge; 1 C = 1 A*s.
        volt,           // Electric potential; 1 V = 1 W/A.
        farad,          // Capacitance; 1 F = 1 C/V.
        ohm,            // Electrical resistance; 1 ohm = 1 V/A.
        siemens,        // Electrical conductance; 1 S = 1 A/V.
        weber,          // Magnetic flux; 1 Wb = 1 V*s.
        tesla,          // Magnetic flux density; 1 T = 1 Wb/m^2.
        henry,          // Inductance; 1 H = 1 Wb/A.
        degree_celsius, // Celsius temperature; 0 deg C = 273.15 K.
        lumen,          // Luminous flux; 1 lm = 1 cd*sr.
        lux,            // Illuminance; 1 lx = 1 lm/m^2.
        becquerel,      // Activity of a radionuclide; 1 Bq = 1 s^-1.
        gray,           // Absorbed dose; 1 Gy = 1 J/kg.
        sievert,        // Dose equivalent; 1 Sv = 1 J/kg.
        katal,          // Catalytic activity; 1 kat = 1 mol/s.

        minute, // Time; 1 min = 60 s.
        hour,   // Time; 1 h = 60 min = 3600 s.
        day,    // Time; 1 d = 24 h = 86400 s.

        turn,      // Plane angle of one complete revolution; 1 turn = 2*pi rad.
        degree,    // Plane angle; 1 deg = pi/180 rad.
        arcminute, // Plane angle; 1 arcmin = 1/60 deg.
        arcsecond, // Plane angle; 1 arcsec = 1/60 arcmin.

        are,     // Area; 1 a = 100 m^2.
        hectare, // Area; 1 ha = 100 a = 1e4 m^2.
        liter,   // Volume; 1 L = 1e-3 m^3.
        tonne,   // Mass; 1 t = 1000 kg.
        gal,     // Acceleration; 1 Gal = 1e-2 m/s^2.
        bar,     // Pressure; 1 bar = 1e5 Pa.

        var,     // Reactive power; dimensionally equivalent to watt.
        calorie, // Thermochemical energy unit; 1 cal = 4.184 J exactly.

        angstrom,     // Length; 1 angstrom = 1e-10 m.
        barn,         // Area used for cross sections; 1 b = 1e-28 m^2.
        dalton,       // Mass; 1 Da is 1/12 the mass of a carbon-12 atom.
        electronvolt, // Energy; 1 eV = 1.602176634e-19 J exactly.

        astronomical_unit, // Length; 1 au = 149597870700 m exactly.
        nautical_mile,     // Length; 1 nautical mile = 1852 m exactly.
        knot,              // Speed; 1 kn = 1 nautical mile per hour.

        neper, // Logarithmic ratio unit based on the natural logarithm.
        bel,   // Logarithmic ratio unit based on the base-10 logarithm.

        percent, // Dimensionless ratio; 1 percent = 1e-2.
        ppm,     // Dimensionless ratio; 1 ppm = 1e-6.
    };

    namespace unit::index
    {
        constexpr auto first{ std::to_underlying(Unit::second) };
        constexpr auto last_base{ std::to_underlying(Unit::radian) };
        constexpr auto last{ std::to_underlying(Unit::ppm) };
    }

    namespace unit
    {
        using enum Unit;

        constexpr bool is_valid(Unit unit)
        {
            return std::to_underlying(unit) >= index::first and
                   std::to_underlying(unit) <= index::last;
        }

        constexpr bool is_si_base(Unit unit) { return unit >= second and unit <= candela; }

        constexpr bool is_logarithmic(Unit unit) { return unit == neper or unit == bel; }

        constexpr bool has_affine_scale(Unit unit) { return unit == Unit::degree_celsius; }

        constexpr bool is_dimensionless(Unit unit)
        {
            switch (unit)
            {
                case neper:
                case bel:
                case percent:
                case ppm: return true;

                default: return false;
            }
        }

        constexpr bool is_si_dimensionless(Unit unit)
        {
            switch (unit)
            {
                case radian:
                case steradian:
                case turn:
                case degree:
                case arcminute:
                case arcsecond: return true;

                default: return is_dimensionless(unit);
            }
        }

        constexpr bool is_prefixable(Unit unit)
        {
            switch (unit)
            {
                case minute:
                case hour:
                case day:
                case turn:
                case degree:
                case arcminute:
                case arcsecond:
                case are:
                case hectare:
                case astronomical_unit:
                case nautical_mile:
                case knot:
                case percent:
                case ppm: return false;

                default: return is_valid(unit);
            }
        }

        constexpr long double factor(Unit unit)
        {
            constexpr auto pi{ std::numbers::pi_v<long double> };
            constexpr auto ln10{ std::numbers::ln10_v<long double> };

            switch (unit)
            {
                case gram: return 1E-3L;

                case minute: return 60;
                case hour: return 60 * factor(minute);
                case day: return 24 * factor(hour);

                case turn: return 2 * pi;
                case degree: return 2 * pi / 360;
                case arcminute: return factor(degree) / 60;
                case arcsecond: return factor(arcminute) / 60;

                case are: return 1E2L;
                case hectare: return 1E4L;

                case liter: return 1E-3L;
                case tonne: return 1E3L;
                case gal: return 1E-2L;
                case bar: return 1E5L;

                case var: return 1;
                case calorie: return 4.184L;

                case angstrom: return 1E-10L;
                case barn: return 1E-28L;
                case dalton: return 1.660'539'068'92E-27L;
                case electronvolt: return 1.602'176'634E-19L;

                case astronomical_unit: return 149'597'870'700L;
                case nautical_mile: return 1'852;
                case knot: return factor(nautical_mile) / factor(hour);

                case bel: return ln10 / 2;

                case percent: return 1E-2L;
                case ppm: return 1E-6L;

                default: return 1;
            }
        }

        constexpr long double offset(Unit unit)
        {
            switch (unit)
            {
                case degree_celsius: return 273.15L;

                default: return 0;
            }
        }

        constexpr std::string_view symbol(Unit unit)
        {
            switch (unit)
            {
                case second: return "s";
                case meter: return "m";
                case gram: return "g";
                case ampere: return "A";
                case kelvin: return "K";
                case mole: return "mol";
                case candela: return "cd";

                case radian: return "rad";
                case steradian: return "sr";

                case hertz: return "Hz";
                case newton: return "N";
                case pascal: return "Pa";
                case joule: return "J";
                case watt: return "W";
                case coulomb: return "C";
                case volt: return "V";
                case farad: return "F";
                case ohm: return "\u03A9"; // Greek capital omega
                case siemens: return "S";
                case weber: return "Wb";
                case tesla: return "T";
                case henry: return "H";
                case degree_celsius: return "\u00B0C"; // degree sign
                case lumen: return "lm";
                case lux: return "lx";
                case becquerel: return "Bq";
                case gray: return "Gy";
                case sievert: return "Sv";
                case katal: return "kat";

                case minute: return "min";
                case hour: return "h";
                case day: return "d";

                case turn: return "tr";
                case degree: return "\u00B0";    // degree sign
                case arcminute: return "\u2032"; // prime
                case arcsecond: return "\u2033"; // double prime

                case are: return "a";
                case hectare: return "ha";
                case barn: return "b";

                case liter: return "l";
                case tonne: return "t";
                case gal: return "Gal";
                case bar: return "bar";

                case var: return "var";
                case calorie: return "cal";

                case angstrom: return "\u00C5"; // Latin capital A with ring above
                case dalton: return "Da";
                case electronvolt: return "eV";

                case astronomical_unit: return "au";
                case nautical_mile: return "nmi";
                case knot: return "kn";

                case neper: return "Np";
                case bel: return "B";

                case percent: return "%";
                case ppm: return "ppm";

                default: return {};
            }
        }

        constexpr std::string_view ascii_symbol(Unit unit)
        {
            switch (unit)
            {
                case ohm: return "Ohm";
                case degree_celsius: return "degC";
                case degree: return "deg";
                case arcminute: return "arcmin";
                case arcsecond: return "arcsec";
                case angstrom: return "Angstrom";
                case percent: return "percent";
                default: return {};
            }
        }
    }
}
