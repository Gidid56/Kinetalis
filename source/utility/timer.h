#pragma once
#include <chrono>
#include <format>

namespace utility
{
    class Timer
    {
    public:
        using Clock = std::chrono::steady_clock;

        explicit Timer(bool start = false)
        {
            if (start)
                this->start();
        }

        bool is_running() const noexcept { return running_; }

        void set(Timer const& other) noexcept { *this = other; }

        void start() noexcept
        {
            if (running_)
                return;

            start_ = Clock::now();
            running_ = true;
        }

        void stop() noexcept
        {
            if (not running_)
                return;

            duration_ += Clock::now() - start_;
            running_ = false;
        }

        Clock::duration duration() const noexcept
        {
            if (running_)
                return duration_ + Clock::now() - start_;
            return duration_;
        }

    private:
        bool running_{ false };
        Clock::time_point start_{};
        Clock::duration duration_{};
    };

    class Guard
    {
    public:
        explicit Guard(Timer& timer) : timer_{ timer }, owns_run_{ not timer.is_running() }
        {
            if (owns_run_)
                timer_.start();
        }

        Guard(Guard const&) = delete;
        Guard& operator=(Guard const&) = delete;
        Guard(Guard&&) = delete;
        Guard& operator=(Guard&&) = delete;

        ~Guard()
        {
            if (owns_run_)
                timer_.stop();
        }

    private:
        Timer& timer_;
        bool owns_run_;
    };
}
