#include "dealloying/kinetics/composition_dependent.h"
#include "dealloying/kinetics/linear_bond_breaking.h"
#include "dealloying/kinetics/descriptor_format.h"
#include "lattice/lattice.h"

#include <boost/ut.hpp>
#include <stdexcept>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using namespace kmc;
    namespace kinetics = studies::dealloying::kinetics;
    using enum chemistry::Element;

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
            const kmc::event::Location location{ source };

            kinetics::CompositionDependent composition;
            kinetics::LinearBondBreaking linear;
            auto const first_composition{ composition.classify(
                kmc::event::Type::diffusion, location, lattice) };
            auto const first_linear{ linear.classify(
                kmc::event::Type::diffusion, location, lattice) };
            auto const original_coordination{ source.coordination() };

            neighbor->set_element(lattice, Ag);
            ut::expect(source.coordination() == original_coordination);

            auto const second_composition{ composition.classify(
                kmc::event::Type::diffusion, location, lattice) };
            auto const second_linear{ linear.classify(
                kmc::event::Type::diffusion, location, lattice) };

            ut::expect(first_composition != second_composition)
                << "composition descriptor distinguishes changed neighbor chemistry";
            ut::expect(composition.class_event(first_composition) == kmc::event::Type::diffusion);
            ut::expect(output::DescriptorFormat<kinetics::CompositionDependent::Descriptor>::label(composition.descriptor(first_composition)) !=
                output::DescriptorFormat<kinetics::CompositionDependent::Descriptor>::label(composition.descriptor(second_composition)))
                << "model-owned class labels distinguish neighbor chemistry";
            ut::expect(output::DescriptorFormat<kinetics::CompositionDependent::Descriptor>::row(composition.descriptor(first_composition)) !=
                output::DescriptorFormat<kinetics::CompositionDependent::Descriptor>::row(composition.descriptor(second_composition)))
                << "CSV metadata preserves model-specific descriptors";
            ut::expect(first_linear == second_linear)
                << "coordination descriptor ignores irrelevant neighbor chemistry";
            ut::expect(ut::eq(composition.number_keys(), std::size_t{ 3 }));
            ut::expect(composition.rate(first_composition) != composition.rate(second_composition))
                << "composition-dependent rates differ for inequivalent classes";
        };
    };
}
