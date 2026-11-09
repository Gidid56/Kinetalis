#pragma once
#include <cmath>
#include <concepts>
#include <stdexcept>
#include <vector>

namespace sequence
{
    template <class Type>
    constexpr auto exponential(Type begin, Type end, std::floating_point auto step_factor)
    {
        if (end < begin)
            throw std::invalid_argument{ "Sequence end precedes begin." };

        std::vector<Type> steps;

        if (not(begin < end))
        {
            steps.emplace_back(end);
            return steps;
        }

        if (not(Type{} < begin))
            throw std::invalid_argument{ "Exponential sequence begin must be positive." };

        if (not std::isfinite(step_factor) or step_factor <= 1)
            throw std::invalid_argument{
                "Exponential step factor must be finite and greater than one."
            };

        for (Type step{ begin }; step < end;)
        {
            steps.emplace_back(step);

            const auto scaled{ step_factor * step };
            if (not(scaled < end))
                break;

            const Type next{ math::round<Type>(scaled) };

            if (not(step < next))
                throw std::domain_error{ "Exponential sequence does not progress." };

            step = next;
        }

        steps.emplace_back(end);
        return steps;
    }

    template <class Type>
    constexpr auto linear(Type begin, Type end, Type step_width)
    {
        if (end < begin)
            throw std::invalid_argument{ "Sequence end precedes begin." };

        std::vector<Type> steps;

        if (not(begin < end))
        {
            steps.emplace_back(end);
            return steps;
        }

        if (not(Type{} < step_width))
            throw std::invalid_argument{ "Linear step width must be positive." };

        for (Type step{ begin }; step < end;)
        {
            steps.emplace_back(step);

            if constexpr (std::integral<Type>)
                if (step > std::numeric_limits<Type>::max() - step_width)
                    break;

            const Type next{ step + step_width };

            if (not(step < next))
                throw std::overflow_error{ "Linear sequence does not progress." };

            if (not(next < end))
                break;
            step = next;
        }

        steps.emplace_back(end);
        return steps;
    }
}
