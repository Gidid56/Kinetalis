#pragma once
#include "simulation/base.h"
#include "model.h"
#include "event.h"
#include "rate_tree.h"
#include "setup.h"

#include <stop_token>

class kmc::Simulation : public simulation::Base
{
public:
    Simulation(
        lattice::Generator const& make_lattice,
        model::Generator const& make_model,
        std::optional<simulation::Seed> seed)
        : simulation::Base{ make_lattice, seed },
          model_{ make_model() }
    {
        //model_.update();
    }

    virtual std::string output_settings() const = 0;

    virtual simulation::Status run(AnalysisPlan&, std::stop_token = {}) = 0;

    double rejection_fraction() const
    {
        auto const selections{ step_ + rejections_ };
        return selections == 0 ? 0.0 : static_cast<double>(rejections_) / selections;
    }

    auto time() const { return time_; }
    auto& model() const { return model_; }

    auto& class_rates() const { return class_rates_; }
    auto& class_counts() const { return class_counts_; }
    bool class_initialized(RateClassKey key) const { return class_initialized_.at(key); }

protected:
    std::unique_ptr<kmc::Model> model_;

    Time time_{};
    std::size_t rejections_{};

    Rates class_rates_{ Rate{} };
    std::vector<bool> class_initialized_{ false };
    std::vector<std::size_t> class_counts_{ 0 };
};

template <class Rules>
class kmc::RejectionTreeSimulation final : public Simulation
{
public:
    explicit RejectionTreeSimulation(Setup<Rules> const& setup)
        : Simulation{ setup.make_lattice, setup.make_model, setup.seed },
          rules_{ setup.rules }
    {
        initialize_rates();
        update_all_rates();
    }

    simulation::Status run(AnalysisPlan&, std::stop_token = {}) override;

    std::string output_settings() const override { return rules_.settings(); }

    auto& rate_tree() const { return rate_tree_; }

private:
    Rate block_rate(lattice::Site&);
    RateClassKey class_key(event::Type, event::Location const&);
    void set_class_key(lattice::Index, RateClassKey);

    Rate rate_sum(lattice::Site&);
    event::Location select_random_location();
    event::Record evaluate_event(event::Location) const;

    void initialize_rates();
    void update_all_rates();
    void update_affected_rates(event::Record const&);

    void update(lattice::Site&);

    template <class Visitor>
    void visit_neighbor_tree(lattice::Site&, std::size_t radius, Visitor&);

    std::unique_ptr<rate_tree::Interface> rate_tree_;
    Rates site_rates_;
    RateClassKeys event_class_keys_;
    std::vector<rate_tree::BlockUpdate> block_updates_;

    [[no_unique_address]] Rules rules_;
};

#include "simulation_impl.hpp"
