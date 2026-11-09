#pragma once
#include "types.h"
#include "kmc/setup.h"

#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace batch
{
    struct Campaign
    {
        std::string name{};
        std::string seed{};
        std::size_t replicate_count{ 1 };
        std::vector<kmc::Setup> setups;
        kmc::PlanFactory analysis_plan;
        Postprocessor postprocessor;

        constexpr std::size_t sample_count() const { return setups.size() * replicate_count; }
    };

    namespace campaign
    {
        void aggregate(std::filesystem::path const& campaign_path);
        void merge(std::filesystem::path const& campaign_path, std::string_view dataset);
        void summarize(std::filesystem::path const& campaign_path, std::string_view dataset);
    }
}
