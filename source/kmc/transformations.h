#pragma once
#include "event.h"
#include "utility/enums.h"

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <format>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace kmc
{
    // A transformation maps a local slot to an optional geometrical location.
    template <class T>
    concept Transformation = requires(T const& transformation, lattice::Site& source,
                                      std::size_t slot, lattice::Lattice& lattice,
                                      event::Location const& location) {
        { T::event } -> std::convertible_to<event::Type>;
        { T::slot_count } -> std::convertible_to<std::size_t>;
        { transformation.location(source, slot, lattice) } -> std::same_as<std::optional<event::Location>>;
        { transformation.is_source_candidate(source) } -> std::convertible_to<bool>;
        { transformation.is_candidate(location) } -> std::convertible_to<bool>;
        { transformation.is_rejected(location) } -> std::convertible_to<bool>;
        { transformation.execute(location, lattice) } -> std::same_as<void>;
        { transformation.settings() } -> std::convertible_to<std::string>;
    };

    // The source precondition is established before a slot is examined.
    // Transformations may offer a cheaper residual predicate; otherwise
    // the full candidate check remains the fallback.
    template <Transformation T>
    bool is_candidate_after_source_check(T const& transformation, event::Location const& location)
    {
        if constexpr (requires {
            { transformation.is_candidate_after_source_check(location) } -> std::convertible_to<bool>;
        })
            return transformation.is_candidate_after_source_check(location);
        else
            return transformation.is_candidate(location);
    }

    template <class... Ts>
    requires (Transformation<Ts> && ...)
    class Transformations
    {
    public:
        static constexpr std::size_t slot_count{ (Ts::slot_count + ... + std::size_t{}) };

        template <class T>
        auto& get(this auto& self) noexcept { return std::get<T>(self.transformations_); }

        using SourceCandidates = std::array<bool, sizeof...(Ts)>;

        // Evaluate each source predicate once; the same result drives the
        // all-inactive early exit and the per-transformation slot blocks.
        SourceCandidates source_candidates(lattice::Site const& source) const
        {
            return std::apply([&](auto const&... transformations) {
                return SourceCandidates{ transformations.is_source_candidate(source)... };
            }, transformations_);
        }

        // The array overload reuses already-evaluated predicates.
        static bool is_source_active(SourceCandidates const& candidates)
        {
            return std::ranges::any_of(candidates, std::identity{});
        }

        bool is_source_active(lattice::Site const& site) const
        {
            return is_source_active(source_candidates(site));
        }

        template <class Function>
        void for_each(Function&& function) const
        {
            std::size_t offset{};
            std::apply([&](auto const&... transformations) {
                ((std::invoke(function, transformations,
                    std::exchange(offset, offset +
                        std::remove_cvref_t<decltype(transformations)>::slot_count))), ...);
            }, transformations_);
        }

        template <class Function>
        void visit(std::size_t slot, Function&& function) const
        {
            std::size_t offset{};
            const bool found{ std::apply([&](auto const&... transformations) {
                return (([&] {
                    constexpr auto count{
                        std::remove_cvref_t<decltype(transformations)>::slot_count
                    };
                    const auto first{ std::exchange(offset, offset + count) };
                    if (slot < first or slot - first >= count)
                        return false;

                    std::invoke(function, transformations, slot - first);
                    return true;
                }()) or ...);
            }, transformations_) };

            if (not found)
                throw std::out_of_range{ "Event slot outside transformation layout." };
        }

        std::string settings() const
        {
            std::string result;
            for_each([&](auto const& transformation, std::size_t) {
                result += std::format("[{}]\n{}\n\n",
                    enum_name(std::remove_cvref_t<decltype(transformation)>::event),
                    transformation.settings());
            });
            return result;
        }

    private:
        std::tuple<Ts...> transformations_{};
    };
}
