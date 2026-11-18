#pragma once
#include "event.h"
#include "utility/enums.h"

#include <cassert>
#include <concepts>
#include <cstddef>
#include <format>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace kmc
{
    // Minimal physical-process contract. Geometrical predicates and
    // postselection rejection are optional, with zero-cost compile-time defaults.
    template <class T>
    concept Process = requires(T const& process, lattice::Site const& source,
                               event::Location const& location, lattice::Lattice& lattice) {
        { T::event } -> std::convertible_to<event::Type>;
        { T::cluster_size } -> std::convertible_to<std::size_t>;
        { T::slot_count } -> std::convertible_to<std::size_t>;
        { process.is_candidate(source) } -> std::convertible_to<bool>;
        { process.execute(location, lattice) } -> std::same_as<void>;
        { process.settings() } -> std::convertible_to<std::string>;
    } and (T::slot_count > 0) and
      (T::cluster_size > 0 and T::cluster_size <= event::Location::capacity) and
      (T::cluster_size == 1 or requires(T const& process, lattice::Site& source,
                                       std::size_t slot, lattice::Lattice& lattice) {
          { process.location(source, slot, lattice) } -> std::same_as<event::Location>;
      });

    template <Process T>
    bool has_slot(T const& process, lattice::Site const& source, std::size_t slot)
    {
        if constexpr (requires {
            { process.has_slot(source, slot) } -> std::convertible_to<bool>;
        })
            return process.has_slot(source, slot);
        else
            return true;
    }

    template <Process T>
    event::Location make_location(T const& process, lattice::Site& source,
                                  std::size_t slot, lattice::Lattice& lattice)
    {
        if constexpr (requires {
            { process.location(source, slot, lattice) } -> std::same_as<event::Location>;
        })
            return process.location(source, slot, lattice);
        else
            return event::Location{ &source };
    }

    // The source predicate has already been checked once per process and site.
    template <Process T>
    bool is_location_candidate(T const& process, event::Location const& location)
    {
        if constexpr (requires {
            { process.is_candidate(location) } -> std::convertible_to<bool>;
        })
            return process.is_candidate(location);
        else
            return true;
    }

    template <Process T>
    bool is_rejected(T const& process, event::Location const& location)
    {
        if constexpr (requires {
            { process.is_rejected(location) } -> std::convertible_to<bool>;
        })
            return process.is_rejected(location);
        else
            return false;
    }

    template <class... Ts>
    requires (Process<Ts> && ...)
    class Processes
    {
    public:
        static constexpr std::size_t slot_count{ (Ts::slot_count + ... + std::size_t{}) };
        static_assert(((Ts::cluster_size > 0 and Ts::cluster_size <= event::Location::capacity) && ...));

        template <class T>
        auto& get(this auto& self) noexcept { return std::get<T>(self.processes_); }

        // Short-circuit as soon as any process accepts the source.
        bool is_source_active(lattice::Site const& site) const
        {
            return std::apply([&](auto const&... processes) {
                return (processes.is_candidate(site) or ...);
            }, processes_);
        }

        template <class Function>
        void for_each(Function&& function) const
        {
            std::size_t offset{};
            std::apply([&](auto const&... processes) {
                ((std::invoke(function, processes,
                    std::exchange(offset, offset +
                        std::remove_cvref_t<decltype(processes)>::slot_count))), ...);
            }, processes_);
        }

        template <class Function>
        void visit(std::size_t slot, Function&& function) const
        {
            std::size_t offset{};
            const bool found{ std::apply([&](auto const&... processes) {
                return (([&] {
                    constexpr auto count{
                        std::remove_cvref_t<decltype(processes)>::slot_count
                    };
                    const auto first{ std::exchange(offset, offset + count) };
                    if (slot < first or slot - first >= count)
                        return false;

                    std::invoke(function, processes, slot - first);
                    return true;
                }()) or ...);
            }, processes_) };

            if (not found)
                throw std::out_of_range{ "Event slot outside process layout." };
        }

        event::Record make_record(std::size_t slot, lattice::Site& source,
                                  lattice::Lattice& lattice, RateClassKey class_key) const
        {
            std::optional<event::Record> record;
            visit(slot, [&](auto const& process, std::size_t local_slot) {
                assert(kmc::has_slot(process, source, local_slot));
                auto location{ kmc::make_location(process, source, local_slot, lattice) };
                assert(location.sites().size() ==
                    std::remove_cvref_t<decltype(process)>::cluster_size);
                const bool rejected{ kmc::is_rejected(process, location) };
                record.emplace(std::move(location),
                    std::remove_cvref_t<decltype(process)>::event, rejected, class_key);
            });
            return std::move(*record);
        }

        void execute(std::size_t slot, event::Location const& location,
                     lattice::Lattice& lattice) const
        {
            visit(slot, [&](auto const& process, std::size_t) {
                assert(location.sites().size() ==
                    std::remove_cvref_t<decltype(process)>::cluster_size);
                process.execute(location, lattice);
            });
        }

        std::string settings() const
        {
            std::string result;
            for_each([&](auto const& process, std::size_t) {
                result += std::format("[{}]\n{}\n\n",
                    enum_name(std::remove_cvref_t<decltype(process)>::event),
                    process.settings());
            });
            return result;
        }

    private:
        std::tuple<Ts...> processes_{};
    };
}
