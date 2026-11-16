#include "pch.h"

#include "process.h"

#include "lattice/site.h"

using namespace kmc;
using namespace lattice;
using enum Element;

// TODO fix deposition element to Ag

Process::Process(event::Type event, Element element, Coordination coordination)
    : event_{ event },
      element_{ element },
      coordination_{ coordination }
{ }

Process::Process(event::Type event, Site const& site)
    : event_{ event },
      element_{ site.element() },
      coordination_{ site.coordination() },
      element_neighbors_{ site.element_neighbors() }
{ }

Process::Process(event::Type event, Cluster<2> const& cluster)
    : event_{ event },
      element_{ cluster[0]->element() },
      coordination_{ cluster[0]->coordination() },
      element_neighbors_{ cluster[0]->element_neighbors() }
{ }

static_assert(
    not element_specific_neighbors or not kmc::staticProcessKeys,
    "Element-specific neighbor energies require dynamic process keys.");

// maxCoordination is the exclusive upper bound for active
// source-site coordinations. Fully coordinated bulk atoms are inactive.
namespace offset
{
    constexpr process::Key diffusion{ 1 };

    constexpr process::Key dissolution{ diffusion + max_coordination * std::size(active_elements) };

    constexpr process::Key deposition{ dissolution + max_coordination };

    constexpr std::size_t coordinationBits{ 4 };
}

process::Key Process::static_key() const
{
    if (not is_active())
        return {};

    switch (event_)
    {
        case event::Type::diffusion:
            return offset::diffusion + max_coordination * elementKey(element_) + coordination_;

        case event::Type::dissolution: return offset::dissolution + coordination_;

        case event::Type::deposition: return offset::deposition + coordination_;

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

    if constexpr (element_specific_neighbors)
        for (auto& element : active_elements)
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

    if constexpr (element_specific_neighbors)
    {
        out = std::format_to(out, "{}(", separator);

        for (auto& element : active_elements)
            if (auto neighbors{ process.element_neighbors()[elementKey(element)] })
                out = std::format_to(out, "{}x {}; ", neighbors, symbol(element));

        out = std::format_to(out, ")");
    }

    return out;
}

process::Record::Record(Site& site, event::Type event, bool rejected)
    : outcome{ rejected ? Outcome::rejected : Outcome::accepted },
      process{ event, site },
      cluster_{ &site, nullptr },
      size_{ 1 }
{ }

process::Record::Record(Cluster<2> const& event_cluster, event::Type event, bool rejected)
    : outcome{ rejected ? Outcome::rejected : Outcome::accepted },
      process{ event, event_cluster },
      cluster_{ event_cluster },
      size_{ 2 }
{ }
