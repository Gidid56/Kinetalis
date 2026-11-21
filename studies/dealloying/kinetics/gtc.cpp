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

    // A supplied path saddle overrides the geometric path calculation,
    // but never the final-site energy. The cached result already includes
    // the final-site bound for automatically classified hops.
    const double n_TS{desc.transition_state_coordination
        ? std::min(*desc.transition_state_coordination,
            static_cast<double>(gtc::final_coordination(desc.environment)))
        : gtc::saddle_coordination(desc.environment)};
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
        "Transition-state energy = max(path energy, final lattice-site energy)\n"
        "Transition-state coordination = best path bottleneck (64 samples per arc)\n"
        "No admissible arc = midpoint of the direct hop\n"
        "{}",
        PowerLawBondEnergy::output_text());
}
