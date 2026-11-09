#pragma once
#include "batch/campaign.h"
#include "batch/terminal.h"
#include "batch/types.h"
#include "kmc/types.h"

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

    bool run(CampaignFactory);

    std::size_t sample_count() const;
    std::size_t worker_count() const;

private:
    void initialize(CampaignFactory);

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

    Campaign campaign_;

    std::unique_ptr<Terminal> terminal_{};

    std::filesystem::path results_path_, campaign_path_;

    std::size_t concurrency_count_;

    std::string seed_generator_name_;
    std::vector<kmc::Setup::Seed> seeds_;

    std::map<sample::State, std::size_t> counts_{
        { sample::State::completed, 0 },
        { sample::State::failed, 0 },
    };

    std::atomic<std::size_t> next_sample_{};
    std::mutex counts_mutex_;
};
