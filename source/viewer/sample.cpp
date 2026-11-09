#include "pch.h"

#include "core.h"

#include "sample.h"

using namespace studies::dealloying;

void viewer::gui::Status::setData(kmc::Observer const& observer)
{
    if (auto const* regime{ dynamic_cast<analysis::Regime const*>(&observer) })
    {
        reachedConditions = regime->reached_conditions();
    }

    if (auto const* processStats{ dynamic_cast<output::process::Statistics const*>(&observer) })
    {
        processCounts = processStats->counts();
    }
}

void viewer::gui::Sample::appendData(kmc::Observer const& observer)
{
    if (auto const* phases{ dynamic_cast<output::Phases const*>(&observer) })
    {
        phasesData = phases->last_data();
    }

    else if (auto const* regime{ dynamic_cast<analysis::Regime const*>(&observer) })
    {
        phasesData = regime->phases().last_data();
    }

    if (auto const* eventFlux{ dynamic_cast<output::process::EventFlux const*>(&observer) })
    {
        eventFluxData = eventFlux->last_data();
    }

    else if (auto const* regime{ dynamic_cast<analysis::Regime const*>(&observer) })
    {
        eventFluxData = regime->event_flux().last_data();
    }
}
