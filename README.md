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

The optional viewer needs an OpenGL-capable system and the repository's graphical Git submodules. Unit tests use the header-only Boost.UT library, expected at `external/boost/ut.hpp`. Static analysis with `clang-tidy` is optional; some debug presets enable it.

## Get started

Clone the repository:

~~~sh
git clone https://github.com/Gidid56/Kinetalis.git
cd Kinetalis
~~~

### Windows — Visual Studio 2026

In **Visual Studio Installer**, select **Desktop development with C++**. Check that the individual components include **MSVC x64/x86 build tools**, a **Windows SDK**, and **C++ CMake tools for Windows**. Also select **C++ Clang tools for Windows** if you intend to use the static-analysis-enabled debug presets.

Open the repository using **File → Open → Folder**, select the **`batch-release`** CMake preset, and build the **`kinetalis_batch`** target. No Visual Studio solution file is needed.

Alternatively, from an **MSVC Developer PowerShell**:

~~~powershell
cmake --preset batch-release
cmake --build --preset build-batch-release
.\build\batch-release\Release\bin\kinetalis_batch.exe --help
~~~

### Linux — GCC 14+

With GCC 14, CMake, Ninja, and Git installed, use the Linux preset:

~~~sh
cmake --preset ubuntu-batch-release
cmake --build build/ubuntu-batch-release --target kinetalis_batch --parallel
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

The optional critical-potential fitting script, [`tools/fit_critical_potential.py`](tools/fit_critical_potential.py), runs separately from the C++ application and requires Python with NumPy, pandas, and `firthmodels`.

## Optional components

### Unit tests

The `kinetalis_tests` target uses **Boost.UT**, a header-only testing framework distinct from Boost.Test. Its header belongs at `external/boost/ut.hpp` in the repository; there is no separate Boost installation or Visual Studio Boost.Test adapter to configure.

On Windows, the `tests-debug` preset enables `clang-tidy`:

~~~powershell
cmake --preset tests-debug
cmake --build --preset build-tests-debug
ctest --preset test-tests-debug
~~~

If CMake cannot find `clang-tidy`, check the **C++ Clang tools for Windows** installation. You can supply its full path using `-DKINETALIS_CLANG_TIDY="C:/path/to/clang-tidy.exe"`, or configure the preset without analysis:

~~~powershell
cmake --preset tests-debug -DKINETALIS_ENABLE_CLANG_TIDY=OFF
~~~

On Linux, a test-only build can be configured as follows:

~~~sh
cmake -S . -B build/tests -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DKINETALIS_BUILD_BATCH=OFF \
  -DKINETALIS_BUILD_VIEWER=OFF \
  -DKINETALIS_BUILD_TESTS=ON
cmake --build build/tests --target kinetalis_tests --parallel
ctest --test-dir build/tests --output-on-failure
~~~

### Interactive viewer

The viewer uses **Corrade, Magnum, Magnum Integration, SDL2, Dear ImGui, and ImPlot**. These are tracked as Git submodules and built from source when the viewer is enabled.

~~~powershell
git submodule update --init --recursive
cmake --preset viewer-release
cmake --build --preset build-viewer-release
~~~

Start the Windows executable from its output directory so that the fonts and shaders are found:

~~~powershell
Push-Location .\build\viewer-release\Release\bin
.\kinetalis_viewer.exe
Pop-Location
~~~

The simulation shown is configured in [`apps/viewer/main.cpp`](apps/viewer/main.cpp). Use the same study setup and analysis-plan factories as the batch application.

## Framework overview

Kinetalis separates **what to simulate** from **how to execute and analyze it**:

| Component | Responsibility |
| --- | --- |
| [`lattice`](source/lattice/) | Crystal structures, simulation cells, shapes, periodic boundaries, and atomic configurations |
| [`kmc::Setup`](source/kmc/setup.h) | Selects the initial lattice, physical model, and event rules |
| [`kmc::Simulation`](source/kmc/simulation.h) | Advances simulation time, selects events, and updates rates and atomic states |
| [`kmc::Model`](source/kmc/model.h) and [`source/models/`](source/models/) | Calculates physical event rates from activation barriers and model parameters |
| [`kmc::AnalysisPlan`](source/kmc/analysis_plan.h) and [`source/output/`](source/output/) | Collects measurements, snapshots, and stopping conditions |
| [`batch::Campaign`](source/batch/campaign.h) and [`source/batch/`](source/batch/) | Runs parameter sweeps and replicates, then aggregates results |

The KMC engine represents events such as diffusion and dissolution through [`kmc::Process`](source/kmc/process.h) and study-specific [event rules](studies/dealloying/rules.h). Event rates are maintained in a hierarchical rate-sum tree for efficient random selection. The rate model is independent of the event scheduler, so kinetic assumptions can be changed without rewriting the simulation machinery.

The general-purpose libraries [`math`](source/math/), [`physics`](source/physics/), and [`chemistry`](source/chemistry/) provide numerical operations, dimension-aware physical quantities, and element/composition data. Parameters such as `300_K`, `0.15_eV`, and `30_nm` retain their units in C++ expressions.

### Extend the framework

Most new studies can be developed under `studies/` without modifying the KMC engine. Use the existing dealloying implementation as a reference:

1. Define a lattice and rate model in a [setup factory](studies/dealloying/setups.h).
2. Choose observers and stopping conditions in an [analysis plan](studies/dealloying/analysis_plans.h).
3. Combine setups, replicates, and postprocessing in a [campaign](studies/dealloying/campaigns.h).

For a small end-to-end smoke test, replace `batch.run(...)` in `apps/batch/main.cpp` with a single-sample campaign that stops after 100 accepted KMC events:

~~~cpp
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
~~~

Rebuild the batch application using the commands in [Get started](#get-started), then run it. The result appears under `results/`.

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
