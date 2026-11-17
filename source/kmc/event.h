#pragma once
#include "lattice/site.h"
#include "types.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
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
}
