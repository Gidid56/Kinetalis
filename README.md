# Kinetalis

**Kinetalis** is a C++23 framework for lattice-based kinetic Monte Carlo (KMC) simulations. It combines a configurable crystal-lattice representation, continuous-time stochastic event dynamics, physical rate models, and a flexible system for measurements and automated studies.

The framework was developed for atomistic studies of electrochemical dealloying, but its lattice construction, numerical and physical utilities, KMC machinery, and batch processing can be used independently of that application. The dealloying study provides a complete example of how to assemble these components into a research workflow.

Two applications use the same simulation core:

- **`kinetalis_batch`** runs parameter studies and independent replicates, writes numerical results, and performs campaign-level postprocessing.
- **`kinetalis_viewer`** runs and displays simulations interactively, with camera controls and live inspection of the atomic configuration.

The **core library and batch runner require no separately installed third-party libraries**: supporting headers, including `magic_enum`, are kept in the repository. Unit tests use the vendored, header-only **Boost.UT** (`external/boost/ut.hpp`); no Boost package or Visual Studio test adapter is needed. The optional viewer uses graphical libraries supplied as Git submodules.

## Build requirements

- **C++23 compiler and standard library**, including support for `<format>`, `<print>`, and ranges.
- **CMake 3.25+**, **Ninja**, and **Git**.
- **Optional:** `clang-tidy` for static-analysis-enabled presets; an OpenGL-capable system and the Git submodules for the graphical viewer.

The core and batch application need no separately installed C++ libraries. The unit tests use the bundled Boost.UT header (`external/boost/ut.hpp`); the viewer's graphical dependencies are supplied as submodules. See [Get started](#get-started) for platform-specific toolchains, Visual Studio components, and build presets.

## Get started

For the batch runner, a normal clone is sufficient. Graphical submodules are only needed when building the viewer.

```sh
git clone https://github.com/Gidid56/Kinetalis.git
cd Kinetalis
```

### A. Windows: Visual Studio 2026

1. In **Visual Studio Installer**, select the **Desktop development with C++** workload. Under **Individual components**, make sure the following are installed:
   - **MSVC x64/x86 build tools** and a **Windows 10/11 SDK**.
   - **C++ CMake tools for Windows** (CMake and Ninja support).
   - **C++ Clang tools for Windows** (provides `clang-tidy`), required by the `tests-debug` and `viewer-debug` presets.
2. Open the repository with **File → Open → Folder**, selecting the folder containing the root `CMakeLists.txt`.
3. Select the **`batch-release`** CMake configure preset.
4. Build the **`kinetalis_batch`** target, then run it from Visual Studio or a Developer PowerShell.

Equivalent commands from an **MSVC Developer PowerShell**:

```powershell
cmake --preset batch-release
cmake --build --preset build-batch-release
.\build\batch-release\Release\bin\kinetalis_batch.exe --help
```

