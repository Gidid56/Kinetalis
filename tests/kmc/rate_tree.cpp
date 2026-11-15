#include "kmc/types.h"
#include "lattice/types.h"

#include <algorithm>
#include <array>
#include <bit>
#include <boost/ut.hpp>
#include <cstddef>
#include <cstdint>
#include <random>
#include <ranges>
#include <span>
#include <vector>

#include "kmc/rate_tree.h"

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using kmc::Rate;
    using kmc::rate_tree::BlockUpdate;
    using kmc::rate_tree::Index;

    constexpr auto block_size{ std::size_t{ 1 } << kmc::rate_tree::baseLevel };

    // Independent reference: store the block rates and reduce a complete binary tree
    // bottom-up. This preserves the production tree's summation order without using
    // its recursive implementation.
    struct ReferenceTree
    {
        explicit ReferenceTree(std::size_t sites)
            : blocks((sites + block_size - 1) / block_size)
        { }

        void set(Index site_index, Rate rate)
        {
            blocks.at(site_index / block_size) = rate;
        }

        Rate sum() const
        {
            if (blocks.empty())
                return {};

            std::vector<Rate> level(std::bit_ceil(blocks.size()));
            std::ranges::copy(blocks, level.begin());

            for (auto count{ level.size() }; count > 1; count /= 2)
                for (std::size_t i{}; i < count / 2; ++i)
                    level[i] = level[2 * i] + level[2 * i + 1];

            return level.front();
        }

        std::vector<Rate> blocks;
    };

    void expect_consistent(kmc::rate_tree::Interface const& tree,
        ReferenceTree const& reference, char const* context)
    {
        ut::expect(ut::eq(tree.rate_sum().value(), reference.sum().value()))
            << context << ": root rate";

        for (std::size_t block{}; block < reference.blocks.size(); ++block)
        {
            auto const index{ static_cast<Index>(block * block_size) };
            ut::expect(ut::eq(tree.block_rate(index).value(), reference.blocks[block].value()))
                << context << ": block " << block;
        }
    }

    ut::suite<"kmc.rate_tree"> rate_tree_tests = [] {
        "constructs empty and partially filled trees at boundary sizes"_test = [] {
            constexpr std::array site_counts{
                0u, 1u, 15u, 16u, 17u, 31u, 32u, 33u, 63u, 64u, 65u,
                255u, 256u, 257u, 4095u, 4096u, 4097u, 8193u
            };

            for (auto sites : site_counts)
            {
                auto tree{ kmc::rate_tree::make_entry(sites) };
                ReferenceTree reference{ sites };
                expect_consistent(*tree, reference, "initially empty");

                tree->update_rates(std::span<const BlockUpdate>{});
                expect_consistent(*tree, reference, "empty update");

                if (sites == 0)
                    continue;

                // The index may refer to any site inside the 16-site block.
                tree->update_rate(static_cast<Index>(sites - 1), Rate{ 3.0 });
                reference.set(static_cast<Index>(sites - 1), Rate{ 3.0 });
                expect_consistent(*tree, reference, "last (possibly incomplete) block");

                tree->update_rate(0, Rate{ 2.0 });
                reference.set(0, Rate{ 2.0 });
                expect_consistent(*tree, reference, "first block");
            }
        };

        "single-block changes match an independent bottom-up reduction"_test = [] {
            for (auto sites : { 17u, 64u, 257u, 4097u })
            {
                auto tree{ kmc::rate_tree::make_entry(sites) };
                ReferenceTree reference{ sites };

                for (std::size_t i{}; i < reference.blocks.size(); ++i)
                {
                    auto const index{ static_cast<Index>(i * block_size) };
                    Rate const rate{ static_cast<double>((i % 9) + 1) / 8.0 };
                    tree->update_rate(index, rate);
                    reference.set(index, rate);
                }
                expect_consistent(*tree, reference, "all blocks set");

                for (std::size_t i{ reference.blocks.size() }; i-- > 0;)
                {
                    auto const index{ static_cast<Index>(i * block_size) };
                    tree->update_rate(index, {});
                    reference.set(index, {});
                }
                expect_consistent(*tree, reference, "all blocks reset in reverse order");
            }
        };

        "batch updates modify only the addressed blocks"_test = [] {
            constexpr auto sites{ 16u * 21u + 3u };
            auto tree{ kmc::rate_tree::make_entry(sites) };
            ReferenceTree reference{ sites };

            std::vector<BlockUpdate> const initial{
                { 0u, Rate{ 2.0 } },
                { 16u, Rate{ 1.0 } },
                { 32u, Rate{ 4.0 } },
                { 160u, Rate{ 8.0 } },
                { 336u, Rate{ 3.0 } },
            };
            tree->update_rates(initial);
            for (auto const& update : initial)
                reference.set(update.index, update.rate);
            expect_consistent(*tree, reference, "sparse batch across subtrees");

            std::vector<BlockUpdate> const changes{
                { 0u, Rate{} },
                { 16u, Rate{ 16.0 } },
                { 32u, Rate{ 2.0 } },
                { 48u, Rate{ 1.0 } },
                { 336u, Rate{} },
            };
            tree->update_rates(changes);
            for (auto const& update : changes)
                reference.set(update.index, update.rate);
            expect_consistent(*tree, reference, "overlapping batch and zero transitions");

            // A single-entry batch and the empty batch are both valid.
            std::array const single{ BlockUpdate{ 16u, Rate{ 7.0 } } };
            tree->update_rates(single);
            reference.set(single.front().index, single.front().rate);
            tree->update_rates(std::span<const BlockUpdate>{});
            expect_consistent(*tree, reference, "single and empty batches");
        };

        "single and recursive batch updates remain exactly equivalent"_test = [] {
            constexpr std::array values{
                0.0, 0.000000000001, 0.0625, 0.125, 0.5, 1.0, 2.0, 16.0, 4096.0
            };

            for (auto sites : { 17u, 257u, 4097u, 8193u })
            {
                auto singles{ kmc::rate_tree::make_entry(sites) };
                auto batched{ kmc::rate_tree::make_entry(sites) };
                ReferenceTree reference{ sites };
                std::mt19937_64 rng{ 0x6b6d635f74726565ull + sites };

                for (std::size_t step{}; step < 3000; ++step)
                {
                    // Model the caller's contract: indices sorted, no duplicates,
                    // and each rate is the final sum of its block.
                    std::vector<BlockUpdate> updates;
                    for (std::size_t j{}, count{ rng() % 9 }; j < count; ++j)
                    {
                        auto const index{ static_cast<Index>(
                            (rng() % reference.blocks.size()) * block_size) };
                        Rate const rate{ values[rng() % values.size()] };
                        auto const found{ std::ranges::find(updates, index, &BlockUpdate::index) };
                        if (found == updates.end())
                            updates.push_back({ index, rate });
                        else
                            found->rate = rate;
                    }
                    std::ranges::sort(updates, {}, &BlockUpdate::index);

                    for (auto const& update : updates)
                    {
                        singles->update_rate(update.index, update.rate);
                        reference.set(update.index, update.rate);
                    }
                    batched->update_rates(updates);

                    ut::expect(ut::eq(singles->rate_sum().value(), batched->rate_sum().value()))
                        << "single/batch root, size " << sites << ", step " << step;
                    ut::expect(ut::eq(batched->rate_sum().value(), reference.sum().value()))
                        << "reference root, size " << sites << ", step " << step;

                    if (step % 97 == 0)
                    {
                        expect_consistent(*singles, reference, "single-update checkpoint");
                        expect_consistent(*batched, reference, "batch-update checkpoint");
                    }
                }
                expect_consistent(*singles, reference, "final single-update state");
                expect_consistent(*batched, reference, "final batch-update state");
            }
        };

        "sampling selects the correct nonzero interval and residual"_test = [] {
            constexpr auto sites{ 16u * 65u - 3u };
            auto tree{ kmc::rate_tree::make_entry(sites) };
            ReferenceTree reference{ sites };

            for (std::size_t i{}; i < reference.blocks.size(); ++i)
            {
                auto const index{ static_cast<Index>(i * block_size) };
                Rate const rate{ i % 7 == 0 ? 0.0 : static_cast<double>(i % 4 + 1) };
                tree->update_rate(index, rate);
                reference.set(index, rate);
            }
            expect_consistent(*tree, reference, "sampling setup");

            double prefix{};
            for (std::size_t block{}; block < reference.blocks.size(); ++block)
            {
                auto const amount{ reference.blocks[block].value() };
                if (amount == 0.0)
                    continue;

                for (auto fraction : { 0.0, 0.5 })
                {
                    auto selection{ Rate{ prefix + amount * fraction } };
                    auto const selected_index{ tree->index(selection) };
                    ut::expect(ut::eq(selected_index, static_cast<Index>(block * block_size)))
                        << "selected block " << block << ", fraction " << fraction;
                    ut::expect(ut::eq(selection.value(), amount * fraction))
                        << "remaining rate in block " << block << ", fraction " << fraction;
                }
                prefix += amount;
            }
            ut::expect(ut::eq(prefix, tree->rate_sum().value())) << "sampling total";
        };

        "high dynamic range and zero-rate transitions preserve tree sums"_test = [] {
            auto singles{ kmc::rate_tree::make_entry(17u * 16u) };
            auto batched{ kmc::rate_tree::make_entry(17u * 16u) };
            ReferenceTree reference{ 17u * 16u };
            std::array const values{
                1e-200, 1e200, 1e-150, 1e150, 0.0, 1e-12, 1e12, 0.125
            };

            std::vector<BlockUpdate> updates;
            for (std::size_t i{}; i < values.size(); ++i)
                updates.push_back({ static_cast<Index>(i * block_size), Rate{ values[i] } });
            batched->update_rates(updates);
            for (auto const& update : updates)
            {
                singles->update_rate(update.index, update.rate);
                reference.set(update.index, update.rate);
            }
            expect_consistent(*singles, reference, "wide range single updates");
            expect_consistent(*batched, reference, "wide range batch updates");

            // Removing the dominant rate must not leave a stale contribution.
            singles->update_rate(16u, {});
            reference.set(16u, {});
            std::array const remove{ BlockUpdate{ 16u, Rate{} } };
            batched->update_rates(remove);
            expect_consistent(*singles, reference, "remove dominant rate (single)");
            expect_consistent(*batched, reference, "remove dominant rate (batch)");
        };
    };
}
