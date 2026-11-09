#include "chemistry/atom.h"

#include <boost/ut.hpp>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace chemistry;
    using namespace physics::unit::literals;
    using enum Element;

    ut::suite<"chemistry.atom"> atom_tests = [] {
        "value semantics"_test = [] {
            static_assert(Atom{} == Atom{ none, 0 });
            static_assert(Atom{ Ag, 0 } == Atom{ Ag, 0 });
            static_assert(Atom{ Ag, 0 } != Atom{ Ag, 1 });
            static_assert(Atom{ Ag, 0 } != Atom{ Au, 0 });
        };

        "ionic radius"_test = [] {
            static_assert(Atom{ Cl, -1 }.ionic_radius() == 181_pm);
            static_assert(Atom{ O, -2 }.ionic_radius() == 126_pm);
            static_assert(Atom{ Fe, 3 }.ionic_radius() == 69_pm);
            static_assert(not Atom{ Au, 0 }.ionic_radius().has_value());
        };

        "neutral radius"_test = [] {
            static_assert(Atom{ Au, 0 }.neutral_radius() == 144_pm);
            static_assert(Atom{ O, 0 }.neutral_radius() == 152_pm);
            static_assert(not Atom{ none, 0 }.neutral_radius().has_value());
        };

        "display radius selection"_test = [] {
            static_assert(Atom{ none, 0 }.display_radius().value() == 0.0);
            static_assert(Atom{ Cl, -1 }.display_radius() == 181_pm);
            static_assert(Atom{ Au, 0 }.display_radius() == 144_pm);
            static_assert(Atom{ O, 0 }.display_radius() == 152_pm);
            static_assert(Atom{ Rf, 0 }.display_radius() == Atom::defaultDisplayRadius);
        };

        "display volume"_test = [] {
            static_assert(Atom{ none, 0 }.display_volume().value() == 0.0);
            static_assert(Atom{ Au, 0 }.display_volume().value() > 0.0);
        };
    };
}
