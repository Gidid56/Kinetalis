#include "pch.h"

#include "linear_bond_breaking.h"

using namespace kmc;
using chemistry::Element;
using namespace studies::dealloying::kinetics;

void LinearBondBreaking::ensure_range(Energy bond_energy)
{
    if (not isfinite(bond_energy))
        throw std::invalid_argument{ "Bond energy must be finite." };
}

RateClassKey LinearBondBreaking::static_key(Descriptor const& desc)
{
    constexpr auto elements{ std::size(lattice::active_elements) };
    constexpr auto coordinations{ lattice::max_coordination };

    std::size_t event_index{};
    switch (desc.event)
    {
        case event::Type::diffusion: event_index = 0; break;
        case event::Type::dissolution: event_index = 1; break;
        case event::Type::deposition: event_index = 2; break;
        default: throw std::logic_error{ "Invalid kinetic event type." };
    }

    auto const element{ std::ranges::find(lattice::active_elements, desc.element) };
    if (element == lattice::active_elements.end() or desc.coordination >= coordinations)
        throw std::out_of_range{ "Descriptor lies outside static kinetic classes." };

    return static_cast<RateClassKey>(
        1 + (event_index * elements + (element - lattice::active_elements.begin())) *
            coordinations + desc.coordination);
}

LinearBondBreaking::Descriptor LinearBondBreaking::descriptor(RateClassKey key) const
{
    constexpr std::array events{ event::Type::diffusion,
        event::Type::dissolution,
        event::Type::deposition };
    constexpr auto elements{ std::size(lattice::active_elements) };
    constexpr auto coordinations{ static_cast<std::size_t>(lattice::max_coordination) };

    if (key == 0 or key >= key_count)
        throw std::out_of_range{ "Invalid kinetic class key." };

    std::size_t index{ key - 1 };

    const auto coordination{ static_cast<lattice::Coordination>(index % coordinations) };
    index /= coordinations;

    const auto element{ lattice::active_elements[index % elements] };
    const auto type{ events[index / elements] };

    return { type, element, coordination };
}

LinearBondBreaking::Descriptor
LinearBondBreaking::classify(event::Type type, event::Location const& location, Lattice const&) const
{
    return { type, location.source().element(), location.source().coordination() };
}

std::size_t LinearBondBreaking::number_keys() const noexcept { return key_count; }

Energy LinearBondBreaking::lattice_energy_barrier(Descriptor const& desc) const
{
    return constants_.bond_energy * desc.coordination;
}

Ratio LinearBondBreaking::modulation_ratio(Descriptor const& desc) const
{
    if (modulation_ and desc == modulation_->descriptor)
        return modulation_->ratio;
    return 1;
}

Rate LinearBondBreaking::rate(Descriptor const& desc) const
{
    return ElectrochemicalInterface::rate(
        desc.event, desc.element, lattice_energy_barrier(desc), modulation_ratio(desc));
}

std::string LinearBondBreaking::output_text() const
{
    auto result{ std::format(
        "Model = Linear bond breaking\nBond energy = {:meV}\n{}",
        constants_.bond_energy,
        ElectrochemicalInterface::output_text()) };
    if (modulation_)
        result += std::format(
            "Modulated kinetic class = {}_{}_{}\nModulation ratio = {}\n",
            enum_name(modulation_->descriptor.event),
            modulation_->descriptor.element,
            static_cast<unsigned>(modulation_->descriptor.coordination),
            modulation_->ratio);
    return result;
}
