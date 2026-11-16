#pragma once
#include "lattice/path.h"
#include "lattice/types.h"

namespace kmc
{
    template <class T>
    concept EventLocation = std::same_as<T, lattice::Site> or std::same_as<T, lattice::Path>;

    // The source site and the full event location are evaluated before
    // selection; rejection is evaluated only for a selected candidate.
    template <typename T, class Location>
    concept Rule = EventLocation<Location> and
                   requires(T const& rule,
                            lattice::Site const& source,
                            Location const& location,
                            Location& mutable_location,
                            lattice::Lattice& lattice,
                            kmc::Process const& process) {
                       { T::event } -> std::convertible_to<kmc::Event>;
                       { rule.is_source_candidate(source) } -> std::convertible_to<bool>;
                       { rule.is_candidate(location) } -> std::convertible_to<bool>;
                       { rule.is_rejected(location) } -> std::convertible_to<bool>;
                       { rule.execute(mutable_location, lattice) } -> std::same_as<void>;
                       { rule.supports(process) } -> std::convertible_to<bool>;
                       { rule.settings() } -> std::convertible_to<std::string>;
                   };

    template <class Site, class Path>
    requires Rule<Site, lattice::Site> && Rule<Path, lattice::Path>
    struct EventRules
    {
        [[no_unique_address]] Site site;
        [[no_unique_address]] Path path;

        std::string settings() const
        {
            return std::format(
                "[site: {}]\n{}\n\n"
                "[path: {}]\n{}\n",
                enum_name(Site::event),
                site.settings(),
                enum_name(Path::event),
                path.settings());
        }
    };
}
/*
   Stores essential data required to identify strictly equivalent simulation processes.

   Public Interface:
   - Constructors: Initialization with default values, specified event, chemical element, and lattice neighbors,
	 or directly from a lattice site or path.
   - Access methods for static keys, event, chemical element, and neighbors of the process.
   - Functions to check process allowance and activity.

   Private Attributes:
   - 'Event event_': Represents the specific event associated with the process.
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
    explicit Process(Event, Element, lattice::Coordination);
    explicit Process(Event, lattice::Site const&);
    explicit Process(Event, lattice::Path const&);

    process::Key static_key() const;
    static std::size_t number_keys();

    bool is_active() const { return event_ != Event::none; }

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
            case kmc::Event::dissolution: return -1;
            case kmc::Event::deposition: return 1;
            default: return 0;
        }
    }

    constexpr int neighbor_change(bool front) const
    {
        switch (event_)
        {
            case kmc::Event::dissolution: return front ? -1 : 0;
            case kmc::Event::deposition: return front ? 1 : 0;
            case kmc::Event::diffusion: return front ? -1 : 1;
            default: return 0;
        }
    }

private:
    Event event_{};
    Element element_{};
    lattice::Coordination coordination_{};
    //[[no_unique_address]] ElementNeighbors elementNeighbors_{};
    std::array<lattice::Coordination, lattice::numberActiveElements> element_neighbors_{};
};

class kmc::process::Record
{
public:
    Record(lattice::Site&, Event, bool rejected);
    Record(lattice::Path const&, Event, bool rejected);

    Outcome outcome;
    Process process;
    lattice::Path path;
};

namespace kmc
{
    template <class Location, class R>
    requires Rule<R, Location>
    Process make_process(Location const& location, R const& rule)
    {
        if (not rule.is_candidate(location))
            return Process{};

        return Process{ R::event, location };
    }
}

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
