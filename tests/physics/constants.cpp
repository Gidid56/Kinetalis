#include "physics/physics.h"

#include <boost/ut.hpp>

#include <format>
#include <string>
#include <type_traits>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace physics;
    using namespace physics::constant::symbols;
    using namespace physics::quantity;
    using namespace physics::unit::literals;

    template <class T>
    using RemoveCVRef = std::remove_cvref_t<T>;

    ut::suite<"physics.constants"> constant_tests = [] {
        "SI defining constants expose useful quantity types"_test = [] {
            static_assert(Like<decltype(c), Velocity >,
                "speed of light should be velocity");
            static_assert(
                Like<decltype(h), Action>,
                "Planck constant should be action");
            static_assert(
                Like<decltype(e), ElectricCharge>,
                "elementary charge should be electric charge");
            static_assert(
                Like<decltype(k_B), Entropy>,
                "Boltzmann constant should be energy per temperature");

            ut::expect(ut::eq(299'792'458.0, c.value()));
            ut::expect(ut::eq(6.626'070'15E-34, h.value()));
            ut::expect(ut::eq(1.602'176'634E-19, e.value()));
        };

        "derived constants retain the expected dimensions"_test = [] {
            static_assert(
                Like<decltype(hbar), AngularMomentum>);
            static_assert(
                Like<decltype(mu_B), MagneticMoment>);
            static_assert(
                Like<decltype(sigma), StefanBoltzmannConstant>);

            ut::expect(ut::eq(h.value(), (hbar * constant::full_turn).value()))
                << "hbar * full turn should recover h";
            ut::expect(ut::eq(F.value(), (N_A * e).value()))
                << "Faraday constant identity";
            ut::expect(ut::eq(R.value(), (N_A * k_B).value()))
                << "molar gas constant identity";
        };

        "constants work directly in calculations"_test = [] {
            const auto rest_energy{ 1_kg * c * c };
            const auto photon_energy{ h * 500_THz };

            static_assert(
                Like<decltype(rest_energy), Energy>);
            static_assert(
                Like<decltype(photon_energy), Energy>);

            ut::expect(
                ut::eq(
                    std::string{ "8.988e+16 J" },
                    std::format("{:J;.3e}", rest_energy)));

            ut::expect(
                ut::eq(
                    std::string{ "2.067834 eV" },
                    std::format("{:eV;.6f}", photon_energy)));
        };

        "descriptive names are aliases of symbol names"_test = [] {
            ut::expect(ut::eq(F.value(), constant::faraday.value()));
            ut::expect(ut::eq(hbar.value(), constant::reduced_planck.value()));
            ut::expect(ut::eq(mu_B.value(), constant::bohr_magneton.value()));
            ut::expect(ut::eq(sigma.value(), constant::stefan_boltzmann.value()));
        };
    };
}
