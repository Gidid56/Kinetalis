#pragma once

namespace viewer
{
    enum class Task
    {
        None,
        Run,
        Pause,
        Replay
    };

    enum class ViewMode
    {
        None,
        Elements,
        Coordination,
        ID,
        Layers,
        Pristine,
    };

    struct SiteState
    {
        chemistry::Element element{};
        lattice::Coordination coordination{};
        lattice::Coordination maxNeighbors{};
        lattice::Layer layer{};
        bool pristine{}, highlighted{};

        bool isBulk() const noexcept { return coordination == maxNeighbors; }
    };
}