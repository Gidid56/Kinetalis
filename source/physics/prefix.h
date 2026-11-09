#pragma once
#include "config.h"
#include "math/functions.h"

#include <array>
#include <concepts>
#include <string_view>
#include <utility>

namespace physics
{
    enum class Prefix : Exponent
    {
        quetta = 30,
        ronna = 27,
        yotta = 24,
        zetta = 21,
        exa = 18,
        peta = 15,
        tera = 12,
        giga = 9,
        mega = 6,
        kilo = 3,
        hecto = 2,
        deca = 1,
        none = 0,
        deci = -1,
        centi = -2,
        milli = -3,
        micro = -6,
        nano = -9,
        pico = -12,
        femto = -15,
        atto = -18,
        zepto = -21,
        yocto = -24,
        ronto = -27,
        quecto = -30,
    };

    namespace prefix
    {
        constexpr long double factor(Prefix prefix)
        {
            return math::power<long double>(10.0, std::to_underlying(prefix));
        }

        using enum Prefix;

        constexpr std::string_view symbol(Prefix prefix)
        {
            switch (prefix)
            {
                case quetta: return "Q";
                case ronna: return "R";
                case yotta: return "Y";
                case zetta: return "Z";
                case exa: return "E";
                case peta: return "P";
                case tera: return "T";
                case giga: return "G";
                case mega: return "M";
                case kilo: return "k";
                case hecto: return "h";
                case deca: return "da";

                case deci: return "d";
                case centi: return "c";
                case milli: return "m";
                case micro: return "\u00B5"; // micro sign
                case nano: return "n";
                case pico: return "p";
                case femto: return "f";
                case atto: return "a";
                case zepto: return "z";
                case yocto: return "y";
                case ronto: return "r";
                case quecto: return "q";

                default: return {};
            }
        }

        constexpr std::string_view ascii_symbol(Prefix prefix)
        {
            switch (prefix)
            {
                case micro: return "u";
                default: return {};
            }
        }

        constexpr std::array values = {
            quetta,
            ronna,
            yotta,
            zetta,
            exa,
            peta,
            tera,
            giga,
            mega,
            kilo,
            hecto,
            deca,
            deci,
            centi,
            milli,
            micro,
            nano,
            pico,
            femto,
            atto,
            zepto,
            yocto,
            ronto,
            quecto,
        };
    }
}
