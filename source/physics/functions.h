#pragma once
#include "quantity.h"

#include <cmath>

namespace physics
{
    template <
        Domain constraint,
        IsDimension Dimension,
        Reference reference,
        Domain domain,
        std::floating_point Value>
    constexpr auto ensure(Quantity<Dimension, reference, domain, Value> value)
    {
        return Quantity<Dimension, reference, constraint, Value>{ value.value() };
    }
    constexpr auto ensure_non_negative(quantity::IsQuantity auto value)
    {
        return ensure<Domain::non_negative>(value);
    }
    constexpr auto ensure_unit_interval(quantity::IsQuantity auto value)
    {
        return ensure<Domain::unit_interval>(value);
    }

    template <IsDimension Dimension, Reference reference, Domain domain, std::floating_point Value>
    auto sqrt(Quantity<Dimension, reference, domain, Value> value)
        requires IsRootable<2, Dimension> //and (domain == Domain::non_negative)
    {
        using std::sqrt;

        using ResultDimension = decltype(Dimension{}.template root<2>());
        using ResultValue = decltype(sqrt(value.value()));

        return Quantity<ResultDimension, reference, root_map<2>(domain), ResultValue>{ sqrt(
            value.value()) };
    }

    template <IsDimension Dimension, Reference reference, Domain domain, std::floating_point Value>
    auto cbrt(Quantity<Dimension, reference, domain, Value> value) requires IsRootable<3, Dimension>
    {
        using std::cbrt;

        using ResultDimension = decltype(Dimension{}.template root<3>());
        using ResultValue = decltype(cbrt(value.value()));

        return Quantity<ResultDimension, reference, root_map<3>(domain), ResultValue>{ cbrt(
            value.value()) };
    }

    template <Reference reference, Domain domain, std::floating_point Value>
    auto exp(Quantity<Dimensionless, reference, domain, Value> value)
        //requires(reference == Reference::difference)
    {
        using ResultValue = decltype(std::exp(value.value()));

        return Quantity<Dimensionless, Reference::ratio, Domain::non_negative, ResultValue>{
            std::exp(value.value())
        };
    }

    template <Reference reference, Domain domain, std::floating_point Value>
    auto log(Quantity<Dimensionless, reference, domain, Value> value)
        //requires(reference == Reference::ratio)
    {
        using ResultValue = decltype(std::log(value.value()));
        return Quantity<Dimensionless, Reference::difference, Domain::unrestricted, ResultValue>{
            std::log(value.value())
        };
    }

    template <IsDimension Dimension, Reference reference, Domain domain, std::floating_point Value>
    auto abs(Quantity<Dimension, reference, domain, Value> value)
    {
        using std::abs;

        return Quantity<Dimension, reference, restricted(domain, Domain::non_negative), Value>{ abs(
            value.value()) };
    }

    template <IsDimension Dimension, Reference reference, Domain domain, std::floating_point Value>
    bool isfinite(Quantity<Dimension, reference, domain, Value> value)
    {
        using std::isfinite;
        return isfinite(value.value());
    }

    template <IsDimension Dimension, Reference reference, Domain domain, std::floating_point Value>
    auto round(Quantity<Dimension, reference, domain, Value> value)
    {
        using std::round;
        return Quantity<Dimension, reference, domain, Value>{ round(value.value()) };
    }

    template <IsDimension Dimension, Reference reference, Domain domain, std::floating_point Value>
    auto floor(Quantity<Dimension, reference, domain, Value> value)
    {
        using std::floor;
        return Quantity<Dimension, reference, domain, Value>{ floor(value.value()) };
    }

    template <IsDimension Dimension, Reference reference, Domain domain, std::floating_point Value>
    auto ceil(Quantity<Dimension, reference, domain, Value> value)
    {
        using std::ceil;
        return Quantity<Dimension, reference, domain, Value>{ ceil(value.value()) };
    }

    template <Reference reference, Domain domain, std::floating_point Value>
    auto sin(Quantity<dimension::Angle, reference, domain, Value> angle)
    {
        using std::sin;

        using ResultValue = decltype(sin(angle.value()));
        return Quantity<Dimensionless, Reference::zero, Domain::unrestricted, ResultValue>{ sin(
            angle.value()) }; // TODO: Domain::unit_interval
    }

    template <Reference reference, Domain domain, std::floating_point Value>
    auto cos(Quantity<dimension::Angle, reference, domain, Value> angle)
    {
        using std::cos;

        using ResultValue = decltype(cos(angle.value()));
        return Quantity<Dimensionless, Reference::zero, Domain::unrestricted, ResultValue>{ cos(
            angle.value()) }; // TODO: Domain::unit_interval
    }

    template <Reference reference, Domain domain, std::floating_point Value>
    auto tan(Quantity<dimension::Angle, reference, domain, Value> angle)
    {
        using std::tan;

        using ResultValue = decltype(tan(angle.value()));
        return Quantity<Dimensionless, Reference::zero, Domain::unrestricted, ResultValue>{ tan(
            angle.value()) };
    }

    template <Reference reference, Domain domain, std::floating_point Value>
    auto asin(Quantity<Dimensionless, reference, domain, Value> value)
    {
        using std::asin;

        using ResultValue = decltype(asin(value.value()));
        return Quantity<dimension::Angle, Reference::zero, Domain::unrestricted, ResultValue>{ asin(
            value.value()) }; // TODO: Domain::turn
    }

    template <Reference reference, Domain domain, std::floating_point Value>
    auto acos(Quantity<Dimensionless, reference, domain, Value> value)
    {
        using std::acos;

        using ResultValue = decltype(acos(value.value()));
        return Quantity<dimension::Angle, Reference::zero, Domain::unrestricted, ResultValue>{ acos(
            value.value()) };
    }

    template <Reference reference, Domain domain, std::floating_point Value>
    auto atan(Quantity<Dimensionless, reference, domain, Value> value)
        requires(reference == Reference::difference)
    {
        using std::atan;

        using ResultValue = decltype(atan(value.value()));
        return Quantity<dimension::Angle, Reference::zero, Domain::unrestricted, ResultValue>{ atan(
            value.value()) };
    }

    template <
        IsDimension Dimension,
        Domain y_domain,
        Domain x_domain,
        std::floating_point Y_Value,
        std::floating_point X_Value>
    auto atan2(
        Quantity<Dimension, Reference::difference, y_domain, Y_Value> y,
        Quantity<Dimension, Reference::difference, x_domain, X_Value> x)
    {
        using std::atan2;

        using ResultValue = decltype(atan2(y.value(), x.value()));
        return Quantity<dimension::Angle, Reference::zero, Domain::unrestricted, ResultValue>{
            atan2(y.value(), x.value())
        };
    }
}
