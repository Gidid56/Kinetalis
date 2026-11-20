#pragma once
#include "writer.h"
#include "utility/csv.h"

class control::StepLimit : public kmc::Observer
{
public:
    explicit StepLimit(kmc::Step limit) : limit_{ limit } { }

    bool stop_requested(kmc::Simulation const&) const override;
    Subscription subscriptions() const override { return Subscription::stopping; }

private:
    std::string_view name() const override { return "step_limit"; }
    std::string settings() const override { return std::format("limit = {}", limit_); }

    kmc::Step limit_{};
};

class control::DurationLimit : public kmc::Observer
{
public:
    explicit DurationLimit(Duration limit) : limit_{ limit } { }

    bool stop_requested(kmc::Simulation const&) const override;
    Subscription subscriptions() const override { return Subscription::stopping; }

    void set_cap(Duration limit) { limit_ = limit; }

private:
    std::string_view name() const override { return "duration_limit"; }
    std::string settings() const override { return std::format("limit = {}", limit_); }

    Duration limit_{};
};

class output::Config : public output::Writer
{
public:
    using Writer::Writer;

private:
    std::string_view name() const override { return "config"; }
    bool uses_snapshot_resolution() const override { return false; }

    void write_init(kmc::Simulation const&) override;
};

class output::Performance : public output::Writer
{
public:
    using Writer::Writer;

private:
    std::string_view name() const override { return "performance"; }

    void write_init(kmc::Simulation const&) override;
    void write_snapshot(kmc::Simulation const&) override;
    void write_final(kmc::Simulation const&) override;
    utility::csv::Writer csv_;
};

class output::Time : public output::Writer
{
public:
    using Writer::Writer;

private:
    std::string_view name() const override { return "time"; }

    void write_init(kmc::Simulation const&) override;
    void write_snapshot(kmc::Simulation const&) override;
    void write_final(kmc::Simulation const&) override;
    utility::csv::Writer csv_;
};

class output::Lattice : public output::Writer
{
public:
    using Writer::Writer;

    auto& set_save_bulk(bool enabled)
    {
        save_bulk_ = enabled;
        return *this;
    }

private:
    std::string_view name() const override { return "lattice"; }
    std::string settings() const override { return std::format("save_bulk = {}", save_bulk_); }

    void write_init(kmc::Simulation const&) override;
    void write_snapshot(kmc::Simulation const&) override;
    void write_final(kmc::Simulation const&) override;

    bool save_bulk_{ false };
};
