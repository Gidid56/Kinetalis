#pragma once
#include "lattice/site.h"
#include "types.h"

#include <array>
#include <concepts>
#include <cstdint>
#include <format>
#include <functional>
#include <optional>
#include <stdexcept>
#include <tuple>
#include <string>
#include <type_traits>
#include <utility>

namespace kmc::event
{
    enum class Type : std::uint8_t
    {
        none,
        diffusion,
        dissolution,
        deposition,
    };

    enum class Outcome : std::uint8_t
    {
        active,
        accepted,
        rejected,
    };

    // Lattice occupancy changes associated with an event type.
    constexpr int solid_change(Type type) noexcept
    {
        switch (type)
        {
            case Type::dissolution: return -1;
            case Type::deposition: return 1;
            default: return 0;
        }
    }

    constexpr int neighbor_change(Type type, bool source) noexcept
    {
        switch (type)
        {
            case Type::dissolution: return source ? -1 : 0;
            case Type::deposition: return source ? 1 : 0;
            case Type::diffusion: return source ? -1 : 1;
            default: return 0;
        }
    }

    class Location
    {
    public:
        static constexpr std::size_t capacity{ 2 };

        explicit Location(lattice::Site& site) noexcept : cluster_{ &site }, size_{ 1 } { }

        template <std::size_t N>
        requires(N > 0 and N <= capacity)
        explicit Location(lattice::Cluster<N> const& cluster) noexcept
            : size_{ static_cast<std::uint8_t>(N) }
        {
            std::ranges::copy(cluster, cluster_.begin());
        }

        std::span<lattice::Site* const> sites() const noexcept
        {
            return { cluster_.data(), size_ };
        }

        lattice::Site& source() const noexcept { return *cluster_[0]; }

    private:
        lattice::Cluster<capacity> cluster_{};
        std::uint8_t size_{};
    };

    class Record
    {
    public:
        Record(Location location, Type event, bool rejected, RateClassKey class_key) noexcept
            : type{ event },
              outcome{ rejected ? Outcome::rejected : Outcome::accepted },
              class_key{ class_key },
              location_{ std::move(location) }
        { }

        Location const& location() const noexcept { return location_; }
        auto sites() const noexcept { return location_.sites(); }

        Type type;
        Outcome outcome;
        RateClassKey class_key;

    private:
        Location location_;
    };

    // A transformation maps a local slot to an optional geometrical location.
    template <class T>
    concept Transformation = requires(T const& transformation, lattice::Site& source,
                                      std::size_t slot, lattice::Lattice& lattice,
                                      Location const& location) {
        { T::event } -> std::convertible_to<Type>;
        { T::slot_count } -> std::convertible_to<std::size_t>;
        { transformation.location(source, slot, lattice) } -> std::same_as<std::optional<Location>>;
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
    bool is_candidate_after_source_check(T const& transformation, Location const& location)
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
