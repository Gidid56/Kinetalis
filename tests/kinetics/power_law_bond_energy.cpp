#include "dealloying/kinetics/power_law_bond_energy.h"
#include "dealloying/kinetics/linear_bond_breaking.h"
#include "physics/equations/kinetics.h"

#include <boost/ut.hpp>
#include <cmath>
#include <string>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using namespace kmc;
    namespace kinetics = studies::dealloying::kinetics;
    using enum chemistry::Element;

    using Descriptor = kinetics::PowerLawBondEnergy::Descriptor;

    void expect_energy(Energy actual, Energy expected)
    {
        ut::expect(std::abs((actual - expected) / 1_eV) < 1e-12);
    }

    void expect_rate(Rate actual, Rate expected)
    {
        ut::expect(std::abs(actual.value() / expected.value() - 1.0) < 1e-12);
    }

    ut::suite<"kinetics.power_law_bond_energy"> power_law_bond_energy_tests = [] {
        "diffusion transition state has coordination two for every element"_test = [] {
            kinetics::PowerLawBondEnergy model;
            static_assert(kinetics::PowerLawBondEnergy::diffusion_transition_state_coordination == 2.0);

            for (auto element : { Ag, Au, Cu })
                for (lattice::Coordination n : { 0, 1, 2 })
                    expect_energy(model.lattice_energy_barrier(
                        { event::Type::diffusion, element, n }), 0_eV);

            expect_energy(model.lattice_energy_barrier(
                { event::Type::diffusion, Ag, 6 }), 0.7362505481644082_eV);
            expect_energy(model.lattice_energy_barrier(
                { event::Type::diffusion, Ag, 9 }), 1.0803778230589547_eV);
            expect_energy(model.lattice_energy_barrier(
                { event::Type::diffusion, Au, 6 }), 0.6795864568475839_eV);
            expect_energy(model.lattice_energy_barrier(
                { event::Type::diffusion, Au, 9 }), 0.9814613961596104_eV);
            expect_energy(model.lattice_energy_barrier(
                { event::Type::diffusion, Cu, 6 }), 0.6895234201021916_eV);
        };

        "dissolution retains the full lattice binding energy"_test = [] {
            kinetics::PowerLawBondEnergy model;
            expect_energy(model.lattice_energy_barrier(
                { event::Type::dissolution, Ag, 0 }), 0_eV);
            expect_energy(model.lattice_energy_barrier(
                { event::Type::dissolution, Ag, 9 }), 2.8280405915212112_eV);
            expect_energy(model.lattice_energy_barrier(
                { event::Type::dissolution, Au, 9 }), 3.13154786008453_eV);
        };

        "diffusion rates use the nTS=2 barrier"_test = [] {
            kinetics::PowerLawBondEnergy model;
            model.set_temperature(300_K).set_potential(0.85_V);

            expect_rate(model.rate({ event::Type::diffusion, Au, 6 }),
                physics::equations::kinetics::tst_rate(0.6795864568475839_eV, 300_K));
            expect_rate(model.rate({ event::Type::diffusion, Ag, 1 }),
                physics::equations::kinetics::tst_rate(0_eV, 300_K));
        };

        "Ag9 dissolution matches the original model at equal E and T"_test = [] {
            kinetics::LinearBondBreaking old_model;
            kinetics::PowerLawBondEnergy new_model;

            const auto reference_potential{
                new_model.ElectrochemicalInterface::constants().reference_potential
            };
            ut::expect(std::abs(
                (reference_potential - (-0.942302727530226_V)) / 1_V) < 1e-12);

            for (auto temperature : { 250_K, 300_K, 350_K })
                for (auto potential : { 0.80_V, 0.85_V, 0.95_V })
                {
                    old_model.set_temperature(temperature).set_potential(potential);
                    new_model.set_temperature(temperature).set_potential(potential);
                    expect_rate(new_model.rate({ event::Type::dissolution, Ag, 9 }),
                        old_model.rate({ event::Type::dissolution, Ag, 9 }));
                }

            ut::expect(new_model.output_text().find("Diffusion TS coordination = 2") !=
                std::string::npos);
        };
    };
}
