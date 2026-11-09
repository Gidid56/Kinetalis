#include "math/vector.h"

#include <boost/ut.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <format>
#include <ranges>
#include <span>
#include <string_view>
#include <type_traits>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace math;

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

    ut::suite<"math.vector"> vector_tests = [] {
        "vectors construct naturally and deduce a common element type"_test = [] {
            constexpr Vector<3, int> explicit_vector{ 1, 2, 3 };
            constexpr Vector deduced{ 1, 2.0, 3.0F };

            static_assert(decltype(deduced)::size() == 3);
            static_assert(
                std::same_as<typename decltype(deduced)::Type, double>,
                "mixed arithmetic elements should use their common type");

            static_assert(explicit_vector == Vector<3, int>{ 1, 2, 3 });
            static_assert(deduced == Vector<3, double>{ 1.0, 2.0, 3.0 });

            constexpr std::array data{ 4, 5, 6 };
            static_assert(
                Vector<3, int>{ data } == Vector<3, int>{ 4, 5, 6 });
        };

        "Cartesian components can be accessed by name index or Axis"_test = [] {
            constexpr Vector vector{ 1, 2, 3, 4 };

            static_assert(vector.size() == 4);
            static_assert(vector.x() == 1);
            static_assert(vector.y() == 2);
            static_assert(vector.z() == 3);
            static_assert(vector.w() == 4);

            static_assert(vector[0] == 1);
            static_assert(vector[Axis::x] == 1);
            static_assert(vector[Axis::y] == 2);
            static_assert(vector[Axis::z] == 3);
            static_assert(vector[Axis::w] == 4);

            static_assert(Vector<4, int>::axis_index(Axis::x) == 0);
            static_assert(Vector<4, int>::axis_index(Axis::w) == 3);

            ut::expect(ut::throws<std::invalid_argument>([] {
                (void)Vector<3, int>::axis_index(Axis::r3);
            })) << "radius flags are not Cartesian coordinate axes";

            ut::expect(ut::throws<std::out_of_range>([] {
                (void)Vector<2, int>::axis_index(Axis::z);
            })) << "z does not exist in a 2D vector";
        };

        "factory vectors produce expected basis directions"_test = [] {
            using V2 = Vector<2, int>;
            using V3 = Vector<3, int>;
            using V4 = Vector<4, int>;

            static_assert(V3::all(7) == V3{ 7, 7, 7 });
            static_assert(V2::unit_x() == V2{ 1, 0 });
            static_assert(V2::unit_y() == V2{ 0, 1 });
            static_assert(V3::unit_x() == V3{ 1, 0, 0 });
            static_assert(V3::unit_y() == V3{ 0, 1, 0 });
            static_assert(V3::unit_z() == V3{ 0, 0, 1 });
            static_assert(V4::unit_w() == V4{ 0, 0, 0, 1 });
        };

        "vectors behave as contiguous STL ranges"_test = [] {
            static_assert(std::ranges::contiguous_range<Vector<3, int>>);
            static_assert(std::ranges::random_access_range<Vector<3, int>>);
            static_assert(std::ranges::sized_range<Vector<3, int>>);

            Vector vector{ 3, 1, 2 };
            std::ranges::sort(vector);

            expect_vector_equal(
                vector,
                Vector{ 1, 2, 3 },
                "std::ranges::sort should reorder vector elements");

            std::span span{ vector };
            ut::expect(ut::that % span.size() == 3u)
                << "span should expose all vector elements";
            ut::expect(ut::that % span.data() == vector.data().data())
                << "span should refer to the vector's contiguous storage";

            vector.data()[0] = 9;
            ut::expect(ut::that % vector.x() == 9)
                << "mutating data() should mutate the vector";
        };

        "reductions produce familiar sums products means and extrema"_test = [] {
            constexpr Vector vector{ 4, 5, 6 };

            static_assert(vector.partial_sum<0, 2>() == 9);
            static_assert(vector.partial_sum<1, 3>() == 11);
            static_assert(vector.sum() == 15);

            static_assert(vector.partial_product<0, 2>() == 20);
            static_assert(vector.partial_product<1, 3>() == 30);
            static_assert(vector.product() == 120);

            static_assert(vector.mean<int>() == 5);
            static_assert(vector.mean<double>() == 5.0);

            static_assert(vector.min() == 4);
            static_assert(vector.max() == 6);

            static_assert(Vector{ 0, 0, 0 }.is_zero());
            static_assert(not Vector{ 0, 1, 0 }.is_zero());
        };

        "Euclidean norm agrees with the Pythagorean result"_test = [] {
            constexpr Vector vector{ 3.0, 4.0, 12.0 };

            static_assert(vector.r_squared<0>() == 0.0);
            static_assert(vector.r_squared<2>() == 25.0);
            static_assert(vector.norm_squared() == 169.0);

            expect_close(vector.norm(), 13.0, 1E-12, "norm of [3, 4, 12]");
        };

        "normalization returns a unit vector and can operate in place"_test = [] {
            const Vector vector{ 3.0, 4.0, 0.0 };
            const auto normalized{ vector.normalized() };

            expect_close(normalized.x(), 0.6, 1E-12, "normalized x component");
            expect_close(normalized.y(), 0.8, 1E-12, "normalized y component");
            expect_close(normalized.z(), 0.0, 1E-12, "normalized z component");
            expect_close(normalized.norm(), 1.0, 1E-12, "normalized vector length");

            auto in_place{ vector };
            auto& result{ in_place.normalize() };

            ut::expect(ut::that % &result == &in_place)
                << "normalize() should return *this";
            expect_close(in_place.norm(), 1.0, 1E-12, "in-place normalized length");

            ut::expect(ut::throws<std::domain_error>([] {
                (void)Vector<3>{}.normalized();
            })) << "normalizing a zero vector should fail";

            ut::expect(ut::throws<std::domain_error>([] {
                auto zero{ Vector<3>{} };
                zero.normalize();
            })) << "in-place normalization of a zero vector should fail";
        };

        "dot and cross products follow standard 3D geometry"_test = [] {
            constexpr Vector a{ 4, 5, 6 };
            constexpr Vector b{ -7, 8, 9 };

            static_assert(a.dot(b) == 66);
            static_assert(a.cross(b) == Vector{ -3, -78, 67 });

            constexpr auto x{ Vector<3, int>::unit_x() };
            constexpr auto y{ Vector<3, int>::unit_y() };
            constexpr auto z{ Vector<3, int>::unit_z() };

            static_assert(x.dot(y) == 0);
            static_assert(x.cross(y) == z);
            static_assert(y.cross(x) == -z);
        };

        "Cartesian product enumerates all ordered triples"_test = [] {
            constexpr auto product{ Vector{ 1, 2 }.cartesian_product() };

            static_assert(product.size() == 8);
            static_assert(product[0] == Vector{ 1, 1, 1 });
            static_assert(product[1] == Vector{ 1, 1, 2 });
            static_assert(product[3] == Vector{ 1, 2, 2 });
            static_assert(product[4] == Vector{ 2, 1, 1 });
            static_assert(product[7] == Vector{ 2, 2, 2 });
        };

        "approx_equal applies the supplied absolute tolerance"_test = [] {
            const Vector left{ 1.0, 2.0, 3.0 };
            const Vector right{ 1.001, 1.999, 3.0005 };

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

            ut::expect(ut::throws<std::invalid_argument>([&] {
                (void)left.approx_equal(right, -1.0);
            })) << "negative approximation tolerance should fail";
        };

        "explicit element conversion converts each component"_test = [] {
            constexpr Vector source{ 1.25, 2.75, -3.5 };
            constexpr auto integers{ source.to<int>() };
            constexpr auto floats{ integers.to<float>() };

            static_assert(
                std::same_as<
                    std::remove_cvref_t<decltype(integers)>,
                    Vector<3, int>>);
            static_assert(integers == Vector{ 1, 2, -3 });
            static_assert(floats == Vector<3, float>{ 1.0F, 2.0F, -3.0F });
        };

        "pointer vectors can be dereferenced component-wise"_test = [] {
            int x{ 1 };
            int y{ 2 };
            int z{ 3 };

            const Vector pointers{ &x, &y, &z };
            const auto values{ pointers.deref() };

            static_assert(
                std::same_as<
                    std::remove_cvref_t<decltype(values)>,
                    Vector<3, int>>);

            expect_vector_equal(
                values,
                Vector{ 1, 2, 3 },
                "dereferenced pointer vector");
        };

        "arithmetic is element-wise unless dot or cross is requested"_test = [] {
            constexpr Vector a{ 4, 5, 6 };
            constexpr Vector b{ -7, 8, 9 };

            static_assert(a + b == Vector{ -3, 13, 15 });
            static_assert(a - b == Vector{ 11, -3, -3 });
            static_assert(a + 2 == Vector{ 6, 7, 8 });
            static_assert(a - 2 == Vector{ 2, 3, 4 });

            static_assert(a * 2 == Vector{ 8, 10, 12 });
            static_assert(2 * a == Vector{ 8, 10, 12 });
            static_assert(a / 2 == Vector{ 2, 2, 3 });

            static_assert(a * b == Vector{ -28, 40, 54 });
        };

        "mixed arithmetic chooses a common result type"_test = [] {
            constexpr Vector<3, int> integers{ 1, 2, 3 };
            constexpr Vector<3, double> doubles{ 0.5, 1.0, 2.0 };

            const auto product{ integers * doubles };
            const auto quotient{ integers / doubles };

            static_assert(
                std::same_as<
                    std::remove_cvref_t<decltype(product)>,
                    Vector<3, double>>);
            static_assert(
                std::same_as<
                    std::remove_cvref_t<decltype(quotient)>,
                    Vector<3, double>>);

            expect_vector_equal(
                product,
                Vector<3, double>{ 0.5, 2.0, 6.0 },
                "mixed element-wise product");
            expect_vector_equal(
                quotient,
                Vector<3, double>{ 2.0, 2.0, 1.5 },
                "mixed element-wise quotient");
        };

        "compound assignment mutates the same vector"_test = [] {
            Vector vector{ 1.0, 2.0, 3.0 };

            vector += Vector{ 1.0, 1.0, 1.0 };
            expect_vector_equal(
                vector,
                Vector{ 2.0, 3.0, 4.0 },
                "vector += vector");

            vector -= Vector{ 1.0, 1.0, 1.0 };
            expect_vector_equal(
                vector,
                Vector{ 1.0, 2.0, 3.0 },
                "vector -= vector");

            vector += 1.0;
            vector -= 1.0;
            vector *= 2.0;
            vector /= 2.0;
            expect_vector_equal(
                vector,
                Vector{ 1.0, 2.0, 3.0 },
                "scalar compound assignments should compose predictably");

            vector *= Vector{ 2.0, 3.0, 4.0 };
            expect_vector_equal(
                vector,
                Vector{ 2.0, 6.0, 12.0 },
                "vector *= vector");

            vector /= Vector{ 2.0, 3.0, 4.0 };
            expect_vector_equal(
                vector,
                Vector{ 1.0, 2.0, 3.0 },
                "vector /= vector");
        };

        "absolute value floor ceil and round apply component-wise"_test = [] {
            expect_vector_equal(
                abs(Vector{ -1.0, 2.0, -3.0 }),
                Vector{ 1.0, 2.0, 3.0 },
                "abs(vector)");

            expect_vector_equal(
                floor(Vector{ 1.2, -1.2 }),
                Vector{ 1.0, -2.0 },
                "floor(vector)");

            expect_vector_equal(
                ceil(Vector{ 1.2, -1.2 }),
                Vector{ 2.0, -1.0 },
                "ceil(vector)");

            expect_vector_equal(
                round(Vector{ 1.4, 1.5, -1.5 }),
                Vector{ 1.0, 2.0, -2.0 },
                "round(vector)");
        };
    };
}
