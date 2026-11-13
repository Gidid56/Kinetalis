#pragma once
#include "dealloying/analysis/electrochemistry.h"
#include "dealloying/analysis/regime.h"
#include "kmc/analysis_plan.h"
#include "output/basics.h"
#include "output/phases.h"
#include "output/process_kinetics.h"
#include "output/process_stats.h"

namespace studies::dealloying::analysis_plans
{
    using namespace physics::unit::literals;

    inline kmc::AnalysisPlan basic(std::filesystem::path const& path)
    {
        kmc::AnalysisPlan plan;

        plan.add<output::Config>(path);
        plan.add<output::Rates>(path);
        plan.add<output::Composition>(path);
        plan.add<output::Performance>(path);

        return plan;
    }

    inline kmc::AnalysisPlan statistics(std::filesystem::path const& path)
    {
        kmc::AnalysisPlan plan = basic(path);

        plan.add<output::Time>(path);
        plan.add<output::Phases>(path);
        plan.add<output::Surface>(path);
        plan.add<output::process::EventFlux>(path);
        plan.add<output::process::Statistics>(path).set_resolution(
            { .min_snapshot_step = 1000, .snapshot_scale_factor = 1.2 });

        return plan;
    }

    inline kmc::AnalysisPlan regime_outcome(std::filesystem::path const& path)
    {
        kmc::AnalysisPlan plan = basic(path);

        plan.add<control::DurationCap>(1_h);

        //plan.add<analysis::Electrochemistry>(path);
        //plan.add<output::process::Kinetics>(path);
        //plan.add<output::process::Statistics>(path).set_resolution(
        //    { .min_snapshot_step = 1000, .snapshot_scale_factor = 1.2 });

        using enum analysis::Regime::Condition;

        plan.add<analysis::Regime>(path)
            .set_resolution({ .min_snapshot_step = 10, .snapshot_scale_factor = 1.01 })
            .set_save_lattice(false)
            .set_stop_conditions(porous | arrested)
            .set_marker_conditions(bifurcated | passivated | porous | arrested)

            // Regime-detection thresholds and lookback periods
            // Values correspond to the criteria used in the published analysis [...]
            .set_threshold(bifurcated, 1.25) // R_vol / ℓ_ac
            .set_threshold(passivated, 1.0) // f_pas
            .set_threshold(porous, 0.8)     // φ_pristine
            .set_threshold(arrested, 1e-4)  // Γ_dis / Γ_tot

            .set_stop_conditions(bifurcated | passivated)
            //.set_lookback_period(coarsening, 0.1)
            //.set_stop_conditions(coarsening)
            ;

        return plan;
    }

    inline kmc::AnalysisPlan geometry(std::filesystem::path const& path)
    {
        kmc::AnalysisPlan plan = regime_outcome(path);

        //plan.get<control::DurationCap>().set_cap(1_d);

        plan.get<analysis::Regime>().set_stop_conditions(analysis::Regime::Condition::none);

        //plan.add<output::Lattice>(path).set_save_bulk(false).set_resolution(
        //    { .min_snapshot_step = 10, .snapshot_scale_factor = 1.01 });

        plan.add<output::Surface>(path);
        plan.add<output::process::EventFlux>(path);
        plan.add<output::process::Kinetics>(path);
        plan.add<output::process::Statistics>(path).set_resolution(
            { .min_snapshot_step = 1000, .snapshot_scale_factor = 1.2 });

        return plan;
    }
}
