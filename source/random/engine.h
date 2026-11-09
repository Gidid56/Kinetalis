#pragma once
#include "lattice/types.h"

#include <random>

// "random" conflicts with the POSIX/glibc global random() function and can prevent Linux compilation
namespace random_ { class Engine; }

class random_::Engine
{
public:
    using seed_word_type = std::uint32_t;
    static constexpr std::size_t seed_words{ 8 }; // 256-bit seed
    using seed_type = std::array<seed_word_type, seed_words>;

    using state_word_type = std::uint64_t;
    static constexpr std::size_t state_words{ 4 };
    using state_type = std::array<state_word_type, state_words>;

    static constexpr std::string_view engine_name{ "xoshiro256+ 1.0" };

    explicit Engine(seed_type const& seed) : seed_{ seed }, state_{ make_state(seed) } { }

    explicit Engine(std::optional<seed_type> const& seed) : Engine(seed ? *seed : random_seed()) { }

    explicit Engine(std::string_view seed) : Engine(seed.empty() ? random_seed() : parse_seed(seed))
    { }

    explicit Engine() : Engine(random_seed()) { }

    seed_type const& seed() const noexcept { return seed_; }
    state_type const& state() const noexcept { return state_; }

    [[nodiscard]] seed_type generate_seed() noexcept;

    // Generates a random floating-point number in the range [0, 1).
    physics::Probability probability() noexcept
    {
        static_assert(
            std::numeric_limits<double>::is_iec559 and std::numeric_limits<double>::digits == 53);

        // Use the upper 53 bits to construct an exactly representable binary64 fraction.
        return static_cast<double>(next() >> 11) * 0x1.0p-53;
    }

    // Generates a random Boolean value from the most significant output bit.
    bool coin_flip() noexcept { return static_cast<bool>(next() >> 63); }

    // Generates an unbiased random integer in the range [0, max].
    template <std::unsigned_integral UInt>
    requires(not std::same_as<UInt, bool>)
    UInt integer(UInt max) noexcept
    {
        static_assert(sizeof(UInt) <= sizeof(state_word_type));

        if constexpr (sizeof(UInt) == sizeof(state_word_type))
            if (max == std::numeric_limits<UInt>::max())
                return static_cast<UInt>(next());

        state_word_type const range{ static_cast<state_word_type>(max) + 1 };
        return static_cast<UInt>(bounded(range));
    }

    // Generates a normalized 3D vector pointing into a random direction.
    math::Vector<3> direction();

    // Selects a random element from the given composition, according to its fractional content.
    chemistry::Element element(chemistry::Composition const&);

    // Selects a random position inside the given shape.
    lattice::Position position(lattice::Shape const&);

    // Selects a random Miller index from [1,0,0] to [maxIndex,maxIndex,maxIndex].
    lattice::Indices miller_indices(int max_index);

    static std::string seed_to_string(seed_type const&, std::string_view separator = "-");

    static seed_type parse_seed(std::string_view);

    static seed_type random_seed()
    {
        std::random_device seed_generator{};

        seed_type seed;
        std::ranges::generate(seed, std::ref(seed_generator));
        return seed;
    }

    static state_type make_state(seed_type const& seed)
    {
        state_type state{};
        for (std::size_t i{}; i < state.size(); ++i)
            state[i] = (static_cast<state_word_type>(seed[2 * i]) << 32) |
                       static_cast<state_word_type>(seed[2 * i + 1]);

        if (std::ranges::all_of(state, [](state_word_type word) { return word == 0; }))
            throw std::invalid_argument{ "xoshiro256+ state must not be entirely zero" };

        return state;
    }

private:
    state_word_type next() noexcept
    {
        const state_word_type result{ state_[0] + state_[3] };
        const state_word_type temporary{ state_[1] << 17 };

        state_[2] ^= state_[0];
        state_[3] ^= state_[1];
        state_[1] ^= state_[2];
        state_[0] ^= state_[3];
        state_[2] ^= temporary;
        state_[3] = std::rotl(state_[3], 45);

        return result;
    }

    // Returns an unbiased integer in [0, range), where range > 0.
    // Only the most significant output bits are used because xoshiro256+ has
    // weaker linear complexity in its lowest three bits.
    state_word_type bounded(state_word_type range) noexcept
    {
        if (range == 1)
            return 0;

        const auto bits{ std::bit_width(range - 1) };
        state_word_type value;
        do
            value = next() >> (64 - bits);
        while (value >= range);

        return value;
    }

    seed_type seed_;
    state_type state_;
};

template <>
struct std::formatter<random_::Engine> : public std::formatter<std::string_view>
{
    std::format_context::iterator format(random_::Engine const&, std::format_context&) const;
};
