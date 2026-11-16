#pragma once
#include "lattice/site.h"
#include "types.h"

#include <concepts>
#include <cstdint>
#include <format>
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

    // Source prefiltering only needs the source site; all other rule
    // operations receive the complete ordered event location.
    template <class T>
    concept Rule = requires(T const& rule, lattice::Site const& source,
                            Location const& location, lattice::Lattice& lattice) {
        { T::event } -> std::convertible_to<kmc::event::Type>;
        { rule.is_source_candidate(source) } -> std::convertible_to<bool>;
        { rule.is_candidate(location) } -> std::convertible_to<bool>;
        { rule.is_rejected(location) } -> std::convertible_to<bool>;
        { rule.execute(location, lattice) } -> std::same_as<void>;
        { rule.settings() } -> std::convertible_to<std::string>;
    };

    template <class SiteRule, class ClusterRule>
    requires Rule<SiteRule> && Rule<ClusterRule>
    struct Rules
    {
        [[no_unique_address]] SiteRule site;
        [[no_unique_address]] ClusterRule cluster;

        std::string settings() const
        {
            return std::format(
                "[site: {}]\n{}\n\n"
                "[cluster: {}]\n{}\n",
                enum_name(SiteRule::event),
                site.settings(),
                enum_name(ClusterRule::event),
                cluster.settings());
        }
    };
}
