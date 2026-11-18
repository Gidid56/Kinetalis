#pragma once
#include "lattice/site.h"
#include "kmc/types.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace kmc::event
{
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
}
