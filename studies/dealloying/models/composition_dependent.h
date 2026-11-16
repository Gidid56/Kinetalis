#pragma once
#include "electrochemical_interface.h"
#include <map>

namespace studies::dealloying::models
{
    class CompositionDependent : public ElectrochemicalInterface
    {
    public:
        struct Descriptor
        {
            kmc::event::Type event{};
            kmc::Element element{};
            lattice::Coordination coordination{};
            std::array<lattice::Coordination, std::size(lattice::active_elements)> element_neighbors{};
            auto operator<=>(Descriptor const&) const = default;
        };

        CompositionDependent();
        kmc::RateClassKey classify(kmc::event::Type, kmc::event::Location const&, lattice::Lattice const&) override;
        std::size_t number_keys() const noexcept override { return infos_.size(); }
        kmc::ClassInfo const& class_info(kmc::RateClassKey) const override;
        Descriptor const& descriptor(kmc::RateClassKey key) const { return descriptors_.at(key); }
        std::string output_text() const override;

    private:
        Energy lattice_energy_barrier(kmc::RateClassKey) const override;
        std::map<Descriptor, kmc::RateClassKey> keys_;
        std::vector<Descriptor> descriptors_{ Descriptor{} };
        std::vector<kmc::ClassInfo> infos_{ kmc::ClassInfo{} };
    };

    constexpr std::uint16_t bond_pair(kmc::Element first, kmc::Element second)
    {
        static_assert(std::is_same_v<std::underlying_type_t<kmc::Element>, std::uint8_t>);
        if (first > second) std::swap(first, second);
        return (static_cast<std::uint16_t>(std::to_underlying(first)) << 8) |
               static_cast<std::uint16_t>(std::to_underlying(second));
    }
}
