#pragma once

enum class CampaignType
{
    passivation,
    bifurcation,
    dealloying,
};

constexpr std::array seeds{
    "a6398046-93eac882-daab407f-43ad7f8a-9e1dec6b-2cfa7509-d5d06115-cdedda63",
    "b0cfadb8-0da64832-ceb7db8e-c80485e3-8383e059-8242fe5f-d67432fa-090fd6bf",
    "a7a6e3f6-e81ddb7a-c111cbec-81da6d1c-bc0080ef-b15004d6-5d7d64cf-77bc1ae3",
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
        case 2: potentials = { 0.7379_V, 0.7179_V, 0.6879_V }; break;

        case 4: potentials = { 0.7829_V, 0.7673_V, 0.7379_V }; break;
        case 7: potentials = { 0.8329_V, 0.8098_V, 0.7929_V }; break;

        case 10: potentials = { 0.8546_V, 0.8371_V, 0.8215_V }; break;

        case 15: potentials = { 0.8829_V, 0.8686_V, 0.8579_V }; break;
        case 20: potentials = { 0.9029_V, 0.8906_V, 0.8829_V }; break;
        case 25: potentials = { 0.9229_V, 0.9096_V, 0.9029_V }; break;
        case 30: potentials = { 0.9379_V, 0.9233_V, 0.9229_V }; break;
        case 35: potentials = { 0.9629_V, 0.9433_V, 0.9479_V }; break;
        case 40: potentials = { 0.9829_V, 0.9624_V, 0.9729_V }; break;

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
