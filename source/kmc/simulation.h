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
    Simulation(lattice::Generator const& make_lattice, std::optional<simulation::Seed> seed)
        : simulation::Base{ make_lattice, seed }
    { }

    virtual std::string output_settings() const = 0;
    virtual std::string model_settings() const = 0;
    virtual Temperature temperature() const = 0;
    virtual simulation::Status run(AnalysisPlan&, std::stop_token = {}) = 0;

    double rejection_fraction() const
    {
        auto const selections{ step_ + rejections_ };
        return selections == 0 ? 0.0 : static_cast<double>(rejections_) / selections;
    }

    auto time() const { return time_; }
    auto& class_rates() const { return class_rates_; }
    auto& class_counts() const { return class_counts_; }
    bool class_initialized(RateClassKey key) const { return class_initialized_.at(key); }
    event::Type class_event(RateClassKey key) const { return class_events_.at(key); }

protected:
    Time time_{};
    std::size_t rejections_{};

    Rates class_rates_{ Rate{} };
    std::vector<bool> class_initialized_{ false };
    std::vector<std::size_t> class_counts_{ 0 };
    std::vector<event::Type> class_events_{ event::Type::none };
};

template <class ModelType>
class kmc::RejectionTreeSimulation final : public Simulation
{
public:
    explicit RejectionTreeSimulation(Setup<ModelType> const& setup)
        : Simulation{ setup.make_lattice, setup.seed },
          model_{ setup.model }
    {
        initialize_rates();
        update_all_rates();
    }

    simulation::Status run(AnalysisPlan&, std::stop_token = {}) override;
    std::string output_settings() const override { return model_.transformations().settings(); }
    std::string model_settings() const override { return model_.kinetics().output_text(); }
    Temperature temperature() const override { return model_.kinetics().temperature(); }

    auto& model(this auto& self) noexcept { return self.model_; }
    auto& rate_tree() const { return rate_tree_; }
    auto const& event_class_keys() const noexcept { return event_class_keys_; }

private:
    struct Selection
    {
        lattice::Index source;
        SlotId slot;
        RateClassKey class_key;
    };

    Rate block_rate(lattice::Site&);
    RateClassKey class_key(event::Type, event::Location const&);
    void set_class_key(std::size_t, RateClassKey);

    Rate rate_sum(lattice::Site&);
    Selection select_random_location(Rate total_rate);
    event::Record evaluate_event(Selection const&);

    void initialize_rates();
    void update_all_rates();
    void update_affected_rates(event::Record const&);
    void update(lattice::Site&);

    template <class Visitor>
    void visit_neighbor_tree(lattice::Site&, std::size_t radius, Visitor&);

    ModelType model_;
    std::unique_ptr<rate_tree::Interface> rate_tree_;
    Rates site_rates_;
    RateClassKeys event_class_keys_;
    std::vector<rate_tree::BlockUpdate> block_updates_;
    std::vector<lattice::Site*> affected_sites_;
};

#include "simulation_impl.hpp"
