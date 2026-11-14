#pragma once

namespace batch
{
    template <class Rules>
    struct Campaign;

    template <class Rules>
    class Runner;
    class Terminal;

    namespace sample
    {
        enum class State
        {
            waiting,
            running,
            completed,
            failed,
        };
    }

    namespace statistics
    {
        class Aggregator;
        class Merger;
        class Samples;
    }

    using WallClock = std::chrono::system_clock;
    using ProgressClock = std::chrono::steady_clock;

    using Postprocessor = std::function<void(std::filesystem::path const&)>;
    template <class Rules>
    using CampaignFactory = std::function<Campaign<Rules>()>;
}
