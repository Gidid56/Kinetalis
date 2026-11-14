#include "pch.h"

#include "engine.h"

#include "chemistry/composition.h"
#include "lattice/shape.h"

using namespace simulation;

/*
	xoshiro256+ 1.0 was written by David Blackman and Sebastiano Vigna and
	dedicated to the public domain. The state transition and output function
	used by Engine::next() are adapted from their reference
	implementation to an instance-owned C++ state.
*/

auto Engine::parse_seed(std::string_view text) -> Seed
{
    constexpr std::size_t word_digits{ 8 };
    constexpr std::size_t expected_size{ seed_words * word_digits + seed_words - 1 };

    if (text.size() != expected_size)
        throw std::invalid_argument{ "Invalid RNG seed length" };

    Seed seed{};

    for (std::size_t word{}; word < seed.size(); ++word)
    {
        auto const offset{ word * (word_digits + 1) };
        auto const first{ text.data() + offset };
        auto const last{ first + word_digits };

        auto const [ptr, error]{ std::from_chars(first, last, seed[word], 16) };

        if (error != std::errc{} or ptr != last)
            throw std::invalid_argument{ "Invalid RNG seed word" };

        if (word + 1 < seed.size() and text[offset + word_digits] != '-')
            throw std::invalid_argument{ "Invalid RNG seed separator" };
    }

    return seed;
}

Seed Engine::generate_seed() noexcept
{
    Seed seed;

    for (std::size_t word{}; word < state_words; ++word)
    {
        auto const value{ next() };
        seed[2 * word] = static_cast<Seed::value_type>(value >> 32);
        seed[2 * word + 1] = static_cast<Seed::value_type>(value);
    }

    return seed;
}

math::Vector<3> Engine::direction()
{
    const double polar_angle{ 2 * std::numbers::pi * probability() };
    const double z_projection{ 2 * probability() - 1 };
    const double r2d_projection{ std::sqrt(std::max(0.0, 1 - math::square(z_projection))) };

    return {
        std::cos(polar_angle) * r2d_projection,
        std::sin(polar_angle) * r2d_projection,
        z_projection,
    };
}

chemistry::Element Engine::element(chemistry::Composition const& composition)
{
    const auto total_quantity{ composition.total_quantity() };
    if (total_quantity == 0)
        throw std::logic_error{ "Cannot select an element from an empty composition" };

    const auto random_quantity{ integer(static_cast<std::uint64_t>(total_quantity) - 1) };

    for (chemistry::Composition::Quantity sum{}; auto& [element, quantity] : composition)
    {
        sum += quantity;
        if (random_quantity < static_cast<std::uint64_t>(sum))
            return element;
    }

    throw std::logic_error{ "No element found in composition" };
}

lattice::Position Engine::position(lattice::Shape const& shape)
{
    lattice::Position position;
    do
        position = shape.position({ probability(), probability(), probability() });
    while (not shape.contains(position));
    return position;
}

lattice::Indices Engine::miller_indices(int max_index)
{
    if (max_index <= 0)
        throw std::invalid_argument{ "maxIndex must be greater than zero" };

    const auto span{ 2ULL * static_cast<std::uint64_t>(max_index) };

    std::array<int, 3> indices{};

    do
    {
        std::ranges::generate(indices, [this, max_index, span] {
            return static_cast<int>(static_cast<std::int64_t>(integer(span)) - max_index);
        });
    }
    while (std::ranges::all_of(indices, math::is_zero<int>));

    std::ranges::sort(indices, std::ranges::greater{}, math::abs<int>);

    if (indices.front() < 0)
        for (auto& index : indices)
            index *= -1;

    std::ranges::stable_sort(indices, std::ranges::greater{}, math::sign<int>);

    const auto divisor{
        std::gcd(std::gcd(std::abs(indices[0]), std::abs(indices[1])), std::abs(indices[2]))
    };

    for (auto& index : indices)
        index /= divisor;

    return { indices[0], indices[1], indices[2] };
}

std::string Engine::seed_to_string(Seed const& seed, std::string_view separator)
{
    std::string text;
    text.reserve(seed_words * 8 + (seed_words - 1) * separator.size());

    for (std::size_t word{}; word < seed_words; ++word)
    {
        if (word != 0)
            text += separator;

        std::format_to(std::back_inserter(text), "{:08x}", seed[word]);
    }
    return text;
}

std::format_context::iterator std::formatter<simulation::Engine>::format(
    simulation::Engine const& rng,
    std::format_context& context) const
{
    return std::format_to(
        context.out(),
        "Engine {}\nSeed = {}\n",
        simulation::Engine::engine_name,
        simulation::Engine::seed_to_string(rng.seed()));
}
