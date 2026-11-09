#include "chemistry/chemistry.h"

#include <boost/ut.hpp>
#include <format>
#include <string_view>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace chemistry;
    using enum Element;

    ut::suite<"chemistry.format"> format_tests = [] {
        "element formatting"_test = [] {
            ut::expect(std::format("{}", Ti) == "Ti");
            ut::expect(std::format("{:s}", Ti) == "Ti");
            ut::expect(std::format("{:l}", Si) == "Silicon");
        };

        "atom formatting"_test = [] {
            ut::expect(std::format("{}", Atom{ Au, 0 }) == "Au");
            ut::expect(std::format("{:s}", Atom{ Ag, 1 }) == "Ag+");
            ut::expect(std::format("{:s}", Atom{ Fe, 3 }) == "Fe3+");
            ut::expect(std::format("{:s}", Atom{ Cl, -1 }) == "Cl-");
            ut::expect(std::format("{:s}", Atom{ O, -2 }) == "O2-");

            ut::expect(std::format("{:l}", Atom{ Au, 0 }) == "Gold (charge 0)");
            ut::expect(std::format("{:l}", Atom{ Fe, 3 }) == "Iron (charge 3)");
            ut::expect(std::format("{:l}", Atom{ O, -2 }) == "Oxygen (charge -2)");
        };

        "composition formatting"_test = [] {
            const Composition vacancies{ { none, 25 }, { Ag, 75 } };
            const Composition elemental{ { Ag, 1 }, { Au, 2 } };

            ut::expect(std::format("{}", vacancies) == std::string_view{ "None25Ag75" });
            ut::expect(std::format("{}", elemental) == std::string_view{ "AgAu2" });
        };
    };
}
