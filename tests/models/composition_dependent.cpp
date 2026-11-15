#include "models/composition_dependent.h"

#include <boost/ut.hpp>
#include <stdexcept>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using namespace kmc;

    ut::suite<"models.composition_dependent"> composition_dependent_tests = [] {
        "model"_test = [] {
            using enum Element;

            static_assert(models::bond_pair(Ag, Au) == models::bond_pair(Au, Ag));

            auto construct_model = [] {
                models::CompositionDependent model;
                model.set_potential(1.0_V);
                model.set_temperature(300_K);
            };

            if constexpr (lattice::element_specific_neighbors)
                ut::expect(boost::ut::nothrow(construct_model));
            else
                ut::expect(boost::ut::throws<std::logic_error>(construct_model));
        };
    };
}
