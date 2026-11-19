#include "pch.h"
#include "power_law_bond_energy.h"

using namespace kmc;
using chemistry::Element;
using namespace studies::dealloying::kinetics;

namespace studies::dealloying::kinetics::power_law_bond_energy
{
    using enum chemistry::Element;

    Energy dimer_bond_energy(Element element)
    {
        switch (element)
        {
            case Al: return 1.609_eV;
            case Ca: return 0.873_eV;
            case Ni: return 2.107_eV;
            case Cu: return 1.656_eV;
            case Sr: return 0.816_eV;
            case Rh: return 2.728_eV;
            case Pd: return 1.846_eV;
            case Ag: return 1.400_eV;
            case Yb: return 0.759_eV;
            case Ir: return 3.293_eV;
            case Pt: return 2.771_eV;
            case Au: return 1.808_eV;
            case Pb: return 0.963_eV;
            default:
                throw std::logic_error{
                    std::format("dimer bond energy not defined for {}", symbol(element))
                };
        }
    }

    double coordination_exponent(Element element)
    {
        switch (element)
        {
            case Ag: return 0.32;
            case Cu: return 0.27;
            case Au: return 0.25;
            default:
                throw std::logic_error{
                    std::format("coordination exponent not defined for {}", symbol(element))
                };
        }
    }

}


Energy PowerLawBondEnergy::lattice_energy_barrier(Descriptor const& desc) const
{
    return power_law_bond_energy::dimer_bond_energy(desc.element) *
           std::pow(static_cast<double>(desc.coordination),
                    power_law_bond_energy::coordination_exponent(desc.element));
}

Rate PowerLawBondEnergy::rate(Descriptor const& desc) const
{
    return ElectrochemicalInterface::rate(
        desc.event, desc.element, lattice_energy_barrier(desc), modulation_ratio(desc));
}

std::string PowerLawBondEnergy::output_text() const
{
    return std::format("Model = Power law bond energy\n{}",
        ElectrochemicalInterface::output_text());
}
