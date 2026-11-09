#include "chemistry/element.h"

#include "utility/utility.h"

#include <boost/ut.hpp>

#include <cstdint>
#include <utility>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace chemistry;
    using enum Element;
    using magic_enum::enum_values;

    ut::suite<"chemistry.element"> element_tests = [] {
        "representation and range"_test = [] {
            static_assert(sizeof(Element) == 1);
            static_assert(std::to_underlying(none) == 0);
            static_assert(std::to_underlying(H) == 1);
            static_assert(std::to_underlying(Og) == 118);
        };

        "validity"_test = [] {
            static_assert(not is_valid(none));
            static_assert(is_valid(H));
            static_assert(is_valid(Au));
            static_assert(is_valid(Og));
            static_assert(not is_valid(static_cast<Element>(255)));
        };

        "classification"_test = [] {
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
        };

        "symbol and name"_test = [] {
            static_assert(symbol(Ti) == "Ti");
            static_assert(name(Si) == "Silicon");
            static_assert(symbol(none) == "None");
            static_assert(name(none) == "no element");
        };

        "database coverage"_test = [] {
            for (auto element : enum_values<Element>())
            {
                if (element == none)
                    continue;

                ut::expect(is_valid(element));
                ut::expect(not symbol(element).empty());
                ut::expect(not name(element).empty());
            }
        };
    };
}
