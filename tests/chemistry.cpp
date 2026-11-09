#include "chemistry/chemistry.h"

#include "utility/utility.h"

#include <array>
#include <boost/ut.hpp>
#include <format>
#include <utility>

using boost::ut::eq;
using boost::ut::expect;
using boost::ut::suite;
using namespace boost::ut::literals;

namespace chemistry
{
    using enum Element;

    static_assert(sizeof(Element) == 1);

    static_assert(std::to_underlying(H) == 1);
    static_assert(std::to_underlying(Og) == 118);

    static_assert(not is_valid(none));
    static_assert(is_valid(H));
    static_assert(is_valid(Au));
    static_assert(is_valid(Og));
    static_assert(not is_valid(static_cast<Element>(255)));

    static_assert(is_noble_gas(He));
    static_assert(is_noble_gas(Og));
    static_assert(not is_noble_gas(Au));

    static_assert(is_non_metal(N));
    static_assert(is_non_metal(Ts));
    static_assert(not is_non_metal(Fe));

    static_assert(is_metal(Fe));
    static_assert(is_metal(Au));
    static_assert(not is_metal(none));
    static_assert(not is_metal(static_cast<Element>(255)));

    static_assert(symbol(Ti) == "Ti");
    static_assert(name(Si) == "Silicon");

    suite chemistry = [] {
        "atom"_test = [] {
            constexpr std::array atoms{
                Atom{ Au, 0 },
                Atom{ Ag, 0 },
                Atom{ Ag, 1 },
                Atom{ Fe, 3 },
                Atom{ Cl, -1 },
                Atom{ O, -2 },
            };

            for (auto const& atom : atoms)
            {
                expect(not std::format("{:s}", atom).empty());
                expect(not std::format("{:l}", atom).empty());
            }
        };

        "composition"_test = [] {
            Composition vacancies{ { none, 25 }, { Ag, 75 } };

            expect(eq(vacancies.quantity(none), 25));
            expect(eq(vacancies.quantity(Ag), 75));
            expect(eq(vacancies.quantity(Au), 0));
            expect(eq(vacancies.total_quantity(), 100));
            expect(eq(vacancies.size(), 2u));

            Composition zero_quantity{ { none, 0 }, { Ag, 75 } };

            expect(eq(zero_quantity.size(), 1u));
            expect(eq(std::format("{}", vacancies), std::string_view{ "None25Ag75" }));
        };

        "element database"_test = [] {
            for (auto element : enum_values<Element>())
            {
                if (element == none)
                    continue;

                expect(is_valid(element));
                expect(not symbol(element).empty());
                expect(not name(element).empty());
            }
        };
    };
}
