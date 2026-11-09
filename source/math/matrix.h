#pragma once
#include "vector.h"

#include <ranges>

namespace math
{
    // Row major data storage
    template <std::size_t N, std::size_t M = N, typename T = double>
    requires(N >= 1 and M >= 1)
    class Matrix
    {
    public:
        using Row = Vector<M, T>;
        using Column = Vector<N, T>;
        using Data = Vector<N, Row>;

    protected:
        Data data_;

    public:
        constexpr Matrix() = default;

        explicit constexpr Matrix(Data const& data) : data_{ data } { }

        template <typename... Rows>
        requires(sizeof...(Rows) == N and (std::same_as<std::remove_cvref_t<Rows>, Row> and ...))
        constexpr Matrix(Rows&&... rows) : data_{ std::forward<Rows>(rows)... }
        { }

        template <typename... Values>
        requires(sizeof...(Values) == N and (std::constructible_from<T, Values const&> and ...))
        constexpr Matrix(RawVector<Values, M>... rows)
            : data_{ Row{ rows, std::make_index_sequence<M>() }... }
        { }

        template <typename Other>
        requires std::constructible_from<T, Other const&>
        constexpr Matrix(Matrix<N, M, Other> const& other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] = other[i];
        }

        static constexpr Matrix all(T value) { return Matrix{ Data::all(Row::all(value)) }; }
        static constexpr Matrix identity() requires(N == M)
        {
            Matrix matrix;
            for (std::size_t i{}; i < N; ++i)
                matrix[i][i] = T{ 1 };
            return matrix;
        }

        constexpr Row& x() requires(N >= 1) { return data_[0]; }
        constexpr Row& y() requires(N >= 2) { return data_[1]; }
        constexpr Row& z() requires(N >= 3) { return data_[2]; }
        constexpr Row& w() requires(N >= 4) { return data_[3]; }

        constexpr Row const& x() const requires(N >= 1) { return data_[0]; }
        constexpr Row const& y() const requires(N >= 2) { return data_[1]; }
        constexpr Row const& z() const requires(N >= 3) { return data_[2]; }
        constexpr Row const& w() const requires(N >= 4) { return data_[3]; }

        constexpr Row& operator[](Axis axis) { return data_[axis]; }
        constexpr Row const& operator[](Axis axis) const { return data_[axis]; }

        constexpr Row& operator[](std::size_t i) noexcept { return data_[i]; }
        constexpr Row const& operator[](std::size_t i) const noexcept { return data_[i]; }

        constexpr Data& data() noexcept { return data_; }
        constexpr Data const& data() const noexcept { return data_; }

        constexpr auto begin() noexcept { return data_.begin(); }
        constexpr auto end() noexcept { return data_.end(); }

        constexpr auto begin() const noexcept { return data_.begin(); }
        constexpr auto end() const noexcept { return data_.end(); }
        constexpr auto size() const noexcept { return data_.end(); }

        [[nodiscard]] static constexpr std::size_t elements() noexcept { return N * M; }

        template <typename To>
        constexpr auto to() const
        {
            Matrix<N, M, To> casted;
            for (std::size_t i{}; i < N; ++i)
                casted[i] = data_[i].template to<To>();
            return casted;
        }

        constexpr auto sum_rows() const
        {
            Column sum;
            for (std::size_t i{}; i < N; ++i)
                sum[i] = row(i).sum();
            return sum;
        }
        constexpr auto sum_columns() const
        {
            Row sum;
            for (std::size_t i{}; i < M; ++i)
                sum[i] = column(i).sum();
            return sum;
        }

        constexpr auto products() const
        {
            using Product = decltype(data_[0].product());
            Vector<N, Product> products;
            for (std::size_t i{}; i < N; ++i)
                products[i] = data_[i].product();
            return products;
        }
        template <typename To = double>
        constexpr auto means() const
        {
            using Mean = decltype(data_[0].template mean<To>());
            Vector<N, Mean> means;
            for (std::size_t i{}; i < N; ++i)
                means[i] = data_[i].template mean<To>();
            return means;
        }
        constexpr auto norms() const
        {
            using Norm = decltype(data_[0].norm());
            Vector<N, Norm> norms;
            for (std::size_t i{}; i < N; ++i)
                norms[i] = data_[i].norm();
            return norms;
        }

