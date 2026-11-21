#include "pch.h"
#include "power_law_bond_energy.h"
#include "physics/constants.h"

#include <algorithm>
#include <cmath>
#include <iterator>

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


PowerLawBondEnergy::PowerLawBondEnergy()
{
    // Preserve the v0.6.1 Ag9 dissolution rate at the same potential and
    // temperature. The diffusion transition state does not enter this calibration.
    using physics::constant::symbols::e;
    constexpr lattice::Coordination reference_coordination{ 9 };

    auto parameters{ ElectrochemicalInterface::constants() };
    const Energy old_barrier{ reference_coordination * 0.15_eV };
    const Energy new_barrier{ lattice_energy_barrier(
        { event::Type::dissolution, Element::Ag, reference_coordination }) };

    parameters.reference_potential +=
        (old_barrier - new_barrier) / (parameters.charge_transfer_coefficient * e);
    set_constants(parameters);
    set_potential(parameters.reference_potential);
}

Energy PowerLawBondEnergy::lattice_energy_barrier(Descriptor const& desc) const
{
    const auto coefficient{ power_law_bond_energy::dimer_bond_energy(desc.element) };
    const auto exponent{ power_law_bond_energy::coordination_exponent(desc.element) };
    const Energy binding_energy{
        coefficient * std::pow(static_cast<double>(desc.coordination), exponent)
    };

    if (desc.event == event::Type::diffusion)
    {
        // Approximate every diffusion transition state by coordination two,
        // independent of element and hopping geometry (no GTC correction).
        const Energy transition_state_binding{
            coefficient * std::pow(diffusion_transition_state_coordination, exponent)
        };
        return std::max<Energy>(0_eV, binding_energy - transition_state_binding);
    }

    // Dissolution transfers the atom to a lattice-free reference (n_TS = 0).
    // The electrochemical barrier shift is applied by ElectrochemicalInterface.
    return binding_energy;
}

Rate PowerLawBondEnergy::rate(Descriptor const& desc) const
{
    return ElectrochemicalInterface::rate(
        desc.event, desc.element, lattice_energy_barrier(desc), modulation_ratio(desc));
}

std::string PowerLawBondEnergy::output_text() const
{
    std::string text{ std::format(
        "Model = Power law bond energy\nDiffusion TS coordination = {}\n",
        diffusion_transition_state_coordination) };
    for (auto const element : lattice::active_elements)
        std::format_to(std::back_inserter(text), "{}: coefficient = {:meV}; exponent = {}\n",
            element, power_law_bond_energy::dimer_bond_energy(element),
            power_law_bond_energy::coordination_exponent(element));

    return text + ElectrochemicalInterface::output_text();
}
