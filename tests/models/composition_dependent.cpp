#include "models/composition_dependent.h"

#include <boost/ut.hpp>
#include <stdexcept>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using namespace kmc;
    using enum Element;

    ut::suite<"models.composition_dependent"> composition_dependent_tests = [] {
        "bond pair keys are symmetric and distinguish different pairs"_test = [] {
            static_assert(models::bond_pair(Ag, Au) == models::bond_pair(Au, Ag));
            static_assert(models::bond_pair(Cu, Ag) == models::bond_pair(Ag, Cu));
            static_assert(models::bond_pair(Ag, Ag) != models::bond_pair(Ag, Au));
            static_assert(models::bond_pair(Ag, Au) != models::bond_pair(Cu, Au));
        };

        "construction requires element-specific neighbor tracking"_test = [] {
            auto construct_model = [] {
                models::CompositionDependent model;
                model.set_potential(1.0_V);
                model.set_temperature(300_K);
            };

            if constexpr (lattice::element_specific_neighbors)
                ut::expect(ut::nothrow(construct_model));
            else
                ut::expect(ut::throws<std::logic_error>(construct_model));
        };
    };
}
