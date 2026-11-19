#pragma once
#include "types.h"

#include <algorithm>
#include <stdexcept>
#include <string_view>

class kmc::Observer
{
public:
    enum class Subscription : std::uint8_t
    {
        none = 0,
        selected = 1 << 0,
        processed = 1 << 1,
        snapshot = 1 << 2,
        stopping = 1 << 3,
        all = selected | processed | snapshot | stopping,
    };

    static constexpr bool has_subscription(Subscription flags, Subscription flag) noexcept
    {
        return (static_cast<std::uint8_t>(flags) & static_cast<std::uint8_t>(flag)) != 0;
    }

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

    static void validate(Resolution const& resolution)
    {
        if (resolution.min_snapshot_step == 0)
            throw std::invalid_argument{ "Minimum snapshot step must be positive." };

        if (resolution.snapshot_scale_factor < 1)
            throw std::invalid_argument{ "Update snapshot scale factor must be at least one." };
    }

    virtual ~Observer() = default;

    auto& set_resolution(this auto& self, Resolution const& resolution)
    {
        validate(resolution);
        self.resolution_ = resolution;
        return self;
    }

    virtual void observe_started(Simulation const&) { }

    virtual void observe_step(Simulation const& simulation, Step step) final
    {
        if (step >= next_snapshot_step_)
        {
            take_snapshot(simulation);

            next_snapshot_step_ = std::max(
                next_snapshot_step_ + resolution_.min_snapshot_step,
                math::round<Step>(next_snapshot_step_ * resolution_.snapshot_scale_factor));
        }
    }

    virtual void observe_completed(Simulation const&) { }

    virtual void observe_selected(event::Record const&) { }

    virtual void observe_processed(event::Record const&) { }

    virtual bool stop_requested() const { return false; }

    // Opt-out metadata only; all callbacks remain enabled for existing observers.
    virtual Subscription subscriptions() const { return Subscription::all; }

    Step next_snapshot_step() const noexcept { return next_snapshot_step_; }

    virtual std::string_view name() const = 0;
    virtual std::string settings() const { return {}; }

    virtual bool uses_snapshot_resolution() const { return false; }
    virtual bool requires_class_counts() const { return false; }

    auto& resolution() const { return resolution_; }

protected:

    virtual void take_snapshot(Simulation const&) { }

    Resolution resolution_{};

private:
    Step next_snapshot_step_{};
};

namespace kmc
{
    constexpr Observer::Subscription operator|(
        Observer::Subscription lhs, Observer::Subscription rhs) noexcept
    {
        return static_cast<Observer::Subscription>(
            static_cast<std::uint8_t>(lhs) | static_cast<std::uint8_t>(rhs));
    }
}
