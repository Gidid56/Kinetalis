#pragma once
#include <array>
#include <chrono>
#include <cstdint>
#include <print>
#include <string_view>

namespace kmc_perf
{
    enum class Stage : std::size_t
    {
        Select, Evaluate, Update, Enumerate, Classify, Block, Count
    };

    struct Stat
    {
        std::uint64_t calls{};
        std::uint64_t ns{};
    };

    inline std::array<Stat, static_cast<std::size_t>(Stage::Count)> stats{};

    inline void reset() { stats = {}; }

    struct Scope
    {
        Stage stage;
        std::chrono::steady_clock::time_point start{ std::chrono::steady_clock::now() };

        explicit Scope(Stage stage) : stage{ stage } {}

        ~Scope()
        {
            auto const elapsed{ std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now() - start).count() };
            auto& stat{ stats[static_cast<std::size_t>(stage)] };
            ++stat.calls;
            stat.ns += static_cast<std::uint64_t>(elapsed);
        }
    };

    inline void print()
    {
        constexpr std::array<std::string_view, static_cast<std::size_t>(Stage::Count)> names{
            "select", "evaluate", "update", "enumerate", "classify", "block"
        };
        for (std::size_t i{}; i < stats.size(); ++i)
            std::println("TRACE stage={} calls={} ns={}", names[i], stats[i].calls, stats[i].ns);
    }
}

#ifdef KINETALIS_PERF_TRACE
#define KMC_PERF_SCOPE(stage) ::kmc_perf::Scope kmc_perf_scope{ ::kmc_perf::Stage::stage }
#else
#define KMC_PERF_SCOPE(stage)
#endif
