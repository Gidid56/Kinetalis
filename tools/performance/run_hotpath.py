#!/usr/bin/env python3
"""Benchmark independent hot-path ablations on the same runner and trajectory."""
import json
import math
from pathlib import Path
import statistics
import sys

from run_compare import benchmark, fmt_delta

binary = Path(sys.argv[1])
variants = {
    "baseline": binary,
    "reuse_total_rate": binary.with_name("kmc_benchmark_reuse_rate"),
    "skip_redundant_source": binary.with_name("kmc_benchmark_target_only"),
    "short_circuit_visit": binary.with_name("kmc_benchmark_short_visit"),
    "dedup_neighbor_visits": binary.with_name("kmc_benchmark_dedup_visits"),
    "skip_inactive_clear": binary.with_name("kmc_benchmark_skip_clears"),
}
scenarios = [(4.0, 1800), (6.0, 2200), (8.0, 3500)]
repetitions = 9
report_path = Path("performance-report.json")
report = json.loads(report_path.read_text())
report["hotpath_ablations"] = []

for size, steps in scenarios:
    print(f"\nHOTPATH {size}nm/{steps}steps", flush=True)
    samples = {name: [] for name in variants}
    errors = {}
    baseline = benchmark(binary, size, steps)
    for name, exe in variants.items():
        try:
            benchmark(exe, size, steps)  # warmup
        except Exception as e:
            errors[name] = str(e)
            print(f"  FAILED warmup for {name}: {e}", flush=True)

    for i in range(repetitions):
        names = list(variants) if i % 2 == 0 else list(reversed(variants))
        for name in names:
            if name in errors: continue
            row = benchmark(variants[name], size, steps)
            physical_fields = ("sites","accepted","fingerprint","rate_sum","simulation_time","rejection_fraction")
            if any(not math.isclose(row[field], baseline[field], rel_tol=1e-12, abs_tol=1e-12)
                   for field in physical_fields):
                errors[name] = f"state mismatch on sample {i+1} ({row})"
                print(f"  FAILED physical agreement for {name}", flush=True)
                continue
            samples[name].append(row)

    medians = {
        name: statistics.median(row["run_ms"] for row in rows)
        for name, rows in samples.items() if rows
    }
    assert "baseline" in medians
    for name in variants:
        if name in medians:
            print(f"  {name:24s} median={medians[name]:.4f} ms "
                  f"delta={fmt_delta(medians['baseline'], medians[name])}", flush=True)
        else:
            print(f"  {name:24s} invalid: {errors.get(name)}", flush=True)

    report["hotpath_ablations"].append({
        "size_nm": size,"steps": steps, "repetitions": repetitions,
        "medians_ms": medians,"errors": errors,"samples": samples
    })
    report_path.write_text(json.dumps(report, indent=2))
