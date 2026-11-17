#!/usr/bin/env python3
"""Prepare compile-time performance ablations in the CI checkout only."""
from pathlib import Path
import sys
path = Path(sys.argv[1])
s=path.read_text()
a='''        Rate sum{};
        bool has_events{};

        model_.transformations().for_each'''
b='''        Rate sum{};
        bool has_events{};

#ifdef KINETALIS_PERF_EARLY_SITE_EXIT
        bool any_candidate{};
        model_.transformations().for_each([&](auto const& transformation, std::size_t) {
            any_candidate |= transformation.is_source_candidate(site);
        });
        if (not any_candidate)
        {
            if (site.is_active())
                for (SlotId slot{}; slot < ModelType::slots_per_site; ++slot)
                    set_class_key(first + slot, RateClassKey{});
            site.set_active(false);
            return {};
        }
#endif

        model_.transformations().for_each'''
assert a in s
s=s.replace(a,b,1)
a='''            const bool source_candidate{ transformation.is_source_candidate(site) };

            for (SlotId local{}; local < count; ++local)'''
b='''            const bool source_candidate{ transformation.is_source_candidate(site) };

#ifdef KINETALIS_PERF_SKIP_INACTIVE
            if (not source_candidate)
            {
                for (SlotId local{}; local < count; ++local)
                    set_class_key(first + offset + local, RateClassKey{});
                return;
            }
#endif
            for (SlotId local{}; local < count; ++local)'''
assert a in s
s=s.replace(a,b,1)
a='''                has_events |= key != RateClassKey{};
                sum += class_rates_.at(key);'''
b='''                has_events |= key != RateClassKey{};
#ifdef KINETALIS_PERF_SKIP_ZERO
                if (key != RateClassKey{})
                    sum += class_rates_.at(key);
#else
                sum += class_rates_.at(key);
#endif'''
assert a in s
s=s.replace(a,b,1)
path.write_text(s)
print(f"Prepared early-exit and inactive/zero-slot ablations in {path}")
