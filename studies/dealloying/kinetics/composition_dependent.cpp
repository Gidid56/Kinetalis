#include "pch.h"
#include "composition_dependent.h"
#include "lattice/lattice.h"

using namespace kmc;
using namespace studies::dealloying::kinetics;

namespace studies::dealloying::kinetics::composition_dependent
{
    using enum Element;

    Energy bond_energy(Element element)
    {
        switch (element)
        {
            case Cu: return 0.150_eV;
            case Ag: return 0.120_eV;
            case Au: return 0.150_eV;
            default:
                throw std::logic_error{
                    std::format("linear bond energy not defined for {}", symbol(element))
                };
        }
    }

    Energy mixed_bond_correction(Element first, Element second)
    {
        switch (bond_pair(first, second))
        {
            case bond_pair(Ag, Au): return 0.016_eV;
            case bond_pair(Cu, Au): return 0.030_eV;
            case bond_pair(Cu, Ag): return -0.027_eV;
            default:
                throw std::logic_error{ std::format(
                    "mixed bond correction not defined for {} and {}",
                    symbol(first),
                    symbol(second)) };
        }
    }

    Energy bond_energy(Element first, Element second)
    {
        if (first == second)
            return bond_energy(first);

        return (bond_energy(first) + bond_energy(second)) / 2 +
               mixed_bond_correction(first, second);
    }
}


CompositionDependent::CompositionDependent() = default;

RateClassKey CompositionDependent::classify(
    event::Type type, event::Location const& location, lattice::Lattice const& lattice)
{
    auto const& source{ location.source() };
    Descriptor desc{ type, source.element(), source.coordination(), {} };
    for (auto const* neighbor : lattice.neighbors(source))
        if (neighbor->is_occupied())
            ++desc.element_neighbors[lattice::elementKey(neighbor->element())];
    if (auto it{ keys_.find(desc) }; it != keys_.end()) return it->second;

    if (descriptors_.size() > std::numeric_limits<RateClassKey>::max())
        throw std::overflow_error{ "Increase RateClassKey size." };

    auto const key{ static_cast<RateClassKey>(descriptors_.size()) };
    keys_.emplace(desc, key);
    descriptors_.push_back(desc);
    return key;
}

event::Type CompositionDependent::class_event(RateClassKey key) const
{
    return descriptor(key).event;
}

Element CompositionDependent::class_element(RateClassKey key) const
{
    return descriptor(key).element;
}

Energy CompositionDependent::lattice_energy_barrier(RateClassKey key) const
{
    auto const& desc{ descriptors_.at(key) };
    Energy energy{};
    for (auto&& [element, count] :
        std::views::zip(lattice::active_elements, desc.element_neighbors))
        energy += count * composition_dependent::bond_energy(desc.element, element);
    return energy;
}

Rate CompositionDependent::rate(RateClassKey key) const
{
    return ElectrochemicalInterface::rate(
        class_event(key), class_element(key), lattice_energy_barrier(key));
}

std::string CompositionDependent::output_text() const
{
    return std::format("Model = Composition dependent bond energy\n{}",
        ElectrochemicalInterface::output_text());
}
