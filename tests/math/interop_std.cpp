#include "math/math.h"

#include <boost/ut.hpp>

#include <algorithm>
#include <cmath>
#include <complex>
#include <iterator>
#include <ranges>
#include <span>
#include <string_view>
#include <type_traits>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace math;
    using Complex = std::complex<double>;

    template <class T>
    concept RuntimePowerable = requires(T value) {
        math::power(value, 2);
    };

    template <std::size_t N, typename Actual, typename Expected>
    void expect_vector_equal(
        Vector<N, Actual> const& actual,
        Vector<N, Expected> const& expected,
        std::string_view context)
    {
        for (std::size_t i{}; i < N; ++i)
        {
            ut::expect(ut::that % actual[i] == expected[i])
                << context << "; component " << i;
        }
    }

    void expect_complex_equal(
        Complex actual,
        Complex expected,
        std::string_view context)
    {
        ut::expect(ut::that % actual.real() == expected.real())
            << context << "; real component";
        ut::expect(ut::that % actual.imag() == expected.imag())
            << context << "; imaginary component";
    }

    void expect_complex_close(
        Complex actual,
        Complex expected,
        double tolerance,
        std::string_view context)
    {
        const auto real_error{ std::abs(actual.real() - expected.real()) };
        const auto imag_error{ std::abs(actual.imag() - expected.imag()) };

        ut::expect(ut::that % real_error <= tolerance)
            << context
            << "; real component; expected " << expected.real()
            << ", actual " << actual.real()
            << ", error " << real_error;

        ut::expect(ut::that % imag_error <= tolerance)
            << context
            << "; imaginary component; expected " << expected.imag()
            << ", actual " << actual.imag()
            << ", error " << imag_error;
    }

    ut::suite<"math.interop.std"> standard_library_tests = [] {
        "built-in arithmetic types interoperate without adapters"_test = [] {
            static_assert(Arithmetic<int>);
            static_assert(Arithmetic<float>);
            static_assert(Arithmetic<double>);
            static_assert(Arithmetic<long double>);

            constexpr Vector mixed{ 1, 2.0F, 3.0 };
            static_assert(decltype(mixed)::size() == 3);
            static_assert(
                std::same_as<typename decltype(mixed)::Type, double>);

            const auto scaled{ Vector<3, int>{ 1, 2, 3 } * 0.5F };
            static_assert(
                std::same_as<
                    std::remove_cvref_t<decltype(scaled)>,
                    Vector<3, float>>);

            expect_vector_equal(
                scaled,
                Vector<3, float>{ 0.5F, 1.0F, 1.5F },
                "integer vector scaled by float");
        };

        "vectors and matrices participate in STL range algorithms"_test = [] {
            static_assert(std::ranges::contiguous_range<Vector<4, int>>);
            static_assert(std::ranges::random_access_range<Vector<4, int>>);
            static_assert(std::ranges::contiguous_range<Matrix<2, 2, int>>);

            Vector values{ 4, 1, 3, 2 };
            std::ranges::sort(values);

            expect_vector_equal(
                values,
                Vector{ 1, 2, 3, 4 },
                "std::ranges::sort(vector)");

            std::span vector_span{ values };
            ut::expect(ut::that % vector_span.size() == 4u)
                << "span over Vector<4>";

            Matrix matrix{ { 1, 2 }, { 3, 4 } };
            const auto rows{ std::distance(matrix.begin(), matrix.end()) };
            ut::expect(ut::that % rows == 2)
                << "distance between matrix row iterators";
        };

        "compile-time algebra works with std::complex even though Arithmetic does not"_test = [] {
            static_assert(not Arithmetic<Complex>);
            static_assert(not RuntimePowerable<Complex>);

            const Complex value{ 1.0, 2.0 };

            expect_complex_equal(
                square(value),
                Complex{ -3.0, 4.0 },
                "square(1+2i)");

            expect_complex_equal(
                cube(value),
                Complex{ -11.0, -2.0 },
                "cube(1+2i)");

            expect_complex_equal(
                power<2>(value),
                Complex{ -3.0, 4.0 },
                "power<2>(1+2i)");

            expect_complex_equal(
                power<3>(value),
                Complex{ -11.0, -2.0 },
                "power<3>(1+2i)");
        };

        "complex vectors support bilinear vector algebra"_test = [] {
            const Vector<3, Complex> left{
                Complex{ 1.0, 1.0 },
                Complex{ 2.0, 0.0 },
                Complex{ 0.0, 1.0 },
            };

            const Vector<3, Complex> right{
                Complex{ 1.0, 0.0 },
                Complex{ 0.0, 1.0 },
                Complex{ 2.0, 0.0 },
            };

            expect_complex_equal(
                left.sum(),
                Complex{ 3.0, 2.0 },
                "complex vector sum");

            expect_complex_equal(
                left.dot(right),
                Complex{ 1.0, 5.0 },
                "complex bilinear dot product");

            const auto scaled{ left * Complex{ 0.0, 1.0 } };
            expect_complex_equal(
                scaled[0],
                Complex{ -1.0, 1.0 },
                "complex vector scaling component 0");
            expect_complex_equal(
                scaled[1],
                Complex{ 0.0, 2.0 },
                "complex vector scaling component 1");
            expect_complex_equal(
                scaled[2],
                Complex{ -1.0, 0.0 },
                "complex vector scaling component 2");

            const auto cross{ left.cross(right) };
            ut::expect(ut::that % cross.size() == 3u)
                << "3D complex cross product should remain 3D";
        };

        "complex matrices support determinant and inverse"_test = [] {
            const Matrix<2, 2, Complex> matrix{
                { Complex{ 1.0, 1.0 }, Complex{} },
                { Complex{}, Complex{ 2.0, -1.0 } },
            };

            expect_complex_equal(
                matrix.determinant(),
                Complex{ 3.0, 1.0 },
                "complex matrix determinant");

            const auto inverse{ matrix.inverse() };
            const auto identity{ matrix.dot(inverse) };

            expect_complex_close(
                identity[0][0],
                Complex{ 1.0, 0.0 },
                1E-12,
                "complex matrix inverse identity [0][0]");
            expect_complex_close(
                identity[0][1],
                Complex{},
                1E-12,
                "complex matrix inverse identity [0][1]");
            expect_complex_close(
                identity[1][0],
                Complex{},
                1E-12,
                "complex matrix inverse identity [1][0]");
            expect_complex_close(
                identity[1][1],
                Complex{ 1.0, 0.0 },
                1E-12,
                "complex matrix inverse identity [1][1]");
        };
    };
}
