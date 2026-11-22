#include "dealloying/kinetics/composition_dependent.h"
#include "dealloying/kinetics/linear_bond_breaking.h"
#include "dealloying/kinetics/descriptor_format.h"
#include "lattice/lattice.h"
#include "physics/equations/kinetics.h"

#include <cmath>

#include <boost/ut.hpp>
#include <stdexcept>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using namespace kmc;
    namespace kinetics = studies::dealloying::kinetics;
    using chemistry::Element;
    using enum chemistry::Element;

    using Descriptor = kinetics::CompositionDependent::Descriptor;

    Descriptor make_descriptor(event::Type type, Element element,
                               lattice::Coordination ag, lattice::Coordination au)
    {
        Descriptor result{ type, element, static_cast<lattice::Coordination>(ag + au), {} };
        result.element_neighbors[lattice::elementKey(Ag)] = ag;
        result.element_neighbors[lattice::elementKey(Au)] = au;
        return result;
    }

    void expect_rate(kmc::Rate actual, kmc::Rate expected)
    {
        ut::expect(std::abs(actual.value() / expected.value() - 1.0) < 1e-12);
    }

    ut::suite<"kinetics.composition_dependent"> composition_dependent_tests = [] {
        "bond pair keys are symmetric and distinguish different pairs"_test = [] {
            static_assert(kinetics::bond_pair(Ag, Au) == kinetics::bond_pair(Au, Ag));
            static_assert(kinetics::bond_pair(Cu, Ag) == kinetics::bond_pair(Ag, Cu));
            static_assert(kinetics::bond_pair(Ag, Ag) != kinetics::bond_pair(Ag, Au));
            static_assert(kinetics::bond_pair(Ag, Au) != kinetics::bond_pair(Cu, Au));
        };

        "composition model works without globally stored element neighbors"_test = [] {
            kinetics::CompositionDependent model;
            model.set_potential(1.0_V);
            model.set_temperature(300_K);
            ut::expect(ut::eq(model.number_keys(), std::size_t{ 1 }));
        };

        "EMT tangent barriers use bond slopes and z=0 species offsets"_test = [] {
            kinetics::CompositionDependent model;
            model.set_temperature(300_K);
            model.set_potential(model.constants().reference_potential);

            using physics::equations::kinetics::tst_rate;
            auto const ag6{ make_descriptor(event::Type::diffusion, Ag, 6, 0) };
            auto const au6{ make_descriptor(event::Type::diffusion, Au, 0, 6) };
            auto const mixed{ make_descriptor(event::Type::diffusion, Ag, 5, 1) };

            expect_rate(model.rate(ag6), tst_rate(0.72_eV, 300_K));
            expect_rate(model.rate(au6), tst_rate(0.90_eV, 300_K));
            expect_rate(model.rate(mixed), tst_rate(0.751_eV, 300_K));

            auto const ag6_dissolution{ make_descriptor(event::Type::dissolution, Ag, 6, 0) };
            auto const au6_dissolution{ make_descriptor(event::Type::dissolution, Au, 0, 6) };
            auto const mixed_dissolution{ make_descriptor(event::Type::dissolution, Ag, 5, 1) };
            expect_rate(model.rate(ag6_dissolution), tst_rate(2.46_eV, 300_K));
            expect_rate(model.rate(au6_dissolution), tst_rate(3.21_eV, 300_K));
            expect_rate(model.rate(mixed_dissolution), tst_rate(2.491_eV, 300_K));
            expect_rate(model.rate(make_descriptor(event::Type::dissolution, Ag, 0, 0)),
                tst_rate(1.74_eV, 300_K));
        };

        "pure Ag9 dissolution matches v0.6.1 reference at every temperature"_test = [] {
            kinetics::LinearBondBreaking old_model;
            kinetics::CompositionDependent new_model;
            ut::expect(std::abs(
                (new_model.constants().reference_potential - (-0.934262136009015_V)) / 1_V
            ) < 1e-12);

            auto const ag9{ make_descriptor(event::Type::dissolution, Ag, 9, 0) };
            kinetics::LinearBondBreaking::Descriptor old_ag9{
                event::Type::dissolution, Ag, 9
            };

            for (auto temperature : { 250_K, 300_K, 350_K })
            {
                old_model.set_temperature(temperature).set_potential(0.85_V);
                new_model.set_temperature(temperature).set_potential(0.85_V);
                expect_rate(new_model.rate(ag9), old_model.rate(old_ag9));
            }
        };

        "kinetic descriptors distinguish changing element neighbors only when needed"_test = [] {
            auto cell{ lattice::UnitCell::conventional<lattice::Bravais::cF>(400_pm)
                .fill(chemistry::Atom{ Au }) };
            lattice::Lattice lattice{ cell };
            lattice.scale_shape(lattice::Parameters::all(1600_pm));
            using enum math::Axis;
            lattice.set_periodic_boundaries(x | y | z);
            lattice.construct_sites();
            lattice.construct_neighbor_list();

            auto& source{ lattice.sites().front() };
            auto neighbors{ lattice.neighbors(source) };
            auto it{ neighbors.begin() };
            // Static coordination descriptors deliberately exclude fully
            // coordinated bulk sites; introduce a vacancy first.
            (*it)->set_element(lattice, Element::none);
            auto* neighbor{ *++it };
            const kmc::event::Location location{ &source };

            kinetics::CompositionDependent composition;
            kinetics::LinearBondBreaking linear;
            auto const first_composition{ composition.classify(
                kmc::event::Type::diffusion, location, lattice) };
            auto const first_linear{ linear.classify(
                kmc::event::Type::diffusion, location, lattice) };
            auto const original_coordination{ source.coordination() };

            // Must also pass when lattice::element_specific_neighbors is disabled:
            // classification then reconstructs the same counts on demand.
            ut::expect(first_composition.element_neighbors[lattice::elementKey(Ag)] == 0);
            ut::expect(first_composition.element_neighbors[lattice::elementKey(Au)] ==
                original_coordination);

            neighbor->set_element(lattice, Ag);
            ut::expect(source.coordination() == original_coordination);

            auto const second_composition{ composition.classify(
                kmc::event::Type::diffusion, location, lattice) };
            auto const second_linear{ linear.classify(
                kmc::event::Type::diffusion, location, lattice) };

            ut::expect(second_composition.element_neighbors[lattice::elementKey(Ag)] == 1);
            ut::expect(second_composition.element_neighbors[lattice::elementKey(Au)] ==
                original_coordination - 1);
            ut::expect(first_composition != second_composition)
                << "composition descriptor distinguishes changed neighbor chemistry";
            ut::expect(first_composition.event == kmc::event::Type::diffusion);
            ut::expect(output::DescriptorFormat<kinetics::CompositionDependent::Descriptor>::label(first_composition) !=
                output::DescriptorFormat<kinetics::CompositionDependent::Descriptor>::label(second_composition))
                << "model-owned class labels distinguish neighbor chemistry";
            ut::expect(output::DescriptorFormat<kinetics::CompositionDependent::Descriptor>::row(first_composition) !=
                output::DescriptorFormat<kinetics::CompositionDependent::Descriptor>::row(second_composition))
                << "CSV metadata preserves model-specific descriptors";
            ut::expect(first_linear == second_linear)
                << "coordination descriptor ignores irrelevant neighbor chemistry";
            ut::expect(ut::eq(composition.number_keys(), std::size_t{ 1 }));
            auto const first_key{ composition.key(first_composition) };
            auto const second_key{ composition.key(second_composition) };
            ut::expect(first_key != second_key);
            ut::expect(ut::eq(composition.number_keys(), std::size_t{ 3 }));
            ut::expect(composition.descriptor(first_key) == first_composition);
            ut::expect(composition.descriptor(second_key) == second_composition);
            ut::expect(composition.rate(first_composition) != composition.rate(second_composition))
                << "composition-dependent rates differ for inequivalent classes";
        };
    };
}
