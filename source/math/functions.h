#pragma once
#include <cmath>
#include <concepts>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace math
{
    // Axis flags
    // Each bit represents an axis
    enum class Axis : std::uint8_t
    {
        none,
        x = 1 << 0,
        y = 1 << 1,
        z = 1 << 2,
        // Axis of 4th dimension
        w = 1 << 3,
        // Axis of 2 dimensional radius
        r2 = 1 << 6,
        // Axis of 3 dimensional radius
        r3 = 1 << 7,
    };

    enum class Sign
    {
        negative = -1,
        none = 0,
        positive = 1,
    };

    enum class Tolerance
    {
        absolute,
        relative,
    };

    inline namespace concepts
    {
        template <typename T>
        concept Arithmetic = std::is_arithmetic_v<std::remove_cvref_t<T>>;
    }

    inline namespace functions
    {
        constexpr bool is_nan(std::floating_point auto value) { return value != value; }
        constexpr bool is_finite(std::floating_point auto value)
        {
            return value >= std::numeric_limits<decltype(value)>::lowest() and
                   value <= std::numeric_limits<decltype(value)>::max();
        }
        constexpr bool is_zero(Arithmetic auto n) { return n == 0; }
        constexpr bool is_nonzero(Arithmetic auto n) { return n != 0; }
        constexpr bool is_odd(std::integral auto n) { return (n & 1) != 0; }
        constexpr bool is_even(std::integral auto n) { return (n & 1) == 0; }

        template <class T>
        constexpr int binary_sign(T x)
        {
            return x >= T{} ? 1 : -1;
        }
        template <class T>
        constexpr int sign(T x)
        {
            return x > T{} ? 1 : (x < T{} ? -1 : 0);
        }
        template <class T>
        constexpr auto abs(T x)
        {
            return x < T{} ? -x : x;
        }

        constexpr auto identity(auto x) { return x; }
        constexpr auto square(auto x) { return x * x; }
        constexpr auto cube(auto x) { return x * x * x; }

        template <Arithmetic Base, std::integral Exponent>
        constexpr Base power(Base base, Exponent exp)
        {
            if (exp == 1)
                return base;
            else if (exp == 0)
                return Base{ 1 };
            else if constexpr (std::signed_integral<Exponent>)
                if (exp < 0)
                    return Base{ 1 } / power(base, -exp);
            return (is_odd(exp) ? base : Base{ 1 }) * square(power(base, exp / 2));
        }

        template <std::integral auto exp>
        constexpr auto power(auto base)
        {
            if constexpr (exp == 1)
                return base;
            else if constexpr (exp == 0)
                return 1;
            else if constexpr (exp < 0)
                return 1.0 / power<-exp>(base);
            else if constexpr (is_odd(exp))
                return base * square(power<exp / 2>(base));
            else
                return square(power<exp / 2>(base));
        }

        template <Tolerance tolerance, std::floating_point Type>
        constexpr bool is_close(Type lhs, Type rhs, Type epsilon_factor = Type{ 8 })
        {
            if (lhs == rhs)
                return true;

            if (not std::isfinite(lhs) or not std::isfinite(rhs))
                return false;

            const auto difference{ std::abs(lhs - rhs) };
            const auto epsilon{ epsilon_factor * std::numeric_limits<Type>::epsilon() };

            if constexpr (tolerance == Tolerance::absolute)
                return difference <= epsilon;

            else if constexpr (tolerance == Tolerance::relative)
                return difference <= epsilon * std::max(std::abs(lhs), std::abs(rhs));
        }

        template <std::integral To>
        constexpr auto round(std::floating_point auto value)
        {
            if (not std::isfinite(value))
                throw std::domain_error{ "Cannot convert a non-finite value to an integer." };

            const auto rounded{ std::round(value) };

            if (rounded < static_cast<long double>(std::numeric_limits<To>::lowest()) or
                rounded > static_cast<long double>(std::numeric_limits<To>::max()))
                throw std::out_of_range{ "Rounded value is outside the target integer range." };

            return static_cast<To>(rounded);
        }

        constexpr auto
        round_to(std::floating_point auto value, std::floating_point auto precision = 1.0)
        {
            if (not(precision > 0) or not std::isfinite(precision))
                throw std::invalid_argument{ "Rounding precision must be finite and positive." };
            return std::round(value / precision) * precision;
        }

        template <int digits>
        constexpr auto round_to(std::floating_point auto value)
        {
            constexpr double precision{ math::power<-digits>(10.0) };

            return round_to(value, precision);
        }
    }

    template <std::integral auto N>
    requires(N >= 0 and N <= 4)
    constexpr bool is_dim(Axis axis)
    {
        return bool(axis) and (std::to_underlying(axis) >> N) == 0;
    }

    constexpr std::string_view symbol(Sign sign)
    {
        switch (sign)
        {
            case Sign::negative: return "-";
            default:
            case Sign::none: return "";
            case Sign::positive: return "+";
        }
    }

    constexpr std::string_view ordinal_suffix(std::integral auto number)
    {
        switch (abs(number) % 100)
        {
            case 11:
            case 12:
            case 13: return "th";
        }
        switch (abs(number) % 10)
        {
            case 1: return "st";
            case 2: return "nd";
            case 3: return "rd";
        }
        return "th";
    }

    constexpr std::string ordinal(std::integral auto number)
    {
        return std::to_string(number).append(ordinal_suffix(number));
    }
}
