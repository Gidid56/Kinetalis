#include "math/math.h"

#include <boost/ut.hpp>

#include <format>
#include <string>
#include <string_view>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace math;

    ut::suite<"math.format"> format_tests = [] {
        "vectors format like compact coordinate lists"_test = [] {
            const auto integers{ std::format("{}", Vector{ 1, 2, 3 }) };
            const auto decimals{ std::format("{}", Vector{ 1.5, 2.5 }) };

            ut::expect(ut::that % integers == std::string{ "[1, 2, 3]" })
                << "default integer vector formatting";
            ut::expect(ut::that % decimals == std::string{ "[1.5, 2.5]" })
                << "default floating-point vector formatting";
        };

        "vector element formatting is forwarded"_test = [] {
            const auto fixed{ std::format("{:.2f}", Vector{ 1.234, 5.678 }) };
            const auto signed_width{ std::format("{:+04d}", Vector{ 1, -2 }) };

            ut::expect(ut::that % fixed == std::string{ "[1.23, 5.68]" })
                << "floating-point format specification should be applied to every element";
            ut::expect(ut::that % signed_width == std::string{ "[+001, -002]" })
                << "integer format specification should be applied to every element";
        };

        "matrices format one row per line"_test = [] {
            const auto text{ std::format("{}", Matrix{ { 1, 2 }, { 3, 4 } }) };

            ut::expect(
                ut::that % text ==
                std::string{ "[[1, 2]\n [3, 4]]" })
                << "default 2x2 matrix formatting";
        };

        "matrix element formatting is forwarded"_test = [] {
            const auto text{
                std::format("{:.1f}", Matrix{ { 1.25, 2.5 }, { 3.75, 4.0 } })
            };

            ut::expect(
                ut::that % text ==
                std::string{ "[[1.2, 2.5]\n [3.8, 4.0]]" })
                << "matrix formatter should forward the element format specification";
        };
    };
}
