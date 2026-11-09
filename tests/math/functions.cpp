#include "math/functions.h"

#include <boost/ut.hpp>

#include <cmath>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace math;

    template <std::floating_point Actual, std::floating_point Expected>
    void expect_close(
        Actual actual,
        Expected expected,
        long double tolerance,
        std::string_view context)
    {
        const auto error{
            std::abs(
                static_cast<long double>(actual) -
                static_cast<long double>(expected))
        };

        ut::expect(ut::that % error <= tolerance)
            << context
            << "; expected " << expected
            << ", actual " << actual
            << ", absolute error " << error
            << ", tolerance " << tolerance;
    }

    ut::suite<"math.functions"> function_tests = [] {
        "Arithmetic identifies built-in arithmetic values"_test = [] {
            static_assert(Arithmetic<bool>);
            static_assert(Arithmetic<char>);
            static_assert(Arithmetic<int>);
            static_assert(Arithmetic<float>);
            static_assert(Arithmetic<double>);
            static_assert(Arithmetic<long double>);

            static_assert(
                not Arithmetic<std::string_view>,
                "text should not satisfy the Arithmetic concept");
        };

        "zero parity and sign helpers behave naturally"_test = [] {
            static_assert(is_zero(0));
            static_assert(is_zero(0.0));
            static_assert(not is_zero(1));
            static_assert(is_nonzero(-1));
            static_assert(not is_nonzero(0));

            static_assert(is_odd(-3));
            static_assert(is_odd(3));
            static_assert(not is_odd(2));

            static_assert(is_even(-2));
            static_assert(is_even(0));
            static_assert(not is_even(3));

            static_assert(binary_sign(-1) == -1);
            static_assert(binary_sign(0) == 1);
            static_assert(binary_sign(1) == 1);

            static_assert(sign(-3) == -1);
            static_assert(sign(0) == 0);
            static_assert(sign(3) == 1);

            static_assert(math::abs(-3) == 3);
            static_assert(math::abs(0) == 0);
            static_assert(math::abs(3) == 3);
        };

        "identity square and cube preserve useful result types"_test = [] {
            static_assert(identity(42) == 42);
            static_assert(square(-3) == 9);
            static_assert(cube(-3) == -27);

            static_assert(
                std::same_as<decltype(square(2.0F)), float>,
                "squaring a float should stay float");
            static_assert(
                std::same_as<decltype(cube(2.0L)), long double>,
                "cubing a long double should stay long double");
        };

        "runtime power handles positive zero and negative exponents"_test = [] {
            static_assert(power(2, 0) == 1);
            static_assert(power(2, 1) == 2);
            static_assert(power(2, 10) == 1024);
            static_assert(power(2, 10u) == 1024);
            static_assert(power(-2, 3) == -8);
            static_assert(power(-2, 4) == 16);
            static_assert(power(2.0, -2) == 0.25);
            static_assert(power(4.0F, -1) == 0.25F);
        };

        "compile-time power changes the result type only when necessary"_test = [] {
            static_assert(power<0>(2.0) == 1);
            static_assert(power<1>(2.0) == 2.0);
            static_assert(power<3>(2.0) == 8.0);
            static_assert(power<-2>(2.0) == 0.25);

            static_assert(
                std::same_as<decltype(power<0>(2.0F)), int>,
                "x^0 is the integer multiplicative identity");
            static_assert(
                std::same_as<decltype(power<2>(2.0F)), float>,
                "positive powers should preserve the base type");
            static_assert(
                std::same_as<decltype(power<-1>(2.0F)), double>,
                "negative compile-time powers use a floating reciprocal");
        };

        "round converts finite floating values to integers"_test = [] {
            const auto one{ round<int>(1.4) };
            const auto two{ round<int>(1.5) };
            const auto minus_two{ round<int>(-1.5) };
            const auto forty_two{ round<long long>(42.0) };

            ut::expect(ut::that % one == 1) << "round<int>(1.4)";
            ut::expect(ut::that % two == 2) << "round<int>(1.5)";
            ut::expect(ut::that % minus_two == -2) << "round<int>(-1.5)";
            ut::expect(ut::that % forty_two == 42LL) << "round<long long>(42.0)";

            ut::expect(ut::throws<std::domain_error>([] {
                (void)round<int>(std::numeric_limits<double>::infinity());
            })) << "converting infinity to an integer should fail";

            ut::expect(ut::throws<std::domain_error>([] {
                (void)round<int>(std::numeric_limits<double>::quiet_NaN());
            })) << "converting NaN to an integer should fail";

            ut::expect(ut::throws<std::out_of_range>([] {
                (void)round<std::int8_t>(128.0);
            })) << "a rounded value outside the target integer range should fail";
        };

        "round_to rounds to a requested precision"_test = [] {
            expect_close(round_to(1.24, 0.1), 1.2, 1E-12L, "1.24 to tenths");
            expect_close(round_to(1.25, 0.1), 1.3, 1E-12L, "1.25 to tenths");
            expect_close(round_to(-1.25, 0.1), -1.3, 1E-12L, "-1.25 to tenths");

            expect_close(round_to<0>(1.6), 2.0, 1E-12L, "1.6 to units");
            expect_close(round_to<1>(1.26), 1.3, 1E-12L, "1.26 to one decimal");
            expect_close(round_to<2>(1.234), 1.23, 1E-12L, "1.234 to two decimals");
            expect_close(round_to<-1>(14.0), 10.0, 1E-12L, "14 to tens");

            ut::expect(ut::throws<std::invalid_argument>([] {
                (void)round_to(1.0, 0.0);
            })) << "zero precision should fail";

            ut::expect(ut::throws<std::invalid_argument>([] {
                (void)round_to(1.0, -1.0);
            })) << "negative precision should fail";

            ut::expect(ut::throws<std::invalid_argument>([] {
                (void)round_to(
                    1.0,
                    std::numeric_limits<double>::infinity());
            })) << "non-finite precision should fail";
        };

        "Axis reports whether a Cartesian coordinate exists in a dimension"_test = [] {
            static_assert(not is_dim<0>(Axis::none));

            static_assert(is_dim<1>(Axis::x));
            static_assert(not is_dim<1>(Axis::y));

            static_assert(is_dim<2>(Axis::x));
            static_assert(is_dim<2>(Axis::y));
            static_assert(not is_dim<2>(Axis::z));

            static_assert(is_dim<3>(Axis::x));
            static_assert(is_dim<3>(Axis::y));
            static_assert(is_dim<3>(Axis::z));
            static_assert(not is_dim<3>(Axis::w));

            static_assert(is_dim<4>(Axis::w));
            static_assert(not is_dim<4>(Axis::r2));
            static_assert(not is_dim<4>(Axis::r3));
        };

        "sign and ordinal text helpers produce familiar notation"_test = [] {
            static_assert(symbol(Sign::negative) == "-");
            static_assert(symbol(Sign::none).empty());
            static_assert(symbol(Sign::positive) == "+");

            static_assert(ordinal_suffix(1) == "st");
            static_assert(ordinal_suffix(2) == "nd");
            static_assert(ordinal_suffix(3) == "rd");
            static_assert(ordinal_suffix(4) == "th");
            static_assert(ordinal_suffix(11) == "th");
            static_assert(ordinal_suffix(12) == "th");
            static_assert(ordinal_suffix(13) == "th");
            static_assert(ordinal_suffix(21) == "st");
            static_assert(ordinal_suffix(22) == "nd");
            static_assert(ordinal_suffix(23) == "rd");
            static_assert(ordinal_suffix(111) == "th");
            static_assert(ordinal_suffix(-22) == "nd");

            const auto first{ ordinal(1) };
            const auto twelfth{ ordinal(12) };
            const auto twenty_third{ ordinal(23) };
            const auto negative_third{ ordinal(-3) };

            ut::expect(ut::that % first == std::string{ "1st" });
            ut::expect(ut::that % twelfth == std::string{ "12th" });
            ut::expect(ut::that % twenty_third == std::string{ "23rd" });
            ut::expect(ut::that % negative_third == std::string{ "-3rd" });
        };
    };
}
