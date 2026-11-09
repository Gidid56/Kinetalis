#pragma once
#include "linear_bond_breaking.h"

class kmc::models::CompositionDependent : public LinearBondBreaking
{
public:
    Energy lattice_energy_barrier(Process const&) const override;

    std::string output_text() const override;
};

namespace kmc::models
{
    constexpr std::uint16_t bond_pair(Element first, Element second)
    {
        static_assert(std::is_same_v<std::underlying_type_t<Element>, std::uint8_t>);

        if (first > second)
            std::swap(first, second);

        auto a{ std::to_underlying(first) };
        auto b{ std::to_underlying(second) };

        return (static_cast<std::uint16_t>(a) << 8) | static_cast<std::uint16_t>(b);
    }
}