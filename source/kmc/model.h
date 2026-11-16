#pragma once
#include "event.h"
#include "types.h"

#include <cstddef>
#include <string>
#include <string_view>

namespace kmc
{
    class Model
    {
    public:
        static constexpr Temperature standard_temperature{ 25_degC };
        virtual ~Model() = default;

        // Models own kinetic equivalence and stable key registration. Key zero
        // is reserved for inactive events; registered keys must never change identity.
        virtual RateClassKey classify(event::Type, event::Location const&, lattice::Lattice const&) = 0;
        virtual std::size_t number_keys() const noexcept = 0;
        virtual event::Type class_event(RateClassKey) const = 0;
        virtual Rate rate(RateClassKey) const = 0;
        virtual void refresh_rates() = 0;

        virtual std::size_t invalidation_radius() const noexcept { return 1; }
        virtual std::string output_text() const = 0;

        auto& set_temperature(this auto& self, Temperature temperature)
        {
            self.temperature_ = temperature;
            return self;
        }

        auto temperature() const { return temperature_; }

    protected:
        Temperature temperature_{ standard_temperature };
    };
}