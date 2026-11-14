#pragma once
#include "basics.h"
#include "lattice/types.h"

class output::Phases : public output::Writer
{
public:
    enum class Phase
    {
        pore,
        solid,
        pristine,
        silver,
    };

    using Count = std::size_t;

    static constexpr std::array active_phases{
        Phase::pore,
        Phase::solid,
        Phase::pristine,
        Phase::silver,
    };

    struct Data : public kmc::Observer::Snapshot
    {
        std::map<Phase, Fraction> fractions{};
        std::map<Phase, Length> correlation_lengths{};
    };

    static constexpr lattice::Coordination max_coordination{ 12 };

    using Writer::Writer;

    static bool is_phase(lattice::Site const&, Phase);

    void capture_init(kmc::Simulation const&) override;
    void capture_snapshot(kmc::Simulation const&) override;

    void observe_selected(kmc::process::Record const&) override;
    void observe_processed(kmc::process::Record const&) override;

    void write_init(kmc::Simulation const&) override;
    void write_snapshot(kmc::Simulation const&) override;

    auto count(Phase const phase) const { return counts_.at(phase); }
    auto fraction(Phase phase) const { return last_data_.fractions.at(phase); }
    auto correlation_length(Phase phase) const { return last_data_.correlation_lengths.at(phase); }
    auto& lattice() const { return *lattice_; }

    auto& last_data() const { return last_data_; }

protected:
    Phases::Data last_data_{};

private:
    std::string_view name() const override { return "phases"; }
    std::string settings() const override;

    Count phase_neighbor_count(lattice::Site const&, Phase) const;

    Length calculate_correlation_length(Phase phase);

    lattice::Lattice const* lattice_{};

    std::map<Phase, Count> counts_, correlation_;

    lattice::Distance next_neighbor_distance_{};
    Area outer_surface_area_{};
    std::size_t site_count_{};

    // TODO by layers
};
