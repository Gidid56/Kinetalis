#pragma once
#include "physics/constants.h"
#include "physics/functions.h"
#include "physics/quantities.h"

namespace physics::equations::kinetics
{
    using namespace physics::quantity;

    using physics::constant::symbols::h;
    using physics::constant::symbols::k_B;
    using physics::constant::symbols::N_A;

    // Characteristic thermal energy k_B T.
    constexpr auto thermal_energy(Like<AbsoluteTemperature> auto temperature)
    {
        return k_B * temperature;
    }

    // Characteristic thermal voltage k_B T / e.
    constexpr auto thermal_voltage(Like<AbsoluteTemperature> auto temperature)
    {
        return k_B * temperature / constant::elementary_charge;
    }

    // Transition-state-theory attempt frequency k_B T / h.
    constexpr auto tst_attempt_frequency(Like<AbsoluteTemperature> auto temperature)
    {
        return thermal_energy(temperature) / h;
    }

    // Boltzmann factor for an energy per particle.
    constexpr auto
    boltzmann_factor(Like<Energy> auto energy, Like<AbsoluteTemperature> auto temperature)
    {
        return physics::exp(-energy / thermal_energy(temperature));
    }

    // Boltzmann factor for an energy per amount of substance.
    constexpr auto
    boltzmann_factor(Like<MolarEnergy> auto energy, Like<AbsoluteTemperature> auto temperature)
    {
        return physics::exp(-energy / (N_A * thermal_energy(temperature)));
    }

    // Transition-state-theory rate from activation Gibbs energy.
    constexpr auto tst_rate(
        Like<Energy, MolarEnergy> auto activation_gibbs_energy,
        Like<AbsoluteTemperature> auto temperature)
    {
        return tst_attempt_frequency(temperature) *
               boltzmann_factor(activation_gibbs_energy, temperature);
    }

    // Transition-state-theory rate from activation enthalpy and entropy.
    constexpr auto tst_rate(
        Like<Energy, MolarEnergy> auto activation_enthalpy,
        Like<Entropy, MolarEntropy> auto activation_entropy,
        Like<AbsoluteTemperature> auto temperature)
    {
        return tst_rate(activation_enthalpy - temperature * activation_entropy, temperature);
    }

    // Transition-state-theory rate including a transmission coefficient.
    constexpr auto tst_rate(
        Like<Fraction> auto transmission_coefficient,
        Like<Energy, MolarEnergy> auto activation_gibbs_energy,
        Like<AbsoluteTemperature> auto temperature)
    {
        return transmission_coefficient * tst_rate(activation_gibbs_energy, temperature);
    }
}
