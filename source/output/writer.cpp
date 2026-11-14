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

        //if (uses_snapshot_resolution())
        //    register_resolution();
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

//void Writer::register_resolution() const
//{
//    auto const file{ root_path() / "analysis_plan" };
//    auto const write_header{ not std::filesystem::exists(file) or
//                             std::filesystem::file_size(file) == 0 };
//
//    std::string text;
//
//    if (write_header)
//        text = "key:writer,min_snapshot_step,snapshot_scale_factor\n";
//
//    std::format_to(
//        std::back_inserter(text),
//        "{},{},{}\n",
//        name(),
//        resolution_.min_snapshot_step,
//        resolution_.snapshot_scale_factor);
//
//    file::append<csv>(file, text);
//}

std::string Writer::snapshot_name(kmc::Step step) const
{
    return std::format("{:06} step_{}", snapshot_index_, step);
}
