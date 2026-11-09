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

    ut::suite<"physics.arithmetic"> arithmetic_tests = [] {
        "a trip calculation works with familiar units"_test = [] {
            const auto distance{ 120_km };
            const auto duration{ 2_h };
            const auto speed{ distance / duration };

            static_assert(
                Like<decltype(speed), Velocity>,
                "distance / time should produce a velocity quantity");

            constexpr auto expected{ 120'000.0 / 7'200.0 };
            const auto error{ std::abs(speed.value() - expected) };

            ut::expect(ut::that % error < 1E-12)
                << "120 km / 2 h: expected " << expected
                << " m/s, actual " << speed.value() << " m/s";

            const auto formatted{ std::format("{:km/h}", speed) };
            ut::expect(ut::eq(std::string{ "60 km/h" }, formatted))
                << "120 km / 2 h should format naturally";
        };

        "different dimensions cannot accidentally be added"_test = [] {
            static_assert(
                not CanAdd<Length, Time>,
                "a user must not be able to add a length to a time");
            static_assert(
                not CanSubtract<Energy, Length>,
                "a user must not be able to subtract a length from an energy");
        };

        "ordinary dimensional arithmetic produces useful quantities"_test = [] {
            const auto acceleration{ 20_m / (2_s * 2_s) };
            const auto force{ 3_kg * acceleration };
            const auto energy{ force * 4_m };

            static_assert(
                Like<decltype(acceleration), Acceleration>,
                "length / time^2 should be acceleration");
            static_assert(
                Like<decltype(force), Force>,
                "mass * acceleration should be force");
            static_assert(
                Like<decltype(energy), Energy>,
                "force * length should be energy");

            ut::expect(ut::eq(5.0, acceleration.value()))
                << "20 m / (2 s * 2 s)";
            ut::expect(ut::eq(15.0, force.value()))
                << "3 kg * 5 m/s^2";
            ut::expect(ut::eq(60.0, energy.value()))
                << "15 N * 4 m";
        };

        "dimensionless results remain quantities until a scalar is requested"_test = [] {
            const auto ratio{ 10_m / 2_m };

            static_assert(
                Like<decltype(ratio), Factor>,
                "length / length should remain a dimensionless Quantity");
            static_assert(
                std::convertible_to<Factor, double>,
                "a naked dimensionless Quantity should convert to its stored scalar type");

            ut::expect(ut::eq(5.0, ratio.value()))
                << "10 m / 2 m should produce a ratio of 5";

            const auto formatted{ std::format("{:%}", ratio) };
            ut::expect(ut::eq(std::string{ "500 %" }, formatted))
                << "dimensionless results should remain formatable as quantities";

            const double scalar{ ratio };
            ut::expect(ut::eq(5.0, scalar))
                << "a naked dimensionless quantity should convert to double";
        };

        "scalar scaling preserves a quantity"_test = [] {
            const Length length{ 2_m };
            const auto left_scaled{ 3 * length };
            const auto right_scaled{ length * 3.0 };

            static_assert(
                Like<decltype(left_scaled), Length>,
                "scalar * length should remain a length");
            static_assert(
                Like<decltype(right_scaled), Length>,
                "length * scalar should remain a length");

            ut::expect(ut::eq(6.0, left_scaled.value()));
            ut::expect(ut::eq(6.0, right_scaled.value()));
        };

        "rotational mechanics keeps angle information"_test = [] {
            const MomentOfInertia inertia{ 2.0 };
            const AngularVelocity omega{ 3.0 };
            const AngularAcceleration alpha{ 4.0 };

            const auto rotational_energy{ inertia * omega * omega };
            const auto torque{ inertia * alpha };
            const auto angular_momentum{ inertia * omega };

            static_assert(
                Like<decltype(rotational_energy), Energy>,
                "I * omega^2 should produce energy");
            static_assert(
                Like<decltype(torque), Torque>,
                "I * angular acceleration should produce torque");
            static_assert(
                Like<decltype(angular_momentum), AngularMomentum>,
                "I * angular velocity should produce angular momentum");

            ut::expect(ut::eq(18.0, rotational_energy.value()));
            ut::expect(ut::eq(8.0, torque.value()));
            ut::expect(ut::eq(6.0, angular_momentum.value()));
        };
    };
}
