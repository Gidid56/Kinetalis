#include "math/matrix.h"

#include <boost/ut.hpp>

#include <cmath>
#include <format>
#include <iterator>
#include <ranges>
#include <string_view>
#include <type_traits>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace math;

    template <
        std::size_t N,
        std::size_t M,
        typename Actual,
        typename Expected>
    void expect_matrix_equal(
        Matrix<N, M, Actual> const& actual,
        Matrix<N, M, Expected> const& expected,
        std::string_view context)
    {
        for (std::size_t i{}; i < N; ++i)
        {
            for (std::size_t j{}; j < M; ++j)
            {
                ut::expect(ut::that % actual[i][j] == expected[i][j])
                    << context << "; row " << i << ", column " << j;
            }
        }
    }

    template <std::size_t N, std::size_t M>
    void expect_matrix_close(
        Matrix<N, M, double> const& actual,
        Matrix<N, M, double> const& expected,
        double tolerance,
        std::string_view context)
    {
        for (std::size_t i{}; i < N; ++i)
        {
            for (std::size_t j{}; j < M; ++j)
            {
                const auto error{ std::abs(actual[i][j] - expected[i][j]) };

                ut::expect(ut::that % error <= tolerance)
                    << context
                    << "; row " << i << ", column " << j
                    << "; expected " << expected[i][j]
                    << ", actual " << actual[i][j]
                    << ", absolute error " << error
                    << ", tolerance " << tolerance;
            }
        }
    }

    void expect_close(
        double actual,
        double expected,
        double tolerance,
        std::string_view context)
    {
        const auto error{ std::abs(actual - expected) };

        ut::expect(ut::that % error <= tolerance)
            << context
            << "; expected " << expected
            << ", actual " << actual
            << ", absolute error " << error
            << ", tolerance " << tolerance;
    }

    ut::suite<"math.matrix"> matrix_tests = [] {
        "matrices construct from rows and deduce their element type"_test = [] {
            constexpr Matrix<2, 3, int> explicit_matrix{
                { 1, 2, 3 },
                { 4, 5, 6 },
            };

            constexpr Matrix deduced{
                { 1.0, 2.0 },
                { 3.0, 4.0 },
            };

            static_assert(decltype(deduced)::elements() == 4);
            static_assert(
                std::same_as<typename decltype(deduced)::Row::Type, double>);

            static_assert(explicit_matrix[0] == Vector{ 1, 2, 3 });
            static_assert(explicit_matrix[1] == Vector{ 4, 5, 6 });
            static_assert(deduced[1][0] == 3.0);
        };

        "rows and elements are accessible through the expected interfaces"_test = [] {
            constexpr Matrix<2, 3, int> matrix{
                { 1, 2, 3 },
                { 4, 5, 6 },
            };

            static_assert(matrix.elements() == 6);
            static_assert(matrix.x() == Vector{ 1, 2, 3 });
            static_assert(matrix.y() == Vector{ 4, 5, 6 });
            static_assert(matrix[0][1] == 2);
            static_assert(matrix[Axis::y][2] == 6);

            static_assert(std::ranges::contiguous_range<Matrix<2, 3, int>>);
            static_assert(std::ranges::sized_range<Matrix<2, 3, int>>);
        };

        "all and identity create useful matrices"_test = [] {
            static_assert(
                Matrix<2, 3, int>::all(4) ==
                Matrix<2, 3, int>{
                    { 4, 4, 4 },
                    { 4, 4, 4 },
                });

            static_assert(
                Matrix<3, 3, int>::identity() ==
                Matrix<3, 3, int>{
                    { 1, 0, 0 },
                    { 0, 1, 0 },
                    { 0, 0, 1 },
                });
        };

        "matrices expose row-major range storage"_test = [] {
            Matrix matrix{
                { 1, 2 },
                { 3, 4 },
            };

            const auto rows{ std::distance(matrix.begin(), matrix.end()) };
            ut::expect(ut::that % rows == 2)
                << "a 2-row matrix should expose two row iterators";

            matrix.data()[0][0] = 9;
            ut::expect(ut::that % matrix[0][0] == 9)
                << "mutating data() should mutate matrix storage";
        };

        "row and column access returns the requested slice"_test = [] {
            const Matrix matrix{
                { 1, 2, 3 },
                { 4, 5, 6 },
            };

            const auto row0{ matrix.row(0) };
            const auto row1{ matrix.row(1) };
            const auto column0{ matrix.column(0) };
            const auto column1{ matrix.column(1) };
            const auto column2{ matrix.column(2) };

            for (std::size_t i{}; i < 3; ++i)
            {
                ut::expect(ut::that % row0[i] == Vector{ 1, 2, 3 }[i])
                    << "row(0), component " << i;
                ut::expect(ut::that % row1[i] == Vector{ 4, 5, 6 }[i])
                    << "row(1), component " << i;
            }

            for (std::size_t i{}; i < 2; ++i)
            {
                ut::expect(ut::that % column0[i] == Vector{ 1, 4 }[i])
                    << "column(0), component " << i;
                ut::expect(ut::that % column1[i] == Vector{ 2, 5 }[i])
                    << "column(1), component " << i;
                ut::expect(ut::that % column2[i] == Vector{ 3, 6 }[i])
                    << "column(2), component " << i;
            }

            ut::expect(ut::throws<std::out_of_range>([&] {
                (void)matrix.row(2);
            })) << "row index 2 is outside a 2-row matrix";

            ut::expect(ut::throws<std::out_of_range>([&] {
                (void)matrix.column(3);
            })) << "column index 3 is outside a 3-column matrix";
        };

        "row and column reductions preserve matrix orientation"_test = [] {
            constexpr Matrix matrix{
                { 1, 2, 3 },
                { 4, 5, 6 },
            };

            static_assert(matrix.sum_rows() == Vector{ 6, 15 });
            static_assert(matrix.sum_columns() == Vector{ 5, 7, 9 });
            static_assert(matrix.products() == Vector{ 6, 120 });
            static_assert(matrix.means<double>() == Vector{ 2.0, 5.0 });
            static_assert(matrix.min() == 1);
            static_assert(matrix.max() == 6);
        };

        "row norms and normalized rows match familiar vectors"_test = [] {
            const Matrix<2> matrix{
                { 3.0, 4.0 },
                { 0.0, 5.0 },
            };

            const auto norms{ matrix.norms() };
            for (std::size_t i{}; i < 2; ++i)
            {
                ut::expect(ut::that % norms[i] == 5.0)
                    << "row norm " << i;
            }

            const auto normalized{ matrix.normalized_rows() };
            expect_matrix_close(
                normalized,
                Matrix<2>{
                    { 0.6, 0.8 },
                    { 0.0, 1.0 },
                },
                1E-12,
                "normalized rows");
        };

        "remove_row and remove_column create minors without mutating the source"_test = [] {
            constexpr Matrix matrix{
                { 1, 2, 3 },
                { 4, 5, 6 },
                { 7, 8, 9 },
            };

            static_assert(
                matrix.remove_row(1) ==
                Matrix<2, 3, int>{
                    { 1, 2, 3 },
                    { 7, 8, 9 },
                });

            static_assert(
                matrix.remove_column(1) ==
                Matrix<3, 2, int>{
                    { 1, 3 },
                    { 4, 6 },
                    { 7, 9 },
                });

            auto runtime_matrix{ matrix };

            ut::expect(ut::throws<std::out_of_range>([&] {
                (void)runtime_matrix.remove_row(3);
            })) << "row 3 is outside a 3-row matrix";

            ut::expect(ut::throws<std::out_of_range>([&] {
                (void)runtime_matrix.remove_column(3);
            })) << "column 3 is outside a 3-column matrix";
        };

        "transpose works for square and rectangular matrices"_test = [] {
            constexpr Matrix square{
                { 1, 2 },
                { 3, 4 },
            };

            static_assert(
                square.transposed() ==
                Matrix{
                    { 1, 3 },
                    { 2, 4 },
                });

            constexpr Matrix<2, 3, int> rectangular{
                { 1, 2, 3 },
                { 4, 5, 6 },
            };

            static_assert(
                rectangular.transposed() ==
                Matrix<3, 2, int>{
                    { 1, 4 },
                    { 2, 5 },
                    { 3, 6 },
                });

            auto in_place{ square };
            auto& result{ in_place.transpose() };

            ut::expect(ut::that % &result == &in_place)
                << "transpose() should return *this";
            expect_matrix_equal(
                in_place,
                square.transposed(),
                "in-place transpose should match transposed()");
        };

        "trace determinant and invertibility follow linear algebra"_test = [] {
            constexpr Matrix trace_matrix{
                { 1, 2, 3 },
                { 4, 5, 6 },
                { 7, 8, 9 },
            };
            static_assert(trace_matrix.trace() == 15);

            constexpr Matrix<1, 1, int> one{ { 7 } };
            constexpr Matrix two{
                { 1, 2 },
                { 3, 4 },
            };
            constexpr Matrix three{
                { 1, 2, 3 },
                { 4, 5, 6 },
                { 7, 8, 8 },
            };
            constexpr auto four{ Matrix<4, 4, int>::identity() };

            static_assert(one.determinant() == 7);
            static_assert(two.determinant() == -2);
            static_assert(three.determinant() == 3);
            static_assert(four.determinant() == 1);

            static_assert(Matrix<3, 3, int>::identity().is_invertible());
            static_assert(not Matrix<3, 3, int>{}.is_invertible());
        };

        "inverse reconstructs the identity matrix"_test = [] {
            const Matrix matrix{
                { 4.0, 7.0 },
                { 2.0, 6.0 },
            };

            const Matrix expected{
                { 0.6, -0.7 },
                { -0.2, 0.4 },
            };

            const auto inverse{ matrix.inverse() };

            expect_matrix_close(
                inverse,
                expected,
                1E-12,
                "inverse of [[4, 7], [2, 6]]");

            expect_matrix_close(
                matrix.dot(inverse),
                Matrix<2>::identity(),
                1E-12,
                "matrix * inverse should be identity");

            ut::expect(ut::throws<std::domain_error>([] {
                const Matrix singular{
                    { 1.0, 2.0 },
                    { 2.0, 4.0 },
                };
                (void)singular.inverse();
            })) << "singular matrices cannot be inverted";
        };

        "triangular diagonal and symmetry predicates classify common matrices"_test = [] {
            constexpr Matrix lower{
                { 1, 0, 0 },
                { 2, 3, 0 },
                { 4, 5, 6 },
            };

            constexpr Matrix upper{
                { 1, 2, 3 },
                { 0, 4, 5 },
                { 0, 0, 6 },
            };

            constexpr Matrix diagonal{
                { 1, 0, 0 },
                { 0, 2, 0 },
                { 0, 0, 3 },
            };

            constexpr Matrix symmetric{
                { 1, 2, 3 },
                { 2, 4, 5 },
                { 3, 5, 6 },
            };

            constexpr Matrix skew{
                { 0, 2, -1 },
                { -2, 0, 3 },
                { 1, -3, 0 },
            };

            static_assert(lower.is_lower_triangular());
            static_assert(not lower.is_upper_triangular());
            static_assert(upper.is_upper_triangular());
            static_assert(not upper.is_lower_triangular());
            static_assert(diagonal.is_diagonal());
            static_assert(symmetric.is_symmetric());
            static_assert(skew.is_skew_symmetric());
        };

        "matrix dot products implement matrix and vector multiplication"_test = [] {
            constexpr Matrix<2, 3, int> left{
                { 1, 2, 3 },
                { 4, 5, 6 },
            };

            constexpr Matrix<3, 2, int> right{
                { 7, 8 },
                { 9, 10 },
                { 11, 12 },
            };

            static_assert(
                left.dot(right) ==
                Matrix<2, 2, int>{
                    { 58, 64 },
                    { 139, 154 },
                });

            static_assert(left.dot(Vector{ 1, 2, 3 }) == Vector{ 14, 32 });
            static_assert(
                left.transpose_dot(Vector{ 2, 3 }) ==
                Vector{ 14, 19, 24 });
        };

        "approx_equal applies the supplied element tolerance"_test = [] {
            const Matrix left{
                { 1.0, 2.0 },
                { 3.0, 4.0 },
            };

            const Matrix right{
                { 1.001, 2.0 },
                { 3.0, 3.999 },
            };

            const auto loose{ left.approx_equal(right, 0.002) };
            const auto tight{ left.approx_equal(right, 0.0001) };

            ut::expect(ut::that % loose == true)
                << "left=" << std::format("{}", left)
                << ", right=" << std::format("{}", right)
                << ", epsilon=0.002";
            ut::expect(ut::that % tight == false)
                << "left=" << std::format("{}", left)
                << ", right=" << std::format("{}", right)
                << ", epsilon=0.0001";
        };

        "scalar and element-wise matrix arithmetic are distinct from dot"_test = [] {
            constexpr Matrix matrix{
                { 1, 2 },
                { 3, 4 },
            };

            static_assert(matrix + 1 == Matrix{ { 2, 3 }, { 4, 5 } });
            static_assert(matrix - 1 == Matrix{ { 0, 1 }, { 2, 3 } });
            static_assert(matrix * 2 == Matrix{ { 2, 4 }, { 6, 8 } });
            static_assert(2 * matrix == Matrix{ { 2, 4 }, { 6, 8 } });

            static_assert(
                matrix * Matrix{ { 2, 3 }, { 4, 5 } } ==
                Matrix{ { 2, 6 }, { 12, 20 } });

            static_assert(
                matrix * Vector{ 2, 3 } ==
                Matrix{ { 2, 4 }, { 9, 12 } });

            static_assert(
                matrix + Vector{ 10, 20 } ==
                Matrix{ { 11, 22 }, { 13, 24 } });
        };

        "compound assignment mutates matrix elements in place"_test = [] {
            Matrix matrix{
                { 1.0, 2.0 },
                { 3.0, 4.0 },
            };

            matrix += 1.0;
            matrix -= 1.0;
            matrix *= 2.0;
            matrix /= 2.0;

            expect_matrix_equal(
                matrix,
                Matrix{ { 1.0, 2.0 }, { 3.0, 4.0 } },
                "scalar compound assignments");

            matrix += Matrix{ { 1.0, 1.0 }, { 1.0, 1.0 } };
            matrix -= Matrix{ { 1.0, 1.0 }, { 1.0, 1.0 } };

            matrix *= Vector{ 2.0, 3.0 };
            expect_matrix_equal(
                matrix,
                Matrix{ { 2.0, 4.0 }, { 9.0, 12.0 } },
                "matrix *= row-scale vector");

            matrix /= Vector{ 2.0, 3.0 };
            expect_matrix_equal(
                matrix,
                Matrix{ { 1.0, 2.0 }, { 3.0, 4.0 } },
                "matrix /= row-scale vector");
        };

        "absolute value floor ceil and round apply element-wise"_test = [] {
            expect_matrix_equal(
                abs(Matrix{ { -1.0, 2.0 }, { -3.0, 4.0 } }),
                Matrix{ { 1.0, 2.0 }, { 3.0, 4.0 } },
                "abs(matrix)");

            expect_matrix_equal(
                floor(Matrix{ { 1.2, -1.2 }, { 2.9, -2.9 } }),
                Matrix{ { 1.0, -2.0 }, { 2.0, -3.0 } },
                "floor(matrix)");

            expect_matrix_equal(
                ceil(Matrix{ { 1.2, -1.2 }, { 2.1, -2.1 } }),
                Matrix{ { 2.0, -1.0 }, { 3.0, -2.0 } },
                "ceil(matrix)");

            expect_matrix_equal(
                round(Matrix{ { 1.4, 1.5 }, { -1.4, -1.5 } }),
                Matrix{ { 1.0, 2.0 }, { -1.0, -2.0 } },
                "round(matrix)");
        };

        "look_at creates an orthonormal basis with the requested up direction"_test = [] {
            const auto identity{
                Matrix<3>::look_at(Vector{ 0.0, 0.0, 1.0 })
            };

            expect_matrix_close(
                identity,
                Matrix<3>::identity(),
                1E-12,
                "look_at along +z");

            const auto basis{
                Matrix<3>::look_at(Vector{ 1.0, 2.0, 3.0 })
            };

            expect_close(basis.x().norm(), 1.0, 1E-12, "look_at x-axis norm");
            expect_close(basis.y().norm(), 1.0, 1E-12, "look_at y-axis norm");
            expect_close(basis.z().norm(), 1.0, 1E-12, "look_at z-axis norm");

            expect_close(
                basis.x().dot(basis.y()),
                0.0,
                1E-12,
                "look_at x dot y");
            expect_close(
                basis.x().dot(basis.z()),
                0.0,
                1E-12,
                "look_at x dot z");
            expect_close(
                basis.y().dot(basis.z()),
                0.0,
                1E-12,
                "look_at y dot z");

            const auto expected_up{
                Vector{ 1.0, 2.0, 3.0 }.normalized()
            };
            const auto matches_up{
                basis.z().approx_equal(expected_up, 1E-12)
            };

            ut::expect(ut::that % matches_up == true)
                << "look_at z axis=" << std::format("{}", basis.z())
                << ", expected normalized up="
                << std::format("{}", expected_up);
        };
    };
}
