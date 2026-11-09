#pragma once
#include <ranges>

namespace utility
{
    template <std::ranges::input_range Range, typename Init = std::ranges::range_value_t<Range>>
    constexpr auto sum(Range&& range, Init init = Init{})
    {
        return std::ranges::fold_left(range, std::move(init), std::plus<Init>());
    }

    template <class Type, std::size_t size>
    constexpr auto prepend(Type value, std::array<Type, size> const& values)
    {
        std::array<Type, size + 1> result{};
        result.front() = value;
        std::ranges::copy(values, result.begin() + 1);
        return result;
    }

    template <auto array>
    constexpr std::size_t array_key(typename decltype(array)::value_type const& item)
    {
        const auto iterator{ std::ranges::find(array, item) };

        if (iterator == array.end())
            throw std::invalid_argument{ "Item is not configured." };

        return static_cast<std::size_t>(iterator - array.begin());
    }
}
