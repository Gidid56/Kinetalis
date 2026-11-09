#pragma once
#include "prefix.h"
#include "quantity.h"
#include "quantities.h"
#include "unit_dimensions.h"

#include <cmath>

namespace physics::inline literals
{
    constexpr auto operator""_number(long double value)
    {
        return Number{ static_cast<Number::value_type>(value) };
    }

    constexpr auto operator""_number(unsigned long long value)
    {
        return Number{ static_cast<Number::value_type>(value) };
    }

    constexpr auto operator""_factor(long double value)
    {
        return Factor{ static_cast<Factor::value_type>(value) };
    }

    constexpr auto operator""_factor(unsigned long long value)
    {
        return Factor{ static_cast<Factor::value_type>(value) };
    }

    constexpr auto operator""_ratio(long double value)
    {
        return Ratio{ static_cast<Ratio::value_type>(value) };
    }

    constexpr auto operator""_ratio(unsigned long long value)
    {
        return Ratio{ static_cast<Ratio::value_type>(value) };
    }

    constexpr auto operator""_fraction(long double value)
    {
        return Fraction{ static_cast<Fraction::value_type>(value) };
    }

    constexpr auto operator""_fraction(unsigned long long value)
    {
        return Fraction{ static_cast<Fraction::value_type>(value) };
    }
}

namespace physics
{
    constexpr Domain default_domain(Unit unit)
    {
        using enum Unit;
        switch (unit)
        {
            case kelvin:
            case degree_celsius:
            case neper:
            case bel: return Domain::non_negative;

            default: return Domain::unrestricted;
        }
    }
    constexpr Reference default_reference(Unit unit)
    {
        using enum Unit;
        switch (unit)
        {
            case neper:
            case bel: return Reference::ratio;

            default: return Reference::zero;
        }
    }
    template <
        Unit unit,
        Prefix prefix = Prefix::none,
        Reference reference = default_reference(unit),
        Domain domain = default_domain(unit),
        std::floating_point Value = Quantity<>::value_type>
    requires(not unit::is_logarithmic(unit) or reference == Reference::ratio)
    constexpr auto quantity_cast(math::Arithmetic auto value)
    {
        using Dimension = decltype(unit::dimensions::get<unit>());

        const auto linear_value{ static_cast<Value>(
            value * prefix::factor(prefix) * unit::factor(unit)) };

        if constexpr (unit::is_logarithmic(unit))
        {
            return Quantity<Dimension, reference, domain, Value>{ static_cast<Value>(
                std::exp(linear_value)) };
        }
        else
        {
            return Quantity<Dimension, reference, domain, Value>{
                linear_value + static_cast<Value>(unit::offset(unit))
            };
        }
    }
}

namespace physics::unit::inline literals::detail
{
    template <Unit unit, Prefix prefix>
    class Wrapper
    {
        using Dimension = decltype(unit::dimensions::get<unit>());

        static constexpr auto reference{ default_reference(unit) };
        static constexpr auto domain{ default_domain(unit) };

        long double value_;

    public:
        constexpr explicit Wrapper(math::Arithmetic auto value)
            : value_{ static_cast<long double>(value) }
        { }

        template <std::floating_point To>
        constexpr operator Quantity<Dimension, reference, domain, To>() const
        {
            return quantity_cast<unit, prefix, reference, domain, To>(value_);
        }

        constexpr auto operator+() const
        {
            return quantity_cast<unit, prefix, reference, domain, Quantity<>::value_type>(value_);
        }

        constexpr auto operator-() const
        {
            return quantity_cast<unit, prefix, reference, domain, Quantity<>::value_type>(-value_);
        }
    };

    template <Unit unit, Prefix prefix>
    constexpr auto literal(math::Arithmetic auto value)
    {
        if constexpr (unit::is_logarithmic(unit) or unit::has_affine_scale(unit))
            return Wrapper<unit, prefix>{ value };
        else
            return quantity_cast<unit, prefix, Reference::zero, Domain::non_negative>(value);
    }
}

namespace physics::unit::inline literals
{
#include "literals.generated.h"
}
