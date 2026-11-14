#pragma once
#include "batch/campaign.h"
#include "batch/terminal.h"
#include "batch/types.h"
#include "kmc/simulation.h"

#include <concepts>
#include <functional>
#include <utility>
#include <vector>

class batch::Runner
{
public:
    explicit Runner(std::filesystem::path results_path, std::size_t concurrency_count)
        : results_path_{ std::move(results_path) },
          concurrency_count_{ concurrency_count }
    {
        if (concurrency_count == 0)
            throw std::invalid_argument{ "Batch concurrency must be positive." };
    }

    template <class Factory>
    requires std::invocable<Factory&>
    bool run(Factory&& factory)
    {
        return run_impl([factory = std::forward<Factory>(factory)]() mutable {
            return make_campaign_data(std::invoke(factory));
        });
    }

    std::size_t sample_count() const;
    std::size_t worker_count() const;

private:
    struct CampaignData
    {
        std::string name, seed;
        std::size_t replicate_count{ 1 };
        std::vector<std::string> setup_names;
        kmc::PlanFactory analysis_plan;
        Postprocessor postprocessor;
        std::function<void(
            std::size_t,
            simulation::Seed const&,
            std::filesystem::path const&,
            kmc::PlanFactory const&)> execute_sample;

        std::size_t sample_count() const { return setup_names.size() * replicate_count; }
    };

    template <class Rules>
    static CampaignData make_campaign_data(Campaign<Rules> campaign)
    {
        CampaignData data;
        data.name = std::move(campaign.name);
        data.seed = std::move(campaign.seed);
        data.replicate_count = campaign.replicate_count;
        data.analysis_plan = std::move(campaign.analysis_plan);
        data.postprocessor = std::move(campaign.postprocessor);

        data.setup_names.reserve(campaign.setups.size());
        for (auto const& setup : campaign.setups)
            data.setup_names.push_back(setup.name);

        data.execute_sample = [setups = std::move(campaign.setups)](
                                  std::size_t setup_index,
                                  simulation::Seed const& seed,
                                  std::filesystem::path const& path,
                                  kmc::PlanFactory const& plan_factory) {
            auto setup{ setups.at(setup_index) };
            setup.seed = seed;

            kmc::RejectionTreeSimulation<Rules> simulation{ setup };
            auto analysis_plan{ std::invoke(plan_factory, path) };
            simulation.run(analysis_plan);
        };

        return data;
    }

    bool run_impl(std::function<CampaignData()>);

    void initialize(CampaignData);

    void generate_seeds();

    void write_init() const;
    void write_campaign() const;
    void write_setups() const;
    void write_samples() const;
    void write_final() const;

    std::string campaign_info() const;

    void run_samples();
    void run_worker();
    void run_sample(std::size_t) const;
    void record(sample::State);

    void postprocess() const;

    std::size_t sample_setup_index(std::size_t) const;
    std::size_t sample_replicate_index(std::size_t) const;
    std::string sample_name(std::size_t) const;

    std::filesystem::path sample_path(std::size_t) const;

    bool used_{}; // Runner is one-time use only

    CampaignData campaign_;

    std::unique_ptr<Terminal> terminal_{};

    std::filesystem::path results_path_, campaign_path_;

    std::size_t concurrency_count_;

    std::string seed_generator_name_;
    std::vector<simulation::Seed> seeds_;

    std::map<sample::State, std::size_t> counts_{
        { sample::State::completed, 0 },
        { sample::State::failed, 0 },
    };

    std::atomic<std::size_t> next_sample_{};
    std::mutex counts_mutex_;
};
