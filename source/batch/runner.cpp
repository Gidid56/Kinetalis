#include "pch.h"

#include "runner.h"

#include "kmc/analysis_plan.h"
#include "kmc/simulation.h"
#include "utility/csv.h"
#include "utility/file.h"
#include "utility/format.h"

using namespace batch;
using namespace utility;
using enum file::Type;

template <class Rules>
bool Runner<Rules>::run(CampaignFactory<Rules> campaign)
{
    if (std::exchange(used_, true))
        throw std::logic_error{ "Runner can only be used once." };

    try
    {
        initialize(std::move(campaign));

        write_init();
        run_samples();
        write_final();

        postprocess();

        return counts_[sample::State::failed] == 0;
    }
    catch (std::exception const& exception)
    {
        std::print(stderr, "\n[{:%T}] Error: {}\n", WallClock::now(), exception.what());
        return false;
    }
}

template <class Rules>
void Runner<Rules>::initialize(CampaignFactory<Rules> campaign)
{
    campaign_ = campaign();

    if (campaign_.name.empty())
        campaign_.name = "campaign";

    campaign_path_ = file::create_unique_directory(results_path_, campaign_.name);

    generate_seeds();

    terminal_ = std::make_unique<Terminal>(campaign_.sample_count(), worker_count());
}

template <class Rules>
void Runner<Rules>::generate_seeds()
{
    random_::Engine campaign_engine{ campaign_.seed };

    seed_generator_name_ = std::format("{}", campaign_engine);
    seeds_.resize(campaign_.sample_count());

    auto const setup_count{ campaign_.setups.size() };

    for (std::size_t setup_index{}; setup_index < setup_count; ++setup_index)
    {
        auto const first_seed{ setup_index == 0 ? campaign_engine.seed()
                                                : campaign_engine.generate_seed() };

        random_::Engine replicate_engine{ first_seed };

        for (std::size_t replicate_index{}; replicate_index < campaign_.replicate_count;
            ++replicate_index)
        {
            auto const sample_index{ replicate_index * setup_count + setup_index };

            seeds_[sample_index] =
                replicate_index == 0 ? replicate_engine.seed() : replicate_engine.generate_seed();
        }
    }
}

template <class Rules>
std::string Runner<Rules>::campaign_info() const
{
    return std::format(
        "Campaign Seed {}\n"
        "{} setups × {} replicates = {} total samples.\n",
        seed_generator_name_,
        campaign_.setups.size(),
        campaign_.replicate_count,
        campaign_.sample_count());
}

template <class Rules>
void Runner<Rules>::write_init() const
{
    std::print(
        "\n[{:%T}] Starting campaign '{}'.\n{}\n",
        WallClock::now(),
        campaign_path_.filename().string(),
        campaign_info());

    write_campaign();
    write_setups();
    write_samples();
}

template <class Rules>
void Runner<Rules>::write_campaign() const
{
    file::write<txt>(campaign_path_ / "campaign", campaign_info());
}

template <class Rules>
void Runner<Rules>::write_setups() const
{
    std::string text{ "setup,description\n" };

    for (std::size_t setup_index{}; auto const& setup : campaign_.setups)
        std::format_to(
            std::back_inserter(text),
            "{},{}\n",
            setup_index++,
            csv::Table::encode_field(setup.name));

    file::write<file::Type::csv>(campaign_path_ / "setups", text);
}

template <class Rules>
void Runner<Rules>::write_samples() const
{
    std::string text{ "sample,setup,replicate,seed\n" };

    for (std::size_t sample_index{}; sample_index < seeds_.size(); ++sample_index)
        std::format_to(
            std::back_inserter(text),
            "{},{},{},{}\n",
            sample_name(sample_index),
            sample_setup_index(sample_index),
            sample_replicate_index(sample_index),
            random_::Engine::seed_to_string(seeds_[sample_index]));

    file::write<file::Type::csv>(campaign_path_ / "samples", text);
}

