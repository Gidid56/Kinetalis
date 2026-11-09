#pragma once
#include "kmc/observer.h"
#include "sample.h"
#include "states.h"

class viewer::Bridge final : public kmc::Observer
{
public:
    explicit Bridge(Lattice const&, kmc::AnalysisPlan const&);

    void observe_processed(kmc::process::Record const&) override;
    void take_snapshot(kmc::Simulation const&) override;
    void observe_completed(kmc::Simulation const&) override;

    int replayRate() const { return replayRate_.load(std::memory_order_relaxed); }

    SiteState state(lattice::Index) const noexcept;

    template <class Function>
    bool consume(Function&& function)
    {
        bool changed{};

        for (std::size_t word{}; word < dirtyWordCount_; ++word)
        {
            if (dirty_[word].load(std::memory_order_acquire) == 0)
                continue;

            auto bits{ dirty_[word].exchange(0, std::memory_order_acquire) };

            while (bits != 0)
            {
                const auto bit{ static_cast<std::size_t>(std::countr_zero(bits)) };
                const auto index{ word * bitsPerWord + bit };

                if (index < size_)
                    std::invoke(
                        function,
                        static_cast<lattice::Index>(index),
                        state(static_cast<lattice::Index>(index)));

                bits &= bits - 1;
                changed = true;
            }
        }

        return changed;
    }

    void setHighlighted(lattice::Index index, bool highlighted) noexcept;
    bool swapHighlight(lattice::Index first, lattice::Index second) noexcept;

    void toggleHighlighted(lattice::Index index) noexcept
    {
        setHighlighted(index, not isHighlighted(index));
    }
    void removeHighlight(lattice::Index index) noexcept { setHighlighted(index, false); }
    bool isHighlighted(lattice::Index index) const noexcept
    {
        return highlighted_[index].load(std::memory_order_acquire);
    }

    void consumeGui(gui::Status& status, std::vector<gui::Sample>& samples);

    void requestTask(Task task) noexcept { taskRequest_.store(task, std::memory_order_release); }

    std::optional<Task> consumeTaskRequest() noexcept
    {
        const auto request{ taskRequest_.exchange(Task::None, std::memory_order_acq_rel) };
        if (request == Task::None)
            return std::nullopt;
        else
            return request;
    }

    void publishTask(Task task) noexcept { task_.store(task, std::memory_order_release); }

    Task task() const noexcept { return task_.load(std::memory_order_acquire); }

    void setReplayRate(int rate) noexcept { replayRate_.store(rate, std::memory_order_relaxed); }

private:
    void updateGUI(kmc::Simulation const&, bool appendSample);
    void signalUpdate(lattice::Index index) noexcept;
    void publish(lattice::Site const&) noexcept;
    void publishNeighborhood(lattice::Site const&) noexcept;

    static PackedState pack(SiteState) noexcept;
    static SiteState unpack(PackedState) noexcept;
    static SiteState capture(lattice::Site const&) noexcept;
    static gui::Status capture(kmc::Simulation const&) noexcept;

    static constexpr std::size_t bitsPerWord{ std::numeric_limits<std::uint64_t>::digits };

    lattice::Lattice const& lattice_;
    kmc::AnalysisPlan const& analysisPlan_;

    std::size_t size_{};
    std::size_t dirtyWordCount_{};

    std::unique_ptr<std::atomic<PackedState>[]> states_;
    std::unique_ptr<std::atomic<std::uint64_t>[]> dirty_;

    std::unique_ptr<std::atomic_bool[]> highlighted_;

    mutable std::mutex guiMutex_;
    gui::Status guiStatus_{};
    std::vector<gui::Sample> pendingSamples_;
    std::atomic<bool> guiUpdateRequired_{};

    std::atomic<Task> task_{ Task::Pause }, taskRequest_{ Task::None };
    std::atomic<int> replayRate_{};
};