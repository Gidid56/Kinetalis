#include "physics/physics.h"

#include <boost/ut.hpp>

#include <cmath>
#include <concepts>
#include <limits>
#include <numbers>
#include <type_traits>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace physics;
    using namespace physics::quantity;
    using namespace physics::unit::literals;

    template <class T>
    concept CanSquareRoot = requires(T value) { physics::sqrt(value); };

    template <class T>
    concept CanRound = requires(T value) { physics::round(value); };

    template <class T>
    concept CanExp = requires(T value) { physics::exp(value); };

    template <class T>
    concept CanSin = requires(T value) { physics::sin(value); };

    ut::suite<"physics.functions"> function_tests = [] {
        "roots preserve dimensional meaning"_test = [] {
            const Area area{ 9.0 };
            const Volume volume{ 27.0 };
            const auto from_area{ sqrt(area) };
            const auto from_volume{ cbrt(volume) };

            static_assert(
                Like<decltype(from_area), Length>,
                "sqrt(area) should be length");
            static_assert(
                Like<decltype(from_volume), Length>,
                "cbrt(volume) should be length");
            static_assert(
                not CanSquareRoot<Length>,
                "sqrt(length) cannot be represented with integral dimension exponents");

            ut::expect(ut::eq(3.0, from_area.value()));
            ut::expect(ut::eq(3.0, from_volume.value()));
        };

        "exp works on an ordinary dimensionless quantity"_test = [] {
            const auto result{ exp(Factor{ 1.0 }) };

            static_assert(
                Like<decltype(result), Factor>);

            ut::expect(ut::that % std::abs(result.value() - std::numbers::e) < 1E-12)
                << "exp(1): expected " << std::numbers::e
                << ", actual " << result.value();
        };

        "rounding is reserved for ordinary quantities"_test = [] {
            ut::expect(ut::eq(2.0, round(Length{ 1.6 }).value()));
            ut::expect(ut::eq(1.0, floor(Length{ 1.6 }).value()));
            ut::expect(ut::eq(2.0, ceil(Length{ 1.2 }).value()));
        };

        "trigonometric functions take angles and return dimensionless quantities"_test = [] {
            const auto sine{ sin(90_deg) };
            const auto cosine{ cos(180_deg) };
            const auto tangent{ tan(45_deg) };

            static_assert(
                Like<decltype(sine), SignedNumber>,
                "sin(angle) should remain in the Quantity framework");

            ut::expect(ut::that % std::abs(sine.value() - 1.0) < 1E-12)
                << "sin(90 deg): actual " << sine.value();
            ut::expect(ut::that % std::abs(cosine.value() + 1.0) < 1E-12)
                << "cos(180 deg): actual " << cosine.value();
            ut::expect(ut::that % std::abs(tangent.value() - 1.0) < 1E-12)
                << "tan(45 deg): actual " << tangent.value();

            static_assert(
                not CanSin<Factor>,
                "sin should require a physical angle");
        };

        "inverse trigonometry and atan2 return angles"_test = [] {
            const auto angle{ asin(Factor{ 1.0 }) };
            const auto quadrant{ physics::atan2(1_m - 0_m, -1_m - 0_m) };

            static_assert(
                Like<decltype(angle), PlaneAngle>);
            static_assert(
                Like<decltype(quadrant), PlaneAngle>);

            const auto asin_expected{ std::numbers::pi / 2 };
            const auto atan_expected{ 3 * std::numbers::pi / 4 };

            ut::expect(ut::that % std::abs(angle.value() - asin_expected) < 1E-12)
                << "asin(1): expected " << asin_expected
                << ", actual " << angle.value();

            ut::expect(ut::that % std::abs(quadrant.value() - atan_expected) < 1E-12)
                << "atan2(1 m, -1 m): expected " << atan_expected
                << ", actual " << quadrant.value();
        };

        "finiteness is independent of quantity properties"_test = [] {
            ut::expect(ut::that % isfinite(1_m) == true);
            ut::expect(
                ut::that %
                isfinite(Length{ std::numeric_limits<double>::infinity() }) ==
                false);

            const AbsoluteTemperature temperature{ 20_degC };
            const auto logarithmic{ +20_dB };

            ut::expect(ut::that % isfinite(temperature) == true);
            ut::expect(ut::that % isfinite(logarithmic) == true);
        };
    };
}
