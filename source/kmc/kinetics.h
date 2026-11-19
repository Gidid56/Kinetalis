#pragma once
#include "types.h"
#include "event.h"

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace kmc
{
    // Rates and event types are cached by kinetics; populations are not.
    struct KineticClass
    {
        Rate rate{};
        event::Type type{ event::Type::none };

        bool is_initialized() const noexcept { return type != event::Type::none; }
        bool operator==(KineticClass const&) const = default;
    };

    // Concrete models define descriptors, key mappings and physical rate laws.
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

        auto const& kinetic_classes() const noexcept { return kinetic_classes_; }
        void resize_classes(std::size_t count) { kinetic_classes_.resize(count); }

        template <class Calculate>
        RateClassKey register_class(RateClassKey key, event::Type type, Calculate&& calculate)
        {
            if (key == RateClassKey{})
                throw std::logic_error{ "Active event maps to reserved key zero." };
            if (key >= kinetic_classes_.size())
                kinetic_classes_.resize(static_cast<std::size_t>(key) + 1);

            auto& entry{ kinetic_classes_[key] };
            if (not entry.is_initialized())
            {
                entry.rate = calculate();
                entry.type = type;
            }
            return key;
        }

        template <class Calculate>
        void refresh_classes(Calculate&& calculate)
        {
            for (std::size_t key{ 1 }; key < kinetic_classes_.size(); ++key)
                if (kinetic_classes_[key].is_initialized())
                    kinetic_classes_[key].rate = calculate(static_cast<RateClassKey>(key));
        }

    protected:
        Temperature temperature_{ standard_temperature };

    private:
        std::vector<KineticClass> kinetic_classes_{ KineticClass{} };
    };
}
