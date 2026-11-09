#include "pch.h"

#include "power_law_bond_energy.h"

#include "kmc/process.h"

using namespace kmc;

namespace kmc::models::power_law_bond_energy
{
    using enum Element;

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

    Energy binding_energy(Element element, lattice::Coordination coordination)
    {
        return dimer_bond_energy(element) *
               std::pow(static_cast<double>(coordination), coordination_exponent(element));
    }
}

Energy models::PowerLawBondEnergy::lattice_energy_barrier(Process const& process) const
{
    auto coordination{ static_cast<double>(process.coordination()) };

    return power_law_bond_energy::dimer_bond_energy(process.element()) *
           std::pow(coordination, power_law_bond_energy::coordination_exponent(process.element()));
}

std::string models::PowerLawBondEnergy::output_text() const
{
    return std::format(
        "Model = Power law bond energy\n"
        "{}",
        ElectrochemicalInterface::output_text());
}