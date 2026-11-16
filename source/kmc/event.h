#pragma once
#include "lattice/site.h"
#include "types.h"

#include <concepts>
#include <cstdint>
#include <format>
#include <string>
#include <type_traits>

namespace kmc::event
{
    enum class Type : std::uint8_t
    {
        none,
        diffusion,
        dissolution,
        deposition,
    };

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

    // Every rule examines an ordered event location. For the source
    // prefilter, the location contains only its source site.
    template <class T>
    concept Rule = requires(T const& rule, Location const& location, lattice::Lattice& lattice) {
        { T::event } -> std::convertible_to<kmc::event::Type>;
        { rule.is_source_candidate(location) } -> std::convertible_to<bool>;
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
