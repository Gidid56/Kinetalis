#pragma once
#include "functions.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <format>

namespace math
{
    template <typename T, std::size_t N>
    using RawVector = T const (&)[N];

    template <std::size_t N, typename T = double>
    requires(N >= 1)
    class Vector
    {
    public:
        using Type = T;
        using Data = std::array<T, N>;

    protected:
        Data data_{};

    public:
        constexpr Vector() = default;

        explicit constexpr Vector(Data const& data) : data_{ data } { }

        template <typename... Args>
        requires(sizeof...(Args) == N and (std::constructible_from<T, Args &&> and ...))
        constexpr Vector(Args&&... args) : data_{ static_cast<T>(std::forward<Args>(args))... }
        { }

        template <typename Value, auto... I>
        requires(sizeof...(I) == N and std::constructible_from<T, Value const&>)
        constexpr Vector(RawVector<Value, sizeof...(I)> vector, std::index_sequence<I...>)
            : data_{ static_cast<T>(vector[I])... }
        { }

        template <typename Other>
        requires std::constructible_from<T, Other const&>
        constexpr Vector(Vector<N, Other> const& other)
        {
            for (std::size_t i = 0; i < N; ++i)
                data_[i] = other[i];
        }

        template <typename Other>
        requires std::assignable_from<T&, Other const&>
        constexpr Vector& operator=(Vector<N, Other> const& other)
        {
            for (std::size_t i = 0; i < N; ++i)
                data_[i] = other[i];

            return *this;
        }

        static constexpr Vector all(T const& value)
        {
            Vector vector;
            std::ranges::fill(vector.data_, value);
            return vector;
        }

        static constexpr Vector unit(Axis axis) requires std::constructible_from<T, int>
        {
            Vector vector{};
            vector[axis] = T{ 1 };
            return vector;
        }
        static constexpr Vector unit_x() requires(N >= 1) { return unit(Axis::x); }
        static constexpr Vector unit_y() requires(N >= 2) { return unit(Axis::y); }
        static constexpr Vector unit_z() requires(N >= 3) { return unit(Axis::z); }
        static constexpr Vector unit_w() requires(N >= 4) { return unit(Axis::w); }

        constexpr auto operator<=>(Vector const&) const = default;

        constexpr T& x() requires(N >= 1) { return data_[0]; }
        constexpr T& y() requires(N >= 2) { return data_[1]; }
        constexpr T& z() requires(N >= 3) { return data_[2]; }
        constexpr T& w() requires(N >= 4) { return data_[3]; }

        constexpr T const& x() const requires(N >= 1) { return data_[0]; }
        constexpr T const& y() const requires(N >= 2) { return data_[1]; }
        constexpr T const& z() const requires(N >= 3) { return data_[2]; }
        constexpr T const& w() const requires(N >= 4) { return data_[3]; }

        static constexpr std::size_t axis_index(Axis axis)
        {
            std::size_t index{};

            switch (axis)
            {
                case Axis::x: index = 0; break;
                case Axis::y: index = 1; break;
                case Axis::z: index = 2; break;
                case Axis::w: index = 3; break;

                default: throw std::invalid_argument{ "Axis is not a Cartesian coordinate axis." };
            }
            if (index >= N)
                throw std::out_of_range{ "Axis exceeds vector dimension." };
            return index;
        }

        constexpr T& operator[](std::size_t i) { return data_[i]; }
        constexpr T const& operator[](std::size_t i) const { return data_[i]; }

        constexpr T& operator[](Axis axis) { return data_[axis_index(axis)]; }
        constexpr T const& operator[](Axis axis) const { return data_[axis_index(axis)]; }

        constexpr Data& data() noexcept { return data_; }
        constexpr Data const& data() const noexcept { return data_; }

        constexpr auto begin() noexcept { return data_.begin(); }
        constexpr auto end() noexcept { return data_.end(); }

        constexpr auto begin() const noexcept { return data_.begin(); }
        constexpr auto end() const noexcept { return data_.end(); }

        [[nodiscard]] static constexpr std::size_t size() noexcept { return N; }

