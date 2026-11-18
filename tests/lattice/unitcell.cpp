#include "lattice/unitcell.h"

#include <algorithm>
#include <array>
#include <boost/ut.hpp>
#include <cmath>
#include <cstddef>
#include <numbers>
#include <ranges>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using namespace lattice;
    using chemistry::Atom;
    using enum chemistry::Element;

    ut::suite<"lattice.unitcell"> unitcell_tests = [] {
        "cubic centerings contain the expected basis sites"_test = [] {
            auto const simple{ UnitCell::conventional<Bravais::cP>(400_pm) };
            auto const body{ UnitCell::conventional<Bravais::cI>(400_pm) };
            auto const face{ UnitCell::conventional<Bravais::cF>(400_pm) };

            ut::expect(ut::eq(simple.size(), std::size_t{ 1 }));
            ut::expect(ut::eq(body.size(), std::size_t{ 2 }));
            ut::expect(ut::eq(face.size(), std::size_t{ 4 }));
            ut::expect(simple.bravais() == Bravais::cP);
            ut::expect(body.bravais() == Bravais::cI);
            ut::expect(face.bravais() == Bravais::cF);
            ut::expect(face.system() == System::cubic);
        };

        "fill and set change only the requested atomic species"_test = [] {
            auto cell{ UnitCell::conventional<Bravais::cF>(405_pm).fill(Atom{ Au }) };
            ut::expect(std::ranges::all_of(cell.sites(), [](auto const& site) {
                return site.atom == Atom{ Au };
            }));

            cell.set(Index{ 0 }, Atom{ Cu });
            ut::expect(cell.sites()[0].atom == Atom{ Cu });
            ut::expect(std::ranges::all_of(cell.sites() | std::views::drop(1),
                [](auto const& site) { return site.atom == Atom{ Au }; }));

            cell.fill(Atom{ Ag });
            ut::expect(std::ranges::all_of(cell.sites(), [](auto const& site) {
                return site.atom == Atom{ Ag };
            }));
        };

        "ordered binary alloy cells preserve their stoichiometry"_test = [] {
            auto au3_cu{ UnitCell::conventional<Bravais::cF>(405_pm)
                .fill(Atom{ Au })
                .set(Index{ 0 }, Atom{ Cu }) };
            auto cu3_au{ UnitCell::conventional<Bravais::cF>(377_pm)
                .fill(Atom{ Cu })
                .set(Index{ 0 }, Atom{ Au }) };

            ut::expect(ut::eq(au3_cu.size(), std::size_t{ 4 }));
            ut::expect(ut::eq(cu3_au.size(), std::size_t{ 4 }));
            ut::expect(ut::eq(std::ranges::count_if(au3_cu.sites(), [](auto const& site) {
                return site.atom == Atom{ Cu };
            }), std::ptrdiff_t{ 1 }));
            ut::expect(ut::eq(std::ranges::count_if(cu3_au.sites(), [](auto const& site) {
                return site.atom == Atom{ Au };
            }), std::ptrdiff_t{ 1 }));
        };

        "ionic and compound unit cells preserve site counts and charges"_test = [] {
            auto cs_cl{ UnitCell::conventional<Bravais::cI>(421_pm)
                .set(Index{ 0 }, Atom{ Cs, 1 })
                .set(Index{ 1 }, Atom{ Cl, -1 }) };
            ut::expect(ut::eq(cs_cl.size(), std::size_t{ 2 }));
            ut::expect(cs_cl.sites()[0].atom == Atom{ Cs, 1 });
            ut::expect(cs_cl.sites()[1].atom == Atom{ Cl, -1 });

            auto na_cl{ UnitCell::conventional<Bravais::cF>(569_pm)
                .fill(Atom{ Na, 1 })
                .insert({ site::center, Atom{ Cl, -1 } })
                .insert({ unit::x / 2, Atom{ Cl, -1 } })
                .insert({ unit::y / 2, Atom{ Cl, -1 } })
                .insert({ unit::z / 2, Atom{ Cl, -1 } }) };
            ut::expect(ut::eq(na_cl.size(), std::size_t{ 8 }));
            ut::expect(ut::eq(std::ranges::count_if(na_cl.sites(), [](auto const& site) {
                return site.atom == Atom{ Na, 1 };
            }), std::ptrdiff_t{ 4 }));
            ut::expect(ut::eq(std::ranges::count_if(na_cl.sites(), [](auto const& site) {
                return site.atom == Atom{ Cl, -1 };
            }), std::ptrdiff_t{ 4 }));

            auto diamond{ UnitCell::conventional<Bravais::cF>(448_pm)
                .insert({ site::origin + site::center / 2 })
                .insert({ site::front + site::center / 2 })
                .insert({ site::side + site::center / 2 })
                .insert({ site::base + site::center / 2 })
                .fill(Atom{ C }) };
            ut::expect(ut::eq(diamond.size(), std::size_t{ 8 }));
            ut::expect(std::ranges::all_of(diamond.sites(), [](auto const& site) {
                return site.atom == Atom{ C };
            }));
        };

        "representative noncubic cells construct with the correct basis"_test = [] {
            auto hcp{ UnitCell::hcp(Atom{ Co }) };
            auto graphite{ UnitCell::conventional<Bravais::hP>({ 246_pm, 670_pm })
                .insert({ site::hcp })
                .insert({ { .0, .0, .5 } })
                .insert({ { 2. / 3., 1. / 3., .0 } })
                .fill(Atom{ C }) };
            auto const polonium{ UnitCell::conventional<Bravais::cP>(Atom{ Po }) };
            auto const iron{ UnitCell::conventional<Bravais::cI>(Atom{ Fe }) };
            auto const silver{ UnitCell::conventional<Bravais::cF>(Atom{ Ag }) };

            ut::expect(hcp.bravais() == Bravais::hP);
            ut::expect(ut::eq(hcp.size(), std::size_t{ 2 }));
            ut::expect(std::ranges::all_of(hcp.sites(), [](auto const& site) {
                return site.atom == Atom{ Co };
            }));
            ut::expect(graphite.system() == System::hexagonal);
            ut::expect(ut::eq(graphite.size(), std::size_t{ 4 }));
            ut::expect(ut::eq(polonium.size(), std::size_t{ 1 }));
            ut::expect(ut::eq(iron.size(), std::size_t{ 2 }));
            ut::expect(ut::eq(silver.size(), std::size_t{ 4 }));
        };

        "nearest-neighbor spacing and packing fraction have physical values"_test = [] {
            auto const simple{ UnitCell::conventional<Bravais::cP>(400_pm).fill(Atom{ Au }) };
            auto const face{ UnitCell::conventional<Bravais::cF>(400_pm).fill(Atom{ Au }) };

            ut::expect(ut::that %
                std::abs((simple.next_neighbor_distance() / 1_pm).value() - 400.0) < 1E-10);
            ut::expect(ut::that %
                std::abs((face.next_neighbor_distance() / 1_pm).value() -
                    400.0 / std::numbers::sqrt2) < 1E-10);

            ut::expect(ut::that % simple.packing_fraction().value() > 0.0);
            ut::expect(ut::that % simple.packing_fraction().value() < 1.0);
            ut::expect(ut::that % face.packing_fraction().value() > 0.0);
            ut::expect(ut::that % face.packing_fraction().value() < 1.0);
            ut::expect(ut::that %
                face.packing_fraction().value() > simple.packing_fraction().value());
        };

        "fcc [111] supercell has the correct orthogonal basis and volume"_test = [] {
            auto const cell{ UnitCell::conventional<Bravais::cF>(400_pm) };
            auto const shape{ cell.oriented_periodic_supercell(Indices{ 1, 1, 1 }) };
            auto const equivalent{ cell.oriented_periodic_supercell(Indices{ 2, 2, 2 }) };

            constexpr std::array expected{
                std::array{ -1, 1, 0 },
                std::array{ -1, -1, 2 },
                std::array{ 1, 1, 1 },
            };
            auto const parameter{ (400_pm).value() };
            for (std::size_t i{}; i < 3; ++i)
                for (std::size_t j{}; j < 3; ++j)
                    ut::expect(ut::that %
                        std::abs(shape.vectors()[i][j].value() / parameter - expected[i][j]) <
                            1E-12) << "oriented basis component";

            ut::expect(shape.is_rectangular());
            ut::expect(shape.vectors() == equivalent.vectors())
                << "Miller indices must be reduced to a primitive direction";
            ut::expect(ut::that %
                std::abs(shape.volume().value() / (parameter * parameter * parameter) - 6.0) <
                    1E-12) << "the [111] supercell has volume 6a^3";
        };

        "zero Miller indices are rejected"_test = [] {
            auto const cell{ UnitCell::conventional<Bravais::cF>(400_pm) };
            ut::expect(ut::throws<std::invalid_argument>([&] {
                (void)cell.oriented_periodic_supercell(Indices{ 0, 0, 0 });
            }));
        };
    };
}
