#include "pch.h"

#include "writer.h"

#include "kmc/simulation.h"
#include "utility/file.h"

using namespace utility;
using enum file::Type;

using namespace output;

void Writer::observe_started(kmc::Simulation const& kmc)
{
    last_snapshot_.reset();
    snapshot_index_ = {};

    capture_init(kmc);

    if (not root_.empty())
    {
        write_init(kmc);
    }

    if (uses_snapshot_resolution())
        kmc::Observer::observe_step(kmc);
}

void Writer::take_snapshot(kmc::Simulation const& kmc)
{
    kmc::Observer::Snapshot const now{
        .step = kmc.step(),
        .time = kmc.time(),
    };

    if (last_snapshot_ == now)
        return;

    capture_snapshot(kmc);

    if (not root_.empty())
        write_snapshot(kmc);

    last_snapshot_ = now;
    ++snapshot_index_;
}

void Writer::observe_completed(kmc::Simulation const& kmc)
{
    if (uses_snapshot_resolution())
        take_snapshot(kmc);

    capture_final(kmc);

    if (not root_.empty())
        write_final(kmc);
}

std::string Writer::snapshot_name(kmc::Step step) const
{
    return std::format("{:06} step_{}", snapshot_index_, step);
}