        constexpr auto normalized_rows() const
        {
            using NormalizedRow = decltype(data_[0].normalized());
            Matrix<N, M, typename NormalizedRow::Type> normalized;
            for (std::size_t i{}; i < N; ++i)
                normalized[i] = data_[i].normalized();
            return normalized;
        }

        constexpr auto min() const
        {
            return std::ranges::min(data_.data() | std::views::transform(&Row::min));
        }
        constexpr auto max() const
        {
            return std::ranges::max(data_.data() | std::views::transform(&Row::max));
        }

        constexpr auto column(std::size_t j) const
        {
            if (j >= M)
                throw std::out_of_range{ "Invalid matrix column." };
            Column column;
            for (std::size_t i{}; i < N; ++i)
                column[i] = data_[i][j];
            return column;
        }
        constexpr auto& row(std::size_t i) const
        {
            if (i >= N)
                throw std::out_of_range{ "Invalid matrix row." };
            return data_[i];
        }

        constexpr auto trace() const requires(N == M)
        {
            T trace{};
            for (std::size_t i{}; i < N; ++i)
                trace += data_[i][i];
            return trace;
        }

        constexpr auto remove_row(std::size_t row) const requires(N > 1)
        {
            if (row >= N)
                throw std::out_of_range{ "Invalid matrix row." };

            Matrix<N - 1, M, T> sub_matrix;
            for (std::size_t i{}, target{}; i < N; ++i)
                if (i != row)
                    sub_matrix[target++] = data_[i];
            return sub_matrix;
        }
        constexpr auto remove_column(std::size_t column) const requires(M > 1)
        {
            if (column >= M)
                throw std::out_of_range{ "Invalid matrix column." };

            Matrix<N, M - 1, T> sub_matrix;
            for (std::size_t i{}; i < N; ++i)
                for (std::size_t j{}, target{}; j < M; ++j)
                    if (j != column)
                        sub_matrix[i][target++] = data_[i][j];
            return sub_matrix;
        }

        constexpr auto& transpose() requires(N == M)
        {
            for (std::size_t i{}; i < N; ++i)
                for (std::size_t j{ i + 1 }; j < M; ++j)
                    std::swap(data_[i][j], data_[j][i]);
            return *this;
        }
        constexpr auto transposed() const requires(N == M)
        {
            auto transposed{ *this };
            transposed.transpose();
            return transposed;
        }
        constexpr auto transposed() const requires(N != M)
        {
            Matrix<M, N, T> transposed;
            for (std::size_t i{}; i < N; ++i)
                for (std::size_t j{}; j < M; ++j)
                    transposed[j][i] = data_[i][j];
            return transposed;
        }

        constexpr auto determinant() const requires(N == M)
        {
            if constexpr (N == 1)
                return data_[0][0];
            else if constexpr (N == 2)
                return data_[0][0] * data_[1][1] - data_[0][1] * data_[1][0];
            else if constexpr (N == 3)
            {
                return data_[0][0] * (data_[1][1] * data_[2][2] - data_[1][2] * data_[2][1]) +
                       data_[0][1] * (data_[1][2] * data_[2][0] - data_[1][0] * data_[2][2]) +
                       data_[0][2] * (data_[1][0] * data_[2][1] - data_[1][1] * data_[2][0]);
            }
            else
            {
                auto sign = [](auto j) {
                    return j % 2 == 0 ? 1 : -1;
                };
                auto bottom{ this->remove_row(0) };
                decltype(Row{}.product()) determinant{};
                for (std::size_t j{}; j < M; ++j)
                    determinant += sign(j) * data_[0][j] * bottom.remove_column(j).determinant();
                return determinant;
            }
        }

        constexpr auto transpose_invert() const requires(N == M and N <= 3)
        {
            const auto determinant{ this->determinant() };
            if (determinant == decltype(determinant){})
                throw std::domain_error{ "Matrix is not invertible." };

            const auto inv_determinant{ 1.0 / determinant };

            using InverseType = decltype(1.0 / T{});
            Matrix<N, M, InverseType> inverse;

            if constexpr (N == 1)
            {
                inverse[0][0] = inv_determinant;
            }
            if constexpr (N == 2)
            {
                inverse[0][0] = data_[1][1] * inv_determinant;
                inverse[0][1] = -data_[1][0] * inv_determinant;
                inverse[1][0] = -data_[0][1] * inv_determinant;
                inverse[1][1] = data_[0][0] * inv_determinant;
            }
            if constexpr (N == 3)
            {
                inverse[0] = data_[1].cross(data_[2]) * inv_determinant;
                inverse[1] = data_[2].cross(data_[0]) * inv_determinant;
                inverse[2] = data_[0].cross(data_[1]) * inv_determinant;
            }
            return inverse;
        }
        constexpr auto inverse() const requires(N == M and N <= 3)
        {
            return this->transposed().transpose_invert();
        }

