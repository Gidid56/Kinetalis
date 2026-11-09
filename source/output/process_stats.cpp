#include "pch.h"

#include "process_stats.h"

#include "kmc/simulation.h"
#include "utility/file.h"

using namespace utility;
using enum file::Type;

using namespace output;

void process::Statistics::capture_init(kmc::Simulation const&)
{
    last_process_ = kmc::Process{};
    counts_.clear();
    transitions_.clear();
}

void process::Statistics::observe_selected(kmc::process::Record const& record)
{
    ++counts_[record.outcome][record.process];

    if (record.outcome == kmc::process::Outcome::rejected)
        return;

    if (last_process_.event() != kmc::Event::none)
        ++transitions_[last_process_][record.process];

    last_process_ = record.process;
}

void process::Statistics::capture_snapshot(kmc::Simulation const& simulation)
{
    auto& active{ counts_[kmc::process::Outcome::active] };
    active.clear();

    auto const& process_counts{ simulation.process_counts() };
    auto const& process_rates{ simulation.process_rates() };

    for (kmc::process::Key key{ 1 }; key < process_counts.size(); ++key)
    {
        if (process_counts[key] == 0)
            continue;

        active[process_rates[key].first] = process_counts[key];
    }
}

void process::Statistics::write_snapshot(kmc::Simulation const& kmc)
{
    for (auto const& [outcome, counts] : counts_)
    {
        std::string text{ kmc::Process::header };
        text += ",count\n";

        for (auto const& [process, count] : counts)
            std::format_to(std::back_inserter(text), "{},{}\n", process, count);

        file::write<csv>(writer_path() / enum_name(outcome) / snapshot_name(kmc.step()), text);
    }

    if (transitions_.empty() or not output_transitions_)
        return;

    std::string transitions{ std::format(
        "from,,,to,,,\n"
        "{0},{0},count\n",
        kmc::Process::header) };

    for (auto const& [from, destinations] : transitions_)
        for (auto const& [to, count] : destinations)
            std::format_to(std::back_inserter(transitions), "{},{},{}\n", from, to, count);

    file::write<csv>(writer_path() / "transitions" / snapshot_name(kmc.step()), transitions);
}
