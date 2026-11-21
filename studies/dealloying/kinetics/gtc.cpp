#include "pch.h"
#include "gtc.h"

using namespace kmc;
using namespace studies::dealloying::kinetics;

GTC::Descriptor GTC::classify(
    event::Type type, event::Location const& location, lattice::Lattice const&) const
{
    if (type == event::Type::diffusion and location.size() != 2)
        throw std::invalid_argument{"GTC diffusion requires source and target."};

    // TODO: retain complete geometrical paths, evaluate saddle coordination
    // along each path, select the admissible lowest-barrier path, and define
    // a stable environment descriptor suitable for the KMC rate cache.
    auto const& source{location.source()};
    return {type, source.element(), source.coordination(), std::nullopt};
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

    if (desc.event != event::Type::diffusion or
        not desc.transition_state_coordination)
        return PowerLawBondEnergy::rate(baseline);

    const double n_TS{*desc.transition_state_coordination};
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
        "Model = GTC (framework; geometry-dependent rates NOT enabled)\n"
        "Transition-state inference = unresolved; fallback n_TS = {}\n{}",
        diffusion_transition_state_coordination, PowerLawBondEnergy::output_text());
}
