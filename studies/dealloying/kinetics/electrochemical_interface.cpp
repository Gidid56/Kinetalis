#include "pch.h"
#include "electrochemical_interface.h"
#include "chemistry/atom.h"
#include "physics/equations/kinetics.h"

using namespace kmc;
using chemistry::Element;
using namespace studies::dealloying::kinetics;

void ElectrochemicalInterface::ensure_range(MolarConcentration concentration)
{
    if (concentration < MolarConcentration{} or not isfinite(concentration))
        throw std::invalid_argument{ "Concentration must be non-negative and finite." };
}

void ElectrochemicalInterface::ensure_range(Constants const& constants)
{
    if (constants.solvation_energy <= Energy{} or not isfinite(constants.solvation_energy))
        throw std::invalid_argument{ "Solvation energy must be positive and finite." };
    if (not std::isfinite(constants.charge_transfer_coefficient) or
        constants.charge_transfer_coefficient < 0 or constants.charge_transfer_coefficient > 1)
        throw std::invalid_argument{
            "Charge transfer coefficient must be finite and between 0 and 1."
        };
}

namespace
{
    constexpr auto fcc_molar_volume(Element element)
    {
        auto atomic_radius{ chemistry::Atom{ element }.metallic_radius() };
        auto lattice_constant{ *atomic_radius * 2 * std::numbers::sqrt2 };
        auto atomic_volume{ math::cube(lattice_constant) / 4 };
        return atomic_volume * physics::constant::avogadro;
    }
}

Ratio ElectrochemicalInterface::transmission_coefficient(event::Type type, Element element) const
{
    switch (type)
    {
        case event::Type::diffusion:
        case event::Type::dissolution: return 1;
        case event::Type::deposition:
            return physics::ensure_unit_interval(fcc_molar_volume(element) * concentration_);
        default: return {};
    }
}

Energy ElectrochemicalInterface::activation_gibbs_energy(event::Type type, Energy lattice_barrier) const
{
    using physics::constant::symbols::e;
    const auto alpha{ constants_.charge_transfer_coefficient };
    const auto E_0{ constants_.reference_potential };
    const auto E{ potential_ };
    switch (type)
    {
        case event::Type::diffusion: return lattice_barrier;
        case event::Type::dissolution:
            return lattice_barrier - alpha * e * (E - E_0);
        case event::Type::deposition:
            return constants_.solvation_energy + (1 - alpha) * e * (E - E_0);
        default: throw std::logic_error{ "Activation Gibbs energy is not defined for event." };
    }
}

Rate ElectrochemicalInterface::rate(event::Type type, Element element, Energy barrier,
                                     Ratio modulation) const
{
    return physics::equations::kinetics::tst_rate(
        transmission_coefficient(type, element),
        std::max<Energy>(0_eV, activation_gibbs_energy(type, barrier)),
        temperature()) * modulation;
}

using namespace physics::equations::kinetics;
static const auto reference_potential_erlebacher{ -log(1e-9) * thermal_voltage(300_K) };

std::string ElectrochemicalInterface::output_text() const
{
    std::string text;
    std::format_to(std::back_inserter(text),
        "Temperature = {}\nPotential = {:V}\n", temperature(), potential_);
    std::format_to(std::back_inserter(text), "Concentration = {:mol/l}\n", concentration_);
    std::format_to(std::back_inserter(text),
        "Symmetry factor = {}\nReference potential = {:V}\n"
        "Reference potential (Erlebacher) = {:V}\n",
        constants_.charge_transfer_coefficient, constants_.reference_potential,
        reference_potential_erlebacher);
    std::format_to(std::back_inserter(text),
        "Solvation energy = {:meV}\n", constants_.solvation_energy);
    return text;
}
