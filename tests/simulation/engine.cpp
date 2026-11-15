#include "simulation/engine.h"
#include "chemistry/composition.h"
#include "lattice/shape.h"

#include <array>
#include <bit>
#include <boost/ut.hpp>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <string>
#include <string_view>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using simulation::Engine;
    constexpr std::string_view seed_text{ "00000000-00000001-00000002-00000003-"
                                          "00000004-00000005-00000006-00000007" };

    ut::suite<"simulation.engine"> engine_tests = [] {
        "seed parsing and formatting round-trip"_test = [] {
            auto const seed{ Engine::parse_seed(seed_text) };
            for (std::size_t i{}; i < seed.size(); ++i)
                ut::expect(ut::eq(seed[i], static_cast<simulation::Seed::value_type>(i)));

            ut::expect(Engine::seed_to_string(seed) == seed_text);
            Engine rng{ seed_text };
            ut::expect(rng.seed() == seed);
        };

        "invalid and all-zero seeds are rejected"_test = [] {
            ut::expect(ut::throws<std::invalid_argument>([] {
                (void)Engine::parse_seed("invalid");
            }));

            auto malformed{ std::string{ seed_text } };
            malformed[8] = ':';
            ut::expect(ut::throws<std::invalid_argument>([&] {
                (void)Engine::parse_seed(malformed);
            }));

            malformed = seed_text;
            malformed[2] = 'g';
            ut::expect(ut::throws<std::invalid_argument>([&] {
                (void)Engine::parse_seed(malformed);
            }));

            ut::expect(ut::throws<std::invalid_argument>([] {
                Engine rng{ simulation::Seed{} };
            }));
        };

        "probability reproduces the reference xoshiro256+ sequence"_test = [] {
            Engine rng{ seed_text };
            constexpr std::array expected{
                UINT64_C(0x3e18000000000000),
                UINT64_C(0x3ee0008000100000),
                UINT64_C(0x3fb0016002100140),
                UINT64_C(0x3fed00a4409580e0),
                UINT64_C(0x3fe2091ba0428912),
            };

            for (auto bits : expected)
                ut::expect(ut::eq(std::bit_cast<std::uint64_t>(rng.probability()), bits));
        };

        "identical seeds generate reproducible independent streams"_test = [] {
            Engine first{ seed_text }, second{ seed_text };
            for (std::size_t i{}; i < 10'000; ++i)
                ut::expect(first.probability() == second.probability())
                    << "sample " << i;

            Engine seed_first{ seed_text }, seed_second{ seed_text };
            ut::expect(seed_first.generate_seed() == seed_second.generate_seed());
        };

        "bounded integers include both limits and remain in range"_test = [] {
            Engine rng{ seed_text };
            for (std::size_t i{}; i < 10'000; ++i)
            {
                ut::expect(ut::eq(rng.integer(std::uint64_t{ 0 }), std::uint64_t{ 0 }));
                ut::expect(ut::that % rng.integer(std::uint64_t{ 99 }) <= 99u);
            }

            Engine full_range{ seed_text }, reference{ seed_text };
            auto const full_word{
                full_range.integer(std::numeric_limits<std::uint64_t>::max()) };
            auto const expected{ reference.probability().value() };
            ut::expect(ut::eq(static_cast<double>(full_word >> 11) * 0x1.0p-53, expected));
        };

        "element sampling handles empty and single-species compositions"_test = [] {
            Engine rng{ seed_text };
            chemistry::Composition const empty;
            ut::expect(ut::throws<std::logic_error>([&] { (void)rng.element(empty); }));

            chemistry::Composition const silver{ { chemistry::Element::Ag, 3 } };
            for (std::size_t i{}; i < 100; ++i)
                ut::expect(rng.element(silver) == chemistry::Element::Ag);
        };

        "random directions are normalized and positions stay inside the shape"_test = [] {
            Engine rng{ seed_text };
            auto const cube{ lattice::shape::cube(1_nm) };
            for (std::size_t i{}; i < 100; ++i)
            {
                auto const direction{ rng.direction() };
                ut::expect(ut::that % std::abs(direction.norm() - 1.0) < 1E-12);
                ut::expect(cube.contains(rng.position(cube)));
            }
        };

        "Miller indices are bounded, nonzero and primitive"_test = [] {
            Engine rng{ seed_text };
            ut::expect(ut::throws<std::invalid_argument>([&] {
                (void)rng.miller_indices(0);
            }));
            ut::expect(ut::throws<std::invalid_argument>([&] {
                (void)rng.miller_indices(-2);
            }));

            for (auto max_index : { 1, 3, 5 })
                for (std::size_t i{}; i < 100; ++i)
                {
                    auto const indices{ rng.miller_indices(max_index) };
                    auto const x{ std::abs(indices.x()) };
                    auto const y{ std::abs(indices.y()) };
                    auto const z{ std::abs(indices.z()) };
                    ut::expect(ut::that % x <= max_index);
                    ut::expect(ut::that % y <= max_index);
                    ut::expect(ut::that % z <= max_index);
                    ut::expect(ut::that % (x + y + z) > 0);
                    ut::expect(ut::eq(std::gcd(std::gcd(x, y), z), 1));
                }
        };
    };
}
