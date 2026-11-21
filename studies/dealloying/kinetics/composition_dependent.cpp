#include "pch.h"
#include "composition_dependent.h"
#include "lattice/lattice.h"
#include "physics/constants.h"

using namespace kmc;
using chemistry::Element;
using namespace studies::dealloying::kinetics;

namespace studies::dealloying::kinetics::composition_dependent
{
    using enum chemistry::Element;

    // Positive bond-breaking costs: tangent slopes of the EMT binding
    // energies at coordination six (see thesis, Sec. 1.3.1).
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

    // Signed z=0 intercepts of the same EMT tangents. These cancel from
    // diffusion barriers, but not from lattice-to-solution dissolution.
    Energy zero_bond_energy(Element element)
    {
        switch (element)
        {
            case Ag: return -1.74_eV;
            case Cu: return -1.92_eV;
            case Au: return -2.31_eV;
            default:
                throw std::logic_error{
                    std::format("zero-bond energy not defined for {}", symbol(element))
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
    // Calibrate the all-Ag, ninefold coordinated dissolution rate to the
    // LinearBondBreaking v0.6.1 reference (0.15 eV per bond, alpha = 1).
    // The z=0 offset only changes the dissolution reference, not diffusion.
    using physics::constant::symbols::e;
    constexpr int coordination{ 9 };
    auto constants{ ElectrochemicalInterface::constants() };
    const auto old_barrier{ coordination * 0.15_eV };
    const auto new_barrier{ coordination * composition_dependent::bond_energy(Element::Ag) -
        composition_dependent::zero_bond_energy(Element::Ag) };
    constants.reference_potential +=
        (old_barrier - new_barrier) / (constants.charge_transfer_coefficient * e);
    set_constants(constants);
    set_potential(constants.reference_potential);
}

CompositionDependent::Descriptor CompositionDependent::classify(
    event::Type type,
    event::Location const& location,
    lattice::Lattice const&) const
{
    auto const& source{ location.source() };

    return { type, source.element(), source.coordination(), source.element_neighbors() };
}

RateClassKey CompositionDependent::key(Descriptor const& desc)
{
    if (auto it{ keys_.find(desc) }; it != keys_.end()) return it->second;

    if (descriptors_.size() > std::numeric_limits<RateClassKey>::max())
        throw std::overflow_error{ "Increase RateClassKey size." };

    auto const key{ static_cast<RateClassKey>(descriptors_.size()) };
    keys_.emplace(desc, key);
    descriptors_.push_back(desc);
    return key;
}

Energy CompositionDependent::lattice_energy_barrier(Descriptor const& desc) const
{
    // All bonds are broken at the transition state. For diffusion the
    // lattice-bound z=0 offset cancels; dissolution leaves the lattice and
    // additionally pays the cost of that offset.
    Energy barrier{};
    for (auto&& [element, count] :
        std::views::zip(lattice::active_elements, desc.element_neighbors))
        barrier += count * composition_dependent::bond_energy(desc.element, element);

    if (desc.event == event::Type::dissolution)
        barrier -= composition_dependent::zero_bond_energy(desc.element);

    return barrier;
}

Rate CompositionDependent::rate(Descriptor const& desc) const
{
    return ElectrochemicalInterface::rate(
        desc.event, desc.element, lattice_energy_barrier(desc));
}

std::string CompositionDependent::output_text() const
{
    return std::format("Model = Composition-dependent linear EMT tangent (z = 6)\n{}",
        ElectrochemicalInterface::output_text());
}
