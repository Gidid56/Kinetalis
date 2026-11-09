#include "physics/physics.h"

#include <boost/ut.hpp>
#include <cmath>
#include <concepts>
#include <format>
#include <numbers>
#include <type_traits>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace physics;
    using namespace physics::quantity;
    using namespace physics::unit::literals;

    template <class Left, class Right>
    concept CanAdd = requires(Left left, Right right) { left + right; };

    template <class Left, class Right>
    concept CanSubtract = requires(Left left, Right right) { left - right; };

    ut::suite<"physics.logarithmic"> logarithmic_tests = [] {
        "decibel literals store the underlying linear ratio"_test = [] {
            const auto gain{ +20_dB };
            const auto attenuation{ -20_dB };

            static_assert(
                std::remove_cvref_t<decltype(gain)>::reference_value == Reference::ratio,
                "a dB value should retain logarithmic algebraic state");
            static_assert(
                not std::convertible_to<decltype(20_dB), Factor>,
                "the wrapper must not discard the logarithmic property");

            ut::expect(ut::that % std::abs(gain.value() - 10.0) < 1E-12)
                << "20 dB: expected stored field ratio 10, actual " << gain.value();
            ut::expect(ut::that % std::abs(attenuation.value() - 0.1) < 1E-12)
                << "-20 dB: expected stored field ratio 0.1, actual " << attenuation.value();

            ut::expect(ut::eq(std::string{ "20 dB" }, std::format("{:dB}", gain)));
            ut::expect(ut::eq(std::string{ "-20 dB" }, std::format("{:dB}", attenuation)));
        };

        "neper literals use the natural-logarithm convention"_test = [] {
            const auto value{ +1_Np };

            ut::expect(ut::that % std::abs(value.value() - std::numbers::e) < 1E-12)
                << "1 Np: expected stored ratio e, actual " << value.value();

            ut::expect(ut::eq(std::string{ "1 Np" }, std::format("{:Np}", value)));
        };

        "scaling changes the underlying linear ratio"_test = [] {
            const auto gain{ +20_dB };
            const auto doubled{ gain * 2.0 };
            const auto halved{ gain / Factor{ 2.0 } };

            static_assert(
                std::remove_cvref_t<decltype(doubled)>::reference_value ==
                Reference::ratio);
            //static_assert(
            //    std::remove_cvref_t<decltype(halved)>::constraints_value.reference ==
            //    Reference::ratio);

            ut::expect(ut::that % std::abs(doubled.value() - 20.0) < 1E-12)
                << "doubling the stored ratio 10: actual " << doubled.value();
            ut::expect(ut::that % std::abs(halved.value() - 5.0) < 1E-12)
                << "halving the stored ratio 10: actual " << halved.value();

            ut::expect(ut::eq(std::string{ "26.020600 dB" }, std::format("{:dB;.6f}", doubled)));
        };

        "roots of logarithmic quantities are meaningful"_test = [] {
            const auto gain{ +20_dB };
            const auto root{ sqrt(gain) };

            static_assert(
                std::remove_cvref_t<decltype(root)>::reference_value == Reference::ratio,
                "root should preserve logarithmic state");

            const auto expected{ std::sqrt(10.0) };
            ut::expect(ut::that % std::abs(root.value() - expected) < 1E-12)
                << "sqrt of stored ratio 10: expected " << expected << ", actual " << root.value();

            ut::expect(ut::eq(std::string{ "10 dB" }, std::format("{:dB}", root)));
        };

        "dimensional multiplication removes logarithmic state"_test = [] {
            const auto result{ (+20_dB) * 2_m };

            static_assert(
                Like<decltype(result), Length>,
                "a logarithmic factor times a length should become an ordinary length");

            ut::expect(ut::that % std::abs(result.value() - 20.0) < 1E-12)
                << "ratio 10 * 2 m: actual " << result.value();
        };
    };
}
