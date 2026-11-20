#include "pch.h"

#include "event_flux.h"

#include "kmc/simulation.h"
#include "utility/file.h"

using namespace utility;
using enum file::Type;

using namespace output;

void EventFlux::capture_init(kmc::Simulation const& simulation)
{
    simulation_ = &simulation;
    last_time_ = simulation.time();
    residence_time_ = {};
    data_.clear();

    // Event types are fixed independently of the lazy kinetic-class rate cache.
    for (auto event : enum_values<kmc::event::Type>())
        if (event != kmc::event::Type::none)
            data_.try_emplace(event);
}

void EventFlux::observe_selected(kmc::event::Record const&)
{
    auto const& simulation{ *simulation_ };
    auto const time_delta{ simulation.time() - last_time_ };

    last_time_ = simulation.time();
    residence_time_ += time_delta;

    auto const& classes{ simulation.kinetics().kinetic_classes() };
    auto const& counts{ simulation.class_counts() };
    for (std::size_t key{ 1 }; key < classes.size(); ++key)
    {
        auto const& kinetic_class{ classes[key] };
        if (counts[key] != 0 and kinetic_class.is_initialized())
            data_[kinetic_class.type].integral +=
                counts[key] * kinetic_class.rate * time_delta;
    }
}

void EventFlux::capture_snapshot(kmc::Simulation const&)
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

std::optional<double> EventFlux::relative_flux(kmc::event::Type event) const
{
    auto const iterator{ data_.find(event) };

    if (iterator == data_.end())
        return std::nullopt;

    auto const& flux{ iterator->second };

    if (not flux.value or flux.maximum <= kmc::Rate{})
        return std::nullopt;

    return *flux.value / flux.maximum;
}

void EventFlux::write_init(kmc::Simulation const& simulation)
{
    auto header = [this] {
        std::string text{ Writer::default_header };
        for (auto const& [event, _] : data_)
            std::format_to(std::back_inserter(text),
                ",{}_flux/Hz,{}_relative_flux", enum_name(event), enum_name(event));
        return text;
    };
    auto row = [this, &simulation] {
        std::string text{ std::format("{},{:v:}", simulation.step(), simulation.time()) };
        for (auto const& [event, flux] : data_)
        {
            auto const relative{ relative_flux(event) };
            std::format_to(std::back_inserter(text), ",{},{}",
                flux.value->value(),
                relative.value_or(std::numeric_limits<double>::quiet_NaN()));
        }
        return text;
    };
    csv_.initialize(writer_path(), std::move(header), std::move(row));
}

void EventFlux::write_snapshot(kmc::Simulation const&)
{
    if (not data_.empty() and data_.begin()->second.value)
        csv_.append(2 * data_.size());
}

void EventFlux::write_final(kmc::Simulation const&) { csv_.finalize(); }
