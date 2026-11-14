#pragma once
#include "batch/campaign.h"
#include "dealloying/analysis_plans.h"
#include "dealloying/setups.h"
#include "rules.h"

namespace studies::dealloying::postprocess
{
    void find_regime_outcome(std::filesystem::path const& campaignPath)
    {
        batch::campaign::merge(campaignPath, "regimes");
        batch::campaign::summarize(campaignPath, "regimes");
    }

    void find_surface_outcome(std::filesystem::path const& campaignPath)
    {
        batch::campaign::merge(campaignPath, "surface");
        batch::campaign::summarize(campaignPath, "surface");
    }

    void find_surface_excess(std::filesystem::path const& campaignPath)
    {
        batch::campaign::merge(campaignPath, "surface_excess");
        batch::campaign::summarize(campaignPath, "surface_excess");
    }
}

namespace studies::dealloying::campaigns
{
    std::vector const dilute_composition_range{ 4, 7 };
    std::vector const main_composition_range{ 10, 15, 20, 25, 30, 35, 40 };
    std::vector const full_composition_range{ 4, 7, 10, 15, 20, 25, 30, 35, 40 };
    std::vector const upper_composition_range{ 20, 25, 30, 35, 40 };

    std::vector const temperature_range{ sequence::linear<kmc::Temperature>(250_K, 350_K, 25_K) };

    std::vector const critical_potential_search_range{
        sequence::linear<ElectricPotential>(-30_mV, 10_mV, 2_mV)
    };
    std::vector const critical_potential_wide_search_range{
        sequence::linear<ElectricPotential>(-50_mV, 0_mV, 10_mV)
    };
    std::vector const critical_morphology_range{
        sequence::linear<ElectricPotential>(-77.1_mV, 82.9_mV, 20_mV)
    };
    std::vector const subcritical_potentials{
        -200_mV,
        -100_mV,
        -50_mV,
        -20_mV,
    };

    inline auto generate_setups(
        CampaignType campaign_type,
        std::vector<ElectricPotential> overpotentials = { 0_V },
        std::vector<int> compositions = { 10 },
        std::vector<kmc::Temperature> temperatures = { 300_K })
    {
        std::vector<Setup> setups;

        for (auto temperature : temperatures)
            for (lattice::Composition::Quantity const x0 : compositions)
                for (ElectricPotential overpotential : overpotentials)
                    setups.emplace_back(
                        setups::pore_dealloying(campaign_type, x0, overpotential, temperature));

        return setups;
    }

    inline Campaign post_processing()
    {
        return {
            .name = "critical composition",
            .replicate_count = 20,
            .postprocessor = postprocess::find_regime_outcome,
        };
    }

    inline Campaign pore_surface_morphology()
    {
        return {
            .name = "pore surface morphology",
            //.seed = "6791dff7-d3b546b9-93e332b7-bacb20c4-f43b2c56-05008600-6a91e4fc-9a6e2626",
            //.seed = "e7d526b4-723b3def-502439d4-e7c787e8-a5e24a89-6c7537a6-c9856836-93583aab",
            //.seed = "84750a87-00df5231-b7b2b84b-e4fbf896-68b3d209-77fef6ff-23bc881f-f1ae0817",
            //.seed = seeds[0],
            .replicate_count = 1,
            .setups = generate_setups(CampaignType::dealloying, { -40_mV }, full_composition_range),
            .analysis_plan = analysis_plans::geometry,
            .postprocessor = postprocess::find_regime_outcome,
        };
    }

    inline Campaign passivation_study()
    {
        return {
            .name = "subcritical passivation",
            .seed = seeds[1],
            .replicate_count = 20,
            .setups = generate_setups(
                CampaignType::passivation,
                subcritical_potentials,
                full_composition_range),
            .analysis_plan = analysis_plans::regime_outcome,
            .postprocessor = postprocess::find_regime_outcome,
        };
    }

    inline Campaign critical_potential_composition()
    {
        return {
            .name = "critical composition",
            .seed = seeds[2],
            .replicate_count = 20,
            .setups = generate_setups(
                CampaignType::bifurcation,
                critical_potential_search_range,
                full_composition_range),
            .analysis_plan = analysis_plans::regime_outcome,
            .postprocessor = postprocess::find_regime_outcome,
        };
    }

    inline Campaign critical_potential_temperature()
    {
        return {
            .name = "critical temperature",
            .seed = seeds[3],
            .replicate_count = 20,
            .setups = generate_setups(
                CampaignType::bifurcation,
                critical_potential_search_range,
                { 10 },
                temperature_range),
            .analysis_plan = analysis_plans::regime_outcome,
            .postprocessor = postprocess::find_regime_outcome,
        };
    }

    inline Campaign critical_potential_bifurcation()
    {
        return {
            .name = "surface excess (x07)",
            //.seed = seeds[2],
            .replicate_count = 20,
            .setups =
                generate_setups(CampaignType::bifurcation, critical_potential_search_range, { 7 }),
            .analysis_plan = analysis_plans::regime_outcome,
            .postprocessor = postprocess::find_surface_excess,
        };
    }

    inline auto radius_scan(auto min_radius = 1_nm, auto max_radius = 50_nm)
    {
        std::vector<Setup> setups;

        for (bool inverted : { true, false })
            for (auto radius{ 0_nm }; radius < max_radius; radius += min_radius)
                setups.emplace_back(setups::geometry_analysis(radius, inverted));

        return setups;
    }

    inline Campaign surface_geometry()
    {
        return {
            .name = "surface geometry",
            .seed = seeds[4],
            .replicate_count = 20,
            .setups = radius_scan(0.5_nm, 30_nm),
        };
    }

    inline auto generate_modulation()
    {
        std::vector<Setup> setups;

        for (auto& process : modulated_processes())
            for (auto ratio : ratios)
                for (auto overpotential : critical_potential_wide_search_range)
                {
                    kmc::Model::Modulation modulation{ process, ratio };
                    setups.emplace_back(
                        setups::pore_dealloying(
                            CampaignType::bifurcation,
                            10,
                            overpotential,
                            300_K,
                            modulation));
                }

        return setups;
    }

    inline Campaign sesitivity_test()
    {
        return {
            .name = "sesitivity test",
            .seed = seeds[5],
            .replicate_count = 20,
            .setups = generate_modulation(),
            .analysis_plan = analysis_plans::regime_outcome,
            .postprocessor = postprocess::find_regime_outcome,
        };
    }

    inline Campaign bifurcation_radius()
    {
        return {
            .name = "bifurcation radius",
            .seed = seeds[6],
            .replicate_count = 40,
            .setups = generate_setups(
                CampaignType::passivation,
                { 5_mV },
                { 7, 10, 15, 20, 25, 30, 35, 40 }),
            .analysis_plan = analysis_plans::regime_outcome,
            .postprocessor = postprocess::find_regime_outcome,
        };
    }
}
