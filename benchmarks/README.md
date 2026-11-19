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
simulation with a fixed seed, a 6 nm periodic FCC box, and a 1000-accepted-step
limit. It uses only a stopping observer and does not write measurement files.
It performs one untimed warm-up and five independently constructed, timed
simulations.

The simulation object, lattice, rate tables, and analysis plan are constructed
outside the timed interval. The measured duration includes the `run()` call
and its observer initialization and finalization. Reported throughput is
**accepted steps per second**, not selections per second. A run that exhausts
its active event list before the target fails rather than silently measuring a
different workload.

Results are printed to the console (min/median/max elapsed seconds and median
accepted-step throughput). For meaningful commit comparisons, benchmark on the
same machine and compiler settings with minimal competing load. These results
are diagnostic measurements, not CI pass/fail thresholds.
