#include "pch.h"

#include "electrochemistry.h"

#include "kmc/simulation.h"
#include "utility/file.h"

using namespace utility::file;
using enum utility::file::Type;

using namespace studies::dealloying::analysis;

void Electrochemistry::observe_processed(kmc::process::Record const& record)
{
    auto const event{ record.process.event() };

    if (auto const charge{ charge_transfer(event) }; charge != 0)
    {
        charge_transfers_[utility::array_key<events>(event)] += charge;
    }
}

void Electrochemistry::capture_init(kmc::Simulation const&) { charge_transfers_.fill(0); }

void Electrochemistry::capture_snapshot(kmc::Simulation const& kmc)
{
    if (not last_snapshot())
        return;

    Electrochemistry::Data now{ .charge_transfers = charge_transfers_ };

    if (kmc.time() <= last_snapshot()->time)
        throw std::logic_error{ "Electrochemistry snapshots require increasing simulation time." };

    auto time_delta{ kmc.time() - last_snapshot()->time };

    for (size_t index{}; index < events.size(); index++)
    {
        auto charge_delta{ charge_transfers_[index] - last_data_.charge_transfers[index] };
        now.currents[index] = physics::constant::e * charge_delta / time_delta;
    }

    last_data_ = std::move(now);
}

void Electrochemistry::write_init(kmc::Simulation const&)
{
    std::string header{ default_header };

    for (auto& event : events)
        std::format_to(
            std::back_inserter(header),
            ",{0}_charge_transfer,{0}_interval_current/A",
            enum_name(event));

    write<csv>(writer_path(), header += '\n');
}

void Electrochemistry::write_snapshot(kmc::Simulation const& kmc)
{
    std::string line{ std::format("{},{:v:}", kmc.step(), kmc.time()) };

    for (size_t index{}; index < events.size(); index++)
    {
        std::format_to(
            std::back_inserter(line),
            ",{},{:v:}",
            last_data_.charge_transfers[index],
            last_data_.currents[index]);
    }

    append<csv>(writer_path(), line += '\n');
}
