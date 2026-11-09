#pragma once
#include "kmc/process.h"
#include "writer.h"

class output::process::Kinetics : public output::Writer
{
public:
    using Writer::Writer;

    struct SiteAccumulation
    {
        double count{};
        double population_fraction{};

        physics::quantity::TimeDifference count_time{};
        physics::quantity::TimeDifference population_time{};
    };

    struct ProcessAccumulation
    {
        double activity_fraction{};
        physics::quantity::TimeDifference activity_time{};
    };

    struct Data
    {
        std::map<kmc::Process, SiteAccumulation> sites;
        std::map<kmc::Process, ProcessAccumulation> processes;

        physics::quantity::TimeDifference residence_time{};
        std::size_t samples{};
    };

    void observe_selected(kmc::process::Record const&) override;

    void capture_init(kmc::Simulation const&) override;
    void capture_snapshot(kmc::Simulation const&) override;

    auto const& last_data() const { return last_data_; }

private:
    std::string_view name() const override { return "process_kinetics"; }

    void write_init(kmc::Simulation const&) override;
    void write_snapshot(kmc::Simulation const&) override;

    kmc::Simulation const* simulation_{};
    physics::quantity::Time last_time_{};

    Data last_data_{}, accumulation_{};
};
