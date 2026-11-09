#include "pch.h"

#include "core.h"

#include "bridge.h"

#include "kmc/analysis_plan.h"
#include "kmc/simulation.h"
#include "lattice/lattice.h"
#include "lattice/path.h"
#include "lattice/site.h"

using namespace viewer;

namespace
{
    constexpr PackedState mask8{ 0xFF };
    constexpr PackedState mask16{ 0xFFFF };

    constexpr unsigned elementShift{ 0 }, coordinationShift{ 8 }, maxNeighborsShift{ 16 },
        layerShift{ 24 }, flagsShift{ 40 };
}

static_assert(
    std::atomic<PackedState>::is_always_lock_free,
    "Viewer Bridge requires lock-free 64-bit atomics.");

Bridge::Bridge(Lattice const& lattice, kmc::AnalysisPlan const& analysisPlan)
    : lattice_{ lattice },
      analysisPlan_{ analysisPlan },
      size_{ lattice.sites().size() },
      dirtyWordCount_{ (size_ + bitsPerWord - 1) / bitsPerWord },
      states_{ std::make_unique<std::atomic<PackedState>[]>(size_) },
      dirty_{ std::make_unique<std::atomic<std::uint64_t>[]>(dirtyWordCount_) },
      highlighted_{ std::make_unique<std::atomic_bool[]>(size_) }
{
    for (auto&& [index, site] : lattice.sites() | std::views::enumerate)
        states_[index].store(pack(capture(site)), std::memory_order_relaxed);
}

SiteState Bridge::capture(lattice::Site const& site) noexcept
{
    return { .element = site.element(),
        .coordination = site.coordination(),
        .maxNeighbors = site.max_neighbors(),
        .layer = site.layer(),
        .pristine = site.is_pristine() };
}

PackedState Bridge::pack(SiteState state) noexcept
{
    return (static_cast<PackedState>(static_cast<std::uint8_t>(state.element)) << elementShift) |
           (static_cast<PackedState>(state.coordination) << coordinationShift) |
           (static_cast<PackedState>(state.maxNeighbors) << maxNeighborsShift) |
           (static_cast<PackedState>(state.layer) << layerShift) |
           (static_cast<PackedState>(state.pristine) << flagsShift);
}

SiteState Bridge::unpack(PackedState packed) noexcept
{
    return { .element = static_cast<chemistry::Element>((packed >> elementShift) & mask8),
        .coordination = static_cast<lattice::Coordination>((packed >> coordinationShift) & mask8),
        .maxNeighbors = static_cast<lattice::Coordination>((packed >> maxNeighborsShift) & mask8),
        .layer = static_cast<lattice::Layer>((packed >> layerShift) & mask16),
        .pristine = static_cast<bool>((packed >> flagsShift) & mask8) };
}

void Bridge::signalUpdate(lattice::Index index) noexcept
{
    dirty_[index / bitsPerWord].fetch_or(
        std::uint64_t{ 1 } << (index % bitsPerWord),
        std::memory_order_release);
}

void Bridge::publish(lattice::Site const& site) noexcept
{
    const auto index{ site.index() };

    assert(index < size_);

    states_[index].store(pack(capture(site)), std::memory_order_relaxed);

    signalUpdate(index);
}

void Bridge::publishNeighborhood(lattice::Site const& site) noexcept
{
    for (auto neighbor : lattice_.neighbors(site))
        publish(*neighbor);
}

void Bridge::observe_processed(kmc::process::Record const& record)
{
    if (record.outcome != kmc::process::Outcome::accepted)
        return;

    if (&record.path.front() == &record.path.back())
    {
        auto& site{ record.path.front() };

        publish(site);
        publishNeighborhood(site);
    }
    else
    {
        auto& path{ record.path };

        swapHighlight(path.front().index(), path.back().index());

        publishNeighborhood(path.front());
        publishNeighborhood(path.back());
    }
}

SiteState Bridge::state(lattice::Index index) const noexcept
{
    assert(index < size_);

    auto state{ unpack(states_[index].load(std::memory_order_acquire)) };

    state.highlighted = isHighlighted(index);

    return state;
}

void Bridge::setHighlighted(lattice::Index index, bool highlighted) noexcept
{
    assert(index < size_);

    const auto previous{ highlighted_[index].exchange(highlighted, std::memory_order_acq_rel) };

    if (previous != highlighted)
        signalUpdate(index);
}

bool Bridge::swapHighlight(lattice::Index first, lattice::Index second) noexcept
{
    assert(first < size_);
    assert(second < size_);
    assert(first != second);

    const auto firstHighlighted{ highlighted_[first].load(std::memory_order_acquire) };
    const auto secondHighlighted{ highlighted_[second].load(std::memory_order_acquire) };

    if (firstHighlighted == secondHighlighted)
        return false;

    highlighted_[first].store(secondHighlighted, std::memory_order_release);
    highlighted_[second].store(firstHighlighted, std::memory_order_release);

    signalUpdate(first);
    signalUpdate(second);

    return true;
}

gui::Status Bridge::capture(kmc::Simulation const& simulation) noexcept
{
    return {
        .step = simulation.step(),
        .rejectionFraction = simulation.rejection_fraction(),
        .physicalTime = simulation.time(),
        .runTime = simulation.run_duration(),
        .temperature = simulation.model()->temperature(),
    };
}

void Bridge::take_snapshot(kmc::Simulation const& simulation) { updateGUI(simulation, true); }

void Bridge::observe_completed(kmc::Simulation const& simulation) { updateGUI(simulation, false); }

void Bridge::updateGUI(kmc::Simulation const& simulation, bool appendSample)
{
    auto status{ capture(simulation) };

    gui::Sample sample{ .snapshot = {
                            .step = simulation.step(),
                            .time = simulation.time(),
                        } };

    for (auto& observer : analysisPlan_)
    {
        status.setData(*observer);

        if (appendSample)
            sample.appendData(*observer);
    }
    {
        std::scoped_lock lock{ guiMutex_ };

        guiUpdateRequired_ = true;
        guiStatus_ = std::move(status);

        if (appendSample)
            pendingSamples_.emplace_back(std::move(sample));
    }
}

void Bridge::consumeGui(gui::Status& status, std::vector<gui::Sample>& samples)
{
    if (guiUpdateRequired_)
    {
        std::scoped_lock lock{ guiMutex_ };

        status = std::move(guiStatus_);

        samples.clear();
        samples.swap(pendingSamples_);

        guiUpdateRequired_ = false;
    }
}
