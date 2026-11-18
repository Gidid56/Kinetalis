#pragma once
#include "electrochemical_interface.h"
#include <map>

namespace studies::dealloying::kinetics
{
    class CompositionDependent : public ElectrochemicalInterface
    {
    public:
        struct Descriptor
        {
            kmc::event::Type event{};
            chemistry::Element element{};
            lattice::Coordination coordination{};
            std::array<lattice::Coordination, std::size(lattice::active_elements)> element_neighbors{};
            auto operator<=>(Descriptor const&) const = default;
        };

        CompositionDependent();
        kmc::RateClassKey classify(kmc::event::Type, kmc::event::Location const&, lattice::Lattice const&);
        std::size_t number_keys() const noexcept { return descriptors_.size(); }
        kmc::event::Type class_event(kmc::RateClassKey) const;
        Descriptor const& descriptor(kmc::RateClassKey key) const { return descriptors_.at(key); }
        std::string output_text() const;
        kmc::Rate rate(kmc::RateClassKey) const;

    protected:
        chemistry::Element class_element(kmc::RateClassKey) const;

    private:
        Energy lattice_energy_barrier(kmc::RateClassKey) const;
        std::map<Descriptor, kmc::RateClassKey> keys_;
        std::vector<Descriptor> descriptors_{ Descriptor{} };
    };

    constexpr std::uint16_t bond_pair(chemistry::Element first, chemistry::Element second)
    {
        static_assert(std::is_same_v<std::underlying_type_t<chemistry::Element>, std::uint8_t>);
        if (first > second) std::swap(first, second);
        return (static_cast<std::uint16_t>(std::to_underlying(first)) << 8) |
               static_cast<std::uint16_t>(std::to_underlying(second));
    }
}
