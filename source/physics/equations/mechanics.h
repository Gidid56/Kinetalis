#pragma once
#include "math/functions.h"
#include "physics/quantities.h"

namespace physics::equations::mechanics
{
    using namespace physics::quantity;
    using math::functions::square;

    // Average velocity over a displacement and elapsed time.
    constexpr auto average_velocity(Like<Displacement> auto displacement, Like<Time> auto time)
    {
        return displacement / time;
    }

    // Average acceleration over a change in velocity and elapsed time.
    constexpr auto average_acceleration(Like<Velocity> auto velocity_change, Like<Time> auto time)
    {
        return velocity_change / time;
    }

    // Linear momentum.
    constexpr auto momentum(Like<Mass> auto mass, Like<Velocity> auto velocity)
    {
        return mass * velocity;
    }

    // Newton's second law for constant mass.
    constexpr auto force(Like<Mass> auto mass, Like<Acceleration> auto acceleration)
    {
        return mass * acceleration;
    }

    // Impulse from a constant force over an elapsed time.
    constexpr auto impulse(Like<Force> auto force, Like<Time> auto time) { return force * time; }

    // Translational kinetic energy.
    constexpr auto kinetic_energy(Like<Mass> auto mass, Like<Velocity> auto velocity)
    {
        return mass * square(velocity) / 2;
    }

    // Work for force parallel to displacement.
    constexpr auto work(Like<Force> auto force, Like<Displacement> auto displacement)
    {
        return force * displacement;
    }

    // Average power from work over elapsed time.
    constexpr auto power(Like<Work> auto work, Like<Time> auto time) { return work / time; }

    // Instantaneous power for force parallel to velocity.
    constexpr auto power(Like<Force> auto force, Like<Velocity> auto velocity)
    {
        return force * velocity;
    }

    // Centripetal acceleration from tangential speed and radius.
    constexpr auto centripetal_acceleration(Like<Speed> auto speed, Like<Radius> auto radius)
    {
        return square(speed) / radius;
    }

    // Centripetal force from mass, tangential speed, and radius.
    constexpr auto
    centripetal_force(Like<Mass> auto mass, Like<Speed> auto speed, Like<Radius> auto radius)
    {
        return mass * centripetal_acceleration(speed, radius);
    }
}
