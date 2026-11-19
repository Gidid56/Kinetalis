#include "lattice/lattice.h"

#include <algorithm>
#include <boost/ut.hpp>
#include <cstddef>
#include <ranges>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using namespace lattice;
    using chemistry::Element;

    Lattice make_fcc()
    {
        auto cell{ UnitCell::conventional<Bravais::cF>(400_pm).fill(
            chemistry::Atom{ Element::Au }) };
        Lattice lattice{ cell };
        lattice.scale_shape(Parameters::all(1600_pm));
        using enum math::Axis;
        lattice.set_periodic_boundaries(x | y | z);
        lattice.construct_sites();
        lattice.construct_neighbor_list();
        return lattice;
    }

    void expect_coordination_consistent(Lattice const& lattice)
    {
        for (auto const& site : lattice.sites())
        {
            auto const occupied_neighbors{ std::ranges::count_if(
                lattice.neighbors(site), [](auto const* neighbor) {
                    return neighbor->is_occupied();
                }) };
            ut::expect(ut::eq(static_cast<std::size_t>(site.coordination()),
                static_cast<std::size_t>(occupied_neighbors))) << "incremental coordination";
            ut::expect(ut::eq(static_cast<std::size_t>(site.vacant_neighbors()),
                static_cast<std::size_t>(site.max_neighbors() - occupied_neighbors)))
                << "vacant coordination";
        }
        ut::expect(ut::eq(lattice.number_atoms(), static_cast<std::size_t>(
            std::ranges::count_if(lattice.sites(), &Site::is_occupied))))
            << "number_atoms agrees with actual occupancy";
    }

    ut::suite<"lattice.site"> site_tests = [] {
        "site occupancy updates neighbor coordination correctly"_test = [] {
            auto lattice{ make_fcc() };
            auto& site{ lattice.sites()[lattice.sites().size() / 2] };
            auto* adjacent{ *lattice.neighbors(site).begin() };
            auto const initial_atoms{ lattice.number_atoms() };

            expect_coordination_consistent(lattice);
            for (auto& s : lattice.sites())
            {
                s.set_site_update_queued(false);
                s.set_pristine(true);
            }

            site.set_element(lattice, Element::none);
            ut::expect(not site.is_site_update_queued());
            ut::expect(not adjacent->is_site_update_queued());
            ut::expect(not site.is_pristine());
            ut::expect(site.is_vacant());
            ut::expect(ut::eq(lattice.number_atoms(), initial_atoms - 1));
            expect_coordination_consistent(lattice);

            adjacent->set_element(lattice, Element::none);
            ut::expect(ut::eq(lattice.number_atoms(), initial_atoms - 2));
            expect_coordination_consistent(lattice);

            site.set_element(lattice, Element::Ag);
            ut::expect(site.is_occupied());
            ut::expect(ut::eq(lattice.number_atoms(), initial_atoms - 1));
            expect_coordination_consistent(lattice);

            adjacent->set_element(lattice, Element::Au);
            ut::expect(ut::eq(lattice.number_atoms(), initial_atoms));
            expect_coordination_consistent(lattice);

            // Changing species without changing occupancy must not alter coordination.
            site.set_element(lattice, Element::Au);
            expect_coordination_consistent(lattice);
        };

        "site eligibility and update-queue flags are independent"_test = [] {
            auto lattice{ make_fcc() };
            auto& site{ lattice.sites().front() };

            ut::expect(not site.is_source_eligible());
            ut::expect(not site.is_site_update_queued());
            ut::expect(not site.is_block_update_queued());

            site.set_source_eligible(true);
            site.set_site_update_queued(true);
            site.set_block_update_queued(true);

            ut::expect(site.is_source_eligible());
            ut::expect(site.is_site_update_queued());
            ut::expect(site.is_block_update_queued());

            site.set_site_update_queued(false);
            ut::expect(not site.is_site_update_queued());
            ut::expect(site.is_source_eligible());
            ut::expect(site.is_block_update_queued());

            site.set_block_update_queued(false);
            ut::expect(not site.is_block_update_queued());
            ut::expect(site.is_source_eligible());
        };

        "setting an unchanged element leaves coordination untouched"_test = [] {
            auto lattice{ make_fcc() };
            auto& site{ lattice.sites().front() };
            auto const coordination{ site.coordination() };
            site.set_site_update_queued(false);
            site.set_pristine(true);
            site.set_element(lattice, Element::Au);
            ut::expect(not site.is_site_update_queued());
            ut::expect(site.is_pristine());
            ut::expect(ut::eq(site.coordination(), coordination));
            expect_coordination_consistent(lattice);
        };
    };
}
