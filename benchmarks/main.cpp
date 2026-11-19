#include "dealloying/model.h"
#include "dealloying/setups.h"
#include "kmc/analysis_plan.h"
#include "kmc/simulation.h"
#include "output/basics.h"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <print>
#include <stdexcept>
#include <vector>

using namespace physics::unit::literals;

namespace
{
    constexpr kmc::Step target_steps{ 1'000 };
    constexpr std::size_t warmup_runs{ 1 };
    constexpr std::size_t measured_runs{ 5 };

    double run_simulation()
    {
        studies::dealloying::Setup setup;
        setup.name = "KMC benchmark";
        setup.seed = simulation::Seed{ 1, 2, 3, 4, 5, 6, 7, 8 };
        setup.make_lattice = [] {
            return studies::dealloying::setups::bulk_with_vacancy(10, 6_nm);
        };
        setup.model.kinetics().set_temperature(300_K).set_potential(0.86_V);

        kmc::RejectionTreeSimulation<studies::dealloying::Model> simulation{ setup };
        kmc::AnalysisPlan plan;
        plan.add<control::StepLimit>(target_steps);

        // Construct the lattice, rate tables, and analysis plan before timing.
        auto const begin{ std::chrono::steady_clock::now() };
        auto const status{ simulation.run(plan) };
        auto const end{ std::chrono::steady_clock::now() };

        if (status != simulation::Status::analysed or simulation.step() != target_steps)
            throw std::runtime_error{ "Benchmark stopped before reaching its step limit." };

        return std::chrono::duration<double>{ end - begin }.count();
    }
}

int main()
{
    try
    {
        for (std::size_t i{}; i < warmup_runs; ++i)
            (void)run_simulation();

        std::vector<double> seconds;
        seconds.reserve(measured_runs);
        for (std::size_t i{}; i < measured_runs; ++i)
            seconds.push_back(run_simulation());

        std::ranges::sort(seconds);
        auto const median{ seconds[seconds.size() / 2] };

        std::println("KMC | Dealloying | Minimal observers");
        std::println("  Accepted steps/run: {}", target_steps);
        std::println("  Warmup runs: {} | Measured runs: {}", warmup_runs, measured_runs);
        std::println("  Time (s): min {:.6f} | median {:.6f} | max {:.6f}",
            seconds.front(), median, seconds.back());
        std::println("  Median throughput: {:.0f} accepted steps/s", target_steps / median);
        return 0;
    }
    catch (std::exception const& error)
    {
        std::println(stderr, "Benchmark failed: {}", error.what());
        return 1;
    }
}
