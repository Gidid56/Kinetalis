#include "pch.h"

#include "base.h"

#include "lattice/lattice.h"

using namespace simulation;

Base::Base(
    lattice::Generator const& make_lattice,
    std::optional<random_::Engine::seed_type> seed)
    : lattice_{ make_lattice() },
      rng_{ seed }
{
    lattice_.apply_fill_function(rng_);

    for (auto& site : lattice_.sites())
        site.set_pristine(true);
}

double Base::tracked_fraction() const
{
    return static_cast<double>(tracked_.duration().count()) / timer_.duration().count();
}

Duration Base::run_duration() const
{
    return Duration{ std::chrono::duration<double>{ timer_.duration() }.count() };
}

Duration Base::tracked_time() const
{
    return Duration{ std::chrono::duration<double>{ tracked_.duration() }.count() };
}
