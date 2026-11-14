#pragma once
#include "lattice/lattice.h"
#include "random/engine.h"

namespace simulation
{
    enum class Status
    {
        created,
        paused,
        running,
        exhausted,
        analysed,
    };

    class Base
    {
    public:
        Base(Base const&) = delete;

        Base(Base&&) = delete;
        Base& operator=(Base&&) = delete;

        virtual ~Base() = default;

        double tracked_fraction() const;

        Duration run_duration() const;
        Duration tracked_time() const;

        auto& mutable_lattice() { return lattice_; }
        auto& lattice() const { return lattice_; }

        auto step() const { return step_; }

        auto& rng() { return rng_; }
        auto& rng() const { return rng_; }

    protected:
        explicit Base(
            lattice::Generator const& make_lattice,
            std::optional<random_::Engine::seed_type> seed);

        lattice::Lattice lattice_;
        random_::Engine rng_;

        std::uint64_t step_{};
        std::atomic<Status> status_{};

        utility::Timer timer_{}, tracked_{};
    };
}
