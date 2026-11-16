#pragma once
#include "event.h"
#include "types.h"

#include <compare>
#include <cstddef>
#include <format>
#include <string>
#include <string_view>

namespace kmc
{
    // Universal reporting metadata. Only concrete study models own the
    // actual descriptors and decide kinetic equivalence.
    struct ClassInfo
    {
        event::Type type{};
        Element source_element{};
        lattice::Coordination source_coordination{};
        std::string details{};

        bool is_active() const noexcept { return type != event::Type::none; }
        auto event() const noexcept { return type; }
        auto element() const noexcept { return source_element; }
        auto coordination() const noexcept { return source_coordination; }

        std::string label() const
        {
            auto text{ std::format("{}_{}_{}", enum_name(type), source_element,
                static_cast<unsigned>(source_coordination)) };
            if (not details.empty())
                text += "_" + details;
            return text;
        }

        auto operator<=>(ClassInfo const&) const = default;

        static constexpr std::string_view header{
            "key:event,key:element,key:coordination,key:details"
        };
    };

    class Model
    {
    public:
        static constexpr Temperature standard_temperature{ 25_degC };
        virtual ~Model() = default;

        // A model registers its own descriptor and returns its stable class key.
        // Key zero is reserved for an inactive event slot.
        virtual RateClassKey classify(event::Type, event::Location const&, lattice::Lattice const&) = 0;
        virtual std::size_t number_keys() const noexcept = 0;
        virtual ClassInfo const& class_info(RateClassKey) const = 0;
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

template <>
struct std::formatter<kmc::ClassInfo> : std::formatter<std::string_view>
{
    char separator{ ',' };

    constexpr auto parse(std::format_parse_context& context)
    {
        auto it{ context.begin() };
        if (it != context.end() and *it != '}')
            separator = *it++;
        if (it != context.end() and *it != '}')
            throw std::format_error{ "Invalid class info format specifier." };
        return it;
    }

    auto format(kmc::ClassInfo const& info, std::format_context& context) const
    {
        return std::format_to(context.out(), "{}{}{}{}{}{}{}",
            enum_name(info.event()), separator, info.element(), separator,
            static_cast<unsigned>(info.coordination()), separator, info.details);
    }
};
