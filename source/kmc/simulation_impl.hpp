#pragma once
// Template definitions for RejectionTreeSimulation; included by simulation.h.

#include "analysis_plan.h"
#include "lattice/lattice.h"
#include "utility/utility.h"

namespace kmc
{
    template <class ModelT>
    void RejectionTreeSimulation<ModelT>::initialize_rates()
    {
        rate_tree_ = rate_tree::make_entry(lattice_.sites().size());
        site_rates_.resize(lattice_.sites().size());
        slot_class_keys_.resize(lattice_.sites().size() * ModelT::slots_per_site);

        // Each model decides whether to preallocate or register classes lazily.
        kinetic_classes_.resize(model_.kinetics().number_keys());
    }

    template <class ModelT>
    simulation::Status
    RejectionTreeSimulation<ModelT>::run(AnalysisPlan& analysis_plan, std::stop_token token)
    {
        utility::Guard guard{ timer_ };

        if (status_ == simulation::Status::created)
            analysis_plan.observe_started(*this);

        status_ = simulation::Status::running;

        while (not analysis_plan.stop_requested())
        {
            if (token.stop_requested())
                return status_ = simulation::Status::paused;

            const auto total_rate{ rate_tree_->rate_sum() };
            if (total_rate <= Rate{})
            {
                analysis_plan.observe_completed(*this);
                return status_ = simulation::Status::exhausted;
            }

            const auto escape_time{ 1.0 / total_rate };
            time_ += -std::log1p(-rng_.probability()) * escape_time;

            auto const selection{ select_event_slot(total_rate) };
            auto record{ model_.processes().make_record(
                selection.slot,
                lattice_.sites().at(selection.source),
                lattice_,
                selection.class_key) };

            analysis_plan.observe_selected(record);

            if (record.outcome == event::Outcome::accepted)
            {
                ++step_;

                model_.processes().execute(selection.slot, record.location(), lattice_);

                update_affected_rates(record);

                analysis_plan.observe_processed(record);
            }
            else if (record.outcome == event::Outcome::rejected)
            {
                ++rejections_;
            }

            analysis_plan.observe_step(*this, step_);
        }

        analysis_plan.observe_completed(*this);
        return status_ = simulation::Status::analysed;
    }

    template <class ModelT>
    typename RejectionTreeSimulation<ModelT>::Selection
    RejectionTreeSimulation<ModelT>::select_event_slot(Rate total_rate)
    {
        auto selected_rate{ physics::ensure_non_negative(total_rate * rng_.probability()) };
        auto site_index{ rate_tree_->index(selected_rate) };

        while (selected_rate >= site_rates_.at(site_index))
            selected_rate -= site_rates_.at(site_index++);

        const auto first{ static_cast<std::size_t>(site_index) * ModelT::slots_per_site };
        auto offset{ first };
        while (selected_rate >= kinetic_classes_.at(slot_class_keys_.at(offset)).rate)
            selected_rate -= kinetic_classes_.at(slot_class_keys_.at(offset++)).rate;

        if (offset >= first + ModelT::slots_per_site)
            throw std::runtime_error{ "Selected event slot is outside source-site layout." };

        return { site_index, offset - first, slot_class_keys_.at(offset) };
    }

    template <class ModelT>
    void RejectionTreeSimulation<ModelT>::update_affected_rates(event::Record const& record)
    {
        affected_sites_.clear();
        block_updates_.clear();

        for (auto* site : record.sites())
            collect_affected_sites<ModelT::KineticsType::invalidation_hops>(*site);

        for (auto* site : affected_sites_)
            update_site_rate(*site);

        std::ranges::sort(block_updates_, {}, &rate_tree::BlockUpdate::index);
        for (auto& update : block_updates_)
        {
            auto& block_site{ lattice_.sites().at(update.index) };
            update.rate = block_rate(block_site);
            block_site.set_block_update_queued(false);
        }
        rate_tree_->update_rates(block_updates_);
    }

    template <class ModelT>
    template <std::size_t RemainingHops>
    void RejectionTreeSimulation<ModelT>::collect_affected_sites(lattice::Site& site)
    {
        // Traverse neighboring sites without allocating a visited-site container.
        // Repeated paths are allowed; the site-update queue prevents duplicate collection.
        if constexpr (RemainingHops > 0)
            for (auto* neighbor : lattice_.neighbors(site))
                collect_affected_sites<RemainingHops - 1>(*neighbor);

        if (site.is_site_update_queued())
            return;

        if (not site.is_source_eligible() and not model_.processes().is_candidate(site))
            return;

        site.set_site_update_queued(true);
        affected_sites_.push_back(&site);
    }

    template <class ModelT>
    void RejectionTreeSimulation<ModelT>::update_site_rate(lattice::Site& site)
    {
        if (not site.is_site_update_queued())
            return;

        auto& old_rate{ site_rates_.at(site.index()) };

        Rate new_rate{ update_site_classes(site) };

        if (new_rate != old_rate)
        {
            old_rate = new_rate;

            constexpr lattice::Index block_mask{ (lattice::Index{ 1 } << rate_tree::baseLevel) -
                                                 1 };
            auto const block_start{ site.index() & ~block_mask };

            auto& block_site{ lattice_.sites().at(block_start) };
            if (not block_site.is_block_update_queued())
            {
                block_site.set_block_update_queued(true);
                block_updates_.push_back({ block_start, {} });
            }
        }
        site.set_site_update_queued(false);
    }

