#pragma once
#include "math/functions.h"
#include "quantities.h"
#include "literals.h"

#include <numbers>

// SI defining constants and CODATA 2022 recommended values.
// Standard uncertainties are not represented by the Quantity type.
// See data/codata-2022.pdf for details.

namespace physics::dimension
{
    using AvogadroConstant = decltype(-AmountOfSubstance{});
    using GravitationalConstant = decltype(Acceleration{} + Area{} - Mass{});
    using FaradayConstant = decltype(ElectricCharge{} - AmountOfSubstance{});
    using JosephsonConstant = decltype(Frequency{} - ElectricPotential{});
    using CoulombConstant = decltype(-Permittivity{});
    using StefanBoltzmannConstant = decltype(Intensity{} - Temperature{}.power<4>());
}

namespace physics::quantity
{
    using AvogadroConstant = Quantity<dimension::AvogadroConstant>;
    using GravitationalConstant = Quantity<dimension::GravitationalConstant>;
    using FaradayConstant = Quantity<dimension::FaradayConstant>;
    using JosephsonConstant = Quantity<dimension::JosephsonConstant>;
    using CoulombConstant = Quantity<dimension::CoulombConstant>;
    using StefanBoltzmannConstant = Quantity<dimension::StefanBoltzmannConstant>;
}

namespace physics::constant
{
    using namespace physics::unit::literals;

    constexpr auto hyperfine_transition_Cs{ 9'192'631'770_Hz };
    constexpr auto light_speed{ 299'792'458_m / 1_s };
    constexpr auto planck{ 6.626'070'15E-34_J * 1_s };
    constexpr auto elementary_charge{ 1.602'176'634E-19_C };
    constexpr auto boltzmann{ 1.380'649E-23_J / 1_K };
    constexpr auto avogadro{ 6.022'140'76E+23_number / 1_mol };
    constexpr auto luminous_efficacy{ 683_lm / 1_W };
    constexpr auto full_turn{ 1_tr };

    constexpr quantity::GravitationalConstant gravitation{ 6.674'30E-11 };

    constexpr auto fine_structure{ 7.297'352'564'3E-3_ratio };
    constexpr auto electron_g_factor{ -2.002'319'304'360'92_factor };

    constexpr auto electron_mass{ 9.109'383'713'9E-31_kg };
    constexpr auto proton_mass{ 1.672'621'925'95E-27_kg };
    constexpr auto neutron_mass{ 1.674'927'500'56E-27_kg };
    constexpr auto atomic_mass{ 1.660'539'068'92E-27_kg };

    inline namespace symbols
    {
        using math::functions::cube;
        using math::functions::power;
        using math::functions::square;

        // Mathematical ratio of a circle's circumference to its diameter
        constexpr auto pi = std::numbers::pi;

        // Hyperfine transition frequency of Cesium-133
        constexpr auto nu_Cs = hyperfine_transition_Cs;
        // Speed of light in vacuum
        constexpr auto c = light_speed;
        // Planck constant
        constexpr auto h = planck;
        // Elementary charge
        constexpr auto e = elementary_charge;
        // Boltzmann constant
        constexpr auto k_B = boltzmann;
        // Avogadro constant
        constexpr auto N_A = avogadro;
        // Luminous efficacy of monochromatic radiation of frequency 540 THz (green light)
        constexpr auto K_cd = luminous_efficacy;
        // Fine-structure constant
        constexpr auto alpha = fine_structure;
        // Newtonian constant of gravitation
        constexpr auto G = gravitation;
        // Electron g-factor
        constexpr auto g_e = electron_g_factor;
        // Electron mass
        constexpr auto m_e = electron_mass;
        // Proton mass
        constexpr auto m_p = proton_mass;
        // Neutron mass
        constexpr auto m_n = neutron_mass;
        // Atomic mass constant
        constexpr auto m_u = atomic_mass;

        // Faraday constant
        constexpr auto F{ N_A * e };
        // Molar gas constant
        constexpr auto R{ N_A * k_B };
        // Reduced Planck constant
        constexpr auto hbar{ h / full_turn };

        // Vacuum electric permittivity
        constexpr auto epsilon_0{ square(e) / (2 * alpha * h * c) };
        // Vacuum magnetic permeability
        constexpr auto mu_0{ 1 / (epsilon_0 * square(c)) };

        // Rydberg constant
        constexpr auto R_inf{ square(alpha) * m_e * c / (2 * h) };
        // Von Klitzing constant
        constexpr auto R_k{ h / square(e) };
        // Characteristic impedance of vacuum
        constexpr auto Z_0{ mu_0 * c };
        // Conductance quantum
        constexpr auto G_0{ 2 * square(e) / h };
        // Josephson constant
        constexpr auto k_J{ 2 * e / h };
        // Coulomb constant
        constexpr auto k_e{ 1 / (4 * pi * epsilon_0) };

        // Magnetic flux quantum
        constexpr auto phi_0{ h / (2 * e) };
        // Bohr magneton
        constexpr auto mu_B{ e * hbar / (2 * m_e) };
        // Nuclear magneton
        constexpr auto mu_N{ e * hbar / (2 * m_p) };
        // Classical electron radius
        constexpr auto r_e{ square(e) * k_e / (m_e * square(c)) };
        // Hartree energy
        constexpr auto E_h{ 2 * R_inf * h * c };

        // Stefan-Boltzmann constant
        constexpr auto sigma{ 2 * power<5>(pi) * power<4>(k_B) / (15 * square(c) * cube(h)) };
    }
    // Descriptive aliases for derived constants

    constexpr auto faraday = F;
    constexpr auto molar_gas = R;
    constexpr auto reduced_planck = hbar;

    constexpr auto vacuum_permittivity = epsilon_0;
    constexpr auto vacuum_permeability = mu_0;

    constexpr auto rydberg = R_inf;
    constexpr auto von_klitzing = R_k;
    constexpr auto characteristic_impedance_vacuum = Z_0;
    constexpr auto conductance_quantum = G_0;
    constexpr auto josephson = k_J;
    constexpr auto coulomb = k_e;

    constexpr auto magnetic_flux_quantum = phi_0;
    constexpr auto bohr_magneton = mu_B;
    constexpr auto nuclear_magneton = mu_N;
    constexpr auto classical_electron_radius = r_e;
    constexpr auto hartree_energy = E_h;

    constexpr auto stefan_boltzmann = sigma;
}
