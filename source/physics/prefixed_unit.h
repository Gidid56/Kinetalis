#pragma once
#include "prefix.h"
#include "units.h"

#include <string_view>
#include <vector>

namespace physics
{
    struct PrefixedUnit
    {
        Unit unit{ Unit::none };
        Prefix prefix{ Prefix::none };

        auto operator<=>(PrefixedUnit const&) const = default;
    };

    namespace prefixed_unit
    {
        constexpr std::vector<PrefixedUnit> base()
        {
            std::vector<PrefixedUnit> base_units;
            base_units.reserve(unit::index::last_base - unit::index::first + 1);

            for (auto i{ unit::index::first }; i <= unit::index::last_base; ++i)
            {
                const auto unit{ static_cast<Unit>(i) };

                base_units.emplace_back(unit, unit == Unit::gram ? Prefix::kilo : Prefix::none);
            }
            return base_units;
        }

        constexpr PrefixedUnit parse(std::string_view text)
        {
            PrefixedUnit result;
            std::size_t matched_length{};

            for (auto i{ unit::index::first }; i <= unit::index::last; ++i)
            {
                const auto unit_value{ static_cast<Unit>(i) };

                for (auto symbol : {
                         unit::symbol(unit_value),
                         unit::ascii_symbol(unit_value),
                     })
                {
                    if (symbol.empty() or symbol.length() <= matched_length or
                        not text.ends_with(symbol))
                    {
                        continue;
                    }

                    if (text.length() == symbol.length())
                        return { unit_value };

                    if (not unit::is_prefixable(unit_value))
                        continue;

                    const auto prefix_text{ text.substr(0, text.length() - symbol.length()) };

                    for (auto prefix_value : prefix::values)
                    {
                        if (prefix_text == prefix::symbol(prefix_value) or
                            prefix_text == prefix::ascii_symbol(prefix_value))
                        {
                            result = { unit_value, prefix_value };
                            matched_length = symbol.length();
                            break;
                        }
                    }
                }
            }

            return result;
        }
    }
}