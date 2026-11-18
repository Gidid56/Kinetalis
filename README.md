# Kinetalis

**Kinetalis** is a C++23 framework for lattice-based kinetic Monte Carlo (KMC) simulations. It models how atoms move or dissolve through stochastic events whose rates are determined by physical models. The framework was developed to study electrochemical dealloying, but its lattice, simulation, and numerical components can also be used for other atomistic problems.

Kinetalis provides two applications:

- **`kinetalis_batch`** runs parameter studies with independent simulation replicates and saves numerical results.
- **`kinetalis_viewer`** displays simulations interactively in 3D (optional).

The dealloying study in [`studies/dealloying/`](studies/dealloying/) is a working example of how to use the framework.

## Requirements

- A **C++23 compiler and standard library** (MSVC in Visual Studio 2026 or GCC 14+), including support for `<format>`, `<print>`, and ranges.
- **CMake 3.25+**, **Ninja**, and **Git**.
- No separately installed C++ libraries for the core or batch application.

The optional viewer needs an OpenGL-capable system and the repository's graphical Git submodules. Unit tests use the header-only Boost.UT library, expected at `external/boost/ut.hpp`. Static analysis with `clang-tidy` is optional; the Windows analysis preset enables it.

## Get started

Clone the repository:

~~~sh
git clone https://github.com/Gidid56/Kinetalis.git
cd Kinetalis
~~~

### Windows — Visual Studio 2026

In **Visual Studio Installer**, select **Desktop development with C++**. Check that the individual components include **MSVC x64/x86 build tools**, a **Windows SDK**, and **C++ CMake tools for Windows**. Also select **C++ Clang tools for Windows** if you intend to use the static-analysis-enabled debug presets.

Open the repository using **File → Open → Folder**, select the **`windows-batch-release`** CMake preset, and build the **`kinetalis_batch`** target. No Visual Studio solution file is needed.

Alternatively, from an **MSVC Developer PowerShell**:

~~~powershell
cmake --preset windows-batch-release
cmake --build --preset build-windows-batch-release
.\build\windows-batch-release\Release\bin\kinetalis_batch.exe --help
~~~

### Linux — GCC 14+

With GCC 14, CMake, Ninja, and Git installed, use the Linux preset:

~~~sh
cmake --preset ubuntu-batch-release
cmake --build --preset build-ubuntu-batch-release --parallel
./build/ubuntu-batch-release/Release/bin/kinetalis_batch --help
~~~

