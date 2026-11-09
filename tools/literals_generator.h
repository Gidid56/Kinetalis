#pragma once
#include "physics/prefix.h"
#include "physics/units.h"

#include <algorithm>
#include <format>
#include <ostream>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace physics
{
    constexpr std::string_view literal_symbol(Prefix prefix)
    {
        const auto ascii{ prefix::ascii_symbol(prefix) };
        return ascii.empty() ? prefix::symbol(prefix) : ascii;
    }

    constexpr std::string_view literal_symbol(Unit unit)
    {
        const auto ascii{ unit::ascii_symbol(unit) };
        return ascii.empty() ? unit::symbol(unit) : ascii;
    }

    namespace literals_generator
    {
        constexpr bool valid_identifier(std::string_view text)
        {
            const auto letter = [](char c) {
                return (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z');
            };

            const auto valid = [&](char c) {
                return letter(c) or (c >= '0' and c <= '9') or c == '_';
            };

            return not text.empty() and (letter(text.front()) or text.front() == '_') and
                   std::ranges::all_of(text.substr(1), valid);
        }

        inline void generate(std::ostream& output)
        {
            std::set<std::string, std::less<>> generated;

            const auto emit = [&](Unit unit, Prefix prefix) {
                const auto literal{ std::string{ literal_symbol(prefix) } +
                                    std::string{ literal_symbol(unit) } };

                if (not valid_identifier(literal))
                    throw std::runtime_error{ "Invalid literal '" + literal +
                                              "'. Add an ASCII symbol." };

                if (not generated.emplace(literal).second)
                    throw std::runtime_error{ "Duplicate literal '_" + literal + "'." };

                constexpr auto operator_expression{ "constexpr auto operator\"\"_" };

                const auto expression{ std::format(
                    "detail::literal<"
                    "static_cast<::physics::Unit>({}), "
                    "static_cast<::physics::Prefix>({})>(value)",
                    std::to_underlying(unit),
                    std::to_underlying(prefix)) };

                output << operator_expression << literal << "(unsigned long long value) {\n"
                       << "    return " << expression << ";\n"
                       << "}\n";

                output << operator_expression << literal << "(long double value) {\n"
                       << "    return " << expression << ";\n"
                       << "}\n\n";
            };

            for (auto i{ unit::index::first }; i <= unit::index::last; ++i)
            {
                const auto unit{ static_cast<Unit>(i) };

                emit(unit, Prefix::none);

                if (unit::is_prefixable(unit))
                    for (const auto prefix : prefix::values)
                        emit(unit, prefix);
            }
        }
    }
}