        template <auto first, auto last>
        constexpr auto partial_sum() const requires(first >= 0 and first < last and last <= N)
        {
            auto partial_sum{ data_[first] };
            for (auto i{ first + 1 }; i < last; ++i)
                partial_sum += data_[i];
            return partial_sum;
        }
        constexpr auto sum() const { return this->partial_sum<0, N>(); }

        template <auto first, auto last>
        constexpr auto partial_product() const requires(first >= 0 and first < last and last <= N)
        {
            if constexpr (first + 1 == last)
                return data_[first];
            else
                return data_[first] * this->partial_product<first + 1, last>();
        }
        constexpr auto product() const { return this->partial_product<0, N>(); }

        template <typename To = double>
        constexpr auto mean() const
        {
            return this->sum() / static_cast<To>(this->size());
        }

        constexpr auto min() const { return std::ranges::min(data_); }
        constexpr auto max() const { return std::ranges::max(data_); }

        constexpr bool is_zero() const
        {
            return std::ranges::all_of(data_, [](auto const& a) { return a == T{ 0 }; });
        }

        template <auto dim>
        constexpr auto r_squared() const requires(dim >= 0 and dim <= N)
        {
            decltype(square(T{})) r_squared{};
            for (std::size_t i{}; i < dim; ++i)
                r_squared += square(data_[i]);
            return r_squared;
        }
        constexpr auto norm_squared() const { return this->r_squared<N>(); }

        constexpr auto norm() const
        {
            //if constexpr (std::is_constant_evaluated() and std::is_arithmetic_v<T>)
            //	return gcem::sqrt(this->normSquared());

            using std::sqrt;
            return sqrt(this->norm_squared());
        }
        constexpr auto& normalize() requires std::floating_point<T>
        {
            const auto length{ norm() };

            if (length == T{})
                throw std::domain_error{ "Cannot normalize a zero vector." };

            return *this /= length;
        }
        constexpr auto normalized() const
        {
            const auto length{ norm() };

            if (length == decltype(length){})
                throw std::domain_error{ "Cannot normalize a zero vector." };

            return *this / length;
        }

        template <typename T2>
        constexpr auto dot(Vector<N, T2> const& other) const
        {
            decltype(T{} * T2{}) dot_product{};
            for (std::size_t i{}; i < N; ++i)
                dot_product += data_[i] * other[i];
            return dot_product;
        }
        template <typename T2>
        constexpr auto cross(Vector<N, T2> const& other) const requires(N == 3)
        {
            return Vector<N, decltype(T{} * T2{})>{
                this->y() * other.z() - this->z() * other.y(),
                this->z() * other.x() - this->x() * other.z(),
                this->x() * other.y() - this->y() * other.x(),
            };
        }

        constexpr auto cartesian_product() const
        {
            Vector<math::power(N, 3), Vector<3, T>> cartesian_product{};

            std::size_t index{};
            for (auto x : data_)
                for (auto y : data_)
                    for (auto z : data_)
                        cartesian_product[index++] = { x, y, z };

            return cartesian_product;
        }

        constexpr auto approx_equal(Vector const& other, T epsilon) const
        {
            if (epsilon < T{})
                throw std::invalid_argument{ "Approximation tolerance must not be negative." };

            for (std::size_t i{}; i < N; ++i)
            {
                const auto difference{ data_[i] < other[i] ? other[i] - data_[i]
                                                           : data_[i] - other[i] };
                if (not(difference <= epsilon))
                    return false;
            }
            return true;
        }

        template <typename To>
        constexpr auto to() const
        {
            Vector<N, To> casted;
            for (std::size_t i{}; i < N; ++i)
                casted[i] = static_cast<To>(data_[i]);
            return casted;
        }

        constexpr auto deref() const requires requires(T const& value) { *value; }
        {
            using Result = std::decay_t<decltype(*data_[0])>;
            Vector<N, Result> dereferenced;
            for (std::size_t i{}; i < N; ++i)
                dereferenced[i] = *data_[i];
            return dereferenced;
        }

