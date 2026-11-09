#pragma once
#include <array>
#include <cstddef>

namespace utility
{
    template <class T>
    struct NullReference
    {
        constexpr operator T() const noexcept { return {}; }

        constexpr NullReference& operator=(T) noexcept { return *this; }
        constexpr NullReference& operator+=(T) noexcept { return *this; }
        constexpr NullReference& operator-=(T) noexcept { return *this; }

        constexpr NullReference& operator++() noexcept { return *this; }
        constexpr NullReference& operator--() noexcept { return *this; }

        constexpr T operator++(int) noexcept { return {}; }
        constexpr T operator--(int) noexcept { return {}; }
    };

    template <class T, std::size_t size, bool enabled>
    struct ArrayStorage
    {
        ArrayStorage() = default;
        ArrayStorage(std::array<T, size> const& values) { }

        constexpr NullReference<T> operator[](std::size_t) noexcept { return {}; }
        constexpr T operator[](std::size_t) const noexcept { return {}; }

        constexpr std::array<T, 0> const& operator*() const noexcept { return empty; }

    private:
        inline static constexpr std::array<T, 0> empty{};
    };

    template <class T, std::size_t size>
    struct ArrayStorage<T, size, true>
    {
        ArrayStorage() = default;
        ArrayStorage(std::array<T, size> const& values) : values_{ values } { }

        constexpr T& operator[](std::size_t index) noexcept { return values_[index]; }
        constexpr T const& operator[](std::size_t index) const noexcept { return values_[index]; }

        constexpr std::array<T, size> const& operator*() const noexcept { return values_; }

    private:
        std::array<T, size> values_{};
    };
}

namespace utility
{
    struct Empty
    {
    };

    template <class T, bool enabled>
    struct Storage
    {
        T get() const noexcept { return {}; }
        void set(T) noexcept { }
    };

    template <class T>
    struct Storage<T, true>
    {
        T get() const noexcept { return value_; }
        void set(T value) noexcept { value_ = value; }

    private:
        T value_{};
    };
}