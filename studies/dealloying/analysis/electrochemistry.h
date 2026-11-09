#pragma once
#include "output/writer.h"

namespace studies::dealloying::analysis { class Electrochemistry; }

class studies::dealloying::analysis::Electrochemistry : public output::Writer
{
public:
    using ChargeTransfer = std::int64_t;

    // enable or disable event specific output here
    static constexpr std::array events{
        kmc::Event::dissolution,
        //kmc::Event::deposition,
    };

    struct Data
    {
        using ElectricCurrent = physics::quantity::ElectricCurrent;

        // Commulative charge transfers per event
        std::array<ChargeTransfer, events.size()> charge_transfers{};
        // Mean currents since previous snapshot
        std::array<ElectricCurrent, events.size()> currents{};

        auto net_charge_transfer() const { return utility::sum(charge_transfers); }
        auto net_current() const { return utility::sum(currents); }
    };

    static constexpr ChargeTransfer charge_transfer(kmc::Event event)
    {
        switch (event)
        {
            case kmc::Event::dissolution: return 1;
            case kmc::Event::deposition: return -1;
            default: return {};
        }
    }

    using Writer::Writer;

    void observe_processed(kmc::process::Record const&) override;

    auto& last_data() const { return last_data_; }

private:
    std::string_view name() const override { return "electrochemistry"; }

    void capture_init(kmc::Simulation const&) override;
    void capture_snapshot(kmc::Simulation const&) override;

    void write_init(kmc::Simulation const&) override;
    void write_snapshot(kmc::Simulation const&) override;

    Electrochemistry::Data last_data_{};

    // Commulative charge transfer per event
    std::array<ChargeTransfer, events.size()> charge_transfers_{};
};
