#include "pch.h"

#include "simulation.h"

#include "analysis_plan.h"
#include "lattice/lattice.h"
#include "lattice/path.h"

using namespace kmc;
using lattice::Coordination;
using lattice::Path;
using lattice::Site;

static_assert(rate_tree::baseLevel <= rate_tree::maxHeight);
static_assert(rate_tree::allocationBlockLevels > 0);

Simulation::Simulation(
    lattice::Generator const& make_lattice,
    model::Generator const& make_model,
    std::optional<random_::Engine::seed_type> seed)
    : lattice_{ make_lattice() },
      model_{ make_model() },
      rng_{ seed }
{
    lattice_.apply_fill_function(rng_);

    for (auto& site : lattice_.sites())
        site.set_pristine(true);

    rate_tree_ = rate_tree::make_entry(lattice_.sites().size());
    site_rates_.resize(lattice_.sites().size());
    processes_.resize(lattice_.sites().size() + lattice_.neighbors().size());

    //model_.update();
}

template <class Rules>
Simulation::Status KmcSimulation<Rules>::run(AnalysisPlan& analysis_plan, std::stop_token token)
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
process::Record KmcSimulation<Rules>::select_random_process()
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
        return process::Record{ site };

    else
        return process::Record{ Path{ &site, lattice_.neighbor(process_index - site_index - 1) } };
}

template <class Rules>
void KmcSimulation<Rules>::update_neighbors(Site& site)
{
    for (auto neighbor : lattice_.neighbors(site))
        update(*neighbor);
}

template <class Rules>
void KmcSimulation<Rules>::update(Site& site)
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
Rate KmcSimulation<Rules>::rate_sum(Site& site)
{
    Rate rate_sum{};

    if (not site.is_active())
    {
        clear_processes(site);
        return rate_sum;
    }

    lattice::Index process_index{ site.process_index() };

    {
        auto const key{ process_key(Process{ site }) };
        set_process(process_index++, key);
        rate_sum += process_rates_.at(key).second;
    }

    for (auto neighbor : lattice_.neighbors(site))
    {
        auto const key{ process_key(Process{ Path{ &site, neighbor } }) };
        set_process(process_index++, key);
        rate_sum += process_rates_.at(key).second;
    }
    return rate_sum;
}

Rate Simulation::block_rate(Site& site)
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

process::Key Simulation::process_key(Process process)
{
    if constexpr (staticProcessKeys)
        return process.static_key();

    auto iterator{ dynamic_keys_.find(process) };

    if (iterator != std::end(dynamic_keys_))
        return iterator->second;

    if (std::size(dynamic_keys_) > std::numeric_limits<process::Key>::max())
        throw std::overflow_error{ "increase Process::Key size" };

    auto&& [pair, _]{
        dynamic_keys_.emplace(process, static_cast<process::Key>(std::size(dynamic_keys_)))
    };
    process_rates_.emplace_back(process, model_->rate(process));
    return pair->second;
}

template <class Rules>
void KmcSimulation<Rules>::update_rates()
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
void KmcSimulation<Rules>::update_static_rates()
{
    process_rates_.clear();
    process_rates_.reserve(Process::number_keys());
    process_rates_.emplace_back(Process{}, Rate{});

    constexpr auto process_elements{ utility::prepend(Element::none, lattice::activeElements) };

    auto add_process = [&](auto event, auto element, auto coordination) {
        Process process{ event, element, coordination };
        if (not process.is_allowed())
            return;

        auto key{ process.static_key() };
        Rate rate{ model_->rate(process) };

        if (key >= process_rates_.size())
            process_rates_.resize(key + 1);

        process_rates_.at(key) = { process, rate };
    };

    for (auto event : enabledEvents)
        for (auto element : process_elements)
            for (Coordination coord{}; coord < lattice::max_coordination; ++coord)
                add_process(event, element, coord);

    if (process_counts_.empty())
        process_counts_.resize(process_rates_.size());
    else if (process_counts_.size() != process_rates_.size())
        throw std::logic_error{ "Process key space changed after initialization." };
}

void Simulation::update_dynamic_rates()
{
    for (auto&& [process, rate] : process_rates_)
    {
        rate = model_->rate(process);
    }
}

void Simulation::set_process(lattice::Index index, process::Key new_key)
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

void Simulation::clear_processes(Site& site)
{
    auto process_index{ site.process_index() };

    set_process(process_index++, {});

    for ([[maybe_unused]] auto neighbor : lattice_.neighbors(site))
        set_process(process_index++, {});
}

double Simulation::rejection_fraction() const
{
    auto const selections{ step_ + rejections_ };
    return selections == 0 ? 0.0 : static_cast<double>(rejections_) / selections;
}

double Simulation::tracked_fraction() const
{
    return static_cast<double>(tracked_.duration().count()) / timer_.duration().count();
}

Duration Simulation::run_duration() const
{
    return Duration{ std::chrono::duration<double>{ timer_.duration() }.count() };
}

Duration Simulation::tracked_time() const
{
    return Duration{ std::chrono::duration<double>{ tracked_.duration() }.count() };
}
