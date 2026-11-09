#pragma once
#include "math/functions.h"

#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace physics::inline constraints
{
    enum class Reference : std::uint8_t
    {
        zero,       // additive reference at 0
        difference, // no retained reference
        ratio,      // multiplicative reference at 1
    };

    enum class Domain : std::uint8_t
    {
        unrestricted, // [-∞, ∞]
        non_negative, // [0, ∞]
        unit_interval // [0, 1]
    };

    constexpr bool negatable(Domain domain) { return domain == Domain::unrestricted; }

    constexpr bool satisfies(Domain domain, Domain required) { return domain >= required; }

    constexpr Domain restricted(Domain left, Domain right) { return std::max(left, right); }

    constexpr bool summable(Reference reference) { return true; }

    constexpr Reference add(Reference left, Reference right)
    {
        if (left == right)
            return left;

        if (left == Reference::difference)
            return right;

        if (right == Reference::difference)
            return left;

        return Reference::difference;
    }

    constexpr Reference subtract(Reference left, Reference right)
    {
        if (left == Reference::zero and right == Reference::zero)
            return Reference::difference;

        return add(left, right);
    }

    constexpr Reference multiply(Reference left, Reference right)
    {
        if (left == Reference::difference or right == Reference::difference)
            return Reference::difference;

        if (left == Reference::zero or right == Reference::zero)
            return Reference::zero;

        return Reference::ratio;
    }

    constexpr Reference divide(Reference left, Reference right, bool same_dimension)
    {
        if (same_dimension and left == right)
            return Reference::ratio;

        return multiply(left, right);
    }

    constexpr Domain add(Domain left, Domain right)
    {
        if (satisfies(left, Domain::non_negative) and satisfies(right, Domain::non_negative))
        {
            return Domain::non_negative;
        }

        return Domain::unrestricted;
    }

    constexpr Domain subtract(Domain, Domain) { return Domain::unrestricted; }

    constexpr Domain multiply(Domain left, Domain right)
    {
        if (left == Domain::unit_interval and right == Domain::unit_interval)
            return Domain::unit_interval;

        if (satisfies(left, Domain::non_negative) and satisfies(right, Domain::non_negative))
        {
            return Domain::non_negative;
        }

        return Domain::unrestricted;
    }

    constexpr Domain divide(Domain left, Domain right)
    {
        if (satisfies(left, Domain::non_negative) and satisfies(right, Domain::non_negative))
        {
            return Domain::non_negative;
        }

        return Domain::unrestricted;
    }

    template <std::size_t root>
    constexpr Domain root_map(Domain domain)
    {
        static_assert(root > 0);

        if constexpr (root % 2 == 0)
            return restricted(domain, Domain::non_negative);
        else
            return domain;
    }
}
