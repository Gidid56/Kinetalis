#pragma once
#include "kmc/observer.h"
#include "kmc/process.h"
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

    void observe_selected(kmc::process::Record const&) override;

private:
    void makeStep();
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

    std::vector<kmc::process::Record> records_;
};
