#pragma once
#include "types.h"
#include <cstddef>

namespace kmc
{
    // Shared kinetic settings; derived kinetic models use static dispatch.
    class Kinetics
    {
    public:
        static constexpr Temperature standard_temperature{ 25_degC };

        auto& set_temperature(this auto& self, Temperature temperature)
        {
            self.temperature_ = temperature;
            return self;
        }

        auto temperature() const { return temperature_; }
        static constexpr std::size_t invalidation_hops{ 1 };

    protected:
        Temperature temperature_{ standard_temperature };
    };
}
