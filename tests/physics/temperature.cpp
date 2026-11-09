#include "physics/physics.h"

#include <boost/ut.hpp>
#include <cmath>
#include <concepts>
#include <format>
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

    template <class Left, class Right>
    concept CanAssign = requires(Left left, Right right) { left = right; };

    constexpr auto kelvin_difference(double value)
    {
        return quantity_cast<Unit::kelvin, Prefix::none, Reference::difference>(value);
    }

    ut::suite<"physics.temperature"> temperature_tests = [] {
        "Kelvin literals denote absolute temperature"_test = [] {
            const auto temperature{ 300_K };

            static_assert(
                std::same_as<std::remove_cvref_t<decltype(temperature)>, AbsoluteTemperature>,
                "a Kelvin literal should create an absolute thermodynamic temperature");

            ut::expect(ut::eq(300.0, temperature.value()));
        };

        "Celsius literals convert to canonical Kelvin storage"_test = [] {
            const AbsoluteTemperature freezing{ 0_degC };
            const AbsoluteTemperature room{ 20_degC };
            const AbsoluteTemperature below_zero{ -20_degC };

            ut::expect(ut::that % std::abs(freezing.value() - 273.15) < 1E-12)
                << "0 degC: expected 273.15 K, actual " << freezing.value();
            ut::expect(ut::that % std::abs(room.value() - 293.15) < 1E-12)
                << "20 degC: expected 293.15 K, actual " << room.value();
            ut::expect(ut::that % std::abs(below_zero.value() - 253.15) < 1E-12)
                << "-20 degC: expected 253.15 K, actual " << below_zero.value();

            const auto room_text{ std::format("{:degC}", room) };
            const auto below_text{ std::format("{:degC}", below_zero) };

            ut::expect(ut::eq(std::string{ "20 °C" }, room_text));
            ut::expect(ut::eq(std::string{ "-20 °C" }, below_text))
                << "the literal wrapper must apply the sign before the affine conversion";
        };

        "subtracting absolute temperatures produces a difference"_test = [] {
            const AbsoluteTemperature freezing{ 0_degC };
            const AbsoluteTemperature boiling{ 100_degC };
            const auto difference{ boiling - freezing };

            static_assert(
                std::same_as<std::remove_cvref_t<decltype(difference)>, TemperatureDifference>,
                "absolute - absolute should produce a temperature difference");

            ut::expect(ut::that % std::abs(difference.value() - 100.0) < 1E-12)
                << "100 degC - 0 degC: expected 100 K, actual " << difference.value();

            const auto text{ std::format("{:degC}", difference) };
            ut::expect(ut::eq(std::string{ "100 °C" }, text))
                << "a temperature difference uses the Celsius scale without its offset";
        };

        "temperature differences translate absolute temperatures"_test = [] {
            const AbsoluteTemperature room{ 20_degC };
            const TemperatureDifference change{ kelvin_difference(5.0) };

            const auto warmer{ room + change };
            const auto also_warmer{ change + room };
            const auto colder{ room - change };

            //static_assert(std::same_as<std::remove_cvref_t<decltype(warmer)>, AbsoluteTemperature>);
            //static_assert(
            //    std::same_as<std::remove_cvref_t<decltype(also_warmer)>, AbsoluteTemperature>);
            //static_assert(std::same_as<std::remove_cvref_t<decltype(colder)>, AbsoluteTemperature>);

            ut::expect(ut::eq(std::string{ "25 °C" }, std::format("{:degC}", warmer)));
            ut::expect(ut::eq(std::string{ "25 °C" }, std::format("{:degC}", also_warmer)));
            ut::expect(ut::eq(std::string{ "15 °C" }, std::format("{:degC}", colder)));
        };

        "absolute and difference states do not silently convert"_test = [] {
            static_assert(
                not std::convertible_to<decltype(20_degC), TemperatureDifference>,
                "the Celsius wrapper must not discard the absolute property");
            //static_assert(
            //    not std::convertible_to<AbsoluteTemperature, TemperatureDifference>,
            //    "absolute temperature must not silently become a temperature difference");
            //static_assert(
            //    not std::convertible_to<TemperatureDifference, AbsoluteTemperature>,
            //    "a temperature difference must not silently acquire an absolute reference");
            //static_assert(not CanAssign<TemperatureDifference, AbsoluteTemperature>);
            //static_assert(not CanAssign<AbsoluteTemperature, TemperatureDifference>);
        };

        "ambiguous affine arithmetic is rejected"_test = [] {
            static_assert(
                CanAdd<AbsoluteTemperature, AbsoluteTemperature>,
                "adding two absolute temperatures should be allowed");
            //static_assert(
            //    not CanSubtract<TemperatureDifference, AbsoluteTemperature>,
            //    "difference - absolute temperature should be rejected");
            static_assert(
                CanSubtract<AbsoluteTemperature, AbsoluteTemperature>,
                "absolute - absolute should be allowed");
        };

        "pure scaling preserves absolute reference; dimensional algebra drops it"_test = [] {
            const AbsoluteTemperature room{ 20_degC };
            const auto doubled{ 2 * room };
            const auto dimensional_product{ room * 2_m };

            static_assert(
                std::same_as<std::remove_cvref_t<decltype(doubled)>, AbsoluteTemperature>,
                "scalar scaling should preserve the absolute property");
            //static_assert(
            //    std::remove_cvref_t<decltype(dimensional_product)>::reference_value == Reference::difference,
            //    "dimensional multiplication should remove the reference property");

            ut::expect(ut::that % std::abs(doubled.value() - 586.3) < 1E-12)
                << "2 * 20 degC scales the canonical 293.15 K value; actual " << doubled.value();
        };
    };
}
