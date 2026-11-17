#!/usr/bin/env python3
"""Paired A/B timings and nested phase attribution on a single GitHub runner."""
import json
import math
import os
import re
import statistics
import subprocess
import sys
from pathlib import Path

MAIN = Path(sys.argv[1])
REFACTOR = Path(sys.argv[2])
SIZES = [(4.0, 1800), (6.0, 2200)]
REPEATS = 7
REPORT = {"environment": {}, "scenarios": []}


def benchmark(executable: Path, size: float, steps: int) -> dict:
    result = subprocess.run(
        ["/usr/bin/time", "-f", "MEMORY_MAX_RSS_KB=%M", str(executable), str(size), str(steps)],
        capture_output=True, text=True, check=False
    )
    if result.returncode:
        raise RuntimeError(
            f"Benchmark exited {result.returncode} ({executable}):\n"
            f"{result.stdout[-4000:]}\n{result.stderr[-4000:]}"
        )

    lines = result.stdout.splitlines()
    line = next((x for x in lines if x.startswith("RESULT ")), None)
    if line is None:
        raise RuntimeError(f"No RESULT line: {result.stdout[-2000:]}")
    row = dict(x.split("=", 1) for x in line.removeprefix("RESULT ").split())
    row["run_ms"] = float(row["run_ms"])
    row["init_ms"] = float(row["init_ms"])
    row["rate_sum"] = float(row["rate_sum"])
    row["simulation_time"] = float(row["simulation_time"])
    row["accepted"] = int(row["accepted"])
    row["sites"] = int(row["sites"])
    row["fingerprint"] = int(row["fingerprint"])
    row["rejection_fraction"] = float(row["rejection_fraction"])
    match = re.search(r"MEMORY_MAX_RSS_KB=(\d+)", result.stderr)
    row["rss_kb"] = int(match.group(1)) if match else None

    row["trace"] = {}
    for line in lines:
        if line.startswith("TRACE "):
            fields = dict(x.split("=", 1) for x in line.removeprefix("TRACE ").split())
            row["trace"][fields["stage"]] = {"calls": int(fields["calls"]), "ms": int(fields["ns"]) / 1e6}
    return row


def compare_state(a: dict, b: dict, scenario: str) -> dict:
    matched = {
        "sites": a["sites"] == b["sites"],
        "accepted": a["accepted"] == b["accepted"],
        "lattice_fingerprint": a["fingerprint"] == b["fingerprint"],
        "rate_sum": math.isclose(a["rate_sum"], b["rate_sum"], rel_tol=1e-9, abs_tol=1e-9),
        "simulation_time": math.isclose(a["simulation_time"], b["simulation_time"], rel_tol=1e-9, abs_tol=1e-9),
        "rejections": math.isclose(a["rejection_fraction"], b["rejection_fraction"], abs_tol=1e-12)
    }
    print(f"  State agreement for {scenario}: {matched}", flush=True)
    if not all(matched.values()):
        print("  WARNING: behavioral divergence; compare throughput only with caution.", flush=True)
    return matched


def median(rows: list, key: str) -> float:
    return statistics.median(row[key] for row in rows)


def fmt_delta(old: float, new: float) -> str:
    return f"{(new / old - 1) * 100:+.2f}%" if old else "N/A"


def run():
    REPORT["environment"] = {
        "main_executable": str(MAIN), "refactor_executable": str(REFACTOR),
        "repeat_count": REPEATS,
        "cpu_count": os.cpu_count()
    }
    print("Paired release benchmarks on one machine. First run for each binary is a warmup.", flush=True)

    for size, steps in SIZES:
        scenario = f"{size}nm/{steps}steps"
        print(f"\nSCENARIO {scenario}", flush=True)
        variants = {"main": MAIN, "refactor": REFACTOR}
        for key, exe in variants.items():
            benchmark(exe, size, steps)

        samples = {"main": [], "refactor": []}
        for i in range(REPEATS):
            order = ("main", "refactor") if i % 2 == 0 else ("refactor", "main")
            for key in order:
                row = benchmark(variants[key], size, steps)
                samples[key].append(row)
                print(f"  sample {i + 1} {key:8s} run={row['run_ms']:.3f} ms "
                      f"init={row['init_ms']:.3f} ms rss={row['rss_kb']} KB", flush=True)

        medians = {
            key: {field: median(samples[key], field)
                  for field in ("run_ms", "init_ms", "rss_kb")}
            for key in variants
        }
        state = compare_state(samples["main"][0], samples["refactor"][0], scenario)
        print("  Median:", flush=True)
        for field in ("init_ms", "run_ms", "rss_kb"):
            old, new = medians["main"][field], medians["refactor"][field]
            units = "KB" if field == "rss_kb" else "ms"
            print(f"    {field:8s} main={old:.3f} {units} refactor={new:.3f} {units} "
                  f"delta={fmt_delta(old, new)}", flush=True)

        # Separate instrumented builds expose nested stages. They are NOT used
        # for throughput numbers because instrumentation itself has overhead.
        traces = {
            "main": benchmark(MAIN.with_name("kmc_benchmark_trace"), size, steps),
            "refactor": benchmark(REFACTOR.with_name("kmc_benchmark_trace"), size, steps)
        }
        print("  Instrumented stages (nested: update contains enumerate/classify/block):", flush=True)
        for stage in ("select", "evaluate", "update", "enumerate", "classify", "block"):
            a, b = traces["main"]["trace"][stage], traces["refactor"]["trace"][stage]
            print(f"    {stage:9s} main={a['ms']:.3f} ms/{a['calls']} calls "
                  f"refactor={b['ms']:.3f} ms/{b['calls']} calls "
                  f"delta={fmt_delta(a['ms'], b['ms'])}", flush=True)

        REPORT["scenarios"].append({
            "size_nm": size, "steps": steps, "samples": samples,
            "medians": medians, "state_agreement": state, "instrumented": traces
        })

    Path("performance-report.json").write_text(json.dumps(REPORT, indent=2))
    print("\nReport written to performance-report.json", flush=True)


if __name__ == "__main__":
    run()
