#include "pch.h"

#include "dealloying/analysis_plans.h"
#include "dealloying/setups.h"
#include "viewer/core.h"
#include "viewer/viewer.h"

using namespace studies::dealloying;

int run_viewer()
{
    Viewer viewer;

    kmc::Model::Modulation modulation{
        kmc::Process{ kmc::Event::dissolution, chemistry::Element::Ag, 9 },
        0.5_ratio,
    };

    viewer.createSimulation(
        //setups::schematic(3),
        setups::pore_dealloying(CampaignType::passivation, 10, -16_mV, 300_K),
        analysis_plans::regime_outcome
        );

    return viewer.
        run().
        exec();
}

int main()
{
    try
    {
        return run_viewer();
    }
    catch (const std::exception& exception)
    {
        std::print("\nError: {}\n\n", exception.what());
        return EXIT_FAILURE;
    }
}