The preset expects GCC 14 at `/usr/bin/gcc-14` and `/usr/bin/g++-14`. For other compiler locations, use the [custom CMake configuration](#custom-cmake-configuration).

## Run a study

A **campaign** specifies the starting configurations, the number of independent replicates, the measurements to record, and any conditions for stopping the simulations.

Choose a campaign in [`apps/batch/main.cpp`](apps/batch/main.cpp). For example, replace the existing `batch.run(...)` call with:

~~~cpp
auto const successful = batch.run(dealloying::campaigns::critical_potential_composition);
~~~

Rebuild and run `kinetalis_batch`. The campaign is selected **at compile time**, not through a command-line option. To find the available studies, see [`studies/dealloying/campaigns.h`](studies/dealloying/campaigns.h).

| Example campaign | Purpose |
| --- | --- |
| `passivation_study()` | Explore pore growth and passivation below the transition |
| `critical_potential_composition()` | Estimate the critical potential as a function of composition |
| `critical_potential_temperature()` | Study the temperature dependence of the critical potential |
| `pore_surface_morphology()` | Record pore evolution for morphological analysis |

Use `--concurrency N` to run up to `N` independent samples simultaneously (default: 16), `--results-dir PATH` to select the output location, or `--help` to see the options. Some predefined studies contain many simulations and are not intended as quick smoke tests.

### Results and reproducibility

Each campaign run creates a new directory under `results/` by default. Key files include:

| File | Contents |
| --- | --- |
| `campaign.txt` | Campaign seed and sample counts |
| `setups.csv` | Simulation setup descriptions |
| `samples.csv` | Setup/replicate assignments and individual seeds |
| `observer_resolution.csv` | Snapshot schedules configured in the analysis plan |
| `observer_settings.txt` | Observer settings, including stopping and detection criteria |
| `setups/` | Per-sample model, lattice, seed, and measurement files |

The batch runner derives separate random seeds for each replicate, independently of worker scheduling. For scientific reproducibility, retain the output together with the **exact source revision** and the campaign definition. The sampling and classification criteria can be inspected in [`studies/dealloying/analysis_plans.h`](studies/dealloying/analysis_plans.h).

The optional critical-potential fitting script, [`tools/fit_critical_potential.py`](tools/fit_critical_potential.py), runs separately from the C++ application and requires Python with NumPy, pandas, and `firthmodels`. Supply your own `regimes.csv` in the working directory when running it; the script writes `critical_potential_fits.csv` there. Analysis CSV and Excel files under `tools/` are local inputs or outputs and are not tracked in Git.

## Optional components

### Unit tests

The `kinetalis_tests` target uses **Boost.UT**, a header-only testing framework distinct from Boost.Test. Its header belongs at `external/boost/ut.hpp` in the repository; there is no separate Boost installation or Visual Studio Boost.Test adapter to configure.

On Windows, the `windows-tests-debug` preset builds tests without clang-tidy:

~~~powershell
cmake --preset windows-tests-debug
cmake --build --preset build-windows-tests-debug
ctest --preset test-windows-tests-debug
~~~

The `kinetalis_tests` target **builds all eight test executables** without running them: seven individual modules (`chemistry`, `kmc`, `lattice`, `math`, `models`, `physics`, and `simulation`) and `test_all.exe`. In Visual Studio, select `test_all.exe` as the **Startup Item** and run it to execute every suite. The test sources are compiled once and shared between the module executables and `test_all.exe`.

~~~powershell
cmake --build build/windows-tests-debug --target test_all
.\build\windows-tests-debug\Debug\bin\test_all.exe
~~~

`ctest --preset test-windows-tests-debug` also runs all tests, reporting results separately for each of the seven modules.

For optional checks, select `windows-tests-analysis` (clang-tidy enabled) or
`windows-tests-no-pch` (precompiled headers disabled). Both have matching
`build-windows-*` and `test-windows-*` presets. If CMake cannot find clang-tidy,
install **C++ Clang tools for Windows**, or pass
`-DKINETALIS_CLANG_TIDY="C:/path/to/clang-tidy.exe"`.

On Linux, the GCC 14 test presets are:

~~~sh
cmake --preset ubuntu-tests-debug
cmake --build --preset build-ubuntu-tests-debug --parallel
ctest --preset test-ubuntu-tests-debug
~~~

To run a single module, pass a CTest filter, such as
`ctest --test-dir build/windows-tests-debug -R kmc --output-on-failure`.

GitHub Actions validates Debug tests and Release Batch builds on GCC 14 and
MSVC. The Batch `--help` smoke test does not run a simulation campaign.
The No-PCH configuration remains available locally but is not part of CI.

### Interactive viewer

The viewer uses **Corrade, Magnum, Magnum Integration, SDL2, Dear ImGui, and ImPlot**. These are tracked as Git submodules and built from source when the viewer is enabled.

~~~powershell
git submodule update --init --recursive
cmake --preset windows-viewer-release
cmake --build --preset build-windows-viewer-release
~~~

Start the Windows executable from its output directory so that the fonts and shaders are found:

~~~powershell
Push-Location .\build\windows-viewer-release\Release\bin
.\kinetalis_viewer.exe
Pop-Location
~~~

The simulation shown is configured in [`apps/viewer/main.cpp`](apps/viewer/main.cpp). Use the same study setup and analysis-plan factories as the batch application.

## Tools

The [`tools/`](tools/) directory contains optional development and postprocessing tools.
They are not required for ordinary Kinetalis builds or simulations.

| Tool | Purpose |
| --- | --- |
| [`fit_critical_potential.py`](tools/fit_critical_potential.py) | Fit the porous/arrested outcome transition using Firth logistic regression; reads a local `regimes.csv` and writes `critical_potential_fits.csv`. Requires NumPy, pandas, and `firthmodels`. |
| [`generate_physics_literals`](tools/literals_generator.cpp) | CMake target that regenerates `source/physics/literals.generated.h`; invoke only when the literal definitions change. |
| [`setup_dependencies.ps1`](tools/setup_dependencies.ps1) | Windows helper for fetching viewer dependencies. For the repository's pinned versions, prefer `git submodule update --init --recursive`. |
| [`render_results.ps1`](tools/render_results.ps1) | Render simulation XYZ trajectories with OVITO using [`render_script_ovito.py`](tools/render_script_ovito.py) and the [`rendering.ovito`](tools/rendering.ovito) session. Requires an OVITO installation with the configured renderer. |
| [`make_movies.ps1`](tools/make_movies.ps1) | Assemble numbered PNG frames into MP4 movies using FFmpeg. |
| [`git_version_release.ps1`](tools/git_version_release.ps1) | Create a versioned release from the CMake project version; use on a clean `main` branch. |

If PowerShell prevents an inspected, trusted script from running because of its
execution policy, the restriction can be relaxed **for the current session only**:

~~~powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
~~~

This is not required for normal CMake builds and does not change the machine-wide
policy. Review scripts before executing them.

## Framework overview

Kinetalis separates **which events can occur and how fast** from **how events are sampled, executed, and measured**. The KMC core is generic; the dealloying study provides concrete transformations, a kinetic model, and simulation setups.

| Component | Responsibility |
| --- | --- |
| [`lattice`](source/lattice/) | Crystal geometry, sites, neighbor lists, periodic boundaries, and initial configurations |
| [`kmc::Transformation` and `kmc::Transformations<Ts...>`](source/kmc/transformations.h) | Define possible changes to the lattice and the geometric slots associated with them |
| [`kmc::Kinetics`](source/kmc/kinetics.h) and [study-specific kinetic models](studies/dealloying/models/) | Classify candidate events, calculate their rates, and define the local rate-dependency radius |
| [`kmc::Model<KineticsT, TransformationsT>`](source/kmc/model.h) | Compose a concrete kinetic model and a statically typed collection of transformations |
| [`kmc::Setup<ModelT>`](source/kmc/setup.h) | Hold the lattice generator, model configuration, and optional random seed |
| [`kmc::RejectionTreeSimulation<ModelT>`](source/kmc/simulation.h) | Sample events, advance physical time, execute accepted transformations, and update cached rates |
| [`kmc::Simulation`](source/kmc/simulation.h), [`kmc::AnalysisPlan`](source/kmc/analysis_plan.h), and [observers](source/output/) | Provide a non-templated simulation interface, measurements, snapshots, and stopping conditions |
| [`batch::Campaign<ModelT>`](source/batch/campaign.h) and [`batch::Runner<ModelT>`](source/batch/runner.h) | Execute parameter studies and independent replicates, then aggregate results |

### Event lifecycle

A `Transformation` is a C++ concept, not a runtime base class. Each transformation declares an event type and a compile-time `slot_count` per source site. A slot can resolve to an [`event::Location`](source/kmc/event.h) containing the source site and, for example, a diffusion target; nonexistent locations and geometrically invalid candidates contribute no rate. The current `Location` representation supports up to two sites.

A kinetic model assigns each candidate a `RateClassKey` using the event type and its local environment. Key zero denotes an inactive slot. Rates are cached by class, while a hierarchical [rate-sum tree](source/kmc/rate_tree.h) stores the summed rates of source sites for efficient event selection. The simulation resolves the selected slot into a location, evaluates any postselection rejection, and advances physical time for **both accepted and rejected selections**. Only accepted selections modify the lattice, increment the accepted-step counter, and trigger local rate updates.

The KMC scheduler uses static dispatch for the concrete kinetic model and transformations. Its non-templated `kmc::Simulation` interface allows observers and the viewer to operate without knowing the model type. Mathematical utilities, [dimension-aware physical quantities](source/physics/), and [chemical data](source/chemistry/) are shared across studies.

### Extend the framework

Most studies can be implemented under `studies/` without modifying the KMC scheduler. The [dealloying study](studies/dealloying/) provides a complete reference:

1. Define one or more transformation types implementing the [`kmc::Transformation` concept](source/kmc/transformations.h): `event`, `slot_count`, `location(...)`, `is_source_candidate(...)`, `is_candidate(...)`, `is_rejected(...)`, `execute(...)`, and `settings()`. An optional `is_candidate_after_source_check(...)` avoids repeating a source predicate for every slot.
2. Implement a kinetic model, typically deriving from [`kmc::Kinetics`](source/kmc/kinetics.h), with `classify(...)`, `number_keys()`, `class_event(...)`, `rate(...)`, `refresh_rates()`, and `output_text()`. Ensure `invalidation_radius()` covers all source-site rates affected by an accepted lattice change.
3. Combine the types into a `Model` and configure them in [setup factories](studies/dealloying/setups.h). The current study uses these aliases in [`events.h`](studies/dealloying/events.h):

~~~cpp
using Transformations = kmc::Transformations<events::Dissolution, events::Diffusion>;
using Model = kmc::Model<models::LinearBondBreaking, Transformations>;
using Setup = kmc::Setup<Model>;
using Campaign = batch::Campaign<Model>;
~~~

4. Configure observers and stopping conditions in an [analysis plan](studies/dealloying/analysis_plans.h). Combine setups, replicates, and optional postprocessing in a [campaign](studies/dealloying/campaigns.h), and execute it with `batch::Runner<Model>` or construct `kmc::RejectionTreeSimulation<Model>` directly.

For a small end-to-end smoke test, replace `batch.run(...)` in [`apps/batch/main.cpp`](apps/batch/main.cpp) with a single-sample campaign capped at 100 accepted KMC events (or ending earlier if no selectable events remain):

~~~cpp
auto const successful = batch.run([] {
    using namespace physics::unit::literals;

    return dealloying::Campaign{
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
~~~

Rebuild the batch application using the commands in [Get started](#get-started), then run it. The results appear under `results/` unless a different output directory is specified.

## Advanced build configuration

### Custom CMake configuration

For toolchains without a supplied preset, configure only the components you need. For example:

~~~sh
cmake -S . -B build/custom -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DKINETALIS_BUILD_BATCH=ON \
  -DKINETALIS_BUILD_VIEWER=OFF \
  -DKINETALIS_BUILD_TESTS=OFF
cmake --build build/custom --target kinetalis_batch --parallel
~~~

Run CMake in a shell where the desired compiler is available. For GCC installations outside the default path, supply `-DCMAKE_C_COMPILER=...` and `-DCMAKE_CXX_COMPILER=...`.

The main options are:

| CMake option | Default | Description |
| --- | --- | --- |
| `KINETALIS_BUILD_BATCH` | `ON` | Build the batch application |
| `KINETALIS_BUILD_VIEWER` | `OFF` | Build the graphical viewer |
| `KINETALIS_BUILD_TESTS` | `ON` | Build the unit tests |
| `KINETALIS_ENABLE_PCH` | `ON` | Use precompiled headers |
| `KINETALIS_ENABLE_CLANG_TIDY` | `OFF` | Run static analysis during compilation |
| `KINETALIS_WARNINGS_AS_ERRORS` | `OFF` | Treat compiler warnings as errors |

### HPC and Slurm

The repository includes a [TUHH HPC CMake preset](CMakePresets.json) and an example [Slurm submission script](cmake/slurm_batch.sh):

~~~sh
cmake --preset tuhh-hpc-batch-release
cmake --build build/tuhh-hpc-batch-release --target kinetalis_batch --parallel 8
mkdir -p "$HOME/slurm"
sbatch cmake/slurm_batch.sh "$PWD/build/tuhh-hpc-batch-release/Release/bin/kinetalis_batch"
~~~

The preset and script contain site-specific compiler, module, and scheduler settings. Adapt them before use on another cluster, and ensure that the compiler's runtime `libstdc++` is available on compute nodes.

## Citation

Kinetalis is available under the MIT License. For scientific work based on the software, cite the repository and the relevant associated publication(s):

- **G. Henkelmann, D. Waldow, M. Liu, L. Lührs, Y. Li, and J. Weissmüller**, “Self-Detachment and Subsurface Densification of Dealloyed Nanoporous Thin Films,” *Nano Letters* **22**(16), 6787–6793 (2022). https://doi.org/10.1021/acs.nanolett.2c02666
- **G. Henkelmann, X. Wu, and J. Weissmüller**, “Mitigating the Film–Substrate Decohesion in Nanoporous Metals,” *APL Materials* **13**, 091116 (2025). https://doi.org/10.1063/5.0287477

GitHub citation metadata is provided in [`CITATION.cff`](CITATION.cff). The publication on pore growth, passivation, and bifurcation will be added after publication.

## License

Kinetalis is released under the [MIT License](LICENSE). Third-party components retain their respective licenses.
