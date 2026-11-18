#pragma once
#include "simulation/base.h"
#include "model.h"
#include "event.h"
#include "rate_tree.h"
#include "setup.h"

#include <stop_token>

namespace kmc
{
    struct KineticClass
    {
        Rate rate{};
        std::size_t count{};
        event::Type type{ event::Type::none };

        bool is_initialized() const noexcept { return type != event::Type::none; }
        bool operator==(KineticClass const&) const = default;
    };
}

class kmc::Simulation : public simulation::Base
{
public:
    Simulation(lattice::Generator const& make_lattice, std::optional<simulation::Seed> seed)
        : simulation::Base{ make_lattice, seed }
    { }

    virtual std::string transformation_settings() const = 0;
    virtual std::string kinetics_settings() const = 0;
    virtual Temperature temperature() const = 0;
    virtual simulation::Status run(AnalysisPlan&, std::stop_token = {}) = 0;

    double rejection_fraction() const
    {
        auto const selections{ step_ + rejections_ };
        return selections == 0 ? 0.0 : static_cast<double>(rejections_) / selections;
    }

    auto time() const { return time_; }
    auto const& classes() const noexcept { return classes_; }

protected:
    Time time_{};
    std::size_t rejections_{};

    std::vector<KineticClass> classes_{ KineticClass{} };
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
    std::string transformation_settings() const override { return model_.transformations().settings(); }
    std::string kinetics_settings() const override { return model_.kinetics().output_text(); }
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
    RateClassKey register_class(event::Type, event::Location const&);
    void set_class_key(std::size_t, RateClassKey);

    Rate update_site_classes(lattice::Site&);
    Selection select_event_slot(Rate total_rate);
    event::Record evaluate_event(Selection const&);

    void initialize_rates();
    void update_all_rates();
    void update_affected_rates(event::Record const&);
    void update_site_rate(lattice::Site&);

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
