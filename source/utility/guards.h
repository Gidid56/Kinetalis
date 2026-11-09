#pragma once
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace utility
{
    // Removes elements appended after construction.
    template <class T>
    class SizeGuard
    {
    public:
        explicit SizeGuard(std::vector<T>& container, std::size_t size)
            : container_{ container },
              size_{ size }
        {
            if (size > container.size())
                throw std::invalid_argument{ "Guard size exceeds container size." };
        }

        explicit SizeGuard(std::vector<T>& container) : SizeGuard{ container, container.size() } { }

        SizeGuard(SizeGuard const&) = delete;
        SizeGuard& operator=(SizeGuard const&) = delete;
        SizeGuard(SizeGuard&&) = delete;
        SizeGuard& operator=(SizeGuard&&) = delete;

        ~SizeGuard()
        {
            assert(container_.size() >= size_);

            if (container_.size() > size_)
                container_.erase(container_.begin() + size_, container_.end());
        }

        std::size_t size() const noexcept { return size_; }

    private:
        std::vector<T>& container_;
        std::size_t size_;
    };
}