#pragma once
#include "config.h"

#include <array>
#include <concepts>
#include <cstddef>
#include <type_traits>

namespace physics
{
    template <
        Exponent T = 0, // time
        Exponent L = 0, // length
        Exponent M = 0, // mass
        Exponent I = 0, // electric current
        Exponent Θ = 0, // thermodynamic temperature
        Exponent N = 0, // amount of substance
        Exponent J = 0, // luminous intensity
        Exponent A = 0> // plane angle
    class Dimensions
    {
    public:
        static constexpr Exponent time_exponent{ T };
        static constexpr Exponent length_exponent{ L };
        static constexpr Exponent mass_exponent{ M };
        static constexpr Exponent current_exponent{ I };
        static constexpr Exponent temperature_exponent{ Θ };
        static constexpr Exponent substance_exponent{ N };
        static constexpr Exponent luminous_intensity_exponent{ J };
        static constexpr Exponent angle_exponent{ A };

        static constexpr std::size_t size{ 8 };
        using Representation = std::array<Exponent, size>;

        using dimension_type = void;

        consteval auto operator-() const { return Dimensions<-T, -L, -M, -I, -Θ, -N, -J, -A>{}; }

        template <
            Exponent T2,
            Exponent L2,
            Exponent M2,
            Exponent I2,
            Exponent Θ2,
            Exponent N2,
            Exponent J2,
            Exponent A2>
        consteval auto operator+(Dimensions<T2, L2, M2, I2, Θ2, N2, J2, A2>) const
        {
            return Dimensions<T + T2, L + L2, M + M2, I + I2, Θ + Θ2, N + N2, J + J2, A + A2>{};
        }

        template <
            Exponent T2,
            Exponent L2,
            Exponent M2,
            Exponent I2,
            Exponent Θ2,
            Exponent N2,
            Exponent J2,
            Exponent A2>
        consteval auto operator-(Dimensions<T2, L2, M2, I2, Θ2, N2, J2, A2>) const
        {
            return Dimensions<T - T2, L - L2, M - M2, I - I2, Θ - Θ2, N - N2, J - J2, A - A2>{};
        }

        template <Exponent exponent>
        consteval auto power() const
        {
            return Dimensions<
                T * exponent,
                L * exponent,
                M * exponent,
                I * exponent,
                Θ * exponent,
                N * exponent,
                J * exponent,
                A * exponent>{};
        }

        template <Exponent divisor>
        static consteval bool is_multiple()
        {
            if constexpr (divisor == 0)
                return false;

            return T % divisor == 0 and L % divisor == 0 and M % divisor == 0 and
                   I % divisor == 0 and Θ % divisor == 0 and N % divisor == 0 and
                   J % divisor == 0 and A % divisor == 0;
        }

        template <Exponent divisor>
        requires(divisor > 0 and is_multiple<divisor>())
        consteval auto root() const
        {
            return Dimensions<
                T / divisor,
                L / divisor,
                M / divisor,
                I / divisor,
                Θ / divisor,
                N / divisor,
                J / divisor,
                A / divisor>{};
        }

        static constexpr Representation representation()
        {
            return {
                T,
                L,
                M,
                I,
                Θ,
                N,
                J,
                A,
            };
        }
    };

    using Dimensionless = Dimensions<>;

    namespace dimension::inline base
    {
        using Time = Dimensions<1>;
        using Length = Dimensions<0, 1>;
        using Mass = Dimensions<0, 0, 1>;
        using ElectricCurrent = Dimensions<0, 0, 0, 1>;
        using Temperature = Dimensions<0, 0, 0, 0, 1>;
        using AmountOfSubstance = Dimensions<0, 0, 0, 0, 0, 1>;
        using LuminousIntensity = Dimensions<0, 0, 0, 0, 0, 0, 1>;
        using Angle = Dimensions<0, 0, 0, 0, 0, 0, 0, 1>;
    }

    template <typename T>
    concept IsDimension = requires { typename std::remove_cvref_t<T>::dimension_type; };

    template <typename T>
    concept IsDimensionless =
        IsDimension<T> and std::same_as<std::remove_cvref_t<T>, Dimensionless>;

    template <auto root, typename T>
    concept IsRootable = IsDimension<T> and T::template is_multiple<root>() and root > 0;

    namespace dimension
    {
        template <typename D1, typename D2>
        concept Same = IsDimension<D1> and IsDimension<D2> and
                       std::same_as<std::remove_cvref_t<D1>, std::remove_cvref_t<D2>>;

        namespace detail
        {
            template <typename>
            struct SIDimensions;

            template <
                Exponent T,
                Exponent L,
                Exponent M,
                Exponent I,
                Exponent Θ,
                Exponent N,
                Exponent J,
                Exponent A>
            struct SIDimensions<Dimensions<T, L, M, I, Θ, N, J, A>>
            {
                using type = Dimensions<T, L, M, I, Θ, N, J>;
            };
        }

        template <IsDimension D>
        using SI_type = typename detail::SIDimensions<std::remove_cvref_t<D>>::type;

        template <typename D1, typename D2>
        concept SameSI =
            IsDimension<D1> and IsDimension<D2> and std::same_as<SI_type<D1>, SI_type<D2>>;

        template <typename D1, typename D2>
        concept Equivalent = Same<D1, D2> or (config::allow_angle_decay and SameSI<D1, D2>);

        template <typename From, typename To>
        concept Convertible = Same<From, To> or (config::allow_angle_decay and SameSI<From, To> and
                                                    std::remove_cvref_t<To>::angle_exponent == 0);
    }
}
