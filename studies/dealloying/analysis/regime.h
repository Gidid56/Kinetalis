#pragma once
#include "output/phases.h"
#include "output/surface.h"
#include "output/event_flux.h"

namespace studies::dealloying::analysis { class Regime; }

class studies::dealloying::analysis::Regime : public output::Writer
{
public:
    enum class Condition : std::uint8_t
    {
        none = 0,
        bifurcated = 1 << 0,
        porous = 1 << 1,
        coarsening = 1 << 2,
        passivated = 1 << 3,
        arrested = 1 << 4,
    };

    explicit Regime(std::filesystem::path const& root = {})
        : Writer{ root },
          phases_{ root },
          surface_{ root }
    {}

    auto& set_save_lattice(bool enabled)
    {
        save_lattice_ = enabled;
        return *this;
    }

    auto& set_stop_conditions(Condition conditions)
    {
        stop_conditions_ = conditions;
        marker_conditions_ |= conditions;
        return *this;
    }

    auto& set_marker_conditions(Condition conditions)
    {
        marker_conditions_ |= conditions;
        return *this;
    }

    auto& set_lookback_period(Condition condition, Fraction fraction)
    {
        lookback_periods_[condition] = fraction;
        return *this;
    }

    auto& set_threshold(Condition condition, Factor threshold)
    {
        thresholds_[condition] = threshold;
        return *this;
    }

    auto reached_conditions() const { return reached_conditions_; }
    auto& snapshots() const { return snapshots_; }

    bool has_reached(Condition conditions) const
    {
        return (reached_conditions_ & conditions) != Condition::none;
    }

    bool stop_requested() const override { return has_reached(stop_conditions_); }

    auto& phases() const { return phases_; }
    auto& surface() const { return surface_; }
    auto& event_flux() const { return event_flux_; }

private:
    std::string_view name() const override { return "regime"; }

    void capture_init(kmc::Simulation const&) override;
    void observe_selected(kmc::process::Record const&) override;
    void observe_processed(kmc::process::Record const&) override;
    void capture_snapshot(kmc::Simulation const&) override;
    void capture_final(kmc::Simulation const&) override;

    void write_init(kmc::Simulation const&) override;
    void write_snapshot(kmc::Simulation const&) override;
    void write_final(kmc::Simulation const&) override;

    auto lookback_start_step(Condition) const;
    bool has_lookback(Condition) const;
    auto lookback(Condition) const;
    void record(Condition);

    bool meets(Condition) const;
    bool is_bifurcated() const;
    bool is_porous() const;
    bool is_coarsening() const;
    bool is_passivated() const;
    bool is_arrested() const;

    Fraction master_alloy_fraction_{};
    Volume lattice_volume_{};

    double max_surface_excess_{};

    output::Phases phases_;
    output::Surface surface_;
    output::process::EventFlux event_flux_;

    Condition stop_conditions_{}, marker_conditions_{},
        observer_conditions_{}, reached_conditions_{};

    std::map<Condition, output::Phases::Data> snapshots_;

    std::deque<output::Phases::Data> history_;
    std::map<Condition, Fraction> lookback_periods_{};
    std::map<Condition, Factor> thresholds_{};

    bool save_lattice_{};

    //std::map<lattice::Phase, std::vector<std::size_t>> phasesByLayers_;
};
