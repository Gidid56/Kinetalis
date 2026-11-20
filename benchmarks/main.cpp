#include "dealloying/model.h"
#include "dealloying/setups.h"
#include "kmc/analysis_plan.h"
#include "kmc/simulation.h"
#include "output/basics.h"

#include <cmath>
#include <array>
#include <chrono>
#include <cstddef>
#include <numeric>
#include <print>
#include <stdexcept>
#include <string_view>
#include <vector>

using namespace physics::unit::literals;

namespace
{
    using Clock = std::chrono::steady_clock;

    constexpr kmc::Step target_steps{ 100'000 };
    constexpr kmc::Step warmup_steps{ 1'000 };
    constexpr std::size_t measured_runs{ 5 };
    constexpr std::array box_sides{ 5_nm, 10_nm, 20_nm };

    struct Measurement
    {
        double lattice_seconds{};
        double initialization_seconds{};
        double execution_seconds{};
        std::size_t lattice_sites{};
    };

    Measurement run_simulation(decltype(10_nm) box_side, kmc::Step steps)
    {
        double lattice_seconds{};

        studies::dealloying::Setup setup;
        setup.name = "KMC benchmark";
        setup.seed = simulation::Seed{ 1, 2, 3, 4, 5, 6, 7, 8 };
        setup.make_lattice = [box_side, &lattice_seconds] {
            auto const start{ Clock::now() };
            auto lattice{ studies::dealloying::setups::bulk_with_vacancy(10, box_side) };
            lattice_seconds = std::chrono::duration<double>{ Clock::now() - start }.count();
            return lattice;
        };
        setup.model.kinetics().set_temperature(300_K).set_potential(0.86_V);

        // The simulation constructor calls make_lattice(), initializes its
        // atom population and builds its initial kinetic/rate-tree state.
        auto const init_start{ Clock::now() };
        kmc::RejectionTreeSimulation<studies::dealloying::Model> simulation{ setup };
        auto const initialization_seconds{
            std::chrono::duration<double>{ Clock::now() - init_start }.count()
        };
        auto const lattice_sites{ simulation.lattice().sites().size() };

        kmc::AnalysisPlan plan;
        plan.add<control::StepLimit>(steps);

        auto const run_start{ Clock::now() };
        auto const status{ simulation.run(plan) };
        auto const execution_seconds{
            std::chrono::duration<double>{ Clock::now() - run_start }.count()
        };

        if (status != simulation::Status::analysed or simulation.step() != steps)
            throw std::runtime_error{ "Benchmark stopped before reaching its step limit." };

        return {
            .lattice_seconds = lattice_seconds,
            .initialization_seconds = initialization_seconds,
            .execution_seconds = execution_seconds,
            .lattice_sites = lattice_sites,
        };
    }

    struct Statistics
    {
        double mean{};
        double standard_deviation{};
    };

    Statistics summarize(std::vector<double> const& values)
    {
        if (values.empty())
            throw std::invalid_argument{ "Cannot summarize an empty benchmark sample." };

        const auto mean{ std::accumulate(values.begin(), values.end(), 0.0) / values.size() };
        double sum_squared_deviations{};
        for (const auto value : values)
            sum_squared_deviations += (value - mean) * (value - mean);

        // Sample standard deviation (Bessel's correction).
        const auto variance{
            values.size() > 1 ? sum_squared_deviations / (values.size() - 1) : 0.0
        };
        return { mean, std::sqrt(variance) };
    }

    void print_timing(
        std::string_view label,
        std::vector<Measurement> const& samples,
        double Measurement::* field)
    {
        std::vector<double> values;
        values.reserve(samples.size());
        for (auto const& sample : samples)
            values.push_back(sample.*field);

        auto const stats{ summarize(values) };
        std::println("  {:<25} {:.4f} ± {:.4f} s",
            label, stats.mean, stats.standard_deviation);
    }
}

int main()
{
    try
    {
        std::println("KMC | Dealloying | Minimal observers");
        std::println("  Accepted steps/run: {}", target_steps);
        std::println("  Warm-up: {} accepted steps in a {:.0f} nm box",
            warmup_steps, static_cast<double>(box_sides.front() / 1_nm));
        std::println("  Measured runs per box: {}", measured_runs);
        (void)run_simulation(box_sides.front(), warmup_steps);

        for (auto const box_side : box_sides)
        {
            std::println("\nBox side: {:.0f} nm", static_cast<double>(box_side / 1_nm));

            std::vector<Measurement> samples;
            samples.reserve(measured_runs);
            for (std::size_t i{}; i < measured_runs; ++i)
                samples.push_back(run_simulation(box_side, target_steps));

            std::println("  Lattice sites: {}", samples.front().lattice_sites);
            print_timing("Lattice construction", samples, &Measurement::lattice_seconds);
            print_timing("Total simulation init", samples, &Measurement::initialization_seconds);
            print_timing("KMC execution", samples, &Measurement::execution_seconds);

            std::vector<double> nanoseconds_per_step;
            nanoseconds_per_step.reserve(samples.size());
            for (auto const& sample : samples)
                nanoseconds_per_step.push_back(
                    sample.execution_seconds * 1e9 / static_cast<double>(target_steps));

            auto const step_time{ summarize(nanoseconds_per_step) };
            std::println("  KMC time/step: {:.1f} ± {:.1f} ns/accepted step",
                step_time.mean, step_time.standard_deviation);
        }
        return 0;
    }
    catch (std::exception const& error)
    {
        std::println(stderr, "Benchmark failed: {}", error.what());
        return 1;
    }
}
