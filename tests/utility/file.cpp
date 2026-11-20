#include "utility/file.h"

#include <boost/ut.hpp>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    ut::suite<"utility.file"> file_tests = [] {
        "failed file opens respect the retry deadline"_test = [] {
            // A directory cannot be opened as an output file. Zero timeout
            // exercises the failure path without delaying the test suite.
            auto const directory{ std::filesystem::temp_directory_path() };
            ut::expect(ut::throws<std::runtime_error>([&] {
                (void)utility::file::wait_for_open<std::ofstream>(
                    directory, std::ios_base::out, std::chrono::steady_clock::duration::zero());
            }));
        };
    };
}
