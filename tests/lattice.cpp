#include "lattice/lattice.h"

#include <array>
#include <boost/ut.hpp>

using boost::ut::suite;
using boost::ut::expect;
using namespace boost::ut::literals;

namespace lattice::parameters
{
    constexpr math::Vector<2> graphite{ std::numbers::sqrt3, 2. };
    constexpr auto diamond{ 4 * std::numbers::inv_sqrt3 };
}

namespace lattice
{
    suite<"lattice"> lattice_tests = [] {
        "unitcell"_test = [] {
            expect(boost::ut::nothrow([] {
                using chemistry::Atom;
                using enum Element;

                auto polonium{ UnitCell::conventional<Bravais::cP>(Atom{ Po }) };
                auto silver{ UnitCell::conventional<Bravais::cF>(Atom{ Ag }) };
                auto iron{ UnitCell::conventional<Bravais::cI>(Atom{ Fe }) };
                auto cobalt{ UnitCell::hcp(Atom{ Co }) };

                auto cu_au3{ UnitCell::conventional<Bravais::cF>(405_pm)
                        .fill(Atom{ Au })
                        .set(Index{ 0 }, Atom{ Cu }) };

                auto cu3_au{ UnitCell::conventional<Bravais::cF>(377_pm)
                        .fill(Atom{ Cu })
                        .set(Index{ 0 }, Atom{ Au }) };

                auto cs_cl{ UnitCell::conventional<Bravais::cI>(421_pm)
                        .set(Index{ 0 }, Atom{ Cs, 1 })
                        .set(Index{ 1 }, Atom{ Cl, -1 }) };

                auto na_cl{ UnitCell::conventional<Bravais::cF>(569_pm)
                        .fill(Atom{ Na, 1 })
                        .insert({ site::center, Atom{ Cl, -1 } })
                        .insert({ unit::x / 2, Atom{ Cl, -1 } })
                        .insert({ unit::y / 2, Atom{ Cl, -1 } })
                        .insert({ unit::z / 2, Atom{ Cl, -1 } }) };

                auto graphite{ UnitCell::conventional<Bravais::hP>({ 246_pm, 670_pm })
                        .insert({ site::hcp })
                        .insert({ { .0, .0, .5 } })
                        .insert({ { 2. / 3., 1. / 3., .0 } })
                        .fill(Atom{ C }) };

                auto diamond{ UnitCell::conventional<Bravais::cF>(448_pm)
                        .insert({ site::origin + site::center / 2 })
                        .insert({ site::front + site::center / 2 })
                        .insert({ site::side + site::center / 2 })
                        .insert({ site::base + site::center / 2 })
                        .fill(Atom{ C }) };

                std::array const unitcells{
                    polonium,
                    silver,
                    iron,
                    cobalt,
                    cu_au3,
                    cu3_au,
                    cs_cl,
                    na_cl,
                    graphite,
                    diamond,
                };

                for (auto const& unit_cell : unitcells)
                    (void)unit_cell.packing_fraction();
            }));
        };

        // TODO: Add shape tests once expected shape behavior is specified.
    };
}
