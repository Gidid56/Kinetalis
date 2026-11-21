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

    virtual std::string process_settings() const = 0;
    virtual std::string kinetics_settings() const = 0;
    virtual Temperature temperature() const = 0;
    virtual simulation::Status run(AnalysisPlan&, std::stop_token = {}) = 0;

    double rejection_fraction() const
    {
        auto const selections{ step_ + rejections_ };
        return selections == 0 ? 0.0 : static_cast<double>(rejections_) / selections;
    }

    auto time() const { return time_; }
    virtual Kinetics const& kinetics() const noexcept = 0;
    auto const& class_counts() const noexcept { return class_counts_; }
    bool class_counts_enabled() const noexcept { return class_counts_enabled_; }

protected:
    Time time_{};
    std::size_t rejections_{};

    std::vector<std::size_t> class_counts_;
    bool class_counts_enabled_{};
};

template <class ModelT>
class kmc::RejectionTreeSimulation final : public Simulation
{
public:
    explicit RejectionTreeSimulation(Setup<ModelT> const&);

    simulation::Status run(AnalysisPlan&, std::stop_token = {}) override;
    std::string process_settings() const override { return model_.processes().settings(); }
    std::string kinetics_settings() const override { return model_.kinetics().output_text(); }
    Temperature temperature() const override { return model_.kinetics().temperature(); }
    Kinetics const& kinetics() const noexcept override { return model_.kinetics(); }

    auto& model(this auto& self) noexcept { return self.model_; }
    auto& rate_tree() const { return rate_tree_; }
    auto const& slot_class_keys() const noexcept { return slot_class_keys_; }
    // Class populations modified by the most recent accepted event.
    // Keys may repeat; consumers should process the final population once.
    auto const& changed_class_keys() const noexcept { return changed_class_keys_; }

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
    void configure_class_counts(bool);

    Rate update_site_classes(lattice::Site&);

    template <Process P>
    Rate update_process_slots(P const&, lattice::Site&, std::size_t first, bool eligible);

    template <Process P>
    RateClassKey classify_process_slot(P const&, lattice::Site&, SlotId);

    Selection select_event_slot(Rate total_rate);

    void update_all_rates();
    void update_affected_rates(event::Record const&);
    void update_site_rate(lattice::Site&);

    template <std::size_t RemainingHops>
    void collect_affected_sites(lattice::Site&);

    ModelT model_;
    std::unique_ptr<rate_tree::Interface> rate_tree_;
    Rates site_rates_;
    RateClassKeys slot_class_keys_;
    std::vector<RateClassKey> changed_class_keys_;
    std::vector<rate_tree::BlockUpdate> block_updates_;
    std::vector<lattice::Site*> affected_sites_;
};

#include "simulation_impl.hpp"
