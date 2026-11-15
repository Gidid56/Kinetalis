#include "lattice/lattice.h"

#include <algorithm>
#include <array>
#include <boost/ut.hpp>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <ranges>
#include <vector>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using namespace lattice;

    template <Bravais bravais>
    Lattice make_lattice(math::Axis periodic = math::Axis::none,
        Parameters dimensions = Parameters::all(1600_pm), Indices orientation = Indices::unit_z())
    {
        auto unit_cell{ UnitCell::conventional<bravais>(400_pm).fill(chemistry::Atom{ Element::Au }) };
        Lattice result{ unit_cell, orientation };
        result.scale_shape(dimensions);
        result.set_periodic_boundaries(periodic);
        result.construct_sites();
        result.construct_neighbor_list();
        return result;
    }

    // Independent distance-based nearest-neighbor oracle for rectangular boxes.
    // It does not use Lattice::neighbors(), Lattice::displacement(), or cell lists.
    bool nearest_neighbor(Lattice const& lattice, Site const& a, Site const& b, double cutoff_squared)
    {
        if (&a == &b)
            return false;

        auto distance_squared{ 0.0 };
        for (std::size_t axis{}; axis < 3; ++axis)
        {
            auto delta{ b.position()[axis].value() - a.position()[axis].value() };
            if (static_cast<unsigned>(lattice.periodic_boundaries()) & (1u << axis))
            {
                auto const length{ lattice.shape().parameters()[axis].value() };
                delta -= std::round(delta / length) * length;
            }
            distance_squared += delta * delta;
        }

        return std::abs(distance_squared - cutoff_squared) < cutoff_squared * 1e-7;
    }

    void check_neighbor_graph(Lattice const& lattice, std::size_t expected_bulk_coordination = 0)
    {
        auto const& sites{ lattice.sites() };
        auto const& neighbors{ lattice.neighbors() };
        std::size_t offset{};
        auto const cutoff{ lattice.unit_cell().next_neighbor_distance().value() };
        auto const cutoff_squared{ cutoff * cutoff };

        for (std::size_t i{}; i < sites.size(); ++i)
        {
            auto const& site{ sites[i] };
            ut::expect(ut::eq(site.index(), static_cast<Index>(i))) << "site index";
            ut::expect(ut::eq(site.neighbor_index(), static_cast<Index>(offset)))
                << "contiguous neighbor offsets";
            offset += site.max_neighbors();

            std::vector<Index> actual;
            for (auto const* neighbor : lattice.neighbors(site))
            {
                auto const index{ neighbor->index() };
                actual.push_back(index);
                ut::expect(ut::that % index < sites.size()) << "neighbor index in range";
                ut::expect(ut::that % index != i) << "no self neighbors";
            }
            std::ranges::sort(actual);
            ut::expect(std::ranges::adjacent_find(actual) == actual.end())
                << "no duplicate neighbors";

            std::vector<Index> expected;
            for (std::size_t j{}; j < sites.size(); ++j)
                if (nearest_neighbor(lattice, site, sites[j], cutoff_squared))
                    expected.push_back(static_cast<Index>(j));

            ut::expect(std::ranges::equal(actual, expected))
                << "neighbor list matches independent geometric search";
            ut::expect(ut::eq(site.max_neighbors(), static_cast<Coordination>(expected.size())))
                << "coordination matches geometric graph";
            ut::expect(ut::eq(site.coordination(), site.max_neighbors()))
                << "all occupied sites have full occupied coordination";

            if (expected_bulk_coordination)
                ut::expect(ut::eq(expected.size(), expected_bulk_coordination))
                    << "periodic bulk coordination";
        }
        ut::expect(ut::eq(offset, neighbors.size())) << "neighbor storage length";
    }

    ut::suite<"lattice.geometry"> geometry_tests = [] {
        "fcc nonperiodic neighbors match independent geometric search"_test = [] {
            auto lattice{ make_lattice<Bravais::cF>() };
            ut::expect(ut::eq(lattice.sites().size(), std::size_t{ 256 }))
                << "4x4x4 conventional fcc cells";
            check_neighbor_graph(lattice);
        };

        "fully periodic fcc bcc and simple cubic have 12 8 and 6 neighbors"_test = [] {
            using enum math::Axis;
            auto const periodic{ x | y | z };
            check_neighbor_graph(make_lattice<Bravais::cF>(periodic), 12);
            check_neighbor_graph(make_lattice<Bravais::cI>(periodic), 8);
            check_neighbor_graph(make_lattice<Bravais::cP>(periodic), 6);
        };

        "partial periodic boundaries wrap only selected axes"_test = [] {
            using enum math::Axis;
            check_neighbor_graph(make_lattice<Bravais::cF>(x | y));
            check_neighbor_graph(make_lattice<Bravais::cF>(z));
        };

        "axis-permuted fcc supercells preserve geometric nearest neighbors"_test = [] {
            using enum math::Axis;
            check_neighbor_graph(make_lattice<Bravais::cF>(none,
                Parameters::all(1600_pm), Indices{ 1, 0, 0 }));
            check_neighbor_graph(make_lattice<Bravais::cF>(x | y | z,
                Parameters::all(1600_pm), Indices{ 1, 0, 0 }), 12);
        };

        "neighbor construction is not repeatable and too-small periodic boxes fail"_test = [] {
            auto lattice{ make_lattice<Bravais::cF>() };
            ut::expect(ut::throws<std::logic_error>([&] { lattice.construct_sites(); }));
            ut::expect(ut::throws<std::logic_error>([&] { lattice.construct_neighbor_list(); }));

            auto unit_cell{ UnitCell::conventional<Bravais::cF>(400_pm).fill(
                chemistry::Atom{ Element::Au }) };
            Lattice too_small{ unit_cell };
            too_small.set_periodic_boundaries(math::Axis::x);
            too_small.construct_sites();
            ut::expect(ut::throws<std::domain_error>([&] {
                too_small.construct_neighbor_list();
            })) << "periodic box length must exceed twice the cutoff";
        };
    };
}
