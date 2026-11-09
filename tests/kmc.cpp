#include "models/composition_dependent.h"
#include "random/engine.h"

#include <array>
#include <bit>
#include <boost/ut.hpp>
#include <cstdint>
#include <string_view>

using boost::ut::suite;
using boost::ut::expect;
using boost::ut::eq;
using namespace boost::ut::literals;

namespace kmc
{
    suite<"kmc"> kmc_tests = [] {
        "rng"_test = [] {
            constexpr std::string_view seed{ "00000000-00000001-00000002-00000003-"
                                             "00000004-00000005-00000006-00000007" };

            random_::Engine rng{ seed };
            constexpr std::array expected{
                UINT64_C(0x3e18000000000000),
                UINT64_C(0x3ee0008000100000),
                UINT64_C(0x3fb0016002100140),
                UINT64_C(0x3fed00a4409580e0),
                UINT64_C(0x3fe2091ba0428912),
            };

            for (auto expected_bits : expected)
                expect(eq(std::bit_cast<std::uint64_t>(rng.probability()), expected_bits));

            random_::Engine first{ seed }, second{ seed };

            auto reproducible{ true };
            for (std::size_t i{}; i < 10'000; ++i)
                if (first.probability() != second.probability())
                {
                    reproducible = false;
                    break;
                }

            expect(reproducible) << "RNG reproducibility";

            auto bounded{ true };
            for (std::size_t i{}; i < 10'000; ++i)
                if (random_::Engine::state_word_type value{
                        first.integer(random_::Engine::state_word_type{ 99 }) };
                    value > 99)
                {
                    bounded = false;
                    break;
                }

            expect(bounded) << "RNG bounded integer";
        };

        "model"_test = [] {
            using enum Element;

            static_assert(models::bond_pair(Ag, Au) == models::bond_pair(Au, Ag));

            expect(boost::ut::nothrow([] {
                models::CompositionDependent model;
                model.set_potential(1.0_V);
                model.set_temperature(300_K);
            }));
        };
    };
}
