#include "chemistry/composition.h"

#include <boost/ut.hpp>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace chemistry;
    using enum Element;

    ut::suite<"chemistry.composition"> composition_tests = [] {
        "default construction"_test = [] {
            const Composition composition;

            ut::expect(composition.empty());
            ut::expect(composition.size() == 0u);
            ut::expect(composition.total_quantity() == 0);
            ut::expect(composition.quantity(Au) == 0);
        };

        "element-list construction"_test = [] {
            const Composition composition{ Ag, Au, Ag };

            ut::expect(composition.quantity(Ag) == 2);
            ut::expect(composition.quantity(Au) == 1);
            ut::expect(composition.total_quantity() == 3);
            ut::expect(composition.size() == 2u);
        };

        "weighted construction"_test = [] {
            const Composition vacancies{ { none, 25 }, { Ag, 75 } };

            ut::expect(vacancies.quantity(none) == 25);
            ut::expect(vacancies.quantity(Ag) == 75);
            ut::expect(vacancies.quantity(Au) == 0);
            ut::expect(vacancies.total_quantity() == 100);
            ut::expect(vacancies.size() == 2u);
        };

        "zero quantities are ignored"_test = [] {
            const Composition composition{ { none, 0 }, { Ag, 75 } };

            ut::expect(composition.quantity(none) == 0);
            ut::expect(composition.quantity(Ag) == 75);
            ut::expect(composition.total_quantity() == 75);
            ut::expect(composition.size() == 1u);
        };

        "duplicate components accumulate"_test = [] {
            const Composition composition{ { Ag, 25 }, { Au, 10 }, { Ag, 50 } };

            ut::expect(composition.quantity(Ag) == 75);
            ut::expect(composition.quantity(Au) == 10);
            ut::expect(composition.total_quantity() == 85);
            ut::expect(composition.size() == 2u);
        };

        "addition"_test = [] {
            Composition composition{ { Ag, 25 } };

            composition += Composition::Component{ Ag, 50 };
            composition += Composition{ { Au, 10 }, { none, 5 } };

            ut::expect(composition.quantity(Ag) == 75);
            ut::expect(composition.quantity(Au) == 10);
            ut::expect(composition.quantity(none) == 5);
            ut::expect(composition.total_quantity() == 90);
        };
    };
}
