#pragma once
#include "batch/types.h"

class batch::Terminal
{
public:
    explicit Terminal(std::size_t sample_count, std::size_t worker_count);

    void render();

    void report(std::size_t row, sample::State state, std::string text = {});

private:
    struct Entry
    {
        std::string name;
        std::string message;
        sample::State state{ sample::State::waiting };
        ProgressClock::time_point started{};
        ProgressClock::duration elapsed{};

        std::string format_elapsed() const;
        std::string line() const;
    };

    std::size_t sample_count_, worker_count_;

    std::mutex mutex_;
    std::vector<Entry> entries_;
    std::size_t rendered_lines_{};
    std::map<sample::State, std::size_t> counts_;
};
