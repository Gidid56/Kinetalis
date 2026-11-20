#pragma once
#include "constraints.h"
#include "dimension.h"
#include "math/functions.h"

#include <algorithm>
#include <compare>
#include <concepts>
#include <cstdint>
#include <format>
#include <stdexcept>
#include <type_traits>

namespace physics
{
    template <
        IsDimension Dimension = Dimensionless,
        Reference reference = Reference::zero,
        Domain domain = Domain::unrestricted,
        std::floating_point Value = double>
    class Quantity
    {
        Value value_;

        static constexpr void validate_value(Value value)
        {
            if constexpr (satisfies(domain, Domain::non_negative))
                if (not(value >= 0))
                    throw std::domain_error{ std::format("{} must be nonnegative.", value) };

            if constexpr (domain == Domain::unit_interval)
                if (not(value >= 0 and value <= 1))
                    throw std::domain_error{ std::format("{} must be within [0, 1].", value) };
        }

        template <typename Other>
        constexpr void assign_value(Other value) noexcept
        {
            const auto converted{ static_cast<Value>(value) };
            value_ = converted;
        }

    public:
        using quantity_type = void;
        using dimension_type = Dimension;
        using value_type = Value;

        static constexpr Reference reference_value{ reference };
        static constexpr Domain domain_value{ domain };

        constexpr Quantity() noexcept : value_{ reference == Reference::ratio ? 1.0 : 0.0 } { }

        // Domains describe the intended range but are not validated implicitly.
        explicit(not IsDimensionless<Dimension>) constexpr Quantity(Value value) noexcept
            : value_{ value }
        { }

        template <
            IsDimension OtherDimension,
            Reference other_reference,
            Domain other_domain,
            std::floating_point Other>
        requires dimension::Convertible<OtherDimension, Dimension> and
                 //(other_reference == reference) and
                 //(satisfies(other_domain, domain)) and
                 std::convertible_to<Other, Value>
        constexpr Quantity(Quantity<OtherDimension, other_reference, other_domain, Other> other) noexcept
            : Quantity{ static_cast<Value>(other.value()) }
        { }

        template <
            IsDimension OtherDimension,
            Reference other_reference,
            Domain other_domain,
            std::floating_point Other>
        requires dimension::Convertible<OtherDimension, Dimension> and
                 //(other_reference == reference) and
                 //(satisfies(other_domain, domain)) and
                 std::convertible_to<Other, Value>
        constexpr Quantity& operator=(
            Quantity<OtherDimension, other_reference, other_domain, Other> other) noexcept
        {
            assign_value(other.value());
            return *this;
        }

        // Explicitly enforce the declared domain at validation boundaries.
        constexpr void validate() const { validate_value(value_); }

        constexpr Value value() const noexcept { return value_; }

        constexpr operator Value() const noexcept requires IsDimensionless<Dimension>
        {
            return value_;
        }

        template <
            IsDimension OtherDimension,
            Reference other_reference,
            Domain other_domain,
            std::floating_point Other>
        requires dimension::Equivalent<Dimension, OtherDimension> and (reference == other_reference)
        constexpr bool operator==(
            Quantity<OtherDimension, other_reference, other_domain, Other> other) const noexcept
        {
            return value_ == other.value();
        }

        template <
            IsDimension OtherDimension,
            Reference other_reference,
            Domain other_domain,
            std::floating_point Other>
        requires dimension::Equivalent<Dimension, OtherDimension> and (reference == other_reference)
        constexpr auto operator<=>(
            Quantity<OtherDimension, other_reference, other_domain, Other> other) const noexcept
        {
            return value_ <=> other.value();
        }

        template <std::floating_point To>
        constexpr auto to() const noexcept
        {
            return Quantity<Dimension, reference, domain, To>{ static_cast<To>(value_) };
        }

        constexpr auto operator-() const noexcept requires(summable(reference))
        {
            return Quantity<Dimension, reference, Domain::unrestricted, decltype(-value_)>{
                -value_
            };
        }

        template <
            IsDimension OtherDimension,
            Reference other_reference,
            Domain other_domain,
            std::floating_point Other>
        requires dimension::Convertible<OtherDimension, Dimension> and
                 (summable(reference) and summable(other_reference)) and
                 (add(reference, other_reference) == reference)
        constexpr Quantity& operator+=(
            Quantity<OtherDimension, other_reference, other_domain, Other> other) noexcept
        {
            assign_value(value_ + other.value());
            return *this;
        }

        template <
            IsDimension OtherDimension,
            Reference other_reference,
            Domain other_domain,
            std::floating_point Other>
        requires dimension::Convertible<OtherDimension, Dimension> and
                 (summable(reference) and summable(other_reference))
        //and (subtract(reference, other_reference) == reference)
        constexpr Quantity& operator-=(
            Quantity<OtherDimension, other_reference, other_domain, Other> other) noexcept
        {
            assign_value(value_ - other.value());
            return *this;
        }

        template <
            IsDimension OtherDimension,
            Reference other_reference,
            Domain other_domain,
            std::floating_point Other>
        requires dimension::Equivalent<Dimension, OtherDimension> and
                 (summable(reference) and summable(other_reference))
        constexpr auto operator+(
            Quantity<OtherDimension, other_reference, other_domain, Other> other) const noexcept
        {
            using ResultDimension = std::conditional_t<
                dimension::Same<Dimension, OtherDimension>,
                Dimension,
                dimension::SI_type<Dimension>>;

            return Quantity<
                ResultDimension,
                add(reference, other_reference),
                Domain::unrestricted,
                decltype(value_ + other.value())>{ value_ + other.value() };
        }

