#include "math/math.h"
#include "physics/physics.h"

#include <boost/ut.hpp>

#include <cmath>
#include <format>
#include <string>
#include <string_view>
#include <type_traits>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace math;
    using namespace physics;
    using namespace physics::quantity;
    using namespace physics::unit::literals;

    template <class T>
    concept RuntimePowerable = requires(T value) {
        math::power(value, 2);
    };

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

    ut::suite<"math.interop.physics"> physics_tests = [] {
        "generic algebra preserves physical dimensions"_test = [] {
            static_assert(
                not Arithmetic<Length>,
                "physics quantities should not masquerade as primitive arithmetic");
            static_assert(
                not RuntimePowerable<Length>,
                "runtime-exponent power is limited to primitive arithmetic bases");

            constexpr auto length{ 2_m };

            static_assert(identity(length) == 2_m);
            static_assert(sign(-length) == -1);
            static_assert(sign(Length{}) == 0);
            static_assert(math::abs(-length) == 2_m);

            static_assert(
                Like<decltype(square(length)), Area>,
                "square(length) should be area");
            static_assert(
                Like<decltype(cube(length)), Volume>,
                "cube(length) should be volume");
            static_assert(
                Like<decltype(power<2>(length)), Area>,
                "power<2>(length) should be area");

            static_assert(square(length).value() == 4.0);
            static_assert(cube(length).value() == 8.0);
            static_assert(power<2>(length).value() == 4.0);
        };

        "vectors of quantities keep their dimensions through reductions"_test = [] {
            constexpr Vector lengths{ 1_m, 2_m, 3_m };

            static_assert(
                Like<
                    typename std::remove_cvref_t<decltype(lengths)>::Type,
                    Length>);

            static_assert(Like<decltype(lengths.sum()), Length>);
            static_assert(Like<decltype(lengths.product()), Volume>);
            static_assert(Like<decltype(lengths.mean()), Length>);
            static_assert(
                Like<decltype(lengths.norm_squared()), Area>);

            static_assert(lengths.sum().value() == 6.0);
            static_assert(lengths.product().value() == 6.0);

            const auto norm{ Vector{ 3_m, 4_m, 0_m }.norm() };
            static_assert(
                Like<
                    std::remove_cvref_t<decltype(norm)>,
                    Length>);

            expect_close(norm.value(), 5.0, 1E-12, "norm of [3 m, 4 m, 0 m]");

            const auto normalized{
                Vector{ 3_m, 4_m, 0_m }.normalized()
            };
            using Normalized = std::remove_cvref_t<decltype(normalized)>;

            static_assert(Normalized::size() == 3);
            static_assert(
                Like<typename Normalized::Type, Factor>,
                "normalizing a dimensional vector should produce dimensionless quantities, not raw scalars");

            expect_close(
                normalized[0].value(),
                0.6,
                1E-12,
                "normalized quantity vector x");
            expect_close(
                normalized[1].value(),
                0.8,
                1E-12,
                "normalized quantity vector y");
            expect_close(
                normalized[2].value(),
                0.0,
                1E-12,
                "normalized quantity vector z");
        };

        //"quantity vector products propagate physical dimensions"_test = [] {
        //    constexpr Vector a{ 1_m, 2_m, 3_m };
        //    constexpr Vector b{ 4_m, 5_m, 6_m };

        //    static_assert(
        //        Like<decltype(a.dot(b)), Area>,
        //        "length dot length should be area");

        //    using Cross = std::remove_cvref_t<decltype(a.cross(b))>;
        //    static_assert(Cross::size() == 3);
        //    static_assert(
        //        Like<typename Cross::Type, Area>,
        //        "length cross length should have area components");

        //    static_assert(a.dot(b).value() == 32.0);
        //    static_assert(
        //        a.cross(b) ==
        //        Cross{
        //            Area{ -3.0 },
        //            Area{ 6.0 },
        //            Area{ -3.0 },
        //        });

        //    const auto scaled{ Vector{ 1.0, 2.0, 3.0 } * 2_m };
        //    using Scaled = std::remove_cvref_t<decltype(scaled)>;
        //    static_assert(Scaled::size() == 3);
        //    static_assert(Like<typename Scaled::Type, Length>);

        //    for (std::size_t i{}; i < 3; ++i)
        //    {
        //        const double expected[]{ 2.0, 4.0, 6.0 };
        //        expect_close(
        //            scaled[i].value(),
        //            expected[i],
        //            1E-12,
        //            "dimensionless vector scaled by 2 m");
        //    }

        //    const auto ratio{ Vector{ 2_m, 4_m, 8_m } / 2_m };
        //    using Ratio = std::remove_cvref_t<decltype(ratio)>;

        //    static_assert(Ratio::size() == 3);
        //    static_assert(
        //        Like<typename Ratio::Type, Factor>,
        //        "length / length should remain a dimensionless Quantity");

        //    const double expected_ratio[]{ 1.0, 2.0, 4.0 };
        //    for (std::size_t i{}; i < 3; ++i)
        //    {
        //        expect_close(
        //            ratio[i].value(),
        //            expected_ratio[i],
        //            1E-12,
        //            "quantity vector divided by 2 m");
        //    }
        //};

        //"quantity vector comparisons accept dimensional tolerances"_test = [] {
        //    const Vector values{ 1_m, 2_m, 3_m };

        //    expect_close(values.min().value(), 1.0, 1E-12, "minimum length");
        //    expect_close(values.max().value(), 3.0, 1E-12, "maximum length");

        //    ut::expect(ut::that % values.is_zero() == false)
        //        << "nonzero length vector should not report zero";

        //    const auto near{
        //        values.approx_equal(
        //            Vector{ 1.001_m, 2.001_m, 2.999_m },
        //            0.002_m)
        //    };
        //    const auto far{
        //        values.approx_equal(
        //            Vector{ 1.01_m, 2_m, 3_m },
        //            0.001_m)
        //    };

        //    ut::expect(ut::that % near == true)
        //        << "quantity vector should accept differences within 0.002 m";
        //    ut::expect(ut::that % far == false)
        //        << "quantity vector should reject a 0.01 m difference with 0.001 m tolerance";
        //};

        "matrices of quantities keep dimensions through linear algebra"_test = [] {
            constexpr Matrix<2, 2, Length> lengths{
                { 1_m, 2_m },
                { 3_m, 4_m },
            };

            //static_assert(
            //    Like<decltype(lengths.determinant()), Area>,
            //    "determinant of a 2x2 length matrix should have area dimension");
            static_assert(lengths.determinant().value() == -2.0);

            const auto norms{ lengths.norms() };
            using Norms = std::remove_cvref_t<decltype(norms)>;
            static_assert(Norms::size() == 2);
            static_assert(Like<typename Norms::Type, Length>);

            expect_close(
                norms[0].value(),
                std::sqrt(5.0),
                1E-12,
                "first row norm of length matrix");
            expect_close(
                norms[1].value(),
                5.0,
                1E-12,
                "second row norm of length matrix");

            const auto normalized{ lengths.normalized_rows() };
            using Normalized = std::remove_cvref_t<decltype(normalized)>;
            static_assert(Normalized::elements() == 4);
            static_assert(
                Like<typename Normalized::Row::Type, Factor>,
                "normalized quantity matrix rows should contain dimensionless quantities");

            const Matrix<2> numeric{
                { 1.0, 2.0 },
                { 3.0, 4.0 },
            };
            const auto scaled{ numeric * 1_m };
            using Scaled = std::remove_cvref_t<decltype(scaled)>;

            static_assert(Scaled::elements() == 4);
            static_assert(
                Like<typename Scaled::Row::Type, Length>);
        };

        "inverse of a dimensional matrix has reciprocal dimensions"_test = [] {
            const Matrix<2, 2, Length> lengths{
                { 2_m, 0_m },
                { 0_m, 4_m },
            };

            using ReciprocalLength = decltype(1.0 / 1_m);

            const auto inverse{ lengths.inverse() };
            using Inverse = std::remove_cvref_t<decltype(inverse)>;

            static_assert(Inverse::elements() == 4);
            static_assert(
                Like<
                    typename Inverse::Row::Type,
                    ReciprocalLength>);

            const auto first_identity_factor{ inverse[0][0] * 1_m };
            const auto second_identity_factor{ inverse[1][1] * 1_m };

            static_assert(
                Like<
                    std::remove_cvref_t<decltype(first_identity_factor)>,
                    SignedNumber>,
                "reciprocal length times length should stay a dimensionless Quantity");

            expect_close(
                first_identity_factor.value(),
                0.5,
                1E-12,
                "inverse[0][0] * 1 m");
            expect_close(
                second_identity_factor.value(),
                0.25,
                1E-12,
                "inverse[1][1] * 1 m");

            ut::expect(ut::that % lengths.is_invertible() == true)
                << "diagonal nonzero length matrix should be invertible";
        };

        "matrix-vector products multiply physical dimensions"_test = [] {
            const Matrix<2, 2, Length> lengths{
                { 1_m, 2_m },
                { 3_m, 4_m },
            };

            const Vector<2, Time> times{ 2_s, 3_s };
            const auto product{ lengths.dot(times) };

            using LengthTime = decltype(1_m * 1_s);
            using Product = std::remove_cvref_t<decltype(product)>;

            static_assert(Product::size() == 2);
            static_assert(
                Like<typename Product::Type, LengthTime>);

            expect_close(
                product[0].value(),
                8.0,
                1E-12,
                "first matrix-vector product component");
            expect_close(
                product[1].value(),
                18.0,
                1E-12,
                "second matrix-vector product component");
        };

        "math formatting forwards physics unit specifications"_test = [] {
            const Vector lengths{ 1_nm, 2_nm, 3_nm };
            const Matrix matrix{
                { 1_nm, 2_nm },
                { 3_nm, 4_nm },
            };

            const auto vector_text{ std::format("{:nm}", lengths) };
            const auto matrix_text{ std::format("{:nm}", matrix) };

            ut::expect(
                ut::that % vector_text ==
                std::string{ "[1 nm, 2 nm, 3 nm]" })
                << "vector formatter should forward ':nm' to each quantity";

            ut::expect(
                ut::that % matrix_text ==
                std::string{ "[[1 nm, 2 nm]\n [3 nm, 4 nm]]" })
                << "matrix formatter should forward ':nm' to each quantity";
        };
    };
}
