#pragma once
#include <magic_enum/magic_enum_flags.hpp>

#include <bit>
#include <ranges>

using namespace magic_enum::bitwise_operators;

using magic_enum::enum_flags_name;
using magic_enum::enum_flags_test;
using magic_enum::enum_integer;
using magic_enum::enum_name;
using magic_enum::enum_values;

namespace enums
{
    template <class Enum>
    requires std::is_enum_v<Enum>
    constexpr auto selected_flags(Enum value)
    {
        using Underlying = std::make_unsigned_t<std::underlying_type_t<Enum>>;

        return magic_enum::enum_values<Enum>() | std::views::filter([value](Enum flag) {
            const auto bits{ static_cast<Underlying>(magic_enum::enum_integer(flag)) };

            return std::has_single_bit(bits) and magic_enum::enum_flags_test(value, flag);
        });
    }
}

template <typename Enum>
requires std::is_enum_v<Enum>
struct std::formatter<Enum> : public std::formatter<std::string_view>
{
    template <class FormatContext>
    auto format(Enum value, FormatContext& context) const
    {
        return std::format_to(context.out(), "{}", magic_enum::enum_name(value));
    }
};
