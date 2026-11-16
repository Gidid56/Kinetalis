#include "pch.h"
#include "composition_dependent.h"

using namespace kmc;
using namespace studies::dealloying::models;

namespace studies::dealloying::models::composition_dependent
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


CompositionDependent::CompositionDependent()
{
    if constexpr (not lattice::element_specific_neighbors)
        throw std::logic_error{ "CompositionDependent requires element-specific neighbor tracking." };
}

process::Key CompositionDependent::classify(event::Type type, event::Location const& location)
{
    auto const& source{ location.source() };
    Descriptor desc{ type, source.element(), source.coordination(), {} };
    std::ranges::copy(source.element_neighbors(), desc.element_neighbors.begin());
    if (auto it{ keys_.find(desc) }; it != keys_.end()) return it->second;

    if (infos_.size() > std::numeric_limits<process::Key>::max())
        throw std::overflow_error{ "Increase process::Key size." };

    auto const key{ static_cast<process::Key>(infos_.size()) };
    std::string details;
    for (auto&& [element, count] : std::views::zip(lattice::active_elements, desc.element_neighbors))
        if (count) std::format_to(std::back_inserter(details), "{}x{};",
            static_cast<unsigned>(count), element);

    keys_.emplace(desc, key);
    descriptors_.push_back(desc);
    infos_.push_back(ClassInfo{ type, desc.element, desc.coordination, std::move(details) });
    return key;
}

ClassInfo const& CompositionDependent::class_info(process::Key key) const
{
    return infos_.at(key);
}

Energy CompositionDependent::lattice_energy_barrier(process::Key key) const
{
    auto const& desc{ descriptors_.at(key) };
    Energy energy{};
    for (auto&& [element, count] :
        std::views::zip(lattice::active_elements, desc.element_neighbors))
        energy += count * composition_dependent::bond_energy(desc.element, element);
    return energy;
}

std::string CompositionDependent::output_text() const
{
    return std::format("Model = Composition dependent bond energy\n{}",
        ElectrochemicalInterface::output_text());
}
