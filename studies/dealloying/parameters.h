#pragma once

enum class CampaignType
{
    passivation,
    bifurcation,
    dealloying,
};

static constexpr Length min_box_size(CampaignType campaign_type)
{
    static constexpr auto default_box_size = 40.7_nm;

    switch (campaign_type)
    {
        case CampaignType::passivation: return 0.75 * default_box_size;
        case CampaignType::bifurcation: return 1.0 * default_box_size;
        case CampaignType::dealloying: return 1.5 * default_box_size;
        default: return default_box_size;
    }
}

static constexpr Length box_size(lattice::Composition::Quantity x0, CampaignType campaign_type)
{
    static constexpr auto default_x0 = 10.0;

    return min_box_size(campaign_type) * (x0 < default_x0 ? default_x0 / x0 : 1.0);
}

static constexpr ElectricPotential critical_potential(
    lattice::Composition::Quantity x0,
    physics::quantity::AbsoluteTemperature temperature = 300_K,
    ElectricPotential resolution = 0.1_mV)
{
    static constexpr std::array temperatures{
        250_K,
        300_K,
        350_K,
    };

    std::array<ElectricPotential, temperatures.size()> potentials;

    //switch (x0)
    //{
    //    case 2: potentials = { 0.7379_V, 0.7179_V, 0.6879_V }; break;

    //    case 4: potentials = { 0.7829_V, 0.7675_V, 0.7379_V }; break;
    //    case 7: potentials = { 0.8329_V, 0.8117_V, 0.7929_V }; break;

    //    case 10: potentials = { 0.8549_V, 0.8365_V, 0.8185_V }; break;

    //    case 15: potentials = { 0.8829_V, 0.8696_V, 0.8579_V }; break;
    //    case 20: potentials = { 0.9029_V, 0.8940_V, 0.8829_V }; break;
    //    case 25: potentials = { 0.9229_V, 0.9146_V, 0.9029_V }; break;
    //    case 30: potentials = { 0.9379_V, 0.9324_V, 0.9229_V }; break;
    //    case 35: potentials = { 0.9629_V, 0.9520_V, 0.9479_V }; break;
    //    case 40: potentials = { 0.9829_V, 0.9734_V, 0.9729_V }; break;

    //    default: return {};
    //}

    switch (x0)
    {
        case 2: potentials = { 0.7500_V, 0.7300_V, 0.7000_V }; break;

        case 4: potentials = { 0.7950_V, 0.7794_V, 0.7500_V }; break;
        case 7: potentials = { 0.8450_V, 0.8219_V, 0.8050_V }; break;

        case 10: potentials = { 0.8667_V, 0.8492_V, 0.8336_V }; break;

        case 15: potentials = { 0.8950_V, 0.8807_V, 0.8700_V }; break;
        case 20: potentials = { 0.9150_V, 0.9027_V, 0.8950_V }; break;
        case 25: potentials = { 0.9350_V, 0.9217_V, 0.9150_V }; break;
        case 30: potentials = { 0.9500_V, 0.9354_V, 0.9350_V }; break;
        case 35: potentials = { 0.9750_V, 0.9554_V, 0.9600_V }; break;
        case 40: potentials = { 0.9950_V, 0.9745_V, 0.9850_V }; break;

        default: return {};
    }

    if (temperature < temperatures.front() or temperature > temperatures.back())
        return {};

    for (std::size_t i{ 1 }; i < temperatures.size(); ++i)
    {
        if (temperature > temperatures[i])
            continue;

        auto const fraction{ (temperature - temperatures[i - 1]) /
                             (temperatures[i] - temperatures[i - 1]) };

        auto const potential{ potentials[i - 1] + fraction * (potentials[i] - potentials[i - 1]) };

        return std::round(potential / resolution) * resolution;
    }

    return potentials.back();
}

static constexpr std::vector<kmc::Process> modulated_processes()
{
    std::vector<kmc::Process> processes;

    for (lattice::Coordination coordination{ 7 }; coordination <= 10; ++coordination)
        processes.emplace_back(kmc::Event::dissolution, chemistry::Element::Ag, coordination);

    for (lattice::Coordination coordination{ 5 }; coordination <= 7; ++coordination)
        processes.emplace_back(kmc::Event::diffusion, chemistry::Element::Au, coordination);

    return processes;
}

static constexpr std::array ratios{ 0.5, 1.0 / std::numbers::sqrt2, std::numbers::sqrt2, 2.0 };

constexpr std::array seeds{
    "6cdcdd54-fb64f488-b620a9dc-c5bb7f71-cdff365b-3c80e0a6-b73997c2-274f505d",
    "dbb8b3d1-919fc29d-adae49f7-7d15f484-65f84f76-bbed03fb-773a454a-bec55b19",
    "15f1f6e6-04a81617-12dcc955-347e6662-f61ab94d-8ebcb122-e4514d7b-ea4a224f",
    "f6bc3225-293bbc55-55aff5a8-036904c6-b18609f3-da660643-7efd5e4a-f71e9547",
    "e02fce6d-918aa9ff-9b9ef10b-b64ab64c-a42205a1-62cd5c7d-5e3ce747-887a7707",
    "6995b027-5c2c8adf-d53c824a-80303835-17e233cc-c2936033-91ad59b1-dae4793f",
    "ecc5a451-fe78db88-24ac533e-9e219cd5-a0b71b23-efcc8525-94b3c53c-aa554d81",
    "ecb3fdf8-c7aee8e8-f2c709c2-125e82b0-7c8c8f25-c0e7ddef-9880f432-79e4f8f8",
    "2141d92c-785407a0-c6ec0f1a-0f0fdc4a-247c9228-ddb4a89a-9b2eeb3e-398926f0",
    "74fad755-112a3ee6-b6877ff0-68894d24-d135688c-053f4e8d-616db4c3-f78bc7d3",
    "dc57ffec-d70716c2-aeb9aaa1-ec189104-a3bf0479-82760a0d-91d274fc-40b2312d",
    "052ecab0-f3406e23-d1f42ab6-584ebd85-a73edc55-1e188a04-1637ebcf-4442843f",
    "bedaca1c-18c60a28-df0ef6bd-5c8477f8-2e6989b2-666e2ccc-d879a8bc-c792a31b",
    "f08375b7-d90df0b9-d0690bd4-0db0c856-7dbc9cc2-f9333719-794446a2-60794d05",
    "fbc1dfd9-081b0f5f-beba1577-0ea3ef69-7ee9cf15-cfcfb71f-71357fbf-fae2dded",
    "bb4798ed-cd2a09c8-6a7d5528-284cd316-6731cd1e-9e0de6d7-70ea204f-bedf3923",
};