Visual Studio opens Kinetalis as a CMake folder; no Visual Studio solution file is required. Clang tools are used for **static analysis**, not as a replacement for MSVC. The release presets do not enable `clang-tidy`; the `tests-debug` and `viewer-debug` presets do. To work with the graphical application, initialize the submodules and switch to `viewer-release` (see [Interactive viewer](#interactive-viewer)).

### B. General CMake project

Kinetalis can also be configured without an IDE or a named preset. The following builds only the batch application:

```sh
cmake -S . -B build/local-batch -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DKINETALIS_BUILD_BATCH=ON \
  -DKINETALIS_BUILD_VIEWER=OFF \
  -DKINETALIS_BUILD_TESTS=OFF
cmake --build build/local-batch --target kinetalis_batch --parallel
```

Run this in an MSVC Developer shell on Windows, or in a shell with GCC 14 on Linux. With these settings, the executable is located at `build/local-batch/Release/bin/kinetalis_batch` (or `.exe` on Windows).

The main CMake switches are:

| Option | Default | Purpose |
| --- | --- | --- |
| `KINETALIS_BUILD_BATCH` | `ON` | Batch application |
| `KINETALIS_BUILD_VIEWER` | `OFF` | Interactive graphical application |
| `KINETALIS_BUILD_TESTS` | `ON` | Unit-test target |
| `KINETALIS_ENABLE_PCH` | `ON` | Precompiled headers |
| `KINETALIS_ENABLE_CLANG_TIDY` | `OFF` | Run clang-tidy during compilation |
| `KINETALIS_WARNINGS_AS_ERRORS` | `OFF` | Treat warnings as errors |

### C. Linux: local builds and servers

Install GCC 14, CMake, Ninja, and Git. On systems providing GCC 14 at `/usr/bin/gcc-14` and `/usr/bin/g++-14`, use the included Linux preset:

```sh
cmake --preset ubuntu-batch-release
cmake --build build/ubuntu-batch-release --target kinetalis_batch --parallel 8
./build/ubuntu-batch-release/Release/bin/kinetalis_batch --concurrency 8
```

For a different GCC installation, specify the toolchain explicitly:

```sh
cmake -S . -B build/gcc14-batch -G Ninja \
  -DCMAKE_C_COMPILER=gcc-14 -DCMAKE_CXX_COMPILER=g++-14 \
  -DCMAKE_BUILD_TYPE=Release \
  -DKINETALIS_BUILD_VIEWER=OFF -DKINETALIS_BUILD_TESTS=OFF
cmake --build build/gcc14-batch --target kinetalis_batch --parallel 8
```

On a compute server, launch `kinetalis_batch` directly or through a scheduler. The number supplied to `--concurrency` controls how many **independent simulations** run simultaneously.

#### Slurm

The TUHH HPC build preset and example submission script are provided in [`CMakePresets.json`](CMakePresets.json) and [`cmake/slurm_batch.sh`](cmake/slurm_batch.sh):

```sh
cmake --preset tuhh-hpc-batch-release
cmake --build build/tuhh-hpc-batch-release --target kinetalis_batch --parallel 8
mkdir -p "$HOME/slurm"
sbatch cmake/slurm_batch.sh "$PWD/build/tuhh-hpc-batch-release/Release/bin/kinetalis_batch"
```

Before submission on another cluster, edit the script's compiler module, allocation, paths, and job settings. It passes the allocated CPU count to the batch executable as its concurrency limit. When using a non-system compiler, ensure its runtime `libstdc++` is available on the compute nodes.

## Quick tests

The two examples below exercise the batch application and the viewer separately. Both use a small Au particle generated by [`setups::geometry_analysis`](studies/dealloying/setups.h) and a fixed limit of **100 accepted KMC steps**. Insert the indicated example into the application's `main.cpp`, build, and run. Restore the desired study setup when finished.

### Batch: run one small simulation

In [`apps/batch/main.cpp`](apps/batch/main.cpp), replace the existing `batch.run(...)` line inside `batch::run(Options const&)` with:

```cpp
auto const successful = batch.run([] {
    using namespace physics::unit::literals;

    return batch::Campaign{
        .name = "quick-test",
        .seed = "00000001-00000002-00000003-00000004-00000005-00000006-00000007-00000008",
        .replicate_count = 1,
        .setups = { dealloying::setups::geometry_analysis(1_nm, false) },
        .analysis_plan = [](std::filesystem::path const& path) {
            auto plan = dealloying::analysis_plans::basic(path);
            plan.add<control::StepLimit>(100);
            return plan;
        },
    };
});
```

Rebuild and run a single worker:

```powershell
# Windows: MSVC Developer PowerShell
cmake --build --preset build-batch-release
.\build\batch-release\Release\bin\kinetalis_batch.exe --concurrency 1
```

```sh
# Linux: GCC 14 build from section C
cmake --build build/gcc14-batch --target kinetalis_batch --parallel
./build/gcc14-batch/Release/bin/kinetalis_batch --concurrency 1
```

The run creates a new directory under `results/`. Inspect `campaign.txt` for the campaign seed, `setups.csv` for the setup description, `samples.csv` for the individual simulation seed, and `setups/` for the output from the run. The sample stops after 100 accepted steps or when no KMC events remain.

This example exercises lattice initialization, physical rate evaluation, stochastic event selection, an observer-based stop condition, and the batch output pipeline.

### Viewer: inspect a small simulation

First install the graphical submodules and build the viewer:

```powershell
git submodule update --init --recursive
cmake --preset viewer-release
cmake --build --preset build-viewer-release
```

In [`apps/viewer/main.cpp`](apps/viewer/main.cpp), replace the `viewer.createSimulation(...)` call with:

```cpp
viewer.createSimulation(
    setups::geometry_analysis(1_nm, false),
    [](std::filesystem::path const&) {
        kmc::AnalysisPlan plan;
        plan.add<control::StepLimit>(100);
        return plan;
    });
```

After rebuilding, start the executable from its output directory so the bundled fonts and shaders resolve correctly:

```powershell
Push-Location .\build\viewer-release\Release\bin
.\kinetalis_viewer.exe
Pop-Location
```

Explore the particle in the interactive 3D window and use the simulation controls to inspect its evolution. This example runs against the same KMC engine as the batch application; the viewer adds only its graphical and interactive layers.

## Running studies and reproducing results

A **campaign** defines a set of simulation setups, the number of independent replicates, the observers and stopping conditions, and any postprocessing. A setup in turn supplies the initial lattice and physical rate model. In the dealloying study, the campaign factories are collected in [`studies/dealloying/campaigns.h`](studies/dealloying/campaigns.h).

To run a campaign, choose its factory in [`apps/batch/main.cpp`](apps/batch/main.cpp). For example:

```cpp
auto const successful = batch.run(dealloying::campaigns::critical_potential_composition);
```

Rebuild `kinetalis_batch` after making the selection, then launch it. Common options are:

```text
kinetalis_batch [--concurrency N] [--results-dir PATH] [--help]
```

`--concurrency` sets the number of simultaneous samples (default: 16). `--results-dir` selects the output directory (default: `results/` relative to the working directory). For an explicit `--results-dir` value, the current command-line parser uses `HOME` to resolve relative paths; on Windows, use the default directory or configure `HOME`.

### Dealloying campaigns

| Campaign | What it produces |
| --- | --- |
| `passivation_study()` | Subcritical runs across composition and potential, used to study pore growth and arrest by passivation |
| `critical_potential_composition()` | Repeated runs around the transition, for estimating the composition-dependent critical potential |
| `critical_potential_temperature()` | Corresponding potential scans over temperature |
| `pore_surface_morphology()` | Saved configurations along a pore-growth trajectory for morphological analysis and rendering |

Other campaigns in the same file investigate surface geometry, bifurcation size, and kinetic sensitivity. The campaign definitions are ordinary C++ factories, so parameter ranges, replicates, and analysis plans can be adapted directly to a new study.

Each execution creates a separate results directory. The batch runner writes a campaign summary, setup and sample tables, and per-sample measurements; any configured postprocessor can merge and summarize these outputs. The random-number generator is `xoshiro256+`. Campaign seeds determine the seeds of individual replicates, independently of the worker scheduling order. The published study definitions contain the original seeds used to generate the reported trajectories.

For the critical-potential analysis, [`tools/fit_critical_potential.py`](tools/fit_critical_potential.py) performs Firth logistic regression on the classified outcomes. This processing step is separate from the C++ simulation and uses Python with NumPy, pandas, and `firthmodels`.

For reproducibility across later code changes, use the source revision or release associated with the relevant publication, together with its campaign definitions and analysis scripts.

## Architecture

Kinetalis separates the specification of a simulation from the numerical machinery that executes it. A study chooses the lattice, the rate model, and the measurements; the KMC engine handles event scheduling and state updates; the batch runner or viewer determines how the simulation is used.

```text
studies/dealloying/          Study definitions and campaign factories
        │
        ├── kmc::Setup ──────────────── lattice factory + model factory
        │       └── kmc::Simulation ── lattice state + event rates + KMC clock
        │                   │
        │                   └── kmc::AnalysisPlan ── observers / outputs / stop criteria
        │
        └── batch::Campaign ─────────── setups + replicates + seed + postprocessor
                    │
              batch::Runner ─────────── concurrent execution and results

apps/viewer/                  Interactive front end to kmc::Simulation
```

### Crystal lattices, shapes, and compositions

The [`lattice`](source/lattice/) library constructs finite atomistic systems from unit cells and geometric operations. [`UnitCell`](source/lattice/unitcell.h) represents the crystallographic basis, including conventional and primitive constructions for Bravais lattices, HCP cells, and custom basis sites. [`Lattice`](source/lattice/lattice.h) assembles sites and neighbor relationships, applies crystal orientations, and supports periodic boundaries independently along the three axes.

Initial states can be generated by filling regions with atoms or vacancies, sampling a multicomponent composition, assigning exact atom counts, or prescribing a spatial composition gradient. Shapes include spheres, cylindrical and ellipsoidal regions, cones, and general parallelepipeds. These operations make it straightforward to construct bulk alloys, nanopores, nanoparticles, layered systems, and spatially heterogeneous materials.

For example, the dealloying setup below generates an fcc Ag–Au alloy with a central vacancy. The `10` denotes an initial Au content of 10%, and `30_nm` specifies the box size:

```cpp
auto lattice = studies::dealloying::setups::bulk_with_vacancy(10, 30_nm);
```

The implementation in [`studies/dealloying/setups.h`](studies/dealloying/setups.h) demonstrates how to assemble a conventional fcc unit cell, configure three-dimensional periodic boundaries, sample the alloy composition, and modify selected sites. Changing the initial geometry is independent of changing the kinetic event-selection algorithm.

### Units, mathematics, and material data

The [`math`](source/math/) library supplies templated vectors, matrices, geometry operations, and numerical helpers. The [`physics`](source/physics/) library provides compile-time dimension-aware quantities, SI units and prefixes, constants, and physical equations. A parameter such as `300_K`, `0.15_eV`, or `30_nm` remains a physical quantity throughout the calculation, reducing unit-conversion errors in geometric and kinetic models. These libraries can also be used outside KMC.

[`chemistry`](source/chemistry/) supplies element properties and a composition container. For lattice initialization, a composition can represent a mixture of atomic species and vacant sites through integer weights. It is used by the lattice filling functions rather than being built into the KMC scheduler.

### KMC state and event dynamics

[`kmc::Simulation`](source/kmc/simulation.h) owns the evolving lattice, the selected rate model, the random generator, and the event-rate structures. [`kmc::Process`](source/kmc/process.h) identifies an event by its type, participating element, and local coordination. Rates are grouped by process type and stored in a hierarchical rate-sum tree for efficient weighted event selection. After an accepted event, only the affected sites and their neighborhoods require rate updates.

The simulation advances in continuous physical time: an event is selected according to its rate, a stochastic residence time is drawn, and the local atomic state is updated. The present model uses nearest-neighbor vacancy exchange for diffusion and atom removal for dissolution. The current enabled events and species are configured in [`source/kmc/types.h`](source/kmc/types.h) and [`source/lattice/types.h`](source/lattice/types.h).

The event scheduler is independent of the physical rate law. This allows the same lattice and event-selection machinery to be used with different activation barriers, temperature dependences, or model parameters.

### Physical rate models

[`kmc::Model`](source/kmc/model.h) implements the common transition-state-theory rate calculation and exposes hooks for the activation Gibbs energy and transmission coefficient. Models in [`source/models/`](source/models/) provide concrete formulations, including linear bond-breaking, composition-dependent, and coordination-dependent barriers, together with an electrochemical specialization.

A study provides the chosen model through `kmc::Setup::make_model`. For example, [`setups::pore_dealloying`](studies/dealloying/setups.h) constructs a `LinearBondBreaking` model and configures its potential and temperature. Replacing it with another `kmc::Model` implementation changes the kinetics without changing the event scheduler or batch runner.

This separation is useful for controlled model comparisons: two simulations can share the same geometry, event types, analysis, and sampling procedure while differing only in their physical rate law.

### Measurements, snapshots, and stopping conditions

A [`kmc::AnalysisPlan`](source/kmc/analysis_plan.h) collects observers. An observer can read state information, record event statistics, produce time-resolved data, write lattice snapshots, or request termination. [`output::Writer`](source/output/writer.h) provides common file-output hooks, while specialized implementations in [`source/output/`](source/output/) measure composition, surface geometry, event flux, process kinetics, and other quantities.

For example, a study can add a surface measurement and an event-count limit to an existing analysis plan:

```cpp
auto plan = studies::dealloying::analysis_plans::basic(path);
plan.add<output::Surface>(path);
plan.add<control::StepLimit>(100);
```

Observers are attached to the simulation through its analysis plan, so new measurements generally require no changes to the KMC loop. Snapshot sampling frequency can be configured independently for each observer. The dealloying study combines generic writers and stopping criteria with its own [`Regime`](studies/dealloying/analysis/regime.h) analysis for passivation, branching, and terminal-state classification.

### Batch studies and independent replicates

[`batch::Campaign`](source/batch/campaign.h) groups several `kmc::Setup` configurations into one experiment. The [`batch::Runner`](source/batch/runner.h) generates per-sample seeds, distributes independent simulations across worker threads, writes sample-level output, and executes optional postprocessing. A campaign may vary composition, potential, temperature, geometry, or model parameters, with a chosen number of independent replicates for each setup.

The `--concurrency` argument controls throughput without modifying the physical definition of a sample. For statistically meaningful results, independent replicates retain their own RNG streams and output records regardless of the execution order.

### Directory guide

| Path | Contents |
| --- | --- |
| [`source/lattice/`](source/lattice/) | Crystallography, shapes, sites, neighbors, and initial-state operations |
| [`source/kmc/`](source/kmc/) | Simulation state, processes, event selection, rate tree, observer interfaces |
| [`source/models/`](source/models/) | Physical and electrochemical rate laws |
| [`source/math/`](source/math/), [`source/physics/`](source/physics/), [`source/chemistry/`](source/chemistry/) | General mathematical, physical, and material utilities |
| [`source/simulation/`](source/simulation/), [`source/utility/`](source/utility/) | Simulation base, random sampling, file I/O, formatting, and supporting utilities |
| [`source/output/`](source/output/) | Measurements and writers |
| [`source/batch/`](source/batch/) | Parallel campaign execution and aggregation |
| [`studies/dealloying/`](studies/dealloying/) | Dealloying setups, analyses, and campaigns |
| [`apps/batch/`](apps/batch/) | Batch executable entry point |
| [`apps/viewer/`](apps/viewer/), [`source/viewer/`](source/viewer/) | Graphical application and interactive simulation bridge |
| [`tests/`](tests/), [`tools/`](tools/) | Tests and auxiliary data-processing tools |

## Extending Kinetalis

Most new research tasks can be implemented in the study layer while reusing the simulation core. A typical development path is:

1. **Define the material and geometry.** Build a `UnitCell` and `Lattice`, set periodic boundaries, and supply a deterministic or RNG-based filling function in `kmc::Setup::make_lattice`.
2. **Choose the kinetics.** Configure an existing `kmc::Model` or derive a new model that computes activation energies and transmission coefficients. Supply it through `kmc::Setup::make_model`.
3. **Choose what to record.** Compose an `AnalysisPlan` from existing observers or implement a new `kmc::Observer` / `output::Writer` for the desired physical quantity.
4. **Run a parameter study.** Build a `batch::Campaign` with the setups, replicate count, analysis plan, seed, and optional postprocessor, then select it in `apps/batch/main.cpp`.
5. **Inspect the result.** Process the generated data, or use the viewer with the same `kmc::Setup` for interactive exploration.

The dealloying implementation is a useful starting point: [`setups.h`](studies/dealloying/setups.h) demonstrates geometry and model factories, [`analysis_plans.h`](studies/dealloying/analysis_plans.h) demonstrates observer composition, and [`campaigns.h`](studies/dealloying/campaigns.h) demonstrates parameter sweeps and reproducible seeds.

For a new physical event family (rather than a different rate for an existing event), extend the event and process definitions and their local update rules in `source/kmc/`. The current diffusion/dissolution implementation provides the corresponding example.

## Interactive viewer

The graphical viewer uses Corrade, Magnum, Magnum Integration, SDL2, Dear ImGui, and ImPlot. The dependencies are tracked as Git submodules and are built only when `KINETALIS_BUILD_VIEWER=ON`.

```powershell
git submodule update --init --recursive
cmake --preset viewer-release
cmake --build --preset build-viewer-release
```

The entry point [`apps/viewer/main.cpp`](apps/viewer/main.cpp) selects the setup and analysis plan. The GUI and simulation bridge are implemented under [`source/viewer/`](source/viewer/). For an immediate example, use the [viewer quick test](#viewer-inspect-a-small-simulation).

Offline rendering and other analysis tools in [`tools/`](tools/) are separate from this viewer and from the batch build.

## Tests and development tools

The repository includes a `kinetalis_tests` target, formatting rules in `.clang-format`, and static-analysis settings in `.clang-tidy`. Tests use the **vendored Boost.UT** header at `external/boost/ut.hpp`; this is separate from Boost.Test and does not require installing Boost through Visual Studio Installer. Ensure the header is present in your checkout. Unit tests do not require the graphical viewer.

On Windows, use the supplied `tests-debug` preset, which enables `clang-tidy`:

```powershell
cmake --preset tests-debug
cmake --build --preset build-tests-debug
ctest --preset test-tests-debug
```

If CMake reports `Could not find KINETALIS_CLANG_TIDY`, ensure **C++ Clang tools for Windows** is installed and `clang-tidy.exe` is discoverable (try `where.exe clang-tidy`). Alternatively, supply its full path with `-DKINETALIS_CLANG_TIDY="C:/path/to/clang-tidy.exe"` or disable analysis using `cmake --preset tests-debug -DKINETALIS_ENABLE_CLANG_TIDY=OFF`.

On Linux, a test build without static analysis can be configured with:

```sh
cmake -S . -B build/tests -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DKINETALIS_BUILD_BATCH=OFF \
  -DKINETALIS_BUILD_VIEWER=OFF \
  -DKINETALIS_BUILD_TESTS=ON
cmake --build build/tests --target kinetalis_tests --parallel
ctest --test-dir build/tests --output-on-failure
```

## Citation

Kinetalis is available under the MIT License. For scientific work based on the software, please cite the repository and the relevant associated publication(s):

- **G. Henkelmann, D. Waldow, M. Liu, L. Lührs, Y. Li, and J. Weissmüller**, “Self-Detachment and Subsurface Densification of Dealloyed Nanoporous Thin Films,” *Nano Letters* **22**(16), 6787–6793 (2022). https://doi.org/10.1021/acs.nanolett.2c02666
- **G. Henkelmann, X. Wu, and J. Weissmüller**, “Mitigating the Film–Substrate Decohesion in Nanoporous Metals,” *APL Materials* **13**, 091116 (2025). https://doi.org/10.1063/5.0287477

The publication on pore growth, passivation, and bifurcation will be added after publication. GitHub's citation metadata is provided in [`CITATION.cff`](CITATION.cff).

## License

Kinetalis is released under the [MIT License](LICENSE). Third-party components retain their respective licenses.
