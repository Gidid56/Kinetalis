#pragma once
#include <semaphore>

namespace utility
{
    class Permit
    {
    public:
        explicit Permit(std::counting_semaphore<>& semaphore) : semaphore_{ &semaphore }
        {
            semaphore_->acquire();
        }

        Permit(Permit const&) = delete;
        Permit& operator=(Permit const&) = delete;

        Permit(Permit&& other) noexcept : semaphore_{ std::exchange(other.semaphore_, nullptr) } { }

        Permit& operator=(Permit&&) = delete;

        ~Permit()
        {
            if (semaphore_)
                semaphore_->release();
        }

    private:
        std::counting_semaphore<>* semaphore_;
    };
}