#pragma once
#include "observer.h"

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

    void observe_step(Simulation const& simulation)
    {
        for (auto& observer : observers_)
            observer->observe_step(simulation);
    }

    void observe_selected(process::Record const& process)
    {
        for (auto& observer : observers_)
            observer->observe_selected(process);
    }

    void observe_processed(process::Record const& process)
    {
        for (auto& observer : observers_)
            observer->observe_processed(process);
    }

    void observe_completed(Simulation const& simulation)
    {
        for (auto& observer : observers_)
            observer->observe_completed(simulation);
    }

    [[nodiscard]] bool stop_requested() const
    {
        return std::ranges::any_of(observers_, [](auto const& observer) {
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
        observers_.push_back(std::move(observer));
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

    std::string parameters() const
    {
        std::string text{ "key:observer,min_snapshot_step,snapshot_scale_factor,parameters\n" };

        for (auto& observer : observers_)
            std::format_to(
                std::back_inserter(text),
                "{},{},{},{}\n",
                observer->name(),
                observer->resolution().min_snapshot_step,
                observer->resolution().snapshot_scale_factor,
                observer->parameters());

        return text;
    }

private:
    std::vector<std::unique_ptr<Observer>> observers_;
};
