#pragma once
#include "lattice/lattice.h"
#include "model.h"
#include "process.h"
#include "random/engine.h"
#include "rate_tree.h"
#include "setup.h"

class kmc::Simulation
{
public:
    enum class Status
    {
        created,
        paused,
        running,
        exhausted,
        analysed,
    };

    Simulation(Simulation const&) = delete;

    Simulation(Simulation&&) = delete;
    Simulation& operator=(Simulation&&) = delete;

    virtual ~Simulation() = default;

    virtual Status run(AnalysisPlan&, std::stop_token = {}) = 0;


    Rate block_rate(lattice::Site&);

    void set_process(lattice::Index, process::Key);
    void clear_processes(lattice::Site&);
    process::Key process_key(Process);

    void update_dynamic_rates();

    double rejection_fraction() const;
    double tracked_fraction() const;

    Duration run_duration() const;
    Duration tracked_time() const;

    auto& mutable_lattice() { return lattice_; }
    auto& lattice() const { return lattice_; }
    auto& model() const { return model_; }

    auto step() const { return step_; }
    auto time() const { return time_; }

    auto& rate_tree() const { return rate_tree_; }
    auto& process_rates() const { return process_rates_; }
    auto& process_counts() const { return process_counts_; }

    auto& rng() { return rng_; }
    auto& rng() const { return rng_; }

protected:
    explicit Simulation(
        lattice::Generator const& make_lattice,
        model::Generator const& make_model,
        std::optional<random_::Engine::seed_type> seed);

    lattice::Lattice lattice_;
    std::unique_ptr<kmc::Model> model_;
    random_::Engine rng_;

    Step step_{}, rejections_{};
    Time time_{};
    std::atomic<Status> status_{};

    std::unique_ptr<rate_tree::Interface> rate_tree_;
    Rates site_rates_;
    process::Keys processes_;
    std::vector<std::pair<Process, Rate>> process_rates_;
    std::vector<std::size_t> process_counts_;
    std::unordered_map<Process, process::Key> dynamic_keys_;

    static constexpr lattice::Site* reservoir{ nullptr };

    utility::Timer timer_{}, tracked_{};
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
        update_rates();
    }

    Status run(AnalysisPlan&, std::stop_token = {}) override;

private:
    Rate rate_sum(lattice::Site&);
    process::Record select_random_process();

    void update(lattice::Site&);
    void update_neighbors(lattice::Site&);

    void update_rates();
    void update_static_rates();

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
