#include "pch.h"

#include "dealloying/analysis_plans.h"
#include "dealloying/setups.h"
#include "viewer/core.h"
#include "viewer/viewer.h"

using namespace studies::dealloying;

int run_viewer()
{
    Viewer viewer;

    viewer.createSimulation(
    //    setups::thin_film_dealloying(
    //        25,         // 10 at.% Au
    //        500_mV,     // Overpotential
    //        300_K,      // Temperature
    //        20_nm,      // Lateral dimensions
    //        30_nm,      // Total box height
    //        2.5_nm,     // Au substrate / vacuum thickness
    //        { 1, 1, 1 } // Surface orientation
    //        ),
        setups::pore_dealloying(CampaignType::debugging, 10, 60_mV, 300_K),
        analysis_plans::basic
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
