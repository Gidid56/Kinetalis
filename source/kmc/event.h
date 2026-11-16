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

    template <class T>
    struct IsCluster : std::false_type
    {
    };

    template <std::size_t N>
    struct IsCluster<lattice::Cluster<N>> : std::true_type
    {
    };

    template <class T>
    concept IsLocation = std::same_as<T, lattice::Site> or IsCluster<T>::value;

    // The source site and the full event location are evaluated before
    // selection; rejection is evaluated only for a selected candidate.
    template <typename T, class Location_>
    concept Rule = IsLocation<Location_> and requires(
                                                 T const& rule,
                                                 lattice::Site const& source,
                                                 Location_ const& location,
                                                 Location_& mutable_location,
                                                 lattice::Lattice& lattice) {
        { T::event } -> std::convertible_to<kmc::event::Type>;
        { rule.is_source_candidate(source) } -> std::convertible_to<bool>;
        { rule.is_candidate(location) } -> std::convertible_to<bool>;
        { rule.is_rejected(location) } -> std::convertible_to<bool>;
        { rule.execute(mutable_location, lattice) } -> std::same_as<void>;
        { rule.settings() } -> std::convertible_to<std::string>;
    };

    template <class SiteRule, class ClusterRule>
    requires Rule<SiteRule, lattice::Site> && Rule<ClusterRule, lattice::Cluster<2>>
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
