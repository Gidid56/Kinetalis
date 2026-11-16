#include "pch.h"

#include "trajectory.h"

#include "bridge.h"
#include "kmc/process.h"
#include "lattice/path.h"
#include "lattice/site.h"

using namespace viewer;

void Trajectory::observe_selected(kmc::process::Record const& record)
{
    const std::array elements{ record.path.front().element(), record.path.back().element() };
    records_.push_back({ record, elements, elements });
    playbackStep_ = records_.size();
}

void Trajectory::observe_processed(kmc::process::Record const& record)
{
    assert(not records_.empty());
    records_.back().after = { record.path.front().element(), record.path.back().element() };
}

void Trajectory::applyStep(Direction direction)
{
    auto& entry{ records_.at(playbackStep_) };
    auto& record{ entry.record };

    if (viewerBridge_)
        viewerBridge_->observe_selected(record);

    if (record.outcome == kmc::process::Outcome::accepted)
    {
        // Replay recorded states without assuming the physical event type.
        auto const& elements{ direction == Direction::Forwards ? entry.after : entry.before };
        if (&record.path.front() != &record.path.back())
            record.path.back().set_element(lattice_, elements[1]);
        record.path.front().set_element(lattice_, elements[0]);

        if (viewerBridge_)
            viewerBridge_->observe_processed(record);
    }
}

void Trajectory::makeStep(Direction direction)
{
    if (not finished(direction))
    {
        switch (direction)
        {
            case Direction::Forwards:
                applyStep(Direction::Forwards);
                ++playbackStep_;
                break;
            case Direction::Backwards:
                --playbackStep_;
                applyStep(Direction::Backwards);
                break;
        }
    }
}

bool Trajectory::finished(Direction direction) const
{
    switch (direction)
    {
        case Direction::Forwards: return playbackStep_ == records_.size();
        case Direction::Backwards: return playbackStep_ == 0;
        default: return false;
    }
}

void Trajectory::replayAll(std::stop_token token)
{
    while (not token.stop_requested() and not finished(Direction::Forwards))
    {
        makeStep(Direction::Forwards);
    }
}

void Trajectory::replay(std::stop_token token)
{
    assert(viewerBridge_);

    using Clock = std::chrono::steady_clock;

    constexpr std::chrono::milliseconds tick{ 10 };

    auto previous{ Clock::now() };
    double pendingSteps{};
    Direction previousDirection{ Direction::None };

    while (not token.stop_requested())
    {
        const auto start{ Clock::now() };
        const auto elapsed{ std::chrono::duration<double>(start - previous).count() };
        previous = start;

        const auto rate{ viewerBridge_->replayRate() };

        if (rate == 0)
        {
            pendingSteps = 0;
            previousDirection = Direction::None;
            std::this_thread::sleep_until(start + tick);
            continue;
        }

        Direction direction{ rate > 0 ? Direction::Forwards : Direction::Backwards };

        if (direction != previousDirection)
        {
            pendingSteps = 0;
            previousDirection = direction;
        }

        pendingSteps += static_cast<double>(std::abs(rate)) * elapsed;
        const auto steps{ static_cast<std::size_t>(pendingSteps) };
        pendingSteps -= static_cast<double>(steps);

        for (std::size_t step{};
            step < steps and not token.stop_requested() and not finished(direction);
            ++step)
        {
            makeStep(direction);
        }

        if (finished(direction))
            pendingSteps = 0;

        std::this_thread::sleep_until(start + tick);
    }
}
