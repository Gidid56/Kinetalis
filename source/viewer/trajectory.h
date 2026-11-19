#pragma once
#include "kmc/observer.h"
#include "kmc/event.h"
#include "types.h"

class viewer::Trajectory : public kmc::Observer
{
public:
    enum class Direction
    {
        Backwards,
        None,
        Forwards
    };

    explicit Trajectory(Lattice& lattice) : lattice_{ lattice } { }

    void setViewerBridge(Bridge* bridge) { viewerBridge_ = bridge; }

    bool snapshots_enabled() const override { return false; }
    bool checks_stop() const override { return false; }
    void observe_selected(kmc::event::Record const&) override;
    void observe_processed(kmc::event::Record const&) override;

private:
    std::string_view name() const override { return "trajectory_recorder"; }

    void applyStep(Direction);
    void makeStep(Direction);
    bool finished(Direction) const;

public:
    void replayAll(std::stop_token);
    void replay(std::stop_token);
    auto playbackStep() const { return playbackStep_; }

private:
    Lattice& lattice_;
    Bridge* viewerBridge_{};

    kmc::Step playbackStep_{};

    struct RecordedEvent
    {
        kmc::event::Record record;
        std::array<chemistry::Element, 2> before;
        std::array<chemistry::Element, 2> after;
    };

    std::vector<RecordedEvent> records_;
};
