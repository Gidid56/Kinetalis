#pragma once

class kmc::Observer
{
public:
    struct Resolution
    {
        kmc::Step min_snapshot_step{ 100 };
        double snapshot_scale_factor{ 1.05 };
    };

    struct Snapshot
    {
        kmc::Step step{};
        kmc::Time time{};

        friend bool operator==(Snapshot const&, Snapshot const&) = default;
    };

    Observer() = default;

    explicit Observer(Resolution const& resolution) : resolution_{ resolution }
    {
        validate(resolution);
    }

    static void validate(Resolution const& resolution);

    virtual ~Observer() = default;

    auto& set_resolution(this auto& self, Resolution const& resolution)
    {
        validate(resolution);
        self.resolution_ = resolution;
        return self;
    }

    virtual void observe_started(Simulation const&) { }

    virtual void observe_step(Simulation const&) final;

    virtual void observe_completed(Simulation const&) { }

    virtual void observe_selected(process::Record const&) { }

    virtual void observe_processed(process::Record const&) { }

    virtual bool stop_requested() const { return false; }

    virtual std::string_view name() const = 0;
    virtual std::string settings() const { return {}; }

    virtual bool uses_snapshot_resolution() const { return false; }

    auto& resolution() const { return resolution_; }

protected:

    virtual void take_snapshot(Simulation const&) { }

    Resolution resolution_{};

private:
    Step next_snapshot_step_{};
};
