#pragma once

#include "../studies/dealloying/analysis/regime.h"
#include "kmc/model.h"
#include "kmc/observer.h"
#include "output/event_flux.h"
#include "output/phases.h"
#include "output/process_stats.h"

namespace viewer::gui
{
    struct Status
    {
        kmc::Step step{};
        double rejectionFraction{};
        kmc::Time physicalTime{};
        kmc::Time runTime{};
        kmc::Temperature temperature{};
        ElectricPotential potential{};
        studies::dealloying::analysis::Regime::Condition reachedConditions{};

        std::map<kmc::process::Outcome, output::process::CountMap> processCounts;

        void setData(kmc::Observer const&);
    };

    struct Sample
    {
        kmc::Observer::Snapshot snapshot{};

        std::optional<output::Phases::Data> phasesData;
        std::optional<output::process::EventFlux::Data> eventFluxData;

        void appendData(kmc::Observer const&);
    };
}
