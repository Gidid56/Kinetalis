# Kinetalis benchmarks

The standalone `kinetalis_benchmarks` executable is intended for repeatable,
locally run performance measurements. It is separate from CTest and the unit
test suites, and requires no third-party benchmarking framework.

## Windows (MSVC Developer PowerShell)

```powershell
cmake --preset windows-benchmark-release
cmake --build --preset build-windows-benchmark-release
.\build\windows-benchmark-release\Release\bin\kinetalis_benchmarks.exe
```

You can also select **Windows | Benchmark | Release** in Visual Studio and
run `kinetalis_benchmarks` as the startup target.

## Current workload

The initial benchmark runs a small deterministic Ag–Au dealloying KMC
simulation with a fixed seed, periodic FCC boxes with side lengths of 10, 20, and 40 nm, and a
100,000-accepted-step limit for each box. It uses only a stopping observer and does not write measurement files.
It performs one untimed warm-up and five independently constructed, timed
simulations.

Measurements report three separate durations: (1) lattice geometry construction
(including site and neighbor-list construction), (2) complete simulation
initialization (which includes lattice construction, RNG-driven alloy filling,
kinetic-class setup, and initial rates), and (3) the `run()` call, including
observer initialization and finalization. The analysis plan is constructed
outside these intervals. The lattice time is a subset of total initialization,
not an additional cost to add on top. Reported throughput is
**accepted steps per second**, not selections per second. A run that exhausts
its active event list before the target fails rather than silently measuring a
different workload.

Results are printed for each box size (lattice-site count,
min/median/max lattice construction, total initialization, and execution times,
plus median accepted-step throughput). The 40 nm case may require substantial
memory and runtime, so run the executable on a machine with sufficient resources. For meaningful commit comparisons, benchmark on the
same machine and compiler settings with minimal competing load. These results
are diagnostic measurements, not CI pass/fail thresholds.
