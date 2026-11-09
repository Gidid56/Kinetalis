#pragma once

namespace kmc::rate_tree
{
    using Index = lattice::Index;

    inline constexpr std::size_t baseLevel{ 4 };
    inline constexpr std::size_t allocationBlockLevels{ 8 };
    inline constexpr std::size_t maxHeight{ 32 };

    namespace branch
    {
        template <std::size_t level>
        class Base
        {
        public:
            static constexpr std::size_t span{ math::power<level>(2ull) };
            static constexpr std::size_t index_bit{ 1ull << level };

            auto rate_sum() const { return rate_sum_; }

        protected:
            Rate rate_sum_{};
        };
    }

    template <std::size_t level>
    class Branch : public branch::Base<level>
    {
        enum Direction { left = 0, right = 1 };

    public:
        using branch::Base<level>::rate_sum_;
        using Child = Branch<level - 1>;
        using Branches = std::array<Child, 2>;

        static constexpr bool usePtr{ (level - baseLevel) % allocationBlockLevels == 0 };

        void construct(std::size_t required_size)
        {
            if (required_size == 0)
                return;

            if constexpr (usePtr)
                if (not branches_)
                    branches_ = std::make_unique<Branches>();

            branch(Direction::left).construct(std::min(required_size, Child::span));

            if (required_size > Child::span)
                branch(Direction::right)
                    .construct(static_cast<std::size_t>(required_size - Child::span));
        }

        bool direction(Index index) const { return index & Child::index_bit; }

        void update_rate(Index index, Rate new_rate)
        {
            branch(direction(index)).update_rate(index, new_rate);

            rate_sum_ = branch(Direction::left).rate_sum() + branch(Direction::right).rate_sum();
        }

        Rate block_rate(Index index) const { return branch(direction(index)).block_rate(index); }

        Index index(Rate& selected_rate) const
        {
            const auto left_rate{ branch(Direction::left).rate_sum() };
            const auto direction{ selected_rate >= left_rate };

            return direction * Child::index_bit |
                   branch(direction).index(selected_rate -= direction * left_rate);
        }

        auto& branch(Index index)
        {
            if constexpr (usePtr)
                return (*branches_)[index];
            else
                return branches_[index];
        }

        auto const& branch(Index index) const
        {
            if constexpr (usePtr)
                return (*branches_)[index];
            else
                return branches_[index];
        }

    private:
        std::conditional_t<usePtr, std::unique_ptr<Branches>, Branches> branches_{};
    };

    template <>
    class Branch<baseLevel> : public branch::Base<baseLevel>
    {
    public:
        void construct(std::size_t) { }

        void update_rate(Index, Rate new_rate) { rate_sum_ = new_rate; }

        Rate block_rate(Index) const { return rate_sum_; }

        Index index(Rate&) const { return {}; }
    };

    struct Interface
    {
        virtual ~Interface() = default;

        virtual Rate rate_sum() const = 0;
        virtual Rate block_rate(Index) const = 0;
        virtual void update_rate(Index, Rate) = 0;
        virtual Index index(Rate&) const = 0;
    };

    template <std::size_t level>
    struct Entry final : Interface
    {
        explicit Entry(std::size_t required_size) { branch.construct(required_size); }

        Rate rate_sum() const override { return branch.rate_sum(); }

        Rate block_rate(Index index) const override { return branch.block_rate(index); }

        void update_rate(Index index, Rate new_rate) override
        {
            branch.update_rate(index, new_rate);
        }

        Index index(Rate& selected_rate) const override { return branch.index(selected_rate); }

        Branch<level> branch;
    };

    template <std::size_t level = baseLevel>
    std::unique_ptr<Interface> make_entry(std::size_t required_size)
    {
        if (required_size <= Branch<level>::span)
            return std::make_unique<Entry<level>>(required_size);

        if constexpr (level < maxHeight)
            return make_entry<level + 1>(required_size);
        else
            throw std::length_error{ "rate tree exceeds maximum size" };
    }
}