        template <
            IsDimension OtherDimension,
            Reference other_reference,
            Domain other_domain,
            std::floating_point Other>
        requires dimension::Equivalent<Dimension, OtherDimension> and
                 (summable(reference) and summable(other_reference))
        constexpr auto operator-(
            Quantity<OtherDimension, other_reference, other_domain, Other> other) const noexcept
        {
            using ResultDimension = std::conditional_t<
                dimension::Same<Dimension, OtherDimension>,
                Dimension,
                dimension::SI_type<Dimension>>;

            return Quantity<
                ResultDimension,
                subtract(reference, other_reference),
                Domain::unrestricted,
                decltype(value_ - other.value())>{ value_ - other.value() };
        }

        constexpr Quantity& operator*=(math::Arithmetic auto other) noexcept
        {
            assign_value(value_ * other);
            return *this;
        }

        constexpr Quantity& operator/=(math::Arithmetic auto other) noexcept
        {
            assign_value(value_ / other);
            return *this;
        }

        constexpr auto operator*(math::Arithmetic auto other) const noexcept
        {
            return Quantity<Dimension, reference, Domain::unrestricted, decltype(value_ * other)>{
                value_ * other
            };
        }

        constexpr auto operator/(math::Arithmetic auto other) const noexcept
        {
            return Quantity<Dimension, reference, Domain::unrestricted, decltype(value_ / other)>{
                value_ / other
            };
        }

        constexpr auto operator+(math::Arithmetic auto other) const noexcept
            requires IsDimensionless<Dimension> and (summable(reference))
        {
            return Quantity<Dimension, reference, Domain::unrestricted, decltype(value_ + other)>{
                value_ + other
            };
        }

        constexpr auto operator-(math::Arithmetic auto other) const noexcept
            requires IsDimensionless<Dimension> and (summable(reference))
        {
            return Quantity<Dimension, reference, Domain::unrestricted, decltype(value_ - other)>{
                value_ - other
            };
        }

        template <
            IsDimension OtherDimension,
            Reference other_reference,
            Domain other_domain,
            std::floating_point Other>
        constexpr auto operator*(
            Quantity<OtherDimension, other_reference, other_domain, Other> other) const noexcept
        {
            return Quantity<
                decltype(Dimension{} + OtherDimension{}),
                multiply(reference, other_reference),
                Domain::unrestricted,
                decltype(value_ * other.value())>{ value_ * other.value() };
        }

        template <
            IsDimension OtherDimension,
            Reference other_reference,
            Domain other_domain,
            std::floating_point Other>
        constexpr auto operator/(
            Quantity<OtherDimension, other_reference, other_domain, Other> other) const noexcept
        {
            return Quantity<
                decltype(Dimension{} - OtherDimension{}),
                divide(reference, other_reference, dimension::Same<Dimension, OtherDimension>),
                Domain::unrestricted,
                decltype(value_ / other.value())>{ value_ / other.value() };
        }
    };

    template <
        math::Arithmetic Scalar,
        Reference reference,
        Domain domain,
        std::floating_point Value>
    constexpr auto operator+(Scalar left, Quantity<Dimensionless, reference, domain, Value> right) noexcept
        requires(summable(reference))
    {
        return right + left;
    }

    template <
        math::Arithmetic Scalar,
        Reference reference,
        Domain domain,
        std::floating_point Value>
    constexpr auto operator-(Scalar left, Quantity<Dimensionless, reference, domain, Value> right) noexcept
        requires(summable(reference))
    {
        return Quantity<
            Dimensionless,
            Reference::difference,
            Domain::unrestricted,
            decltype(left - right.value())>{ left - right.value() };
    }

    template <
        math::Arithmetic Scalar,
        IsDimension Dimension,
        Reference reference,
        Domain domain,
        std::floating_point Value>
    constexpr auto operator*(Scalar left, Quantity<Dimension, reference, domain, Value> right) noexcept
    {
        return Quantity<Dimension, reference, Domain::unrestricted, decltype(left * right.value())>{
            left * right.value()
        };
    }

    template <
        math::Arithmetic Scalar,
        IsDimension Dimension,
        Reference reference,
        Domain domain,
        std::floating_point Value>
    constexpr auto operator/(Scalar left, Quantity<Dimension, reference, domain, Value> right) noexcept
    {
        return Quantity<
            decltype(-Dimension{}),
            divide(Reference::ratio, reference, false),
            Domain::unrestricted,
            decltype(left / right.value())>{ left / right.value() };
    }

    namespace quantity
    {
        template <typename T>
        concept IsQuantity = requires { typename T::quantity_type; };

        namespace detail
        {
            template <typename T, typename Other>
            concept SameDimension =
                IsQuantity<T> and IsQuantity<Other> and
                dimension::Same<typename T::dimension_type, typename Other::dimension_type>;

            template <typename T, typename Other, Reference reference>
            concept SameReference = SameDimension<T, Other> and (T::reference_value == reference);
        }

        template <typename T, typename... References>
        concept Like = (detail::SameReference<T, References, References::reference_value> or ...);

        //template <typename T, typename... References>
        //concept Difference = (detail::SameReference<T, References, Reference::difference> or ...);

        //template <typename T, typename... References>
        //concept Absolute = (detail::SameReference<T, References, Reference::zero> or ...);
    }
}
