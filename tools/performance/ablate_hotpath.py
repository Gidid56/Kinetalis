#!/usr/bin/env python3
"""Add isolated compile-time hotpath ablations to an ephemeral CI checkout."""
from pathlib import Path
import sys

root = Path(sys.argv[1])


def patch(relpath: str, old: str, new: str):
    p=root/relpath
    s=p.read_text()
    if old not in s:
        raise ValueError(f"Cannot patch {relpath}: {old[:180]}")
    p.write_text(s.replace(old,new,1))

p="source/kmc/simulation.h"
patch(p, "    Selection select_random_location();", """    Selection select_random_location(
#ifdef KMC_PERF_REUSE_TOTAL_RATE
        Rate total_rate
#endif
    );""")
patch(p, "    std::vector<rate_tree::BlockUpdate> block_updates_;", """    std::vector<rate_tree::BlockUpdate> block_updates_;
#ifdef KMC_PERF_DEDUP_VISITS
    std::vector<lattice::Site*> affected_sites_;
#endif""")

p="source/kmc/simulation_impl.hpp"
patch(p, "            auto const selection{ select_random_location() };", """#ifdef KMC_PERF_REUSE_TOTAL_RATE
            auto const selection{ select_random_location(total_rate) };
#else
            auto const selection{ select_random_location() };
#endif""")
patch(p, """    RejectionTreeSimulation<ModelType>::select_random_location()
    {""", """    RejectionTreeSimulation<ModelType>::select_random_location(
#ifdef KMC_PERF_REUSE_TOTAL_RATE
        Rate total_rate
#endif
    )
    {""")
patch(p, """        auto selected_rate{ physics::ensure_non_negative(rate_tree_->rate_sum() * rng_.probability()) };""", """#ifdef KMC_PERF_REUSE_TOTAL_RATE
        auto selected_rate{ physics::ensure_non_negative(total_rate * rng_.probability()) };
#else
        auto selected_rate{ physics::ensure_non_negative(rate_tree_->rate_sum() * rng_.probability()) };
#endif""")
patch(p, """        // First invalidate the entire region, then recalculate. Recomputing a
        // source clears its dirty flag, so overlapping walks are inexpensive.
        auto invalidate = [](lattice::Site& site) { site.set_dirty(true); };
        for (auto* site : record.sites())
            visit_neighbor_tree(*site, radius, invalidate);

        auto recalculate = [this](lattice::Site& site) { update(site); };
        for (auto* site : record.sites())
            visit_neighbor_tree(*site, radius, recalculate);""", """#ifdef KMC_PERF_DEDUP_VISITS
        affected_sites_.clear();
        auto invalidate = [this](lattice::Site& site) {
            if (site.is_dirty()) return;
            site.set_dirty(true);
            affected_sites_.push_back(&site);
        };
        for (auto* site : record.sites())
            visit_neighbor_tree(*site, radius, invalidate);
        for (auto* site : affected_sites_)
            update(*site);
#else
        // First invalidate the entire region, then recalculate. Recomputing a
        // source clears its dirty flag, so overlapping walks are inexpensive.
        auto invalidate = [](lattice::Site& site) { site.set_dirty(true); };
        for (auto* site : record.sites())
            visit_neighbor_tree(*site, radius, invalidate);

        auto recalculate = [this](lattice::Site& site) { update(site); };
        for (auto* site : record.sites())
            visit_neighbor_tree(*site, radius, recalculate);
#endif""")
patch(p, """            if (not transformation.is_source_candidate(site))
            {
                for (SlotId local{}; local < count; ++local)
                    set_class_key(first + offset + local, RateClassKey{});
                return;
            }""", """            if (not transformation.is_source_candidate(site))
            {
#ifdef KMC_PERF_SKIP_INACTIVE_CLEARS
                if (site.is_active())
#endif
                    for (SlotId local{}; local < count; ++local)
                        set_class_key(first + offset + local, RateClassKey{});
                return;
            }""")

p="source/kmc/event.h"
patch(p, """        void visit(std::size_t slot, Function&& function) const
        {
            bool found{};
            for_each([&](auto const& transformation, std::size_t offset) {
                constexpr auto count{ std::remove_cvref_t<decltype(transformation)>::slot_count };
                if (slot >= offset and slot - offset < count)
                {
                    std::invoke(function, transformation, slot - offset);
                    found = true;
                }
            });
            if (not found)
                throw std::out_of_range{ "Event slot outside transformation layout." };
        }""", """        void visit(std::size_t slot, Function&& function) const
        {
#ifdef KMC_PERF_SHORT_VISIT
            std::size_t offset{};
            const bool found = std::apply([&](auto const&... transformations) {
                return (([&]() -> bool {
                    constexpr auto count{ std::remove_cvref_t<decltype(transformations)>::slot_count };
                    const auto first{ std::exchange(offset, offset + count) };
                    if (slot < first or slot - first >= count)
                        return false;
                    std::invoke(function, transformations, slot - first);
                    return true;
                }()) or ...);
            }, transformations_);
#else
            bool found{};
            for_each([&](auto const& transformation, std::size_t offset) {
                constexpr auto count{ std::remove_cvref_t<decltype(transformation)>::slot_count };
                if (slot >= offset and slot - offset < count)
                {
                    std::invoke(function, transformation, slot - offset);
                    found = true;
                }
            });
#endif
            if (not found)
                throw std::out_of_range{ "Event slot outside transformation layout." };
        }""")

p="studies/dealloying/events.h"
patch(p, """            return location.sites().size() == 1 and is_source_candidate(location.source());""", """#ifdef KMC_PERF_TARGET_ONLY_CANDIDATE
            return location.sites().size() == 1;
#else
            return location.sites().size() == 1 and is_source_candidate(location.source());
#endif""")
patch(p, """            return location.sites().size() == 2 and is_source_candidate(location.source()) and
                   location.sites()[1]->is_vacant();""", """#ifdef KMC_PERF_TARGET_ONLY_CANDIDATE
            return location.sites().size() == 2 and location.sites()[1]->is_vacant();
#else
            return location.sites().size() == 2 and is_source_candidate(location.source()) and
                   location.sites()[1]->is_vacant();
#endif""")
print("Injected five isolated hotpath benchmarks, with default behavior unchanged.")
