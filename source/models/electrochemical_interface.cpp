#include "pch.h"

#include "electrochemical_interface.h"

#include "kmc/process.h"

using namespace kmc;

void models::ElectrochemicalInterface::ensure_range(MolarConcentration concentration)
{
    if (concentration < MolarConcentration{} or not isfinite(concentration))
        throw std::invalid_argument{ "Concentration must be non-negative and finite." };
}

void models::ElectrochemicalInterface::ensure_range(Constants const& constants)
{
    //physics::quantity::ensure_range(constants.reference_potential);

    if (constants.solvation_energy <= Energy{} or not isfinite(constants.solvation_energy))
        throw std::invalid_argument{ "Solvation energy must be positive and finite." };

    if (not std::isfinite(constants.charge_transfer_coefficient) or
        constants.charge_transfer_coefficient < 0 or constants.charge_transfer_coefficient > 1)
    {
        throw std::invalid_argument{
            "Charge transfer coefficient must be finite and between 0 and 1."
        };
    }
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

Ratio models::ElectrochemicalInterface::transmission_coefficient(Process const& process) const
{
    switch (process.event())
    {
        case Event::diffusion:
        case Event::dissolution: return 1;

        case Event::deposition:
            return physics::ensure_unit_interval(
                fcc_molar_volume(process.element()) * concentration_);

        default: return {};
    }
}

Energy models::ElectrochemicalInterface::activation_gibbs_energy(Process const& process) const
{
    using physics::constant::symbols::e;
    const auto& alpha{ constants_.charge_transfer_coefficient };
    const auto& E_0{ constants_.reference_potential };
    const auto& E{ potential_ };

    switch (process.event())
    {
        case Event::diffusion: return lattice_energy_barrier(process);

        case Event::dissolution: return lattice_energy_barrier(process) - alpha * e * (E - E_0);

        case Event::deposition: return constants_.solvation_energy + (1 - alpha) * e * (E - E_0);

        default: throw std::logic_error{ "Activation Gibbs energy is not defined for event." };
    }
}

#include "physics/equations/kinetics.h"
using namespace physics::equations::kinetics;

static const auto reference_potential_erlebacher{ -log(1e-9) * thermal_voltage(300_K) };

std::string models::ElectrochemicalInterface::output_text() const
{
    std::string text;

    std::format_to(
        std::back_inserter(text),
        "Temperature = {}\n"
        "Potential = {:V}\n",
        temperature(),
        potential_);

    if constexpr (std::ranges::contains(enabledEvents, Event::deposition))
        std::format_to(std::back_inserter(text), "Concentration = {:mol/l}\n", concentration_);

    std::format_to(
        std::back_inserter(text),
        "Symmetry factor = {}\n"
        "Reference potential = {:V}\n"
        "Reference potential (Erlebacher) = {:V}\n",
        constants_.charge_transfer_coefficient,
        constants_.reference_potential,
        reference_potential_erlebacher);

    if constexpr (std::ranges::contains(enabledEvents, Event::deposition))
        std::format_to(
            std::back_inserter(text),
            "Solvation energy = {:meV}\n",
            constants_.solvation_energy);

    return text;
}
