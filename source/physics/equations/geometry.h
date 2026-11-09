#pragma once
#include "math/functions.h"
#include "physics/constants.h"
#include "physics/functions.h"
#include "physics/quantities.h"

namespace physics::equations
{
    using namespace physics::quantity;

    using math::functions::cube;
    using math::functions::square;

    using physics::constant::symbols::pi;

    namespace sphere
    {
        // Volume from radius.
        constexpr auto volume(Like<Radius> auto radius) { return (4.0 / 3.0) * pi * cube(radius); }

        // Surface area from radius.
        constexpr auto area(Like<Radius> auto radius) { return 4 * pi * square(radius); }

        // Radius from volume.
        auto radius(Like<Volume> auto volume) { return physics::cbrt(3 * volume / (4 * pi)); }

        // Radius from surface area.
        auto radius(Like<Area> auto area) { return physics::sqrt(area / (4 * pi)); }
    }

    namespace circle
    {
        // Area from radius.
        constexpr auto area(Like<Radius> auto radius) { return pi * square(radius); }

        // Circumference from radius.
        constexpr auto circumference(Like<Radius> auto radius) { return 2 * pi * radius; }

        // Radius from area.
        auto radius(Like<Area> auto area) { return physics::sqrt(area / pi); }
    }

    namespace cylinder
    {
        // Volume from radius and height.
        constexpr auto volume(Like<Radius> auto radius, Like<Length> auto height)
        {
            return pi * square(radius) * height;
        }

        // Total surface area from radius and height.
        constexpr auto area(Like<Radius> auto radius, Like<Length> auto height)
        {
            return 2 * pi * radius * (radius + height);
        }
    }
}
