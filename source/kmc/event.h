#pragma once
#include "lattice/site.h"
#include "types.h"

#include <array>
#include <concepts>
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

        template <class... SiteT>
        requires(sizeof...(SiteT) > 0 and sizeof...(SiteT) <= capacity and
                    (std::same_as<SiteT, lattice::Site*> and ...))
        explicit Location(SiteT... sites) noexcept : sites_{ sites... },
                                                     size_{ sizeof...(SiteT) }
        { }

        std::span<lattice::Site* const> sites() const noexcept { return { sites_.data(), size_ }; }
        lattice::Site& source() const noexcept { return *sites_[0]; }
        std::size_t size() const noexcept { return size_; }

    private:
        std::array<lattice::Site*, capacity> sites_{};
        std::size_t size_{};
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
