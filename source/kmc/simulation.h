#pragma once
#include "simulation/base.h"
#include "model.h"
#include "process.h"
#include "rate_tree.h"
#include "setup.h"

class kmc::Simulation : public simulation::Base
{
public:
    explicit Simulation(
        lattice::Generator const& make_lattice,
        model::Generator const& make_model,
        std::optional<random_::Engine::seed_type> seed);

    virtual simulation::Status run(AnalysisPlan&, std::stop_token = {}) = 0;

    void update_dynamic_rates();

    double rejection_fraction() const;

    auto time() const { return time_; }
    auto& model() const { return model_; }

    auto& process_rates() const { return process_rates_; }
    auto& process_counts() const { return process_counts_; }

protected:
    std::unique_ptr<kmc::Model> model_;

    Time time_{};
    std::size_t rejections_{};

    std::vector<std::pair<Process, Rate>> process_rates_{ { Process{}, Rate{} } };
    std::vector<std::size_t> process_counts_{ 0 };
};

namespace kmc
{
    template <class Rules>
    class KmcSimulation;
}

template <class Rules>
class kmc::KmcSimulation final : public Simulation
{
public:
    explicit KmcSimulation(Setup<Rules> const& setup)
        : Simulation{ setup.make_lattice, setup.make_model, setup.seed },
          rules_{ setup.rules }
    {
        initialize_rates();
        update_rates();
    }

    simulation::Status run(AnalysisPlan&, std::stop_token = {}) override;

    auto& rate_tree() const { return rate_tree_; }

private:
    Rate block_rate(lattice::Site&);
    process::Key process_key(Process);
    void set_process(lattice::Index, process::Key);
    void clear_processes(lattice::Site&);

    Rate rate_sum(lattice::Site&);
    process::Record select_random_process();

    void initialize_rates();
    void update_rates();
    void update_static_rates();

    void update(lattice::Site&);
    void update_neighbors(lattice::Site&);

    std::unique_ptr<rate_tree::Interface> rate_tree_;
    Rates site_rates_;
    process::Keys processes_;
    std::unordered_map<Process, process::Key> dynamic_keys_;

    [[no_unique_address]] Rules rules_;
};

template <>
struct std::formatter<utility::Timer> : public std::formatter<physics::quantity::Duration>
{
    auto format(utility::Timer const& timer, std::format_context& context) const
    {
        using namespace physics::unit::literals;

        physics::quantity::Duration const elapsed{
            1_ns * std::chrono::duration_cast<std::chrono::nanoseconds>(timer.duration()).count()
        };

        return std::formatter<physics::quantity::Duration>::format(elapsed, context);
    }
};
