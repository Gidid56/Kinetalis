#include "pch.h"

#include "regime.h"

#include "kmc/simulation.h"
#include "lattice/lattice.h"
#include "physics/equations/geometry.h"
#include "utility/file.h"

using namespace utility::file;
using enum utility::file::Type;

using namespace studies::dealloying;
using namespace studies::dealloying::analysis;
using namespace physics::equations;
using enum output::Phases::Phase;

std::string Regime::settings() const
{
    auto format_conditions = [](Condition conditions) {
        return conditions == Condition::none ? std::string{ enum_name(conditions) }
                                             : enum_flags_name(conditions);
    };
    auto text = std::format(
        "stop_conditions = {}\n"
        "marker_conditions = {}\n",
        format_conditions(stop_conditions_),
        format_conditions(marker_conditions_));

    if (not thresholds_.empty())
        text += "thresholds:\n";

    for (auto const& [condition, threshold] : thresholds_)
    {
        text += std::format("  {} = {}\n", condition, threshold);
    }

    if (not lookback_periods_.empty())
        text += "lookback_periods:\n";

    for (auto const& [condition, fraction] : lookback_periods_)
    {
        text += std::format("  {} = {}\n", condition, fraction);
    }
    text += std::format("save_lattice = {}\n", save_lattice_);

    return text;
}

void Regime::capture_init(kmc::Simulation const& kmc)
{
    phases_.capture_init(kmc);
    surface_.capture_init(kmc);
    event_flux_.capture_init(kmc);

    master_alloy_fraction_ = kmc.lattice().master_alloy().fraction(lattice::Element::Au);
    lattice_volume_ = kmc.lattice().shape().volume();

    observer_conditions_ = marker_conditions_ | stop_conditions_;
    reached_conditions_ = {};

    snapshots_.clear();
    history_.clear();
}

void Regime::observe_selected(kmc::process::Record const& process)
{
    phases_.observe_selected(process);

    if ((observer_conditions_ & Condition::passivated) != Condition::none)
        surface_.observe_selected(process);

    if ((observer_conditions_ & Condition::arrested) != Condition::none)
        event_flux_.observe_selected(process);
}

void Regime::observe_processed(kmc::process::Record const& process)
{
    phases_.observe_processed(process);

    if ((observer_conditions_ & Condition::passivated) != Condition::none)
        surface_.observe_processed(process);
}

auto Regime::lookback_start_step(Condition condition) const
{
    auto const fraction{ lookback_periods_.at(condition) };

    return (1.0 - fraction) * phases_.last_data().step;
}

bool Regime::has_lookback(Condition condition) const
{
    return not history_.empty() and history_.front().step <= lookback_start_step(condition);
}

auto Regime::lookback(Condition condition) const
{
    auto const first_step{ lookback_start_step(condition) };

    return history_ | std::views::drop_while([first_step](output::Phases::Data const& data) {
        return data.step < first_step;
    });
}

void Regime::capture_snapshot(kmc::Simulation const& kmc)
{
    phases_.capture_snapshot(kmc);
    event_flux_.capture_snapshot(kmc);

    auto const pore_volume{ lattice_volume_ * phases_.fraction(pore) };
    auto const surface_excess{
        sphere::radius(pore_volume) / phases_.correlation_length(pore) * 4. / 3. - 1.0
    };
    if (surface_excess > max_surface_excess_)
        max_surface_excess_ = surface_excess;

    for (auto const condition : enum_values<Condition>())
        if ((marker_conditions_ & condition) != Condition::none)
            if (not has_reached(condition) and meets(condition))
                record(condition);

    if (lookback_periods_.empty())
        return;

    history_.emplace_back(phases_.last_data());

    auto const max_lookback{ std::ranges::max(lookback_periods_ | std::views::values) };
    while (history_.front().step < (1 - max_lookback) * history_.back().step)
        history_.pop_front();
}

void Regime::capture_final(kmc::Simulation const&)
{
    if (not has_reached(stop_conditions_))
        snapshots_.emplace(Condition::none, phases_.last_data());
}

bool Regime::meets(Condition condition) const
{
    switch (condition)
    {
        case Condition::bifurcated: return is_bifurcated();
        case Condition::porous: return is_porous();
        case Condition::coarsening: return is_coarsening();
        case Condition::passivated: return is_passivated();
        case Condition::arrested: return is_arrested();
        default: return false;
    }
}

bool Regime::is_bifurcated() const
{
    auto const pore_volume{ lattice_volume_ * phases_.fraction(pore) };

    return sphere::radius(pore_volume) / phases_.correlation_length(pore) >
           thresholds_.at(Condition::bifurcated);
}

bool Regime::is_porous() const
{
    return phases_.fraction(pristine) < thresholds_.at(Condition::porous);
}

bool Regime::is_coarsening() const
{
    if (not has_reached(Condition::porous) or not has_lookback(Condition::coarsening))
        return false;

    auto const current_length{ phases_.correlation_length(solid) };

    return std::ranges::all_of(
        lookback(Condition::coarsening),
        [current_length](output::Phases::Data const& previous) {
            return current_length >= previous.correlation_lengths.at(solid);
        });
}

bool Regime::is_passivated() const
{
    auto const x0{ master_alloy_fraction_ };
    auto const pore_sites{ static_cast<Number>(phases_.count(pore)) };
    auto const surface_sites{ static_cast<Number>(surface_.sites()) };

    auto const passivation_fraction{ x0 / (1 - x0) * pore_sites / surface_sites };

    return passivation_fraction >= thresholds_.at(Condition::passivated);
}

bool Regime::is_arrested() const
{
    if (auto relative_dissolution_flux{ event_flux_.relative_flux(kmc::Event::dissolution) })
        return *relative_dissolution_flux < thresholds_.at(Condition::arrested);

    return false;
}

void Regime::record(Condition condition)
{
    if (has_reached(condition))
        return;

    reached_conditions_ |= condition;
    snapshots_.emplace(condition, phases_.last_data());

    if (save_lattice_)
        write<xyz>(
            root_path() / "lattice" / enum_name(condition),
            phases_.lattice().to_xyz_file(false));
}

void Regime::write_init(kmc::Simulation const& kmc)
{
    phases_.write_init(kmc);
    surface_.write_init(kmc);
    event_flux_.write_init(kmc);
}

void Regime::write_snapshot(kmc::Simulation const& kmc)
{
    phases_.write_snapshot(kmc);
    surface_.write_snapshot(kmc);
    event_flux_.write_snapshot(kmc);
}

void Regime::write_final(kmc::Simulation const&)
{
    std::string text{ "key:condition,step,time/s" };

    for (auto const phase : output::Phases::active_phases)
        std::format_to(std::back_inserter(text), ",{0}_fraction,{0}_correlation_length/nm", phase);

    text += '\n';

    for (auto const& [condition, data] : snapshots_)
    {
        std::format_to(
            std::back_inserter(text),
            "{},{},{:v:}",
            enum_name(condition),
            data.step,
            data.time);

        for (auto const phase : output::Phases::active_phases)
            std::format_to(
                std::back_inserter(text),
                ",{},{:v:nm}",
                data.fractions.at(phase),
                data.correlation_lengths.at(phase));

        text += '\n';
    }

    write<csv>(root_path() / "regimes", text);

    std::string text2{ "key:condition,excess\n" };

    std::format_to(std::back_inserter(text2), "final,{}", max_surface_excess_);

    write<csv>(root_path() / "surface_excess", text2);
}
