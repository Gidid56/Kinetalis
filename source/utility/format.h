#pragma once
#include <format>
#include <map>
#include <string_view>

namespace utility
{
    template <typename Key, typename Value, typename Compare, typename Allocator>
    struct FormattedMap
    {
        std::map<Key, Value, Compare, Allocator> const& values;
        std::string_view key_value_separator;
        std::string_view entry_separator;
    };

    template <typename Key, typename Value, typename Compare, typename Allocator>
    auto format_map(
        std::map<Key, Value, Compare, Allocator> const& values,
        std::string_view key_value_separator = ": ",
        std::string_view entry_separator = ", ")
    {
        return FormattedMap<Key, Value, Compare, Allocator>{ values,
            key_value_separator,
            entry_separator };
    }
}

template <typename Key, typename Value, typename Compare, typename Allocator>
struct std::formatter<utility::FormattedMap<Key, Value, Compare, Allocator>>
{
    constexpr auto parse(std::format_parse_context& context) { return context.begin(); }

    auto format(
        utility::FormattedMap<Key, Value, Compare, Allocator> const& formatted,
        std::format_context& context) const
    {
        auto output{ context.out() };
        std::string_view entry_separator;

        for (auto const& [key, value] : formatted.values)
        {
            output = std::format_to(
                output,
                "{}{}{}{}",
                entry_separator,
                key,
                formatted.key_value_separator,
                value);

            entry_separator = formatted.entry_separator;
        }

        return output;
    }
};