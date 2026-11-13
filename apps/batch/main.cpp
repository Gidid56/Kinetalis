#include "pch.h"

#include "batch/runner.h"
#include "dealloying/campaigns.h"
#include "options.h"

using namespace studies;

int batch::run(Options const& options)
{
    std::println("Starting batch processing...");
    std::println("Concurrency = {}", options.concurrency_count);
    std::println("Working directory = {}", std::filesystem::current_path().string());
    std::println("Results directory = {}", options.results_directory.string());

    batch::Runner<dealloying::Rules> batch{ options.results_directory, options.concurrency_count };

    auto const successful = batch.run(dealloying::campaigns::pore_surface_morphology);

    return successful ? EXIT_SUCCESS : EXIT_FAILURE;
}

int main(int const argc, char* argv[])
{
    try
    {
        auto const options = batch::options::parse({ argv, static_cast<std::size_t>(argc) });

        if (options.show_help)
        {
            std::println(
                batch::options::help_text,
                batch::Options{}.results_directory.string(),
                batch::Options{}.concurrency_count);
            return EXIT_SUCCESS;
        }

        return batch::run(options);
    }
    catch (std::exception const& exception)
    {
        std::println(stderr, "Error: {}", exception.what());

        return EXIT_FAILURE;
    }
}
