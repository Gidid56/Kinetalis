#pragma once
#include "exponent.h"
#include "math/functions.h"
#include "prefixed_unit.h"
#include "quantity.h"
#include "unit_dimensions.h"

#include <algorithm>
#include <cmath>
#include <concepts>
#include <format>
#include <limits>
#include <ranges>
#include <string_view>
#include <vector>

template <
    physics::IsDimension Dimension,
    physics::Reference reference,
    physics::Domain domain,
    std::floating_point Value>
struct std::formatter<physics::Quantity<Dimension, reference, domain, Value>>
    : std::formatter<Value>, std::formatter<std::string_view>
{
    enum class Output
    {
        value,
        unit,
        full,
    };

    Output output{ Output::full };

    std::string_view unit_specification{};
    bool value_format_specified{ false }, use_fraction_slash{ false };

    template <class ParseContext>
    constexpr auto parse(ParseContext& context) -> decltype(context.begin())
    {
        auto it{ context.begin() };

        if (it != context.end())
        {
            auto next{ it };
            ++next;

            if (next != context.end() and *next == ':' and (*it == 'v' or *it == 'u'))
            {
                output = *it == 'v' ? Output::value : Output::unit;
                it = ++next;
            }
        }

        const auto unit_begin{ it };

        while (it != context.end() and *it != '}' and *it != ';')
        {
            if (*it == '/')
                use_fraction_slash = true;

            ++it;
        }

        unit_specification = std::string_view{ unit_begin, it };

        auto result{ it };

        if (it != context.end() and *it == ';')
        {
            ++it;

            value_format_specified = it != context.end() and *it != '}';

            context.advance_to(it);
            result = std::formatter<Value>::parse(context);
        }

        return result;
    }

    using Candidates = std::vector<physics::PrefixedUnit>;

    struct UnitTerm
    {
        physics::Prefix prefix{ physics::Prefix::none };
        physics::Unit unit{ physics::Unit::none };
        physics::Exponent exponent{ 0 };
    };

    struct FormatPlan
    {
        std::vector<UnitTerm> numerators, denominators;
        Value factor{ 1 };
        Value offset{ 0 };
        bool logarithmic{ false };
    };

    Candidates parse_units() const
    {
        using namespace physics;

        Candidates candidates = prefixed_unit::base();

        auto it{ unit_specification.begin() };
        bool has_dimensionless_unit{};

        const auto separator = [](char c) {
            return c == '/' or c == '*' or c == ' ';
        };

        while (it != unit_specification.end())
        {
            if (separator(*it))
            {
                ++it;
                continue;
            }

            const auto end{ std::ranges::find_if(it, unit_specification.end(), separator) };

            const auto prefixed{ prefixed_unit::parse(std::string_view{ it, end }) };

            if (prefixed.unit == Unit::none)
                throw std::format_error{ "Invalid unit." };

            if (unit::is_dimensionless(prefixed.unit))
            {
                if constexpr (not std::same_as<Dimension, Dimensionless>)
                    throw std::format_error{
                        "Dimensionless unit requires a dimensionless quantity."
                    };

                if (has_dimensionless_unit)
                    throw std::format_error{ "Only one dimensionless unit may be specified." };

                has_dimensionless_unit = true;
            }

            candidates.emplace_back(prefixed);
            it = end;
        }

        return candidates;
    }

    FormatPlan make_plan(Candidates candidates) const
    {
        using namespace physics;

        FormatPlan plan;

        constexpr auto default_size{ unit::index::last_base - unit::index::first + 1 };

        std::ranges::sort(candidates.begin() + default_size, candidates.end());

        auto consume = [](auto& dimensions, auto const& unit_dimensions, Exponent direction) {
            for (std::size_t i{}; i < dimensions.size(); ++i)
                dimensions[i] -= direction * unit_dimensions[i];
        };

        auto negate = [](auto representation) {
            for (auto& exponent : representation)
                exponent = -exponent;

            return representation;
        };

        auto remaining_dimensions{ Dimension::representation() };

        for (auto i{ candidates.size() }; i-- > 0;)
        {
            auto [unit, prefix]{ candidates[i] };

            Exponent exponent{};

            const auto unit_dimensions{ unit::dimensions::representation(unit) };

            if (unit_dimensions == Dimensionless::representation())
            {
                if (remaining_dimensions == Dimensionless::representation())
                    exponent = 1;
            }
            else
            {
                while (unit::dimensions::contains(remaining_dimensions, unit))
                {
                    consume(remaining_dimensions, unit_dimensions, 1);
                    ++exponent;
                }

                if (exponent == 0)
                {
                    while (unit::dimensions::contains(negate(remaining_dimensions), unit))
                    {
                        consume(remaining_dimensions, unit_dimensions, -1);
                        --exponent;
                    }
                }
            }

            if (i >= default_size and exponent == 0)
            {
                throw std::format_error{ "Unit is incompatible with quantity dimensions." };
            }

            if (exponent > 0)
                plan.numerators.emplace_back(prefix, unit, exponent);

            if (exponent < 0)
                plan.denominators.emplace_back(prefix, unit, exponent);

            if (exponent != 0 and unit::is_logarithmic(unit))
                plan.logarithmic = true;

            plan.factor *= math::power(prefix::factor(prefix) * unit::factor(unit), -exponent);

            if (exponent != 0)
                plan.offset -= unit::offset(unit);
        }
        if (remaining_dimensions != Dimensionless::representation())
            throw std::format_error{
                "Quantity dimensions cannot be represented by the available units."
            };
        if (plan.logarithmic and
            (not IsDimensionless<Dimension> or reference != Reference::ratio))
        {
            throw std::format_error{ "Logarithmic units can only be used with dimensionless ratios."
            };
        }
        if (plan.logarithmic and (not IsDimensionless<Dimension> or plan.numerators.size() != 1 or
                                     not plan.denominators.empty()))
        {
            throw std::format_error{
                "Logarithmic units cannot be used in compound unit expressions."
            };
        }
        return plan;
    }

    template <class FormatContext>
    auto format(
        physics::Quantity<Dimension, reference, domain, Value> quantity,
        FormatContext& context) const
        -> decltype(context.out())
    {
        using namespace physics;
        auto out{ context.out() };
        auto const format_value{ output != Output::unit };
        auto const format_unit{ output != Output::value };

        const auto plan{ make_plan(parse_units()) };

        if (format_value)
        {
            auto value{ quantity.value() };

            if (plan.logarithmic)
                value = std::log(value);
            else if constexpr (reference == Reference::zero)
                value += plan.offset;

            value *= plan.factor;

            if (value_format_specified)
                out = std::formatter<Value>::format(value, context);
            else
                out = std::format_to(out, "{:.{}g}", value, std::numeric_limits<Value>::digits10);
        }

        auto write = [&](std::string_view text) {
            context.advance_to(out);
            out = std::formatter<std::string_view>::format(text, context);
        };

        if (format_unit)
        {
            const bool brackets{ use_fraction_slash and plan.denominators.size() > 1 };

            bool formatted{ false };
            for (auto* terms : { &plan.numerators, &plan.denominators })
            {
                bool formatted_inner{ false };
                for (auto const& term : *terms)
                {
                    if (not formatted and format_value)
                        *out++ = ' ';

                    if (use_fraction_slash and not formatted_inner and term.exponent < 0)
                    {
                        *out++ = '/';
                        if (brackets)
                            *out++ = '(';
                    }
                    else if (formatted)
                        write(use_fraction_slash ? "\u22C5" : " ");

                    write(prefix::symbol(term.prefix));
                    write(unit::symbol(term.unit));

                    const auto exponent{ use_fraction_slash ? math::abs(term.exponent)
                                                            : term.exponent };
                    if (exponent != 1)
                        write(exponent::to_superscript(exponent));

                    formatted_inner = formatted = true;
                }
            }
            if (brackets)
                *out++ = ')';
        }

        context.advance_to(out);
        return out;
    }
};
