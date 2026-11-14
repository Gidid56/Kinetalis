#pragma once
#include "writer.h"

class control::StepLimit : public kmc::Observer
{
public:
    explicit StepLimit(kmc::Step limit) : limit_{ limit } { }

    bool stop_requested() const override;

private:
    std::string_view name() const override { return "step_limit"; }
    std::string settings() const override { return std::format("limit = {}", limit_); }

    void observe_started(kmc::Simulation const& simulation) override { simulation_ = &simulation; }

    kmc::Step limit_{};
    kmc::Simulation const* simulation_{};
};

class control::DurationLimit : public kmc::Observer
{
public:
    explicit DurationLimit(Duration limit) : limit_{ limit } { }

    bool stop_requested() const override;

    void set_cap(Duration limit) { limit_ = limit; }

private:
    std::string_view name() const override { return "duration_limit"; }
    std::string settings() const override { return std::format("limit = {}", limit_); }

    void observe_started(kmc::Simulation const& simulation) override { simulation_ = &simulation; }

    Duration limit_{};
    kmc::Simulation const* simulation_{};
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
};

class output::Time : public output::Writer
{
public:
    using Writer::Writer;

private:
    std::string_view name() const override { return "time"; }

    void write_init(kmc::Simulation const&) override;
    void write_snapshot(kmc::Simulation const&) override;
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

class output::Rates : public output::Writer
{
public:
    using Writer::Writer;

private:
    std::string_view name() const override { return "rates"; }
    bool uses_snapshot_resolution() const override { return false; }

    void write_init(kmc::Simulation const&) override;
    void write_final(kmc::Simulation const&) override;
};

class output::Composition : public output::Writer
{
public:
    static constexpr std::array active_elements{
        lattice::Element::Ag,
        lattice::Element::Au,
    };

    using Writer::Writer;

private:
    std::string_view name() const override { return "composition"; }
    std::string settings() const override;

    void capture_init(kmc::Simulation const&) override;
    void observe_processed(kmc::process::Record const&) override;
    void write_init(kmc::Simulation const&) override;
    void write_snapshot(kmc::Simulation const&) override;

    std::map<lattice::Element, std::size_t> element_counts_;
};