        constexpr auto& operator+=(Vector const& other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] += other[i];
            return *this;
        }
        constexpr auto& operator-=(Vector const& other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] -= other[i];
            return *this;
        }

        constexpr auto& operator+=(T other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] += other;
            return *this;
        }
        constexpr auto& operator-=(T other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] -= other;
            return *this;
        }

        constexpr auto& operator*=(Arithmetic auto other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] *= other;
            return *this;
        }
        constexpr auto& operator/=(Arithmetic auto other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] /= other;
            return *this;
        }

        template <Arithmetic T2>
        constexpr auto& operator*=(Vector<N, T2> const& other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] *= other[i];
            return *this;
        }
        template <Arithmetic T2>
        constexpr auto& operator/=(Vector<N, T2> const& other)
        {
            for (std::size_t i{}; i < N; ++i)
                data_[i] /= other[i];
            return *this;
        }

        template <typename Arg>
        constexpr auto operator+(Arg&& other) const
        {
            auto sum{ *this };
            sum += std::forward<Arg>(other);
            return sum;
        }
        template <typename Arg>
        constexpr auto operator-(Arg&& other) const
        {
            auto diff{ *this };
            diff -= std::forward<Arg>(other);
            return diff;
        }

        template <typename T2>
        constexpr auto operator*(T2 other) const
        {
            Vector<N, decltype(T{} * T2{})> product;
            for (std::size_t i{}; i < N; ++i)
                product[i] = data_[i] * other;
            return product;
        }
        template <typename T2>
        constexpr auto operator/(T2 other) const
        {
            Vector<N, decltype(T{} / T2{})> quotient;
            for (std::size_t i{}; i < N; ++i)
                quotient[i] = data_[i] / other;
            return quotient;
        }

        template <typename T2>
        constexpr auto operator*(Vector<N, T2> const& other) const
        {
            Vector<N, decltype(T{} * T2{})> product;
            for (std::size_t i{}; i < N; ++i)
                product[i] = data_[i] * other[i];
            return product;
        }
        template <typename T2>
        constexpr auto operator/(Vector<N, T2> const& other) const
        {
            Vector<N, decltype(T{} / T2{})> quotient;
            for (std::size_t i{}; i < N; ++i)
                quotient[i] = data_[i] / other[i];
            return quotient;
        }

        friend constexpr auto operator*(Arithmetic auto left, Vector const& right)
        {
            return right * left;
        }

        constexpr auto operator-() const
        {
            Vector negated{ *this };
            for (std::size_t i{}; i < N; ++i)
                negated[i] *= -1;
            return negated;
        }

        inline friend constexpr auto abs(Vector const& vector)
        {
            Vector absolute;
            for (std::size_t i{}; i < N; ++i)
                absolute[i] = abs(vector[i]);
            return absolute;
        }

        inline friend auto floor(Vector const& vector)
        {
            using std::floor;
            Vector rounded;
            for (std::size_t i{}; i < N; ++i)
                rounded[i] = floor(vector[i]);
            return rounded;
        }

        inline friend auto ceil(Vector const& vector)
        {
            using std::ceil;
            Vector rounded;
            for (std::size_t i{}; i < N; ++i)
                rounded[i] = ceil(vector[i]);
            return rounded;
        }

        inline friend auto round(Vector const& vector)
        {
            using std::round;
            Vector rounded;
            for (std::size_t i{}; i < N; ++i)
                rounded[i] = round(vector[i]);
            return rounded;
        }
    };

    template <typename... Ts>
    Vector(Ts... ts) -> Vector<sizeof...(ts), std::common_type_t<Ts...>>;
}

template <std::size_t N, typename T, class Char>
struct std::formatter<math::Vector<N, T>, Char> : std::formatter<T, Char>
{
    using Base = std::formatter<T, Char>;
    using Vector = math::Vector<N, T>;

    template <class FormatContext>
    auto format(Vector const& vector, FormatContext& context) const
    {
        auto out{ std::format_to(context.out(), "[") };

        context.advance_to(out);
        out = Base::format(vector[0], context);

        for (std::size_t i{ 1 }; i < N; ++i)
        {
            out = std::format_to(out, ", ");
            context.advance_to(out);
            out = Base::format(vector[i], context);
        }
        return std::format_to(out, "]");
    }
};
