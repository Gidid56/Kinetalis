#include "pch.h"

#include "event_flux.h"

#include "kmc/simulation.h"
#include "utility/file.h"

using namespace utility;
using enum file::Type;

using namespace output;

void process::EventFlux::capture_init(kmc::Simulation const& simulation)
{
    simulation_ = &simulation;
    last_time_ = simulation.time();
    residence_time_ = {};
    data_.clear();

    for (kmc::process::Key key{ 1 }; key < simulation.process_rates().size(); ++key)
    {
        auto const event{ simulation.process_rates()[key].first.event() };

        if (event != kmc::Event::none)
            data_.try_emplace(event);
    }
}

void process::EventFlux::observe_selected(kmc::process::Record const& record)
{
    if (record.outcome != kmc::process::Outcome::accepted)
        return;

    auto const& simulation{ *simulation_ };
    auto const time_delta{ simulation.time() - last_time_ };

    last_time_ = simulation.time();
    residence_time_ += time_delta;

    auto const& process_counts{ simulation.process_counts() };
    auto const& process_rates{ simulation.process_rates() };

    for (kmc::process::Key key{ 1 }; key < process_counts.size(); ++key)
    {
        auto const count{ process_counts[key] };

        if (count == 0)
            continue;

        auto const& [process, rate]{ process_rates[key] };
        auto const event{ process.event() };

        if (event != kmc::Event::none)
            data_[event].integral += count * rate * time_delta;
    }
}

void process::EventFlux::capture_snapshot(kmc::Simulation const&)
{
    for (auto& [_, flux] : data_)
        flux.value.reset();

    if (residence_time_ <= physics::quantity::TimeDifference{})
        return;

    for (auto& [_, flux] : data_)
    {
        flux.value = flux.integral / residence_time_;
        flux.maximum = std::max(flux.maximum, *flux.value);
        flux.integral = {};
    }

    residence_time_ = {};
}

std::optional<double> process::EventFlux::relative_flux(kmc::Event event) const
{
    auto const iterator{ data_.find(event) };

    if (iterator == data_.end())
        return std::nullopt;

    auto const& flux{ iterator->second };

    if (not flux.value or flux.maximum <= kmc::Rate{})
        return std::nullopt;

    return *flux.value / flux.maximum;
}

void process::EventFlux::write_init(kmc::Simulation const&)
{
    std::string header{ Writer::default_header };

    for (auto const& [event, _] : data_)
        std::format_to(
            std::back_inserter(header),
            ",{}_flux/Hz,{}_relative_flux",
            enum_name(event),
            enum_name(event));

    header += '\n';

    file::write<csv>(writer_path(), header);
}

void process::EventFlux::write_snapshot(kmc::Simulation const& simulation)
{
    if (data_.empty() or not data_.begin()->second.value)
        return;

    std::string text{ std::format("{},{:v:}", simulation.step(), simulation.time()) };

    for (auto const& [event, flux] : data_)
    {
        auto const relative{ relative_flux(event) };

        std::format_to(
            std::back_inserter(text),
            ",{},{}",
            flux.value->value(),
            relative.value_or(std::numeric_limits<double>::quiet_NaN()));
    }

    text += '\n';

    file::append<csv>(writer_path(), text);
}
