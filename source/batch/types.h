#pragma once

namespace batch
{
    struct Campaign;

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
    using CampaignFactory = std::function<Campaign()>;
}
