#pragma once
#include "observer.h"

#include <array>
#include <bit>
#include <cassert>
#include <limits>
#include <utility>

class kmc::AnalysisPlan
{
public:
    void set_resolution(Observer::Resolution const& resolution)
    {
        for (auto& observer : observers_)
            observer->set_resolution(resolution);
    }

    void observe_started(Simulation const& simulation)
    {
        for (auto& observer : observers_)
            observer->observe_started(simulation);
    }

    void observe_step(Simulation const& simulation, Step step)
    {
        for (auto* observer : subscribers(Subscription::snapshot))
            if (step >= observer->next_snapshot_step())
                observer->observe_step(simulation, step);
    }

    void observe_selected(event::Record const& record)
    {
        for (auto* observer : subscribers(Subscription::selected))
            observer->observe_selected(record);
    }

    void observe_processed(event::Record const& record)
    {
        for (auto* observer : subscribers(Subscription::processed))
            observer->observe_processed(record);
    }

    void observe_completed(Simulation const& simulation)
    {
        for (auto& observer : observers_)
            observer->observe_completed(simulation);
    }

    [[nodiscard]] bool requires_class_counts() const
    {
        return std::ranges::any_of(observers_, [](auto const& observer) {
            return observer->requires_class_counts();
        });
    }

    [[nodiscard]] bool stop_requested() const
    {
        return std::ranges::any_of(subscribers(Subscription::stopping), [](auto const* observer) {
            return observer->stop_requested();
        });
    }

    constexpr auto begin() const { return observers_.begin(); }
    constexpr auto end() const { return observers_.end(); }
    constexpr auto size() const { return observers_.size(); }
    constexpr auto& back() { return observers_.back(); }

    template <std::derived_from<kmc::Observer> ObserverType, typename... Arguments>
    ObserverType& add(Arguments&&... arguments)
    {
        auto observer = std::make_unique<ObserverType>(std::forward<Arguments>(arguments)...);

        auto& result = *observer;
        Observer& base = result;
        auto const flags{ base.subscriptions() };
        observers_.push_back(std::move(observer));
        for (auto subscription : subscription_flags_)
            if (Observer::has_subscription(flags, subscription))
                subscribers(subscription).push_back(&base);
        return result;
    }

    template <std::derived_from<kmc::Observer> ObserverType>
    ObserverType* find()
    {
        for (auto& observer : observers_)
            if (auto* result{ dynamic_cast<ObserverType*>(observer.get()) })
                return result;

        return nullptr;
    }

    template <std::derived_from<kmc::Observer> ObserverType>
    ObserverType& get()
    {
        if (auto* observer{ find<ObserverType>() })
            return *observer;

        throw std::logic_error{
            std::format("Analysis plan does not contain {}", typeid(ObserverType).name())
        };
    }

    std::string output_settings() const
    {
        std::string text;

        for (std::size_t index{}; auto& observer : observers_)
            //if (auto const settings{ observer->settings() }; not settings.empty())
            std::format_to(
                std::back_inserter(text),
                "[{}]\n{}\n\n",
                observer->name(),
                observer->settings());

        return text;
    }

    std::string output_resolution() const
    {
        std::string text{ "key:observer,min_snapshot_step,snapshot_scale_factor\n" };

        for (auto& observer : observers_)
            if (observer->uses_snapshot_resolution())
                std::format_to(
                    std::back_inserter(text),
                    "{},{},{}\n",
                    observer->name(),
                    observer->resolution().min_snapshot_step,
                    observer->resolution().snapshot_scale_factor);

        return text;
    }

private:
    using Subscription = Observer::Subscription;

    static constexpr std::array subscription_flags_{
        Subscription::selected,
        Subscription::processed,
        Subscription::snapshot,
        Subscription::stopping,
    };

    static constexpr std::size_t subscription_index(Subscription subscription) noexcept
    {
        const auto bits{ std::to_underlying(subscription) };
        assert(std::has_single_bit(bits));
        return std::countr_zero(bits);
    }

    auto& subscribers(Subscription subscription) noexcept
    {
        return subscribers_[subscription_index(subscription)];
    }

    auto const& subscribers(Subscription subscription) const noexcept
    {
        return subscribers_[subscription_index(subscription)];
    }

    std::vector<std::unique_ptr<Observer>> observers_;
    std::array<std::vector<Observer*>, subscription_flags_.size()> subscribers_;
};
