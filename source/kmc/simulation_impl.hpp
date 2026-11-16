#pragma once
// Template definitions for RejectionTreeSimulation; included by simulation.h.

#include "analysis_plan.h"
#include "lattice/lattice.h"
#include "utility/utility.h"

namespace kmc
{
    template <class Rules>
    void RejectionTreeSimulation<Rules>::initialize_rates()
    {
        rate_tree_ = rate_tree::make_entry(lattice_.sites().size());
        site_rates_.resize(lattice_.sites().size());
        processes_.resize(lattice_.sites().size() + lattice_.neighbors().size());

        // Each model decides whether to preallocate or register classes lazily.
        process_rates_.resize(model_->number_keys());
        process_counts_.resize(model_->number_keys());
    }

    template <class Rules>
    simulation::Status
    RejectionTreeSimulation<Rules>::run(AnalysisPlan& analysis_plan, std::stop_token token)
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
            if constexpr (stochasticResidenceTime)
                time_ += -std::log1p(-rng_.probability()) * escape_time;
            else
                time_ += escape_time;

            auto record{ evaluate_event(select_random_location()) };

            analysis_plan.observe_selected(record);

            if (record.outcome == event::Outcome::accepted)
            {
                ++step_;

                if (record.sites().size() == 1)
                    rules_.site.execute(record.location(), lattice_);
                else
                    rules_.cluster.execute(record.location(), lattice_);

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

    template <class Rules>
    void RejectionTreeSimulation<Rules>::update_affected_rates(event::Record const& record)
    {
        block_updates_.clear();

        // Model dependencies can extend beyond the nearest neighbors;
        // the current event rules require at least one neighbor shell.
        const auto radius{ std::max<std::size_t>(1, model_->invalidation_radius()) };

        // First invalidate the entire region, then recalculate. Recomputing a
        // source clears its dirty flag, so overlapping walks are inexpensive.
        auto invalidate = [](lattice::Site& site) { site.set_dirty(true); };
        for (auto* site : record.sites())
            visit_neighbor_tree(*site, radius, invalidate);

        auto recalculate = [this](lattice::Site& site) { update(site); };
        for (auto* site : record.sites())
            visit_neighbor_tree(*site, radius, recalculate);

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

    template <class Rules>
    event::Location RejectionTreeSimulation<Rules>::select_random_location()
    {
        auto selected_rate{ physics::ensure_non_negative(rate_tree_->rate_sum() * rng_.probability()) };

        auto site_index{ rate_tree_->index(selected_rate) };

        while (selected_rate >= site_rates_.at(site_index))
            selected_rate -= site_rates_.at(site_index++);

        if constexpr (utility::is_debug)
            if (site_index >= site_rates_.size())
                throw std::runtime_error{ "tree not updated correctly" };

        auto& site{ lattice_.sites().at(site_index) };
        auto process_index{ site.process_index() };

        while (selected_rate >= process_rates_.at(processes_.at(process_index)).second)
            selected_rate -= process_rates_.at(processes_.at(process_index++)).second;

        if constexpr (utility::is_debug)
            if (process_index > site.process_index() + site.max_neighbors())
                throw std::runtime_error{ "tree not updated correctly" };

        if (process_index == site.process_index())
            return event::Location{ site };

        return event::Location{ lattice::Cluster<2>{
            &site, lattice_.neighbor(process_index - site_index - 1) } };
    }

    template <class Rules>
    event::Record RejectionTreeSimulation<Rules>::evaluate_event(event::Location location) const
    {
        if (location.sites().size() == 1)
            return { location, decltype(rules_.site)::event, rules_.site.is_rejected(location) };

        if (location.sites().size() == 2)
            return { location, decltype(rules_.cluster)::event, rules_.cluster.is_rejected(location) };

        throw std::logic_error{ "Unsupported event location size." };
    }

    template <class Rules>
    template <class Visitor>
    void RejectionTreeSimulation<Rules>::visit_neighbor_tree(
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

    template <class Rules>
    void RejectionTreeSimulation<Rules>::update(lattice::Site& site)
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

    template <class Rules>
    Rate RejectionTreeSimulation<Rules>::rate_sum(lattice::Site& site)
    {
        const bool site_source_candidate{ rules_.site.is_source_candidate(site) };
        const bool cluster_source_candidate{ rules_.cluster.is_source_candidate(site) };
        const event::Location source{ site };

        lattice::Index index{ site.process_index() };

        auto clear_site = [&] {
            set_process(index++, {});
        };
        auto clear_clusters = [&] {
            for (std::size_t i{}; i < site.max_neighbors(); ++i)
                set_process(index++, {});
        };

        if (not site_source_candidate and not cluster_source_candidate)
        {
            if (site.is_active())
            {
                clear_site();
                clear_clusters();
                site.set_active(false);
            }
            return {};
        }

        Rate sum{};
        bool has_processes{};

        auto add_process = [&](RateClassKey key) {
            set_process(index++, key);
            has_processes |= key != RateClassKey{};
            sum += process_rates_.at(key).second;
        };

        if (site_source_candidate and rules_.site.is_candidate(source))
            add_process(process_key(decltype(rules_.site)::event, source));
        else
            clear_site();

        if (not cluster_source_candidate)
        {
            clear_clusters();
            site.set_active(has_processes);
            return sum;
        }

        for (auto neighbor : lattice_.neighbors(site))
        {
            const event::Location location{ lattice::Cluster<2>{ &site, neighbor } };
            if (rules_.cluster.is_candidate(location))
                add_process(process_key(decltype(rules_.cluster)::event, location));
            else
                add_process({});
        }

        site.set_active(has_processes);
        return sum;
    }

    template <class Rules>
    void RejectionTreeSimulation<Rules>::set_process(lattice::Index index, RateClassKey new_key)
    {
        auto& old_key{ processes_.at(index) };

        if (old_key == new_key)
            return;

        // key 0 is the empty / invalid process for static process keys
        if (old_key != RateClassKey{})
            --process_counts_.at(old_key);

        old_key = new_key;

        if (new_key != RateClassKey{})
            ++process_counts_.at(new_key);
    }

    template <class Rules>
    Rate RejectionTreeSimulation<Rules>::block_rate(lattice::Site& site)
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

    template <class Rules>
    RateClassKey RejectionTreeSimulation<Rules>::process_key(
        event::Type type, event::Location const& location)
    {
        const auto key{ model_->classify(type, location, lattice_) };
        if (key == RateClassKey{})
            throw std::logic_error{ "Active event maps to reserved key zero." };

        if (key >= process_rates_.size())
        {
            process_rates_.resize(static_cast<std::size_t>(key) + 1);
            process_counts_.resize(static_cast<std::size_t>(key) + 1);
        }

        auto& [cached_info, cached_rate]{ process_rates_.at(key) };
        auto const& info{ model_->class_info(key) };
        if (not cached_info.is_active())
        {
            cached_rate = model_->rate(key);
            cached_info = info;
        }
        else if (cached_info != info)
            throw std::logic_error{ "Rate class key collision." };

        return key;
    }

    template <class Rules>
    void RejectionTreeSimulation<Rules>::update_all_rates()
    {
        model_->refresh_rates();
        for (std::size_t key{ 1 }; key < process_rates_.size(); ++key)
            if (process_rates_[key].first.is_active())
                process_rates_[key].second = model_->rate(static_cast<RateClassKey>(key));

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
