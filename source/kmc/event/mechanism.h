#pragma once
#include "kmc/event/location.h"
#include "kmc/event/types.h"

#include <concepts>
#include <optional>
#include <string>

namespace kmc::event
{
    // A mechanism maps a local slot to an optional geometrical location.
    template <class T>
    concept Mechanism = requires(T const& mechanism, lattice::Site& source,
                                      std::size_t slot, lattice::Lattice& lattice,
                                      Location const& location) {
        { T::event } -> std::convertible_to<Type>;
        { T::slot_count } -> std::convertible_to<std::size_t>;
        { mechanism.location(source, slot, lattice) } -> std::same_as<std::optional<Location>>;
        { mechanism.is_source_candidate(source) } -> std::convertible_to<bool>;
        { mechanism.is_candidate(location) } -> std::convertible_to<bool>;
        { mechanism.is_rejected(location) } -> std::convertible_to<bool>;
        { mechanism.execute(location, lattice) } -> std::same_as<void>;
        { mechanism.settings() } -> std::convertible_to<std::string>;
    };

    // The source precondition is established before a slot is examined.
    // Mechanisms may offer a cheaper residual predicate; otherwise
    // the full candidate check remains the fallback.
    template <Mechanism T>
    bool is_candidate_after_source_check(T const& mechanism, Location const& location)
    {
        if constexpr (requires {
            { mechanism.is_candidate_after_source_check(location) } -> std::convertible_to<bool>;
        })
            return mechanism.is_candidate_after_source_check(location);
        else
            return mechanism.is_candidate(location);
    }


}
