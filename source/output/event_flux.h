#pragma once
#include "kmc/event/record.h"
#include "writer.h"

class output::EventFlux : public output::Writer
{
public:
    struct FluxData
    {
        double integral{};
        std::optional<kmc::Rate> value;
        kmc::Rate maximum{};
    };

    using Data = std::map<kmc::event::Type, FluxData>;

    using Writer::Writer;

    void observe_selected(kmc::event::Record const&) override;

    void capture_init(kmc::Simulation const&) override;
    void capture_snapshot(kmc::Simulation const&) override;

    void write_init(kmc::Simulation const&) override;
    void write_snapshot(kmc::Simulation const&) override;

    std::optional<double> relative_flux(kmc::event::Type event) const;

    auto& last_data() const { return data_; }

private:
    std::string_view name() const override { return "event_flux"; }

    kmc::Simulation const* simulation_{};
    physics::quantity::Time last_time_{};
    physics::quantity::TimeDifference residence_time_{};

    Data data_;
};
