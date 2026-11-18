#include "kmc/simulation.h"
#include "kmc/analysis_plan.h"
#include "output/basics.h"
#include "dealloying/setups.h"
#include "perf_probe.h"

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <format>
#include <print>
#include <string_view>

using namespace physics::unit::literals;

namespace
{
    std::uint64_t fingerprint(lattice::Lattice const& lattice)
    {
        std::uint64_t value{ 14695981039346656037ull };
        for (auto const& site : lattice.sites())
        {
            value ^= static_cast<std::uint8_t>(site.element());
            value *= 1099511628211ull;
            value ^= static_cast<std::uint8_t>(site.coordination());
            value *= 1099511628211ull;
        }
        return value;
    }

    auto make_setup(double size_nm)
    {
        namespace dealloying = studies::dealloying;
        dealloying::Setup setup{};
        setup.seed = simulation::Seed{ 1, 2, 3, 4, 5, 6, 7, 8 };
        setup.make_lattice = [size_nm] {
            return dealloying::setups::bulk_with_vacancy(10, size_nm * 1_nm);
        };

#if defined(KINETALIS_PERF_REFACTOR)
        setup.model.kinetics().set_temperature(300_K).set_potential(0.88_V);
#else
        setup.make_model = [] {
            auto kinetics = std::make_unique<dealloying::kinetics::LinearBondBreaking>();
            kinetics->set_temperature(300_K).set_potential(0.88_V);
            return kinetics;
        };
#endif
        return setup;
    }
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::println(stderr, "Usage: kmc_benchmark <box_side_nm> <accepted_steps>");
        return EXIT_FAILURE;
    }

    const auto size_nm{ std::strtod(argv[1], nullptr) };
    const auto max_steps{ static_cast<kmc::Step>(std::strtoull(argv[2], nullptr, 10)) };
    if (size_nm <= 0 or max_steps == 0)
        return EXIT_FAILURE;

    auto setup{ make_setup(size_nm) };
    const auto before_init{ std::chrono::steady_clock::now() };
#if defined(KINETALIS_PERF_REFACTOR)
    kmc::RejectionTreeSimulation<studies::dealloying::Model> simulation{ setup };
#else
    kmc::RejectionTreeSimulation<studies::dealloying::Rules> simulation{ setup };
#endif
    const auto before_run{ std::chrono::steady_clock::now() };

    kmc_perf::reset();
    kmc::AnalysisPlan plan;
    plan.add<control::StepLimit>(max_steps);
    const auto status{ simulation.run(plan) };
    const auto after_run{ std::chrono::steady_clock::now() };

    const auto init_ms{ std::chrono::duration<double, std::milli>(before_run - before_init).count() };
    const auto run_ms{ std::chrono::duration<double, std::milli>(after_run - before_run).count() };
    std::println("RESULT side_nm={} target={} sites={} accepted={} status={} init_ms={:.6f} run_ms={:.6f} rate_sum={:.12g} simulation_time={:.12g} rejection_fraction={:.9g} fingerprint={}",
        size_nm, max_steps, simulation.lattice().sites().size(), simulation.step(),
        static_cast<int>(status), init_ms, run_ms, simulation.rate_tree()->rate_sum().value(),
        simulation.time().value(), simulation.rejection_fraction(),
        fingerprint(simulation.lattice()));
    kmc_perf::print();
    return simulation.step() == max_steps ? EXIT_SUCCESS : EXIT_FAILURE;
}
