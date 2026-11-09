#include "pch.h"

#include "observer.h"

#include "kmc/simulation.h"

using namespace kmc;

void Observer::validate(Resolution const& resolution)
{
    if (resolution.min_snapshot_step == 0)
        throw std::invalid_argument{ "Minimum snapshot step must be positive." };

    if (resolution.snapshot_scale_factor < 1)
        throw std::invalid_argument{ "Update snapshot scale factor must be at least one." };
}

void Observer::observe_step(Simulation const& simulation)
{
    if (simulation.step() >= next_snapshot_step_)
    {
        this->take_snapshot(simulation);

        next_snapshot_step_ = std::max(
            next_snapshot_step_ + resolution_.min_snapshot_step,
            math::round<Step>(next_snapshot_step_ * resolution_.snapshot_scale_factor));
    }
}

std::format_context::iterator std::formatter<Observer::Resolution>::format(
    Observer::Resolution const& resolution,
    std::format_context& context) const
{
    return std::format_to(
        context.out(),
        "{},{:.6g}",
        resolution.min_snapshot_step,
        resolution.snapshot_scale_factor);
}
