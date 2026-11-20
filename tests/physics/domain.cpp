#include "physics/physics.h"

#include <boost/ut.hpp>
#include <cmath>
#include <concepts>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace physics;
    using namespace physics::quantity;
    using namespace physics::unit::literals;

    template <typename T, Domain expected>
    constexpr bool HasDomain = std::remove_cvref_t<T>::domain_value == expected;

    ut::suite<"physics.domain"> domain_tests = [] {
        "scalar arithmetic drops restrictions in both operand orders"_test = [] {
            const Distance distance{ 2_m };
            const Fraction fraction{ 0.25 };

            const auto negative_left{ -3 * distance };
            const auto negative_right{ distance * -3 };
            static_assert(HasDomain<decltype(negative_left), Domain::unrestricted>);
            static_assert(HasDomain<decltype(negative_right), Domain::unrestricted>);
            ut::expect(ut::eq(negative_left.value(), -6.0));
            ut::expect(ut::eq(negative_right.value(), -6.0));

            static_assert(HasDomain<decltype(3 * distance), Domain::unrestricted>);
            static_assert(HasDomain<decltype(distance / 2), Domain::unrestricted>);
            static_assert(HasDomain<decltype(2 / distance), Domain::unrestricted>);
            static_assert(HasDomain<decltype(-2.0 - fraction), Domain::unrestricted>);
            static_assert(HasDomain<decltype(-2.0 + fraction), Domain::unrestricted>);

            ut::expect(ut::eq((-2.0 - fraction).value(), -2.25));
            ut::expect(ut::eq((2.0 / distance).value(), 1.0));
        };

        "quantity arithmetic resets restrictions on addition subtraction multiplication and division"_test = [] {
            const Distance distance{ 2_m };
            const Fraction fraction{ 0.25 };

            static_assert(HasDomain<decltype(distance + distance), Domain::unrestricted>);
            static_assert(HasDomain<decltype(distance - distance), Domain::unrestricted>);
            static_assert(HasDomain<decltype(distance * distance), Domain::unrestricted>);
            static_assert(HasDomain<decltype(distance / distance), Domain::unrestricted>);
            static_assert(HasDomain<decltype(fraction * fraction), Domain::unrestricted>);
            static_assert(HasDomain<decltype(fraction / fraction), Domain::unrestricted>);

            ut::expect(ut::eq((distance - 3_m).value(), -1.0));
            ut::expect(ut::eq((fraction + fraction).value(), 0.5));
            ut::expect(ut::eq((fraction * fraction).value(), 0.0625));
        };

        "negation and dimensionless scalar arithmetic return unrestricted results"_test = [] {
            const Distance distance{ 2_m };
            const Fraction fraction{ 0.25 };

            static_assert(HasDomain<decltype(-distance), Domain::unrestricted>);
            static_assert(HasDomain<decltype(fraction - 1), Domain::unrestricted>);
            static_assert(HasDomain<decltype(1 - fraction), Domain::unrestricted>);
            static_assert(HasDomain<decltype(fraction + 1), Domain::unrestricted>);
            static_assert(HasDomain<decltype(fraction * -2), Domain::unrestricted>);
            ut::expect(ut::eq((-distance).value(), -2.0));
            ut::expect(ut::eq((fraction - 1).value(), -0.75));
        };

        "mathematically bounded functions retain only guaranteed restrictions"_test = [] {
            const auto magnitude{ physics::abs(-2_m) };
            const auto root{ physics::sqrt(Area{ 9 }) };
            const auto cube_root{ physics::cbrt(Volume{ 27 }) };
            const auto exponential{ physics::exp(Factor{ -1 }) };

            static_assert(HasDomain<decltype(magnitude), Domain::non_negative>);
            static_assert(HasDomain<decltype(root), Domain::non_negative>);
            static_assert(HasDomain<decltype(cube_root), Domain::non_negative>);
            static_assert(HasDomain<decltype(exponential), Domain::non_negative>);
            ut::expect(ut::eq(magnitude.value(), 2.0));
            ut::expect(ut::eq(root.value(), 3.0));
            ut::expect(ut::that % std::abs(cube_root.value() - 3.0) < 1E-12);

            const auto signed_cube_root{ physics::cbrt(Quantity<dimension::Volume>{ -8 }) };
            static_assert(HasDomain<decltype(signed_cube_root), Domain::unrestricted>);
            ut::expect(ut::eq(signed_cube_root.value(), -2.0));

            const auto rounded{ physics::round(Fraction{ 0.3 }) };
            const auto floored{ physics::floor(Fraction{ 0.7 }) };
            const auto ceiled{ physics::ceil(Fraction{ 0.1 }) };
            static_assert(HasDomain<decltype(rounded), Domain::unit_interval>);
            static_assert(HasDomain<decltype(floored), Domain::unit_interval>);
            static_assert(HasDomain<decltype(ceiled), Domain::unit_interval>);
            ut::expect(ut::eq(rounded.value(), 0.0));
            ut::expect(ut::eq(floored.value(), 0.0));
            ut::expect(ut::eq(ceiled.value(), 1.0));
        };

        "construction and arithmetic are unchecked and nonthrowing"_test = [] {
            static_assert(noexcept(Distance{ -1.0 }));
            static_assert(noexcept(Fraction{ 1.5 }));
            static_assert(noexcept(std::declval<Distance&>() += std::declval<Distance>()));
            static_assert(noexcept(std::declval<Distance&>() *= -1));
            static_assert(noexcept(std::declval<Fraction&>() /= 2));
            static_assert(noexcept(std::declval<Distance const&>() + std::declval<Distance>()));
            static_assert(noexcept(std::declval<Distance const&>().template to<float>()));
            static_assert(not noexcept(std::declval<Distance const&>().validate()));
            static_assert(Distance{ -1.0 }.value() == -1.0);

            Distance distance{ 2_m };
            distance *= -1;
            ut::expect(ut::eq(distance.value(), -2.0));

            Fraction fraction{ 0.75 };
            fraction += Fraction{ 0.5 };
            ut::expect(ut::eq(fraction.value(), 1.25));

            fraction = Fraction{ -0.5 };
            ut::expect(ut::eq(fraction.value(), -0.5));

            ut::expect(ut::eq(Distance{ -1.0 }.to<float>().value(), -1.0f));
        };

        "domain validation is explicit through validate and ensure"_test = [] {
            ut::expect(ut::throws<std::domain_error>([] { Distance{ -1.0 }.validate(); }));
            ut::expect(ut::throws<std::domain_error>([] { Fraction{ 1.5 }.validate(); }));
            ut::expect(ut::throws<std::domain_error>([] {
                Fraction{ std::numeric_limits<double>::quiet_NaN() }.validate();
            }));

            Distance{ 2.0 }.validate();
            Fraction{ 0.25 }.validate();

            ut::expect(ut::eq(ensure_non_negative(-(-2_m)).value(), 2.0));
            ut::expect(ut::eq(ensure_unit_interval(Factor{ 0.5 }).value(), 0.5));
            ut::expect(ut::throws<std::domain_error>([] {
                (void)ensure_non_negative(-2_m);
            }));
            ut::expect(ut::throws<std::domain_error>([] {
                (void)ensure_unit_interval(Fraction{ 1.5 });
            }));
        };
    };
}
