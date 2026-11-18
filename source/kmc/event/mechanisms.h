#pragma once
#include "mechanism.h"
#include "utility/enums.h"

#include <format>
#include <functional>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

namespace kmc::event
{
    template <class... Ts>
    requires (Mechanism<Ts> && ...)
    class Mechanisms
    {
    public:
        static constexpr std::size_t slot_count{ (Ts::slot_count + ... + std::size_t{}) };

        template <class T>
        auto& get(this auto& self) noexcept { return std::get<T>(self.mechanisms_); }

        // Short-circuit as soon as any mechanism accepts the source.
        bool is_source_active(lattice::Site const& site) const
        {
            return std::apply([&](auto const&... mechanisms) {
                return (mechanisms.is_source_candidate(site) or ...);
            }, mechanisms_);
        }

        template <class Function>
        void for_each(Function&& function) const
        {
            std::size_t offset{};
            std::apply([&](auto const&... mechanisms) {
                ((std::invoke(function, mechanisms,
                    std::exchange(offset, offset +
                        std::remove_cvref_t<decltype(mechanisms)>::slot_count))), ...);
            }, mechanisms_);
        }

        template <class Function>
        void visit(std::size_t slot, Function&& function) const
        {
            std::size_t offset{};
            const bool found{ std::apply([&](auto const&... mechanisms) {
                return (([&] {
                    constexpr auto count{
                        std::remove_cvref_t<decltype(mechanisms)>::slot_count
                    };
                    const auto first{ std::exchange(offset, offset + count) };
                    if (slot < first or slot - first >= count)
                        return false;

                    std::invoke(function, mechanisms, slot - first);
                    return true;
                }()) or ...);
            }, mechanisms_) };

            if (not found)
                throw std::out_of_range{ "Event slot outside mechanism layout." };
        }

        std::string settings() const
        {
            std::string result;
            for_each([&](auto const& mechanism, std::size_t) {
                result += std::format("[{}]\n{}\n\n",
                    enum_name(std::remove_cvref_t<decltype(mechanism)>::event),
                    mechanism.settings());
            });
            return result;
        }

    private:
        std::tuple<Ts...> mechanisms_{};
    };
}
