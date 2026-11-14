#include "pch.h"

#include "phases.h"

#include "kmc/simulation.h"
#include "utility/file.h"

using namespace utility;
using enum file::Type;

using namespace lattice;

using namespace output;

bool Phases::is_phase(Site const& site, Phase phase)
{
    switch (phase)
    {
        case Phase::pore: return site.is_vacant();
        case Phase::solid: return site.is_occupied();
        case Phase::pristine: return site.is_occupied() and site.is_pristine();
        case Phase::silver: return site.element() == Element::Ag;
    }
    return false;
}

void Phases::capture_init(kmc::Simulation const& kmc)
{
    last_data_ = {};

    lattice_ = &kmc.lattice();
    next_neighbor_distance_ = kmc.lattice().max_path_length();
    outer_surface_area_ = kmc.lattice().shape().surface_area();
    site_count_ = kmc.lattice().sites().size();

    counts_.clear();
    correlation_.clear();

    for (auto& site : kmc.lattice().sites())
        for (auto phase : active_phases)
            if (is_phase(site, phase))
            {
                ++counts_[phase];

                correlation_[phase] +=
                    std::ranges::count_if(kmc.lattice().neighbors(site), [phase](auto neighbor) {
                        return is_phase(*neighbor, phase);
                    });
            }
}

void Phases::capture_snapshot(kmc::Simulation const& kmc)
{
    last_data_ = { kmc.step(), kmc.time() };

    for (auto phase : active_phases)
    {
        last_data_.fractions[phase] = static_cast<double>(counts_[phase]) / site_count_;
        last_data_.correlation_lengths[phase] = calculate_correlation_length(phase);
    }
}

void Phases::observe_selected(kmc::process::Record const& process)
{
    if (process.outcome != kmc::process::Outcome::accepted)
        return;

    const auto& site{ process.path.front() };
    auto solid_change{ process.process.solid_change() };
    auto neighbor_change{ process.process.neighbor_change(true) };

    counts_[Phase::pore] -= solid_change;
    counts_[Phase::solid] += solid_change;

    correlation_[Phase::pore] -= 2 * neighbor_change * site.vacant_neighbors();
    correlation_[Phase::solid] += 2 * neighbor_change * site.coordination();

    for (auto phase : { Phase::pristine, Phase::silver })
        if (std::ranges::contains(active_phases, phase))
            if (is_phase(site, phase))
            {
                counts_[phase] += solid_change;
                correlation_[phase] += 2 * neighbor_change * phase_neighbor_count(site, phase);
            }
}

//if constexpr (useLayers)
//{
//	phasesByLayers_[Phase::pore][site.layer()] -= delta;
//	phasesByLayers_[Phase::solid][site.layer()] += delta;
//	if (site.isPhase(Phase::silver))
//		phasesByLayers_[Phase::silver][site.layer()] += delta;
//	if (site.isPhase(Phase::pristine))
//		phasesByLayers_[Phase::pristine][site.layer()] += delta;
//}

void Phases::observe_processed(kmc::process::Record const& process)
{
    if (process.process.event() != kmc::Event::diffusion)
        return;

    const auto& site{ process.path.back() };
    auto neighbor_change{ process.process.neighbor_change(false) };

    correlation_[Phase::pore] -= 2 * neighbor_change * site.vacant_neighbors();
    correlation_[Phase::solid] += 2 * neighbor_change * site.coordination();

    for (auto phase : { Phase::pristine, Phase::silver })
        if (std::ranges::contains(active_phases, phase))
            if (is_phase(site, phase))
            {
                correlation_[phase] += 2 * neighbor_change * phase_neighbor_count(site, phase);
            }
}

std::string Phases::settings() const
{
    auto text = std::format(
        "active_phases = {}\n"
        "max_coordination = {}\n",
        active_phases,
        max_coordination);

    return text;
}


Phases::Count Phases::phase_neighbor_count(Site const& site, Phase phase) const
{
    return std::ranges::count_if(lattice_->neighbors(site), [phase](auto const neighbor) {
        return is_phase(*neighbor, phase);
    });
}

Length Phases::calculate_correlation_length(Phase phase)
{
    if (counts_[phase] == 0)
        return Length{};

    const auto auto_correlation{ static_cast<double>(correlation_[phase]) /
                                 (max_coordination * counts_[phase]) };
    return next_neighbor_distance_ / (1 - auto_correlation);
}

void Phases::write_init(kmc::Simulation const&)
{
    std::string header{ default_header };

    for (auto phase : active_phases)
        std::format_to(
            std::back_inserter(header),
            ",{0}_fraction,{0}_correlation_length/nm",
            phase);

    file::write<csv>(writer_path(), header += '\n');
}

void Phases::write_snapshot(kmc::Simulation const& kmc)
{
    std::string text{ std::format("{},{:v:}", kmc.step(), kmc.time()) };

    for (auto phase : active_phases)
        std::format_to(
            std::back_inserter(text),
            ",{},{:v:nm}",
            fraction(phase),
            correlation_length(phase));

    file::append<csv>(writer_path(), text += '\n');
}

//if constexpr (lattice::useLayers)
//{
//	std::string text;
//
//	for (auto const& position : kmc.lattice().layers() | std::views::keys)
//		std::format_to(std::back_inserter(text), "{:nm}\n", position);
//
//	write<csv>(root_path() / "layers", text);
//}
