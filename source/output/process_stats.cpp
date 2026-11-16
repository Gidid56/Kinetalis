#include "pch.h"

#include "process_stats.h"

#include "kmc/simulation.h"
#include "utility/file.h"

using namespace utility;
using enum file::Type;

using namespace output;

void process::Statistics::capture_init(kmc::Simulation const& simulation)
{
    simulation_ = &simulation;
    last_process_ = {};
    counts_.clear();
    transitions_.clear();
}

void process::Statistics::observe_selected(kmc::process::Record const& record)
{
    // Use the model's kinetic identity while the location is still unchanged.
    auto const key{ simulation_->model()->classify(record.type, record.location(), simulation_->lattice()) };
    ++counts_[record.outcome][key];

    if (record.outcome == kmc::process::Outcome::rejected)
        return;

    if (last_process_ != kmc::process::Key{})
        ++transitions_[last_process_][key];

    last_process_ = key;
}

void process::Statistics::capture_snapshot(kmc::Simulation const& simulation)
{
    auto& active{ counts_[kmc::process::Outcome::active] };
    active.clear();

    auto const& process_counts{ simulation.process_counts() };
    auto const& process_rates{ simulation.process_rates() };

    for (std::size_t key{ 1 }; key < process_counts.size(); ++key)
    {
        if (process_counts[key] == 0)
            continue;

        active[static_cast<kmc::process::Key>(key)] = process_counts[key];
    }
}

void process::Statistics::write_snapshot(kmc::Simulation const& kmc)
{
    auto const& model{ *kmc.model() };

    for (auto const& [outcome, counts] : counts_)
    {
        std::string text{ kmc::ClassInfo::header };
        text += ",count\n";

        for (auto const& [key, count] : counts)
            std::format_to(std::back_inserter(text), "{},{}\n",
                model.class_info(key), count);

        file::write<csv>(writer_path() / enum_name(outcome) / snapshot_name(kmc.step()), text);
    }

    if (transitions_.empty() or not output_transitions_)
        return;

    std::string transitions{ std::format(
        "from,,,,to,,,,\n{0},{0},count\n", kmc::ClassInfo::header) };

    for (auto const& [from, destinations] : transitions_)
        for (auto const& [to, count] : destinations)
            std::format_to(std::back_inserter(transitions), "{},{},{}\n",
                model.class_info(from), model.class_info(to), count);

    file::write<csv>(writer_path() / "transitions" / snapshot_name(kmc.step()), transitions);
}
