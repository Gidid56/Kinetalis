#include "simulation/engine.h"

#include <array>
#include <bit>
#include <boost/ut.hpp>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    ut::suite<"simulation.engine"> engine_tests = [] {
        "rng"_test = [] {
            constexpr std::string_view seed{ "00000000-00000001-00000002-00000003-"
                                             "00000004-00000005-00000006-00000007" };

            simulation::Engine rng{ seed };
            constexpr std::array expected{
                UINT64_C(0x3e18000000000000),
                UINT64_C(0x3ee0008000100000),
                UINT64_C(0x3fb0016002100140),
                UINT64_C(0x3fed00a4409580e0),
                UINT64_C(0x3fe2091ba0428912),
            };

            for (auto expected_bits : expected)
                ut::expect(ut::eq(std::bit_cast<std::uint64_t>(rng.probability()), expected_bits));

            simulation::Engine first{ seed }, second{ seed };

            auto reproducible{ true };
            for (std::size_t i{}; i < 10'000; ++i)
                if (first.probability() != second.probability())
                {
                    reproducible = false;
                    break;
                }

            ut::expect(reproducible) << "RNG reproducibility";

            auto bounded{ true };
            for (std::size_t i{}; i < 10'000; ++i)
                if (simulation::Engine::State::value_type value{
                        first.integer(simulation::Engine::State::value_type{ 99 }) };
                    value > 99)
                {
                    bounded = false;
                    break;
                }

            ut::expect(bounded) << "RNG bounded integer";
        };

    };
}
