#include "pch.h"

#include "terminal.h"

#include "runner.h"

#include <string>

using namespace batch;

Terminal::Terminal(Runner const& runner)
    : runner_{ runner },
      entries_(runner.sample_count()),
      counts_{
          { sample::State::waiting, runner.sample_count() },
          { sample::State::running, 0 },
          { sample::State::completed, 0 },
          { sample::State::failed, 0 },
      }
{ }

void Terminal::report(std::size_t row, sample::State state, std::string text)
{
    std::scoped_lock lock{ mutex_ };

    auto& entry{ entries_.at(row) };
    auto const now{ ProgressClock::now() };

    --counts_.at(entry.state);
    ++counts_.at(state);

    entry.state = state;

    switch (state)
    {
        case sample::State::running:
            entry.name = std::move(text);
            entry.started = now;
            break;

        case sample::State::completed: entry.elapsed = now - entry.started; break;

        case sample::State::failed:
            entry.elapsed = now - entry.started;
            entry.message = std::move(text);
            break;

        default: break;
    }

    render();
}

std::string Terminal::Entry::format_elapsed() const
{
    auto const duration{ state == sample::State::running ? ProgressClock::now() - started
                                                         : elapsed };
    auto const seconds{ std::chrono::duration_cast<std::chrono::seconds>(duration).count() };

    return std::format("{:02}:{:02}", seconds / 60, seconds % 60);
}

std::string Terminal::Entry::line() const
{
    auto text{ std::format(
        "{:>9}  {}  {:<9}",
        name.empty() ? "pending" : name,
        state == sample::State::waiting ? "--:--" : format_elapsed(),
        enum_name(state)) };

    if (not message.empty())
        std::format_to(std::back_inserter(text), "  ({})", message);

    constexpr std::size_t max_line_length{ 120 };
    if (text.size() > max_line_length)
        text.replace(max_line_length - 3, std::string::npos, "...");

    return text;
}

void Terminal::render()
{
    constexpr std::size_t max_failed{ 5 };

    std::vector<std::string> lines;
    lines.reserve(runner_.worker_count() + max_failed + 5);

    lines.emplace_back(std::format("{:>9}  {}  {:<9}", "sample", "mm:ss", "state"));

    // Most recent failed samples
    auto failed_entries{ entries_ | std::views::filter([](Entry const& entry) {
        return entry.state == sample::State::failed;
    }) };
    auto const failed{ counts_[sample::State::failed] };

    auto const failed_skip{ failed > max_failed ? failed - max_failed : 0 };

    for (auto const& entry : failed_entries | std::views::drop(failed_skip))
        lines.emplace_back(entry.line());

    if (failed > max_failed)
        lines.emplace_back(std::format("... {} earlier failures", failed - max_failed));

    if (failed > 0)
        lines.emplace_back("");

    // Running samples
    for (auto const& entry : entries_)
        if (entry.state == sample::State::running)
            lines.emplace_back(entry.line());

    lines.emplace_back("");
    lines.emplace_back("");

    // Overall state
    for (auto& [state, count] : counts_)
        lines.back() += std::format("{}: {}   ", enum_name(state), count);

    // Return to the beginning of the previous rendering
    if (rendered_lines_ > 0)
        std::print("\x1b[{}F", rendered_lines_);

    std::print("\x1b[?25l");

    // Draw current rendering
    for (auto const& line : lines)
        std::print("\x1b[2K{}\n", line);

    // Clear remnants of a previously taller rendering
    if (rendered_lines_ > lines.size())
    {
        auto const surplus{ rendered_lines_ - lines.size() };

        for (std::size_t line{}; line < surplus; ++line)
            std::print("\x1b[2K\n");

        std::print("\x1b[{}F", surplus);
    }

    std::print("\x1b[?25h");
    std::fflush(stdout);

    rendered_lines_ = lines.size();
}
