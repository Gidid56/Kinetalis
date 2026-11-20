#include "lattice/lattice.h"

#include <algorithm>
#include <array>
#include <boost/ut.hpp>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <random>
#include <ranges>
#include <set>
#include <vector>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using namespace lattice;
    using chemistry::Element;

    Lattice make_fcc(Parameters dimensions = Parameters::all(1600_pm))
    {
        auto cell{ UnitCell::conventional<Bravais::cF>(400_pm).fill(
            chemistry::Atom{ Element::Au }) };
        Lattice lattice{ cell };
        lattice.scale_shape(dimensions);
        lattice.construct_sites();
        lattice.construct_neighbor_list();
        return lattice;
    }

    ut::suite<"lattice.ordering"> ordering_tests = [] {
        "hilbert ordering preserves every fcc position exactly once"_test = [] {
            auto lattice{ make_fcc() };
            constexpr int half_cells{ 8 };
            std::set<std::array<int, 3>> expected;
            for (int x{1}; x <= half_cells; ++x)
                for (int y{1}; y <= half_cells; ++y)
                    for (int z{1}; z <= half_cells; ++z)
                        if ((x + y + z) % 2 == 1)
                            expected.insert({ x, y, z });

            std::set<std::array<int, 3>> actual;
            for (auto const& site : lattice.sites())
            {
                std::array<int, 3> coordinates{};
                auto const position{ lattice.shape().coordinate(site.position()) };
                for (std::size_t axis{}; axis < 3; ++axis)
                {
                    auto const scaled{ position[axis] * half_cells };
                    auto const nearest{ std::round(scaled) };
                    ut::expect(ut::that % std::abs(scaled - nearest) < 1e-7)
                        << "fcc position lies on half-cell lattice";
                    coordinates[axis] = static_cast<int>(nearest);
                }
                actual.insert(coordinates);
            }
            ut::expect(ut::eq(actual.size(), lattice.sites().size()))
                << "no duplicate physical positions";
            ut::expect(actual == expected) << "complete fcc position set";
        };

        "hilbert ordering is deterministic across repeated constructions"_test = [] {
            auto first{ make_fcc() };
            auto second{ make_fcc() };
            ut::expect(ut::eq(first.sites().size(), second.sites().size()));
            for (std::size_t i{}; i < first.sites().size(); ++i)
            {
                auto const& a{ first.sites()[i] };
                auto const& b{ second.sites()[i] };
                ut::expect(a.position() == b.position()) << "position and index reproducibility";
                ut::expect(ut::eq(a.index(), b.index()));
                std::vector<Index> left, right;
                for (auto const* n : first.neighbors(a))
                    left.push_back(n->index());
                for (auto const* n : second.neighbors(b))
                    right.push_back(n->index());
                ut::expect(left == right) << "neighbor ordering reproducibility";
            }
        };

        "hilbert keeps neighboring sites closer than a shuffled index order"_test = [] {
            for (auto dimensions : { Parameters::all(1600_pm),
                     Parameters{ 1600_pm, 2400_pm, 800_pm } })
            {
                auto lattice{ make_fcc(dimensions) };
                std::vector<Index> randomized(lattice.sites().size());
                std::iota(randomized.begin(), randomized.end(), Index{});
                std::mt19937 generator{ 0x5A17u };
                std::ranges::shuffle(randomized, generator);

                std::uint64_t hilbert_distance{}, shuffled_distance{};
                for (auto const& site : lattice.sites())
                    for (auto const* neighbor : lattice.neighbors(site))
                    {
                        auto const i{ site.index() }, j{ neighbor->index() };
                        hilbert_distance += i > j ? i - j : j - i;
                        auto const ri{ randomized[i] }, rj{ randomized[j] };
                        shuffled_distance += ri > rj ? ri - rj : rj - ri;
                    }
                ut::expect(ut::that % hilbert_distance < shuffled_distance)
                    << "Hilbert ordering retains spatial locality";
            }
        };
    };
}
