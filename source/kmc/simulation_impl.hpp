#pragma once
// Template definitions for RejectionTreeSimulation; included by simulation.h.

#include "analysis_plan.h"
#include "lattice/lattice.h"
#include "utility/utility.h"

namespace kmc
{
    template <class ModelType>
    void RejectionTreeSimulation<ModelType>::initialize_rates()
    {
        rate_tree_ = rate_tree::make_entry(lattice_.sites().size());
        site_rates_.resize(lattice_.sites().size());
        event_class_keys_.resize(lattice_.sites().size() * ModelType::slots_per_site);

        // Each model decides whether to preallocate or register classes lazily.
        class_rates_.resize(model_.kinetics().number_keys());
        class_initialized_.resize(model_.kinetics().number_keys());
        class_counts_.resize(model_.kinetics().number_keys());
        class_events_.resize(model_.kinetics().number_keys(), event::Type::none);
    }

    template <class ModelType>
    simulation::Status
    RejectionTreeSimulation<ModelType>::run(AnalysisPlan& analysis_plan, std::stop_token token)
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

            auto const selection{ select_random_location(total_rate) };
            auto record{ evaluate_event(selection) };

            analysis_plan.observe_selected(record);

            if (record.outcome == event::Outcome::accepted)
            {
                ++step_;

                model_.transformations().visit(selection.slot,
                    [&](auto const& transformation, SlotId) {
                        transformation.execute(record.location(), lattice_);
                    });

                update_affected_rates(record);

                analysis_plan.observe_processed(record);
            }
            else if (record.outcome == event::Outcome::rejected)
            {
                ++rejections_;
            }

