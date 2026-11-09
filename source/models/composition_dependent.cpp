#include "pch.h"

#include "composition_dependent.h"

#include "kmc/process.h"

using namespace kmc;

namespace kmc::models::composition_dependent
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

Energy models::CompositionDependent::lattice_energy_barrier(Process const& process) const
{
    Energy energy{};

    for (auto&& [element, coordination] :
        std::views::zip(lattice::activeElements, process.element_neighbors()))
    {
        energy += coordination * composition_dependent::bond_energy(process.element(), element);
    }
    return energy;
}

std::string models::CompositionDependent::output_text() const
{
    return std::format(
        "Model = Composition dependent bond energy\n"
        "{}",
        ElectrochemicalInterface::output_text());
}