template <class Rules>
void Runner<Rules>::write_final() const
{
    file::append<txt>(
        campaign_path_ / "campaign",
        std::format("{}\n", utility::format_map(counts_, "_samples = ", "\n")));

    std::print("\n[{:%T}] Campaign completed successfully.\n", WallClock::now());
}

template <class Rules>
std::size_t Runner<Rules>::sample_count() const
{
    return campaign_.sample_count();
}

template <class Rules>
std::size_t Runner<Rules>::worker_count() const
{
    return std::min(concurrency_count_, sample_count());
}

template <class Rules>
void Runner<Rules>::run_samples()
{
    std::vector<std::jthread> workers;
    workers.reserve(worker_count());

    for (std::size_t worker{}; worker < worker_count(); ++worker)
        workers.emplace_back([this] { run_worker(); });
}

template <class Rules>
void Runner<Rules>::run_worker()
{
    while (true)
    {
        auto const sample_index{ next_sample_.fetch_add(1, std::memory_order_relaxed) };

        if (sample_index >= campaign_.sample_count())
            return;

        auto const name{ sample_name(sample_index) };

        try
        {
            terminal_->report(sample_index, sample::State::running, name);
            run_sample(sample_index);
            terminal_->report(sample_index, sample::State::completed);
            record(sample::State::completed);
        }
        catch (std::exception const& exception)
        {
            terminal_->report(sample_index, sample::State::failed, exception.what());
            record(sample::State::failed);
        }
    }
}

template <class Rules>
void Runner<Rules>::run_sample(std::size_t sample_index) const
{
    auto setup{ campaign_.setups[sample_setup_index(sample_index)] };
    setup.seed = seeds_[sample_index];

    auto const path{ sample_path(sample_index) };
    std::filesystem::create_directories(path);

    kmc::KmcSimulation<Rules> simulation{ setup };
    auto analysis_plan{ std::invoke(campaign_.analysis_plan, path) };

    simulation.run(analysis_plan);
}

template <class Rules>
void Runner<Rules>::record(sample::State state)
{
    std::scoped_lock lock{ counts_mutex_ };
    ++counts_.at(state);
}

template <class Rules>
void Runner<Rules>::postprocess() const
{
    std::print("\n[{:%T}] Starting postprocessor.\n", WallClock::now());

    if (campaign_.replicate_count > 1)
        campaign::aggregate(campaign_path_);

    if (not campaign_.postprocessor)
        return;

    std::invoke(campaign_.postprocessor, campaign_path_);
    std::print("[{:%T}] Postprocessing completed successfully.\n", WallClock::now());
}

template <class Rules>
std::size_t Runner<Rules>::sample_setup_index(std::size_t sample_index) const
{
    return sample_index % campaign_.setups.size();
}

template <class Rules>
std::size_t Runner<Rules>::sample_replicate_index(std::size_t sample_index) const
{
    return sample_index / campaign_.setups.size();
}

std::size_t index_width(std::size_t count) { return std::to_string(count - 1).size(); };

template <class Rules>
std::string Runner<Rules>::sample_name(std::size_t sample_index) const
{
    return std::format(
        "{:0{}}/{:0{}}",
        sample_setup_index(sample_index),
        index_width(campaign_.setups.size()),
        sample_replicate_index(sample_index),
        index_width(campaign_.replicate_count));
}

template <class Rules>
std::filesystem::path Runner<Rules>::sample_path(std::size_t sample_index) const
{
    auto const setup_index{ sample_setup_index(sample_index) };
    auto const setup_count{ campaign_.setups.size() };
    auto const setup_path{ std::format("{:0{}}", setup_index, index_width(setup_count)) };
    auto path{ campaign_path_ / "setups" / setup_path };

    if (campaign_.replicate_count > 1)
        path /= std::format(
            "samples/{:0{}}",
            sample_replicate_index(sample_index),
            index_width(campaign_.replicate_count));

    return path;
}
