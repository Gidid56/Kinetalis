#pragma once
#include "writer.h"
#include "types.h"
#include "kmc/simulation.h"
#include "utility/file.h"

#include <compare>
#include <format>
#include <iterator>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace output
{
    // Exact activity averages, grouped by event type, element and initial
    // coordination. Detailed rate classes remain unchanged in the KMC.
    //
    // An event contributes dt / total_rate and 1 / total_rate to two global
    // integrals. A group's intensity is piecewise constant, so its contribution
    // can be integrated lazily, only when its population changes or at a
    // snapshot. The per-event work is O(1), apart from changed classes.
    template <class ModelT>
    class GroupedKineticActivity : public Writer
    {
    public:
        using Writer::Writer;
        using SimulationT = kmc::RejectionTreeSimulation<ModelT>;

        struct Group
        {
            kmc::event::Type event{};
            chemistry::Element element{};
            lattice::Coordination coordination{};
            auto operator<=>(Group const&) const = default;
        };

        struct Accumulation
        {
            double activity_fraction{};
            double activity_time{}; // seconds
        };

        struct Data
        {
            std::map<Group, Accumulation> groups;
            physics::quantity::TimeDifference residence_time{};
            std::size_t samples{};
        };

        auto const& last_data() const noexcept { return last_data_; }

    private:
        struct GroupState
        {
            double rate{}; // sum of active rates, in Hz
            double last_step_factor{};
            double last_time_factor{};
            Accumulation accumulation{};
        };

        std::string_view name() const override { return "grouped_kinetic_activity"; }
        bool requires_class_counts() const override { return true; }

        Subscription subscriptions() const override
        {
            return Writer::subscriptions() | Subscription::selected | Subscription::processed;
        }

        static Group group_of(typename ModelT::KineticsType::Descriptor const& descriptor)
        {
            return {descriptor.event, descriptor.element, descriptor.coordination};
        }

        void capture_init(kmc::Simulation const& simulation) override
        {
            simulation_ = &dynamic_cast<SimulationT const&>(simulation);
            last_time_ = simulation.time();
            last_data_ = {};
            groups_.clear();
            step_factor_ = 0.0;
            time_factor_ = 0.0;
            samples_ = 0;
            residence_time_ = {};

            auto const& classes{simulation.kinetics().kinetic_classes()};
            auto const& counts{simulation.class_counts()};
            weights_.assign(classes.size(), 0.0);
            for (std::size_t i{1}; i < classes.size(); ++i)
            {
                if (counts[i] == 0)
                    continue;
                const double weight{static_cast<double>(counts[i]) * classes[i].rate.value()};
                weights_[i] = weight;
                const auto descriptor{
                    simulation_->model().kinetics().descriptor(static_cast<kmc::RateClassKey>(i))};
                groups_[group_of(descriptor)].rate += weight;
            }
        }

        void observe_selected(kmc::event::Record const&) override
        {
            // The rates and class counts still refer to the state BEFORE
            // executing this selected (possibly rejected) event.
            const double total{simulation_->rate_tree()->rate_sum().value()};
            if (not (total > 0.0))
                return;
            const auto delta{simulation_->time() - last_time_};
            last_time_ = simulation_->time();
            step_factor_ += 1.0 / total;
            time_factor_ += delta.value() / total;
            residence_time_ += delta;
            ++samples_;
        }

        void flush(GroupState& state)
        {
            state.accumulation.activity_fraction +=
                state.rate * (step_factor_ - state.last_step_factor);
            state.accumulation.activity_time +=
                state.rate * (time_factor_ - state.last_time_factor);
            state.last_step_factor = step_factor_;
            state.last_time_factor = time_factor_;
        }

        void observe_processed(kmc::event::Record const&) override
        {
            // This callback runs after the KMC has updated affected slots.
            // The same key can appear several times in the change list; the
            // cached weight makes repeated notifications harmless.
            auto const& classes{simulation_->kinetics().kinetic_classes()};
            auto const& counts{simulation_->class_counts()};
            weights_.resize(classes.size(), 0.0);

            for (auto const key : simulation_->changed_class_keys())
            {
                const double updated{
                    static_cast<double>(counts[key]) * classes[key].rate.value()};
                auto& previous{weights_[key]};
                if (updated == previous)
                    continue;
                auto const descriptor{simulation_->model().kinetics().descriptor(key)};
                auto& state{groups_[group_of(descriptor)]};
                flush(state);
                state.rate += updated - previous;
                previous = updated;
            }
        }

        void capture_snapshot(kmc::Simulation const&) override
        {
            Data snapshot;
            snapshot.samples = samples_;
            snapshot.residence_time = residence_time_;
            for (auto& [key, state] : groups_)
            {
                flush(state);
                if (state.accumulation.activity_fraction != 0.0 or
                    state.accumulation.activity_time != 0.0)
                    snapshot.groups.emplace(key, state.accumulation);
                state.accumulation = {};
                state.last_step_factor = 0.0;
                state.last_time_factor = 0.0;
            }
            last_data_ = std::move(snapshot);
            step_factor_ = 0.0;
            time_factor_ = 0.0;
            samples_ = 0;
            residence_time_ = {};
        }

        void write_init(kmc::Simulation const&) override
        {
            utility::file::write<utility::file::Type::csv>(writer_path(),
                "key:step,time/s,key:event,key:element,key:coordination,"
                "activity_fraction,time_weighted_activity\n");
        }

        void write_snapshot(kmc::Simulation const& simulation) override
        {
            if (last_data_.samples == 0 or
                last_data_.residence_time <= physics::quantity::TimeDifference{})
                return;

            std::string rows;
            for (auto const& [key, value] : last_data_.groups)
                std::format_to(std::back_inserter(rows),
                    "{},{:v:},{},{},{},{:.16g},{:.16g}\n",
                    simulation.step(), simulation.time(), enum_name(key.event),
                    key.element, static_cast<unsigned>(key.coordination),
                    value.activity_fraction / static_cast<double>(last_data_.samples),
                    value.activity_time / last_data_.residence_time.value());

            if (not rows.empty())
                utility::file::append<utility::file::Type::csv>(writer_path(), rows);
        }

        SimulationT const* simulation_{};
        kmc::Time last_time_{};
        std::vector<double> weights_;
        std::map<Group, GroupState> groups_;
        double step_factor_{}, time_factor_{};
        std::size_t samples_{};
        physics::quantity::TimeDifference residence_time_{};
        Data last_data_{};
    };
}
