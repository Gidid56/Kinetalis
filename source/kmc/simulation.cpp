#include "pch.h"

#include "simulation.h"

#include "analysis_plan.h"
#include "lattice/lattice.h"
#include "lattice/path.h"

using namespace kmc;
using lattice::Coordination;
using lattice::Path;
using lattice::Site;
using simulation::Status;

static_assert(rate_tree::baseLevel <= rate_tree::maxHeight);
static_assert(rate_tree::allocationBlockLevels > 0);

template <class Rules>
void kmc::RejectionTreeSimulation<Rules>::initialize_rates()
{
    rate_tree_ = rate_tree::make_entry(lattice_.sites().size());
    site_rates_.resize(lattice_.sites().size());
    processes_.resize(lattice_.sites().size() + lattice_.neighbors().size());
}

template <class Rules>
simulation::Status RejectionTreeSimulation<Rules>::run(AnalysisPlan& analysis_plan, std::stop_token token)
{
    utility::Guard guard{ timer_ };

    if (status_ == Status::created)
        analysis_plan.observe_started(*this);

    status_ = Status::running;

    while (not analysis_plan.stop_requested())
    {
        if (token.stop_requested())
            return status_ = Status::paused;

        const auto total_rate{ rate_tree_->rate_sum() };
        if (total_rate <= Rate{})
        {
            analysis_plan.observe_completed(*this);
            return status_ = Status::exhausted;
        }

        const auto escape_time{ 1.0 / total_rate };
        if constexpr (stochasticResidenceTime)
            time_ += -std::log1p(-rng_.probability()) * escape_time;
        else
            time_ += escape_time;

        auto process{ select_random_process() };

        analysis_plan.observe_selected(process);

        if (process.outcome == process::Outcome::accepted)
        {
            ++step_;

            process.execute(lattice_);

            for (auto site : process.path.sites())
            {
                update_neighbors(*site);
                update(*site);
            }

            analysis_plan.observe_processed(process);
        }
        else if (process.outcome == process::Outcome::rejected)
        {
            ++rejections_;
        }

        analysis_plan.observe_step(*this);
    }

    analysis_plan.observe_completed(*this);
    return status_ = Status::analysed;
}

template <class Rules>
process::Record RejectionTreeSimulation<Rules>::select_random_process()
{
    auto selected_rate{ rate_tree_->rate_sum() * rng_.probability() };

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
        return process::Record{ site, decltype(rules_.site)::event, rules_.site.allowed(site) };

    auto const path{ Path{ &site, lattice_.neighbor(process_index - site_index - 1) } };

    return process::Record{ path, decltype(rules_.path)::event, rules_.path.allowed(path) };
}

template <class Rules>
void RejectionTreeSimulation<Rules>::update_neighbors(Site& site)
{
    for (auto neighbor : lattice_.neighbors(site))
        update(*neighbor);
}

template <class Rules>
void RejectionTreeSimulation<Rules>::update(Site& site)
{
    //site.setClassification(lattice_.classification(site));

    if (not site.is_dirty())
        return;

    auto& old_rate{ site_rates_.at(site.index()) };

    //if constexpr (results::processesStats)
    //	if (results_ and oldRate > Rate{})
    //		results_->setActive(processRates_[processes_[site.processIndex()]].first, false);

    Rate new_rate{ rate_sum(site) };

    //if constexpr (results::processesStats) if (results_ and newRate > Rate{})
    //	results_->setActive(processRates_[processes_[site.processIndex()]].first, true);

    if (new_rate != old_rate)
    {
        old_rate = new_rate;

        rate_tree_->update_rate(site.index(), block_rate(site));
    }
    site.set_dirty(false);
}

template <class Rules>
Rate RejectionTreeSimulation<Rules>::rate_sum(Site& site)
{
    const bool site_active{ rules_.site.active(site) };
    const bool path_active{ rules_.path.source_active(site) };

    if (not site_active and not path_active)
    {
        clear_processes(site);
        return {};
    }

    lattice::Index index{ site.process_index() };
    Rate sum{};

    auto add_process = [&](process::Key key) {
        set_process(index++, key);
        sum += process_rates_.at(key).second;
    };

    add_process(
        site_active ? process_key(Process{ decltype(rules_.site)::event, site }) : process::Key{});

    if (not path_active)
    {
        for (std::size_t i{}; i < site.max_neighbors(); ++i)
            set_process(index++, {});

        return sum;
    }

    for (auto neighbor : lattice_.neighbors(site))
    {
        auto const path{ Path{ &site, neighbor } };
        add_process(process_key(make_process(path, rules_.path)));
    }

    return sum;
}