            analysis_plan.observe_step(*this);
        }

        analysis_plan.observe_completed(*this);
        return status_ = simulation::Status::analysed;
    }

    template <class ModelType>
    void RejectionTreeSimulation<ModelType>::update_affected_rates(event::Record const& record)
    {
        block_updates_.clear();

        // Model dependencies can extend beyond the nearest neighbors;
        // the current event rules require at least one neighbor shell.
        const auto radius{ std::max<std::size_t>(1, model_.kinetics().invalidation_radius()) };

        // Mark first, then recompute: every source must see the completed
        // lattice transformation. Deduplicate overlapping neighborhood walks
        // through the existing dirty flag and retain first-visit order.
        affected_sites_.clear();
        auto invalidate = [this](lattice::Site& site) {
            if (site.is_dirty())
                return;
            site.set_dirty(true);
            affected_sites_.push_back(&site);
        };
        for (auto* site : record.sites())
            visit_neighbor_tree(*site, radius, invalidate);

        for (auto* site : affected_sites_)
            update(*site);

        if (block_updates_.size() == 1)
        {
            auto& update{ block_updates_.front() };
            update.rate = block_rate(lattice_.sites().at(update.index));
            rate_tree_->update_rate(update.index, update.rate);
        }
        else if (not block_updates_.empty())
        {
            std::ranges::sort(block_updates_, {}, &rate_tree::BlockUpdate::index);
            for (auto& update : block_updates_)
                update.rate = block_rate(lattice_.sites().at(update.index));
            rate_tree_->update_rates(block_updates_);
        }
    }

    template <class ModelType>
    typename RejectionTreeSimulation<ModelType>::Selection
    RejectionTreeSimulation<ModelType>::select_random_location(Rate total_rate)
    {
        auto selected_rate{ physics::ensure_non_negative(total_rate * rng_.probability()) };
        auto site_index{ rate_tree_->index(selected_rate) };

        while (selected_rate >= site_rates_.at(site_index))
            selected_rate -= site_rates_.at(site_index++);

        if constexpr (utility::is_debug)
            if (site_index >= site_rates_.size())
                throw std::runtime_error{ "tree not updated correctly" };

        const auto first{ static_cast<std::size_t>(site_index) * ModelType::slots_per_site };
        auto offset{ first };
        while (selected_rate >= class_rates_.at(event_class_keys_.at(offset)))
            selected_rate -= class_rates_.at(event_class_keys_.at(offset++));

        if (offset >= first + ModelType::slots_per_site)
            throw std::runtime_error{ "Selected event slot is outside source-site layout." };

        return { static_cast<lattice::Index>(site_index), offset - first,
                 event_class_keys_.at(offset) };
    }

    template <class ModelType>
    event::Record RejectionTreeSimulation<ModelType>::evaluate_event(Selection const& selected)
    {
        std::optional<event::Record> record;
        auto& source{ lattice_.sites().at(selected.source) };

        model_.transformations().visit(selected.slot,
            [&](auto const& transformation, SlotId local_slot) {
                auto location{ transformation.location(source, local_slot, lattice_) };
                if (not location)
                    throw std::logic_error{ "Selected slot has no geometric location." };

                const bool rejected{ transformation.is_rejected(*location) };
                record.emplace(std::move(*location),
                    std::remove_cvref_t<decltype(transformation)>::event,
                    rejected, selected.class_key);
            });
        return std::move(*record);
    }

    template <class ModelType>
    template <class Visitor>
    void RejectionTreeSimulation<ModelType>::visit_neighbor_tree(
        lattice::Site& site, std::size_t radius, Visitor& visit)
    {
        // Enumerate graph walks without allocating a visited-site container.
        // Sites may recur because the lattice graph contains cycles; the
        // two-pass dirty flag mechanism prevents duplicate rate calculations.
        if (radius > 0)
            for (auto* neighbor : lattice_.neighbors(site))
                visit_neighbor_tree(*neighbor, radius - 1, visit);

        visit(site);
    }

    template <class ModelType>
    void RejectionTreeSimulation<ModelType>::update(lattice::Site& site)
    {
        if (not site.is_dirty())
            return;

        auto& old_rate{ site_rates_.at(site.index()) };

        Rate new_rate{ rate_sum(site) };

        if (new_rate != old_rate)
        {
            old_rate = new_rate;

            constexpr lattice::Index block_mask{
                (lattice::Index{ 1 } << rate_tree::baseLevel) - 1
            };
            auto const block_start{ site.index() & ~block_mask };

            if (std::ranges::find(block_updates_, block_start, &rate_tree::BlockUpdate::index)
                == block_updates_.end())
                block_updates_.push_back({ block_start, {} });
        }
        site.set_dirty(false);
    }

    template <class ModelType>
    Rate RejectionTreeSimulation<ModelType>::rate_sum(lattice::Site& site)
    {
        const auto first{ static_cast<std::size_t>(site.index()) * ModelType::slots_per_site };
        auto const source_candidates{ model_.transformations().source_candidates(site) };

        if (std::ranges::none_of(source_candidates, std::identity{}))
        {
            // An inactive site has no registered event classes to clear.
            if (site.is_active())
                for (SlotId slot{}; slot < ModelType::slots_per_site; ++slot)
                    set_class_key(first + slot, RateClassKey{});

            site.set_active(false);
            return {};
        }

        Rate sum{};
        bool has_events{};

        std::size_t transformation_index{};
        model_.transformations().for_each([&](auto const& transformation, std::size_t offset) {
            constexpr auto count{ std::remove_cvref_t<decltype(transformation)>::slot_count };
            if (not source_candidates[transformation_index++])
            {
                for (SlotId local{}; local < count; ++local)
                    set_class_key(first + offset + local, RateClassKey{});
                return;
            }

            for (SlotId local{}; local < count; ++local)
            {
                RateClassKey key{};
                if (auto location{ transformation.location(site, local, lattice_) }; location)
                {
                    // The source precondition has already been checked.
                    // Transformations without an optimized predicate retain
                    // the original full candidate check.
                    const bool candidate{ [&] {
                        if constexpr (requires {
                            transformation.is_candidate_after_source_check(*location);
                        })
                            return transformation.is_candidate_after_source_check(*location);
                        else
                            return transformation.is_candidate(*location);
                    }() };
                    if (candidate)
                        key = class_key(std::remove_cvref_t<decltype(transformation)>::event,
                                        *location);
                }

                set_class_key(first + offset + local, key);
                if (key != RateClassKey{})
                {
                    has_events = true;
                    sum += class_rates_.at(key);
                }
            }
        });

        site.set_active(has_events);
        return sum;
    }

    template <class ModelType>
    void RejectionTreeSimulation<ModelType>::set_class_key(std::size_t index, RateClassKey new_key)
    {
        auto& old_key{ event_class_keys_.at(index) };

        if (old_key == new_key)
            return;

        // Key zero denotes an inactive event slot.
        if (old_key != RateClassKey{})
            --class_counts_.at(old_key);

        old_key = new_key;

        if (new_key != RateClassKey{})
            ++class_counts_.at(new_key);
    }

    template <class ModelType>
    Rate RejectionTreeSimulation<ModelType>::block_rate(lattice::Site& site)
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

    template <class ModelType>
    RateClassKey RejectionTreeSimulation<ModelType>::class_key(
        event::Type type, event::Location const& location)
    {
        const auto key{ model_.kinetics().classify(type, location, lattice_) };
        if (key == RateClassKey{})
            throw std::logic_error{ "Active event maps to reserved key zero." };

        if (key >= class_rates_.size())
        {
            class_rates_.resize(static_cast<std::size_t>(key) + 1);
            class_initialized_.resize(static_cast<std::size_t>(key) + 1);
            class_counts_.resize(static_cast<std::size_t>(key) + 1);
            class_events_.resize(static_cast<std::size_t>(key) + 1, event::Type::none);
        }

        if (not class_initialized_[key])
        {
            class_rates_[key] = model_.kinetics().rate(key);
            class_events_[key] = model_.kinetics().class_event(key);
            class_initialized_[key] = true;
        }

        return key;
    }

    template <class ModelType>
    void RejectionTreeSimulation<ModelType>::update_all_rates()
    {
        model_.kinetics().refresh_rates();
        for (std::size_t key{ 1 }; key < class_rates_.size(); ++key)
            if (class_initialized_[key])
                class_rates_[key] = model_.kinetics().rate(static_cast<RateClassKey>(key));

        // A full-rate refresh bypasses per-event block collection. Rebuild each
        // block once after all site rates have been recalculated.
        block_updates_.clear();
        for (auto& site : lattice_.sites())
        {
            site_rates_.at(site.index()) = rate_sum(site);
            site.set_dirty(false);
        }

        constexpr auto block_size{ std::size_t{ 1 } << rate_tree::baseLevel };
        for (std::size_t index{}; index < site_rates_.size(); index += block_size)
            rate_tree_->update_rate(
                static_cast<lattice::Index>(index), block_rate(lattice_.sites().at(index)));
    }
}