        constexpr bool is_lower_triangular() const requires(N == M)
        {
            for (std::size_t i{}; i < N; ++i)
                for (std::size_t j{ i + 1 }; j < M; ++j)
                    if (data_[i][j] != T{})
                        return false;
            return true;
        }
        constexpr bool is_upper_triangular() const requires(N == M)
        {
            for (std::size_t i{}; i < N; ++i)
                for (std::size_t j{}; j < i; ++j)
                    if (data_[i][j] != T{})
                        return false;
            return true;
        }
        constexpr bool is_diagonal() const requires(N == M)
        {
            return is_lower_triangular() and is_upper_triangular();
        }
        constexpr bool is_symmetric() const requires(N == M) { return *this == transposed(); }
        constexpr bool is_skew_symmetric() const requires(N == M) { return *this == -transposed(); }
        constexpr bool is_invertible() const requires(N == M)
        {
            const auto determinant{ this->determinant() };
            return determinant != decltype(determinant){};
        }

        template <std::size_t O, typename U>
        constexpr auto dot(Matrix<M, O, U> const& other) const
        {
            Matrix<N, O, decltype(T{} * U{})> dot_product{};
            for (std::size_t i{}; i < N; ++i)
                for (std::size_t j{}; j < M; ++j)
                    for (std::size_t k{}; k < O; k++)
                        dot_product[i][k] += data_[i][j] * other[j][k];
            return dot_product;
        }

        template <typename U>
        constexpr auto dot(Vector<M, U> const& other) const
        {
            Vector<N, decltype(T{} * U{})> dot_product;
            for (std::size_t i{}; i < N; ++i)
                dot_product[i] = data_[i].dot(other);
            return dot_product;
        }

        template <typename U>
        constexpr auto transpose_dot(Vector<N, U> const& other) const
        {
            Vector<M, decltype(T{} * U{})> dot_product;
            for (std::size_t j{}; j < M; ++j)
                for (std::size_t i{}; i < N; ++i)
                    dot_product[j] += data_[i][j] * other[i];
            return dot_product;
        }

        constexpr auto approx_equal(Matrix const& other, T epsilon) const
        {
            for (std::size_t i{}; i < N; ++i)
                if (not data_[i].approx_equal(other[i], epsilon))
                    return false;
            return true;
        }

