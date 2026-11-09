#include "physics/dimensions.h"
#include "physics/exponent.h"

#include <boost/ut.hpp>

#include <array>
#include <charconv>
#include <concepts>
#include <limits>
#include <string>
#include <type_traits>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace physics;

    template <Exponent Root, class Dimension>
    concept CanTakeRoot = requires(Dimension dimension) {
        dimension.template root<Root>();
    };

    ut::suite<"physics.dimensions"> dimension_tests = [] {
        "the basis tracks seven SI dimensions plus plane angle"_test = [] {
            using Example = Dimensions<1, 2, 3, 4, 5, 6, 7, 8>;

            static_assert(Example::size == 8);
            static_assert(
                Example::representation() ==
                std::array<Exponent, 8>{ 1, 2, 3, 4, 5, 6, 7, 8 });
            static_assert(dimension::Angle::angle_exponent == 1);
            static_assert(dimension::SolidAngle::angle_exponent == 2);
        };

        "named dimensions follow physical algebra"_test = [] {
            static_assert(
                std::same_as<
                    decltype(dimension::Length{} - dimension::Time{}),
                    dimension::Velocity>);
            static_assert(
                std::same_as<
                    decltype(dimension::Mass{} + dimension::Acceleration{}),
                    dimension::Force>);
            static_assert(
                std::same_as<
                    decltype(dimension::Force{} + dimension::Length{}),
                    dimension::Energy>);
            static_assert(
                std::same_as<
                    decltype(dimension::Energy{} - dimension::Angle{}),
                    dimension::Torque>);
            static_assert(
                std::same_as<
                    decltype(dimension::Angle{} - dimension::Time{}),
                    dimension::AngularVelocity>);
        };

        "powers and roots act on dimension exponents"_test = [] {
            static_assert(
                std::same_as<
                    decltype(dimension::Length{}.power<2>()),
                    dimension::Area>);
            static_assert(
                std::same_as<
                    decltype(dimension::Length{}.power<3>()),
                    dimension::Volume>);
            static_assert(CanTakeRoot<2, dimension::Area>);
            static_assert(CanTakeRoot<3, dimension::Volume>);
            static_assert(not CanTakeRoot<2, dimension::Length>);
            static_assert(
                std::same_as<
                    decltype(dimension::Area{}.root<2>()),
                    dimension::Length>);
        };

        "decimal exponent text round-trips representative values"_test = [] {
            constexpr std::array values{
                std::numeric_limits<Exponent>::min(),
                Exponent{ -120 },
                Exponent{ -1 },
                Exponent{},
                Exponent{ 1 },
                Exponent{ 120 },
                std::numeric_limits<Exponent>::max(),
            };

            for (const auto value : values)
            {
                const auto text{ exponent::to_string(value) };

                Exponent parsed{};
                const auto [end, error]{
                    std::from_chars(text.data(), text.data() + text.size(), parsed)
                };

                ut::expect(ut::that % static_cast<int>(error) == static_cast<int>(std::errc{}))
                    << "from_chars failed for '" << text << "'";
                ut::expect(ut::eq(text.data() + text.size(), end))
                    << "parser should consume the complete exponent text: " << text;
                ut::expect(ut::eq(value, parsed))
                    << "round-trip text: " << text;
            }
        };

        "superscript exponent text is readable"_test = [] {
            ut::expect(ut::eq(std::string{ "⁰" }, exponent::to_superscript(0)));
            ut::expect(ut::eq(std::string{ "¹²" }, exponent::to_superscript(12)));
            ut::expect(ut::eq(std::string{ "⁻¹²⁰" }, exponent::to_superscript(-120)));
        };
    };
}
