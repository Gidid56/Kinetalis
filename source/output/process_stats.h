#pragma once
#include "kmc/process.h"
#include "writer.h"

class output::process::Statistics : public output::Writer
{
public:
    using Writer::Writer;

    auto& set_output_transitions(bool enabled)
    {
        output_transitions_ = enabled;
        return *this;
    }

    [[nodiscard]] auto& counts() const { return counts_; }
    [[nodiscard]] auto& transitions() const { return transitions_; }

private:
    std::string_view name() const override { return "processes"; }
    std::string settings() const override
    {
        return std::format("output_transitions = {}", output_transitions_);
    }

    void capture_init(kmc::Simulation const&) override;
    void observe_selected(kmc::process::Record const&) override;
    void capture_snapshot(kmc::Simulation const&) override;
    void write_snapshot(kmc::Simulation const&) override;

    bool output_transitions_{ false };

    kmc::Process last_process_{};
    std::map<kmc::process::Outcome, CountMap> counts_;
    std::map<kmc::Process, CountMap> transitions_;
};
