#pragma once

namespace batch
{
    struct Options
    {
        std::filesystem::path results_directory{ "results" };
        std::size_t concurrency_count{ 16 };
        bool show_help{};
    };

    int run(Options const&);
}

namespace batch::options
{
    [[nodiscard]] std::string_view next_value(
        std::span<char* const> const arguments,
        std::size_t& index,
        std::string_view const option)
    {
        if (++index == arguments.size())
            throw std::invalid_argument{ std::format("{} requires a value", option) };

        return arguments[index];
    }

    [[nodiscard]] std::size_t
    parse_positive_integer(std::string_view const value, std::string_view const option)
    {
        std::size_t result{};
        auto const end = value.data() + value.size();
        auto const [position, error] = std::from_chars(value.data(), end, result);

        if (error != std::errc{} or position != end or result == 0)
            throw std::invalid_argument{
                std::format("{} requires a positive integer, got '{}'", option, value)
            };

        return result;
    }

    [[nodiscard]] batch::Options parse(std::span<char* const> const arguments)
    {
        batch::Options options;

        for (std::size_t i{ 1 }; i < arguments.size(); ++i)
        {
            std::string_view const argument{ arguments[i] };

            if (argument == "--results-dir")
            {
                const auto home{ std::getenv("HOME") };

                if (not home)
                    throw std::runtime_error{ "HOME environment variable is not set." };

                auto path{ std::filesystem::path{ next_value(arguments, i, argument) } };

                if (path.is_relative())
                    path = std::filesystem::path{ home } / path;

                options.results_directory = path.lexically_normal();

            }

            else if (argument == "--concurrency")
                options.concurrency_count =
                    parse_positive_integer(next_value(arguments, i, argument), argument);

            else if (argument == "--help" or argument == "-h")
                options.show_help = true;

            else
                throw std::invalid_argument{ std::format("Unknown argument: '{}'", argument) };
        }

        return options;
    }

    inline constexpr auto help_text{
        "Usage: kinetalis_batch [options]\n"
        "\n"
        "Options:\n"
        "  --results-dir <path>   Results directory; default: {}\n"
        "  --concurrency <count>  Concurrent simulations; default: {}\n"
        "  -h, --help             Show this help"
    };
}
