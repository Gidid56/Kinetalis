#include "pch.h"

#include "composition.h"
#include "kmc/simulation.h"

using namespace output;

void Composition::capture_init(kmc::Simulation const& kmc)
{
    element_counts_.clear();
    columns_.clear();
    for (auto& site : kmc.lattice().sites())
        if (site.is_occupied())
        {
            auto const element{ site.element() };
            if (not element_counts_.contains(element))
                columns_.push_back(element);
            ++element_counts_[element];
        }
}

void Composition::observe_selected(kmc::event::Record const& record)
{
    if (record.outcome != kmc::event::Outcome::accepted)
        return;

    auto const delta{ kmc::event::solid_change(record.type) };
    if (delta != 0)
    {
        auto const element{ record.location().source().element() };
        if (not element_counts_.contains(element))
            columns_.push_back(element);
        element_counts_[element] += delta;
    }
}

void Composition::write_init(kmc::Simulation const& kmc)
{
    auto header = [this] {
        std::string text{ default_header };
        for (auto element : columns_)
            std::format_to(std::back_inserter(text), ",{}_fraction", element);
        return text;
    };

    auto row = [this, &kmc] {
        auto const total{ utility::sum(element_counts_ | std::views::values) };
        std::string text{ std::format("{},{:v:}", kmc.step(), kmc.time()) };
        for (auto element : columns_)
        {
            auto const fraction{ total == 0
                ? std::numeric_limits<double>::quiet_NaN()
                : static_cast<double>(element_counts_.at(element)) / total };
            std::format_to(std::back_inserter(text), ",{}", fraction);
        }
        return text;
    };
    csv_.initialize(writer_path(), std::move(header), std::move(row));
}
