#include "pch.h"

#include "electrochemistry.h"

#include "kmc/simulation.h"
#include "utility/file.h"

using namespace utility::file;
using enum utility::file::Type;

using namespace studies::dealloying::analysis;

void Electrochemistry::observe_processed(kmc::event::Record const& record)
{
    auto const event{ record.type };

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

void Electrochemistry::write_init(kmc::Simulation const& kmc)
{
    auto header = [] {
        std::string text{ default_header };
        for (auto event : events)
            std::format_to(std::back_inserter(text),
                ",{0}_charge_transfer,{0}_interval_current/A", enum_name(event));
        return text;
    };
    auto row = [this, &kmc] {
        std::string text{ std::format("{},{:v:}", kmc.step(), kmc.time()) };
        for (std::size_t i{}; i < events.size(); ++i)
            std::format_to(std::back_inserter(text), ",{},{:v:}",
                last_data_.charge_transfers[i], last_data_.currents[i]);
        return text;
    };
    csv_.initialize(writer_path(), std::move(header), std::move(row));
}

void Electrochemistry::write_snapshot(kmc::Simulation const&)
{
    csv_.append(2 * events.size());
}

void Electrochemistry::write_final(kmc::Simulation const&) { csv_.finalize(); }
