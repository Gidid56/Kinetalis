#pragma once
#include "types.h"
#include "engine.h"
#include "lattice/lattice.h"

class simulation::Base
{
public:
    Base(Base const&) = delete;

    Base(Base&&) = delete;
    Base& operator=(Base&&) = delete;

    virtual ~Base() = default;

    double tracked_fraction() const
    {
        return static_cast<double>(tracked_.duration().count()) / timer_.duration().count();
    }

    Duration run_duration() const
    {
        return Duration{ std::chrono::duration<double>{ timer_.duration() }.count() };
    }

    Duration tracked_time() const
    {
        return Duration{ std::chrono::duration<double>{ tracked_.duration() }.count() };
    }

    auto& mutable_lattice() { return lattice_; }
    auto& lattice() const { return lattice_; }

    auto step() const { return step_; }

    auto& rng() { return rng_; }
    auto& rng() const { return rng_; }

protected:
    Base(lattice::Generator const& make_lattice, std::optional<Seed> seed)
        : lattice_{ make_lattice() },
          rng_{ seed }
    {
        lattice_.apply_fill_function(rng_);

        for (auto& site : lattice_.sites())
            site.set_pristine(true);
    }

    simulation::Engine rng_;
    lattice::Lattice lattice_;

    std::uint64_t step_{};
    std::atomic<Status> status_{};

    utility::Timer timer_{}, tracked_{};
};

template <>
struct std::formatter<utility::Timer> : public std::formatter<physics::quantity::Duration>
{
    auto format(utility::Timer const& timer, std::format_context& context) const
    {
        using namespace physics::unit::literals;

        physics::quantity::Duration const elapsed{
            1_ns * std::chrono::duration_cast<std::chrono::nanoseconds>(timer.duration()).count()
        };

        return std::formatter<physics::quantity::Duration>::format(elapsed, context);
    }
};