template <class Rules>
Rate RejectionTreeSimulation<Rules>::block_rate(Site& site)
{
    constexpr auto block_size{ std::size_t{ 1 } << rate_tree::baseLevel };

    auto const block_start{ site.index() & ~(block_size - 1) };

    Rate sum{};

    for (auto index{ block_start }; index < std::min(block_start + block_size, site_rates_.size());
        ++index)
    {
        sum += site_rates_.at(index);
    }
    return sum;
}

template <class Rules>
process::Key RejectionTreeSimulation<Rules>::process_key(Process process)
{
    if (not process.is_active())
        return {};

    if constexpr (staticProcessKeys)
        return process.static_key();

    if (auto it{ dynamic_keys_.find(process) }; it != dynamic_keys_.end())
        return it->second;

    if (process_rates_.size() > std::numeric_limits<process::Key>::max())
        throw std::overflow_error{ "Increase process::Key size." };

    auto const key{ static_cast<process::Key>(process_rates_.size()) };

    auto const rate{ model_->rate(process) };

    dynamic_keys_.emplace(process, key);
    process_rates_.emplace_back(process, rate);
    process_counts_.emplace_back(0);

    return key;
}

template <class Rules>
void RejectionTreeSimulation<Rules>::update_rates()
{
    if constexpr (staticProcessKeys)
        update_static_rates();
    else
        update_dynamic_rates();

    for (auto& site : lattice_.sites())
    {
        site.set_dirty(true);
        update(site);
    }
}

template <class Rules>
void RejectionTreeSimulation<Rules>::update_static_rates()
{
    process_rates_.clear();
    process_rates_.reserve(Process::number_keys());
    process_rates_.emplace_back(Process{}, Rate{});

    constexpr auto elements{ utility::prepend(Element::none, lattice::activeElements) };

    auto add_rule = [&](auto const& rule) {
        using Rule = std::remove_cvref_t<decltype(rule)>;

        for (auto element : elements)
            for (Coordination coordination{}; coordination < lattice::max_coordination;
                ++coordination)
            {
                Process process{ Rule::event, element, coordination };

                if (not rule.supports(process))
                    continue;

                auto const key{ process.static_key() };
                auto const rate{ model_->rate(process) };

                if (key >= process_rates_.size())
                    process_rates_.resize(key + 1);

                process_rates_.at(key) = { process, rate };
            }
    };

    add_rule(rules_.site);
    add_rule(rules_.path);

    if (process_counts_.size() == 1)
        process_counts_.resize(process_rates_.size());
    else if (process_counts_.size() != process_rates_.size())
        throw std::logic_error{ "Process key space changed after initialization." };
}

template <class Rules>
void RejectionTreeSimulation<Rules>::update_dynamic_rates()
{
    for (auto& [process, rate] : process_rates_ | std::views::drop(1))
        rate = model_->rate(process);
}

template <class Rules>
void RejectionTreeSimulation<Rules>::set_process(lattice::Index index, process::Key new_key)
{
    auto& old_key{ processes_.at(index) };

    if (old_key == new_key)
        return;

    // key 0 is the empty / invalid process for static process keys
    if (old_key != process::Key{})
        --process_counts_.at(old_key);

    old_key = new_key;

    if (new_key != process::Key{})
        ++process_counts_.at(new_key);
}

template <class Rules>
void RejectionTreeSimulation<Rules>::clear_processes(Site& site)
{
    auto index{ site.process_index() };

    set_process(index++, {});

    for (std::size_t i{}; i < site.max_neighbors(); ++i)
        set_process(index++, {});
}

#include "../studies/dealloying/rules.h"

template class RejectionTreeSimulation<studies::dealloying::Rules>;
