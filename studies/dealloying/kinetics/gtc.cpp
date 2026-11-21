#include "pch.h"
#include "gtc.h"
#include "gtc_path_cache.h"
#include <memory>

using namespace kmc;
using namespace studies::dealloying::kinetics;

GTC::Descriptor GTC::classify(
    event::Type type, event::Location const& location, lattice::Lattice const& lattice) const
{
    auto const& source{location.source()};
    Descriptor result{type, source.element(), source.coordination(), std::nullopt, 0};

    if (type == event::Type::diffusion)
    {
        if (location.size() != 2)
            throw std::invalid_argument{"GTC diffusion requires source and target."};

        // Lattice-independent canonical bit meanings; the expensive direction
        // to neighbor-list mapping is established only once per lattice.
        if (not environment_ or not environment_->belongs_to(lattice))
            environment_ = std::make_shared<gtc::FccEnvironment>(lattice);

        result.environment = environment_->classify(source, *location.sites()[1]);
    }

    return result;
}

RateClassKey GTC::key(Descriptor const& desc)
{
    if (desc.transition_state_coordination and
        (not std::isfinite(*desc.transition_state_coordination) or
         *desc.transition_state_coordination < 0.0))
        throw std::invalid_argument{"GTC saddle coordination must be finite and nonnegative."};

    if (auto it{keys_.find(desc)}; it != keys_.end())
        return it->second;

    if (descriptors_.size() > std::numeric_limits<RateClassKey>::max())
        throw std::overflow_error{"Increase RateClassKey size."};

    auto const key{static_cast<RateClassKey>(descriptors_.size())};
    keys_.emplace(desc, key);
    descriptors_.push_back(desc);
    return key;
}

Rate GTC::rate(Descriptor const& desc) const
{
    const PowerLawBondEnergy::Descriptor baseline{
        desc.event, desc.element, desc.coordination
    };

    if (desc.event != event::Type::diffusion)
        return PowerLawBondEnergy::rate(baseline);

    // An explicitly supplied saddle overrides the geometric lookup.
    // Otherwise classify() supplies the complete canonical 18-bit mask.
    const auto saddle{desc.transition_state_coordination
        ? desc.transition_state_coordination
        : gtc::saddle_coordination(desc.environment)};
    if (not saddle)
        return PowerLawBondEnergy::rate(baseline);

    const double n_TS{*saddle};
    if (not std::isfinite(n_TS) or n_TS < 0.0)
        throw std::invalid_argument{"GTC saddle coordination must be finite and nonnegative."};

    const Energy barrier{std::max<Energy>(0_eV,
        binding_energy(desc.element, static_cast<double>(desc.coordination)) -
        binding_energy(desc.element, n_TS))};

    return ElectrochemicalInterface::rate(
        desc.event, desc.element, barrier, modulation_ratio(baseline));
}

std::string GTC::output_text() const
{
    return std::format(
        "Model = GTC (FCC NN-shell transition-state geometry)\n"
        "Geometry = canonical 18-bit occupancy; direct and 2-segment arcs\n"
        "Transition-state coordination = best path bottleneck (64 samples per arc)\n"
        "No admissible path = power-law fallback n_TS = {}\n{}",
        diffusion_transition_state_coordination, PowerLawBondEnergy::output_text());
}
