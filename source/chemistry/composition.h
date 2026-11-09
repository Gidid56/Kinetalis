#pragma once
#include "element.h"
#include "utility/ranges.h"

#include <cstdint>
#include <format>
#include <initializer_list>
#include <map>
#include <ranges>

namespace chemistry
{
    // Represents an elemental composition through non-negative integer weights.
    // The weights may describe a stoichiometric formula or relative sampling
    // proportions, for example during random lattice filling. Element::none is
    // permitted and represents vacant or otherwise unoccupied lattice sites.
    // Equivalent ratios are not reduced automatically.
    //
    // Fractional weights must be represented by an equivalent integer ratio.
    //
    class Composition
    {
    public:
        using Quantity = std::uint64_t;

        struct Component
        {
            Element element{};
            Quantity quantity{ 1 };

            constexpr auto operator<=>(Component const&) const = default;
        };

        Composition() = default;

        explicit Composition(std::initializer_list<Element> elements)
        {
            for (auto element : elements)
                add({ element, 1 });
        }

        explicit Composition(std::initializer_list<Component> components)
        {
            for (auto const& [element, quantity] : components)
                add({ element, quantity });
        }

        Composition& operator+=(Composition const& other)
        {
            for (auto const& [element, quantity] : other.components_)
                add({ element, quantity });

            return *this;
        }

        Composition& operator+=(Component const& component)
        {
            add(component);
            return *this;
        }

        Composition& operator-=(Element element)
        {
            if (not components_.contains(element) or components_.at(element) == 0)
                throw std::out_of_range{ "Composition does not contain element." };

            --components_[element];
            return *this;
        }

        [[nodiscard]] Quantity quantity(Element element) const
        {
            const auto iterator{ components_.find(element) };

            return iterator == components_.end() ? Quantity{} : iterator->second;
        }

        [[nodiscard]] Quantity total_quantity() const
        {
            return utility::sum(components_ | std::views::values);
        }

        [[nodiscard]] auto fraction(Element element) const
        {
            return quantity(element) / static_cast<double>(total_quantity());
        }

        auto begin() const noexcept { return components_.begin(); }
        auto end() const noexcept { return components_.end(); }
        bool empty() const noexcept { return components_.empty(); }
        auto size() const noexcept { return components_.size(); }

    private:
        void add(Component const& component)
        {
            if (component.quantity == 0)
                return;

            components_[component.element] += component.quantity;
        }

        std::map<Element, Quantity> components_;
    };
}

template <>
struct std::formatter<chemistry::Composition>
{
    constexpr auto parse(std::format_parse_context& context)
    {
        auto iterator{ context.begin() };

        if (iterator != context.end() and *iterator != '}')
            throw std::format_error{ "Composition does not support format specifiers." };
        return iterator;
    }

    template <class FormatContext>
    auto format(chemistry::Composition const& composition, FormatContext& context) const
    {
        auto out{ context.out() };

        for (auto const& [element, quantity] : composition)
        {
            out = std::format_to(out, "{}", symbol(element));

            if (quantity > 1)
                out = std::format_to(out, "{}", quantity);
        }

        return out;
    }
};
