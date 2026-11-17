#!/usr/bin/env python3
"""Compare controlled local enumeration ablations to unmodified refactor."""
from collections import defaultdict
import json
from pathlib import Path
import statistics
import sys

from run_compare import benchmark, fmt_delta

binary = Path(sys.argv[1])
scenarios = [(4.0, 1800), (6.0, 2200)]
variants = {
    "baseline": binary,
    "skip_inactive_transformations": binary.with_name("kmc_benchmark_skip_inactive"),
    "skip_zero_rate_addition": binary.with_name("kmc_benchmark_skip_zero"),
    "early_site_exit": binary.with_name("kmc_benchmark_early_exit"),
}
report_path = Path("performance-report.json")
report = json.loads(report_path.read_text())
report["ablations"] = []

for side, steps in scenarios:
    print(f"\nABLATIONS {side}nm/{steps}steps", flush=True)
    for exe in variants.values():
        benchmark(exe, side, steps)  # warmup

    results = defaultdict(list)
    for i in range(7):
        order = list(variants) if i % 2 == 0 else list(reversed(variants))
        for key in order:
            row = benchmark(variants[key], side, steps)
            results[key].append(row)

    base = results["baseline"][0]
    stats = {}
    for name, samples in results.items():
        for sample in samples:
            fields = ["sites", "accepted", "fingerprint", "rate_sum", "simulation_time", "rejection_fraction"]
            for field in fields:
                if sample[field] != base[field]:
                    raise RuntimeError(
                        f"Ablation '{name}' changes physical output: {field} "
                        f"{sample[field]} != {base[field]}"
                    )
        value = statistics.median(x["run_ms"] for x in samples)
        stats[name] = value
    for name, value in stats.items():
        print(f"  {name:31s} median={value:.4f} ms "
              f"delta_vs_refactor={fmt_delta(stats['baseline'], value)}", flush=True)
    report["ablations"].append({
        "side_nm": side, "steps": steps, "medians_ms": stats,
        "samples": dict(results)
    })

report_path.write_text(json.dumps(report, indent=2))