    template <class ModelT>
    Rate RejectionTreeSimulation<ModelT>::update_site_classes(lattice::Site& site)
    {
        const auto first{ static_cast<std::size_t>(site.index()) * ModelT::slots_per_site };

        Rate total_rate{};
        bool is_candidate{};

        model_.processes().for_each([&](auto const& process, std::size_t offset) {
            const bool eligible{ process.is_candidate(site) };
            is_candidate |= eligible;
            total_rate += update_process_slots(process, site, first + offset, eligible);
        });

        site.set_source_eligible(is_candidate);
        return total_rate;
    }

    template <class ModelT>
    template <Process P>
    Rate RejectionTreeSimulation<ModelT>::update_process_slots(
        P const& process, lattice::Site& site, std::size_t first, bool eligible)
    {
        if (not eligible)
        {
            // A site with no previously eligible source processes has no cached event slots.
            if (site.is_source_eligible())
                for (SlotId slot{}; slot < P::slot_count; ++slot)
                    set_class_key(first + slot, RateClassKey{});
            return {};
        }

        Rate total_rate{};
        for (SlotId slot{}; slot < P::slot_count; ++slot)
        {
            const auto key{ classify_process_slot(process, site, slot) };
            set_class_key(first + slot, key);

            if (key != RateClassKey{})
                total_rate += kinetic_classes_.at(key).rate;
        }
        return total_rate;
    }

    template <class ModelT>
    template <Process P>
    RateClassKey RejectionTreeSimulation<ModelT>::classify_process_slot(
        P const& process, lattice::Site& site, SlotId slot)
    {
        if (not kmc::has_slot(process, site, slot))
            return {};

        auto location{ kmc::make_location(process, site, slot, lattice_) };
        assert(location.sites().size() == P::cluster_size);

        if (not kmc::is_location_candidate(process, location))
            return {};

        return register_class(P::event, location);
    }

    template <class ModelT>
    void RejectionTreeSimulation<ModelT>::set_class_key(std::size_t index, RateClassKey new_key)
    {
        auto& old_key{ slot_class_keys_.at(index) };

        if (old_key == new_key)
            return;

        // Key zero denotes an inactive event slot.
        if (old_key != RateClassKey{})
            --kinetic_classes_.at(old_key).count;

        old_key = new_key;

        if (new_key != RateClassKey{})
            ++kinetic_classes_.at(new_key).count;
    }

    template <class ModelT>
    Rate RejectionTreeSimulation<ModelT>::block_rate(lattice::Site& site)
    {
        constexpr auto block_size{ std::size_t{ 1 } << rate_tree::baseLevel };

        auto const block_start{ site.index() & ~(block_size - 1) };

        Rate sum{};

        for (auto index{ block_start };
            index < std::min(block_start + block_size, site_rates_.size());
            ++index)
        {
            sum += site_rates_.at(index);
        }
        return sum;
    }

    template <class ModelT>
    RateClassKey RejectionTreeSimulation<ModelT>::register_class(
        event::Type type,
        event::Location const& location)
    {
        const auto key{ model_.kinetics().classify(type, location, lattice_) };
        if (key == RateClassKey{})
            throw std::logic_error{ "Active event maps to reserved key zero." };

        if (key >= kinetic_classes_.size())
            kinetic_classes_.resize(static_cast<std::size_t>(key) + 1);

        auto& kinetic_class{ kinetic_classes_[key] };
        if (not kinetic_class.is_initialized())
        {
            auto const class_type{ model_.kinetics().class_event(key) };
            if (class_type == event::Type::none)
                throw std::logic_error{ "An active kinetic class cannot have event type none." };

            kinetic_class.rate = model_.kinetics().rate(key);
            kinetic_class.type = class_type;
        }

        return key;
    }

    template <class ModelT>
    void RejectionTreeSimulation<ModelT>::update_all_rates()
    {
        model_.kinetics().refresh_rates();
        for (RateClassKey key{ 1 }; key < kinetic_classes_.size(); ++key)
            if (kinetic_classes_[key].is_initialized())
                kinetic_classes_[key].rate = model_.kinetics().rate(key);

        // A full-rate refresh bypasses per-event block collection. Rebuild each
        // block once after all site rates have been recalculated.
        block_updates_.clear();
        for (auto& site : lattice_.sites())
        {
            site_rates_.at(site.index()) = update_site_classes(site);
            site.set_site_update_queued(false);
            site.set_block_update_queued(false);
        }

        constexpr auto block_size{ std::size_t{ 1 } << rate_tree::baseLevel };
        for (lattice::Index index{}; index < site_rates_.size(); index += block_size)
            rate_tree_->update_rate(index, block_rate(lattice_.sites().at(index)));
    }
}
