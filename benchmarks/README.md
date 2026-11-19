# Kinetalis benchmarks

The standalone `kinetalis_benchmarks` executable measures KMC performance
independently of the unit tests and CTest, using a Release-optimized build and
no third-party benchmarking framework.

## Run on Windows

Select **Windows | Benchmark | Release** in Visual Studio and run the
`kinetalis_benchmarks` startup target, or use an MSVC Developer PowerShell:

```powershell
cmake --preset windows-benchmark-release
cmake --build --preset build-windows-benchmark-release
.\build\windows-benchmark-release\Release\bin\kinetalis_benchmarks.exe
```

## Benchmark workload

The current benchmark runs a deterministic Ag–Au dealloying simulation with:

- Periodic FCC boxes of **5, 10, and 20 nm** side length
- **100,000 accepted KMC steps** per measured run, with only a step-limit observer
- One untimed **1,000-step warm-up** in a 5 nm box before all measurements
- **Five measured runs per box size**, using the same fixed RNG seed
- Alloy composition **10% Au** and model conditions **300 K, 0.86 V**
- No output files or snapshot writers

The benchmark separately times **lattice construction** (geometry, sites, and
neighbor lists), **total simulation initialization** (including lattice
construction, alloy filling, rate classes, and initial rates), and **KMC
execution** (`simulation.run()`). The analysis plan is constructed outside the
timed intervals. Lattice construction is **part of**, not additional to, total
initialization. The benchmark fails if it does not reach the requested accepted
step count.

Reported values are **arithmetic mean ± sample standard deviation** across
the five measured runs. Warm-up measurements are excluded. KMC time per step
is calculated individually from execution time divided by the number of
**accepted** events, in nanoseconds; it includes the computational cost of
any rejected selections. It is not a timing per attempted selection.

## Reference measurements

Results supplied from a local **Windows | Benchmark | Release** run:

| Box side | Lattice sites | Lattice construction (s) | Total simulation initialization (s) | KMC execution (s) | KMC time/accepted step (ns) |
| ---: | ---: | ---: | ---: | ---: | ---: |
| 5 nm | 8,788 | 0.0066 ± 0.0003 | 0.0069 ± 0.0002 | 0.0874 ± 0.0006 | **874.4 ± 6.3** |
| 10 nm | 62,500 | 0.0435 ± 0.0008 | 0.0458 ± 0.0009 | 0.0878 ± 0.0008 | **878.1 ± 7.8** |
| 20 nm | 500,000 | 0.3356 ± 0.0013 | 0.3553 ± 0.0020 | 0.0900 ± 0.0006 | **900.2 ± 5.8** |

### Hardware

| Property | Reported configuration |
| --- | --- |
| Processor | 13th Gen Intel Core i7-13700KF |
| Base clock | 3.40 GHz |
| Cores / logical processors | 16 / 24 (one socket) |
| Cache | L1: 1.4 MB; L2: 24.0 MB; L3: 30.0 MB |
| Observed CPU speed | 5.31 GHz (Task Manager snapshot, not a fixed benchmark clock) |
| CPU utilization at snapshot | 5% |
| Virtualization | Enabled |

The processor's reported speed and utilization are snapshots rather than
controlled benchmark conditions. The exact MSVC compiler version, Windows
version, CPU affinity, and effective core frequency during the timed runs
were not recorded. The measurements are single-simulation,
single-thread execution, not a multi-core scaling benchmark.

## Interpretation and reproducibility

The measured cost rises from 874.4 to 900.2 ns per accepted step as the site
count increases from 8,788 to 500,000. This describes the **specific fixed-step
dealloying workload**, not an isolated complexity measurement of the event
selector: different lattice sizes may have different proportions of diffusion,
dissolution, and rejected events.

For meaningful comparisons between commits or implementations, use the same
hardware, compiler settings, fixed seed, and physical workload, and minimize
competing system activity. These are diagnostic measurements, not CI pass/fail
thresholds. See the [main README](../README.md) for project context.
