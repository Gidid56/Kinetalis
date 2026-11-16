#pragma once
#include "event.h"
#include "lattice/types.h"

#include <cassert>
#include <cstdint>
#include <span>

/*
   Stores essential data required to identify strictly equivalent simulation processes.

   Public Interface:
   - Constructors: Initialization with default values, specified event, chemical element, and lattice neighbors,
	 or directly from a lattice site or path.
   - Access methods for static keys, event, chemical element, and neighbors of the process.
   - Functions to check process allowance and activity.

   Private Attributes:
   - 'event::Type event_': Represents the specific event associated with the process.
   - 'lattice::Coordination coordination_': Stores the count of neighbors in the initial lattice site.
   - 'Element element_': Represents the chemical element related to the process.

   Note:
   - This class serves as a container storing essential data necessary to distinguish and identify simulation
	 processes that are precisely identical in their energetics and execution probabilities.
*/
class kmc::Process
{
public:
    explicit Process() = default;
    explicit Process(event::Type, Element, lattice::Coordination);
    explicit Process(event::Type, lattice::Site const&);
    explicit Process(event::Type, lattice::Cluster<2> const&);

    process::Key static_key() const;
    static std::size_t number_keys();

    bool is_active() const { return event_ != event::Type::none; }

    auto event() const { return event_; }
    auto element() const { return element_; }
    auto coordination() const { return coordination_; }
    auto element_neighbors() const { return element_neighbors_; }

    bool operator==(Process const&) const = default;
    auto operator<=>(Process const&) const = default;

    static constexpr std::string_view header{ "key:event,key:element,key:coordination" };

    constexpr int solid_change() const
    {
        switch (event_)
        {
            case kmc::event::Type::dissolution: return -1;
            case kmc::event::Type::deposition: return 1;
            default: return 0;
        }
    }

    constexpr int neighbor_change(bool front) const
    {
        switch (event_)
        {
            case kmc::event::Type::dissolution: return front ? -1 : 0;
            case kmc::event::Type::deposition: return front ? 1 : 0;
            case kmc::event::Type::diffusion: return front ? -1 : 1;
            default: return 0;
        }
    }

private:
    event::Type event_{};
    Element element_{};
    lattice::Coordination coordination_{};
    //[[no_unique_address]] ElementNeighbors elementNeighbors_{};
    std::array<lattice::Coordination, lattice::numberActiveElements> element_neighbors_{};
};

class kmc::process::Record
{
public:
    Record(lattice::Site&, event::Type, bool rejected);
    Record(lattice::Cluster<2> const&, event::Type, bool rejected);

    // The returned view refers to storage owned by this record.
    // It must not outlive the record.
    std::span<lattice::Site* const> sites() const noexcept
    {
        return { cluster_.data(), size_ };
    }

    // Retain the current two-site rule interface.
    lattice::Cluster<2>& cluster() noexcept
    {
        assert(size_ == 2);
        return cluster_;
    }

    lattice::Cluster<2> const& cluster() const noexcept
    {
        assert(size_ == 2);
        return cluster_;
    }

    Outcome outcome;
    Process process;

private:
    lattice::Cluster<2> cluster_{};
    std::uint8_t size_{};
};


template <>
struct std::hash<kmc::Process>
{
    std::size_t operator()(kmc::Process const&) const;
};

template <>
struct std::formatter<kmc::Process> : public std::formatter<std::string_view>
{
    char separator{ ',' };

    constexpr auto parse(std::format_parse_context& context)
    {
        auto iterator{ context.begin() };

        if (iterator != context.end() && *iterator != '}')
            separator = *iterator++;

        if (iterator != context.end() && *iterator != '}')
            throw std::format_error{ "Invalid Process format specifier." };

        return iterator;
    }

    std::format_context::iterator format(kmc::Process const&, std::format_context&) const;
};
