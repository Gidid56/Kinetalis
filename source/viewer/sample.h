#pragma once

#include "dealloying/analysis/regime.h"
#include "dealloying/model.h"
#include "dealloying/kinetics/descriptor_format.h"
#include "kmc/model.h"
#include "kmc/observer.h"
#include "output/event_flux.h"
#include "output/phases.h"
#include "output/kinetic_statistics.h"

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

        using Model = studies::dealloying::Model;
        using KineticCounts = output::KineticStatistics<Model>::CountMap;
        std::map<kmc::event::Outcome, KineticCounts> kineticCounts;

        void setData(kmc::Observer const&);
    };

    struct Sample
    {
        kmc::Observer::Snapshot snapshot{};

        std::optional<output::Phases::Data> phasesData;
        std::optional<output::EventFlux::Data> eventFluxData;

        void appendData(kmc::Observer const&);
    };
}