        constexpr auto& operator+=(Arithmetic auto other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] += other;
            return *this;
        }

        constexpr auto& operator-=(Arithmetic auto other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] -= other;
            return *this;
        }

        constexpr auto& operator+=(Matrix const& other)
        {
            data_ += other.data_;
            return *this;
        }
        constexpr auto& operator-=(Matrix const& other)
        {
            data_ -= other.data_;
            return *this;
        }

        constexpr auto& operator+=(Vector<M, T> const& other)
        {
            data_ += other;
            return *this;
        }
        constexpr auto& operator-=(Vector<M, T> const& other)
        {
            data_ -= other;
            return *this;
        }

        constexpr auto& operator*=(Arithmetic auto other)
        {
            data_ *= other;
            return *this;
        }
        constexpr auto& operator/=(Arithmetic auto other)
        {
            data_ /= other;
            return *this;
        }

        template <typename U>
        constexpr auto operator*(U other) const
        {
            Matrix<N, M, decltype(T{} * U{})> product;
            for (std::size_t i{}; i < N; ++i)
                product[i] = data_[i] * other;
            return product;
        }
        template <typename U>
        constexpr auto operator/(U other) const
        {
            Matrix<N, M, decltype(T{} / U{})> quotient;
            for (std::size_t i{}; i < N; ++i)
                quotient[i] = data_[i] / other;
            return quotient;
        }

        template <typename U>
        constexpr auto operator*(Vector<N, U> const& other) const
        {
            Matrix<N, M, decltype(T{} * U{})> product;
            for (std::size_t i{}; i < N; ++i)
                product[i] = data_[i] * other[i];
            return product;
        }

        template <typename U>
        constexpr auto operator/(Vector<N, U> const& other) const
        {
            Matrix<N, M, decltype(T{} / U{})> quotient;
            for (std::size_t i{}; i < N; ++i)
                quotient[i] = data_[i] / other[i];
            return quotient;
        }

        template <Arithmetic U>
        constexpr Matrix& operator*=(Vector<N, U> const& other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] *= other[i];
            return *this;
        }

        template <Arithmetic U>
        constexpr Matrix& operator/=(Vector<N, U> const& other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] /= other[i];
            return *this;
        }

        template <typename U>
        constexpr auto operator*(Matrix<N, M, U> const& other) const
        {
            Matrix<N, M, decltype(T{} * U{})> product;
            for (std::size_t i{}; i < N; ++i)
                product[i] = data_[i] * other[i];
            return product;
        }

        template <typename U>
        constexpr auto operator/(Matrix<N, M, U> const& other) const
        {
            Matrix<N, M, decltype(T{} / U{})> quotient;
            for (std::size_t i{}; i < N; ++i)
                quotient[i] = data_[i] / other[i];
            return quotient;
        }

        template <Arithmetic U>
        constexpr Matrix& operator*=(Matrix<N, M, U> const& other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] *= other[i];
            return *this;
        }

        template <Arithmetic U>
        constexpr Matrix& operator/=(Matrix<N, M, U> const& other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] /= other[i];
            return *this;
        }

        constexpr auto operator+(auto const& other) const
        {
            auto sum{ *this };
            sum += other;
            return sum;
        }
        constexpr auto operator-(auto const& other) const
        {
            auto diff{ *this };
            diff -= other;
            return diff;
        }

        friend constexpr auto operator*(Arithmetic auto left, Matrix const& right)
        {
            return right * left;
        }

        constexpr Matrix operator-() const { return Matrix{ -data_ }; }

        constexpr bool operator==(Matrix const&) const = default;
        constexpr bool operator!=(Matrix const&) const = default;

        inline friend constexpr Matrix abs(Matrix const& matrix)
        {
            return Matrix{ abs(matrix.data_) };
        }
        inline friend constexpr Matrix floor(Matrix const& matrix)
        {
            return Matrix{ floor(matrix.data_) };
        }
        inline friend constexpr Matrix ceil(Matrix const& matrix)
        {
            return Matrix{ ceil(matrix.data_) };
        }
        inline friend constexpr Matrix round(Matrix const& matrix)
        {
            return Matrix{ round(matrix.data_) };
        }

        static constexpr Matrix look_at(Row new_up, Row reference_up = Row::unit_z())
            requires(N == 3 and M == 3 and std::floating_point<T>)
        {
            new_up.normalize();
            reference_up.normalize();

            auto new_x{ reference_up.cross(new_up) };

            if (new_x.norm_squared() <= std::numeric_limits<T>::epsilon())
            {
                using std::abs;

                const auto x{ abs(new_up.x()) };
                const auto y{ abs(new_up.y()) };
                const auto z{ abs(new_up.z()) };

                const auto fallback_x{ x <= y and x <= z ? Row::unit_x()
                                       : y <= z          ? Row::unit_y()
                                                         : Row::unit_z() };

                new_x = fallback_x - new_up * fallback_x.dot(new_up);
            }

            new_x.normalize();
            auto new_y{ new_up.cross(new_x).normalized() };

            return { new_x, new_y, new_up };
        }
    };

    template <typename... Rows, std::size_t columns>
    Matrix(RawVector<Rows, columns>...)
        -> Matrix<sizeof...(Rows), columns, std::common_type_t<Rows...>>;
}

template <std::size_t N, std::size_t M, typename T, class Char>
struct std::formatter<math::Matrix<N, M, T>, Char>
    : std::formatter<typename math::Matrix<N, M, T>::Row, Char>
{
    using Matrix = math::Matrix<N, M, T>;
    using RowFormatter = std::formatter<typename Matrix::Row, Char>;

    template <class FormatContext>
    auto format(Matrix const& matrix, FormatContext& context) const
    {
        auto out{ std::format_to(context.out(), "[") };

        context.advance_to(out);
        out = RowFormatter::format(matrix[0], context);

        for (std::size_t i{ 1 }; i < N; ++i)
        {
            out = std::format_to(out, "\n ");
            context.advance_to(out);
            out = RowFormatter::format(matrix[i], context);
        }

        return std::format_to(out, "]");
    }
};
