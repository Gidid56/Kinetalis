#include "pch.h"

#include "composition.h"
#include "kmc/simulation.h"
#include "utility/file.h"

using namespace utility;
using enum file::Type;

template <std::ranges::input_range Range>
std::string format_range(Range const& range)
{
    std::string result{ "[" };
    bool first{ true };

    for (auto const& value : range)
    {
        if (not first)
            result += ", ";

        std::format_to(std::back_inserter(result), "{}", value);
        first = false;
    }

    result += ']';
    return result;
}

std::string output::Composition::settings() const
{
    return std::format("elements = {}", format_range(active_elements));
}

void output::Composition::capture_init(kmc::Simulation const& kmc)
{
    element_counts_.clear();

    for (auto& site : kmc.lattice().sites())
        if (site.is_occupied())
            ++element_counts_[site.element()];
}

void output::Composition::observe_selected(kmc::event::Record const& record)
{
    if (record.outcome != kmc::event::Outcome::accepted)
        return;

    auto const delta{ kmc::event::solid_change(record.type) };
    if (delta != 0)
        element_counts_[record.location().source().element()] += delta;
}

void output::Composition::write_init(kmc::Simulation const&)
{
    std::string header{ default_header };

    for (auto element : active_elements)
        std::format_to(std::back_inserter(header), ",{}_fraction", element);

    file::write<csv>(writer_path(), header += '\n');
}

void output::Composition::write_snapshot(kmc::Simulation const& kmc)
{
    auto const occupied_sites{ utility::sum(element_counts_ | std::views::values) };

    std::string line{ std::format("{},{:v:}", kmc.step(), kmc.time()) };

    for (auto element : active_elements)
    {
        auto const fraction{ occupied_sites == 0
                                 ? std::numeric_limits<double>::quiet_NaN()
                                 : static_cast<double>(element_counts_[element]) / occupied_sites };

        std::format_to(std::back_inserter(line), ",{}", fraction);
    }

    file::append<csv>(writer_path(), line += '\n');
}
