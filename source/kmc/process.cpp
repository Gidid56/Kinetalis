#include "pch.h"

#include "process.h"

#include "lattice/path.h"
#include "lattice/site.h"

using namespace kmc;
using namespace lattice;
using enum Element;

// TODO fix deposition element to Ag, check for allowed via site / model, not in process.

Process::Process(Event event, Element element, Coordination coordination)
    : event_{ event },
      element_{ element },
      coordination_{ coordination }
{ }

Process::Process(Event event, Site const& site)
    : event_{ event },
      element_{ site.element() },
      coordination_{ site.coordination() },
      element_neighbors_{ site.element_neighbors() }
{ }

Process::Process(Event event, Path const& path)
    : event_{ event },
      element_{ path.front().element() },
      coordination_{ path.front().coordination() },
      element_neighbors_{ path.front().element_neighbors() }
{ }

bool Process::is_allowed() const
{
    switch (event_)
    {
        case Event::diffusion: return element_ != Element::none;
        case Event::dissolution: return element_ == Element::Ag;
        case Event::deposition: return element_ == Element::none and coordination_ >= 1;
    }
    return false;
}

static_assert(
    not elementSpecificNeighbors or not kmc::staticProcessKeys,
    "Element-specific neighbor energies require dynamic process keys.");

// maxCoordination is the exclusive upper bound for active
// source-site coordinations. Fully coordinated bulk atoms are inactive.
namespace offset
{
    constexpr process::Key diffusion{ 1 };

    constexpr process::Key dissolution{ diffusion + max_coordination * std::size(activeElements) };

    constexpr process::Key deposition{ dissolution + max_coordination };

    constexpr std::size_t coordinationBits{ 4 };
}

process::Key Process::static_key() const
{
    if (not is_allowed())
        return {};

    switch (event_)
    {
        case Event::diffusion:
            return offset::diffusion + max_coordination * elementKey(element_) + coordination_;

        case Event::dissolution: return offset::dissolution + coordination_;

        case Event::deposition: return offset::deposition + coordination_;

        default: return {};
    }
}

std::size_t Process::number_keys() { return offset::deposition + max_coordination; }

std::size_t std::hash<Process>::operator()(Process const& process) const
{
    std::size_t hash{}, offset{};
    hash += static_cast<std::size_t>(process.event()) << offset;
    offset += 2;
    hash += static_cast<std::size_t>(process.element()) << offset;
    offset += 7;
    hash += static_cast<std::size_t>(process.coordination()) << offset;
    offset += 4;

    if constexpr (elementSpecificNeighbors)
        for (auto& element : activeElements)
        {
            hash += static_cast<std::size_t>(process.element_neighbors()[elementKey(element)])
                    << offset;
            offset += 4;
        }
    return std::hash<std::size_t>{}(hash);
}

std::format_context::iterator
std::formatter<Process>::format(Process const& process, std::format_context& context) const
{
    auto out = std::format_to(
        context.out(),
        "{}{}{}{}{}",
        enum_name(process.event()),
        separator,
        process.element(),
        separator,
        process.coordination());

    if constexpr (elementSpecificNeighbors)
    {
        out = std::format_to(out, "{}(", separator);

        for (auto& element : activeElements)
            if (auto neighbors{ process.element_neighbors()[elementKey(element)] })
                out = std::format_to(out, "{}x {}; ", neighbors, symbol(element));

        out = std::format_to(out, ")");
    }

    return out;
}

process::Record::Record(lattice::Site& site)
    : outcome{ site.is_allowed() ? process::Outcome::accepted : process::Outcome::rejected },
      process{ site },
      path{ &site, &site }
{ }

process::Record::Record(lattice::Path const& path)
    : outcome{ path.is_allowed() ? process::Outcome::accepted : process::Outcome::rejected },
      process{ path },
      path{ path }
{ }

void kmc::process::Record::execute(Lattice& lattice)
{
    if (&path.front() == &path.back())
        path.front().execute_process(lattice);
    else
        path.execute_process(lattice);
}
