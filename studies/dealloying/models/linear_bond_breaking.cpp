#include "pch.h"
#include "linear_bond_breaking.h"

using namespace kmc;
using namespace studies::dealloying::models;

void LinearBondBreaking::ensure_range(Energy bond_energy)
{
    if (not isfinite(bond_energy))
        throw std::invalid_argument{ "Bond energy must be finite." };
}

process::Key LinearBondBreaking::static_key(Descriptor const& desc)
{
    const auto max{ static_cast<std::size_t>(lattice::max_coordination) };
    const auto offset_dissolution{ 1 + max * std::size(lattice::active_elements) };
    const auto offset_deposition{ offset_dissolution + max };
    std::size_t key{};
    switch (desc.event)
    {
        case event::Type::diffusion:
            key = 1 + max * lattice::elementKey(desc.element) + desc.coordination;
            break;
        case event::Type::dissolution:
            key = offset_dissolution + desc.coordination;
            break;
        case event::Type::deposition:
            key = offset_deposition + desc.coordination;
            break;
        default: throw std::logic_error{ "Cannot classify an inactive event." };
    }
    if (desc.coordination >= lattice::max_coordination or key >= key_count)
        throw std::out_of_range{ "Coordination outside static kinetic classes." };
    return static_cast<process::Key>(key);
}

process::Key LinearBondBreaking::classify(
    event::Type type, event::Location const& location, lattice::Lattice const&)
{
    const Descriptor desc{ type, location.source().element(), location.source().coordination() };
    const auto key{ static_key(desc) };
    auto& stored{ descriptors_[key] };
    auto& info{ infos_[key] };
    if (not info.is_active())
    {
        stored = desc;
        info = ClassInfo{ desc.event, desc.element, desc.coordination, {} };
    }
    else if (stored != desc)
        throw std::logic_error{ "Static kinetic class key collision." };
    return key;
}

std::size_t LinearBondBreaking::number_keys() const noexcept { return key_count; }

ClassInfo const& LinearBondBreaking::class_info(process::Key key) const
{
    return infos_.at(key);
}

LinearBondBreaking::Descriptor const& LinearBondBreaking::descriptor(process::Key key) const
{
    if (not infos_.at(key).is_active())
        throw std::logic_error{ "Uninitialized kinetic class." };
    return descriptors_.at(key);
}

Energy LinearBondBreaking::lattice_energy_barrier(process::Key key) const
{
    return constants_.bond_energy * descriptor(key).coordination;
}

Ratio LinearBondBreaking::modulation_ratio(process::Key key) const
{
    if (modulation_ and descriptor(key) == modulation_->process)
        return modulation_->ratio;
    return 1;
}

std::string LinearBondBreaking::output_text() const
{
    auto result{ std::format(
        "Model = Linear bond breaking\nBond energy = {:meV}\n{}",
        constants_.bond_energy, ElectrochemicalInterface::output_text()) };
    if (modulation_)
        result += std::format(
            "Modulated process = {}_{}_{}\nModulation ratio = {}\n",
            enum_name(modulation_->process.event), modulation_->process.element,
            static_cast<unsigned>(modulation_->process.coordination), modulation_->ratio);
    return result;
}
