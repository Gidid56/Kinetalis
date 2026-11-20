#pragma once
#include "writer.h"
#include "utility/csv.h"
#include "types.h"
#include "kmc/simulation.h"
#include "utility/file.h"

#include <concepts>
#include <fstream>
#include <map>
#include <stdexcept>

namespace output
{
    template <class ModelT>
    class KineticActivity : public Writer
    {
    public:
        using Writer::Writer;
        using Descriptor = typename ModelT::KineticsType::Descriptor;

        struct Accumulation
        {
            double activity_fraction{};
            physics::quantity::TimeDifference activity_time{};
        };

        struct Data
        {
            std::map<Descriptor, Accumulation> classes;
            physics::quantity::TimeDifference residence_time{};
            std::size_t samples{};
        };

        auto const& last_data() const noexcept { return last_data_; }

    private:
        std::string_view name() const override { return "kinetic_activity"; }
        bool requires_class_counts() const override { return true; }

        void capture_init(kmc::Simulation const& simulation) override
        {
            simulation_ = &simulation;
            model_ = &dynamic_cast<kmc::RejectionTreeSimulation<ModelT> const&>(
                simulation).model();
            last_time_ = simulation.time();
            last_data_ = {};
            accumulation_ = {};
        }

        Subscription subscriptions() const override
        {
            return Writer::subscriptions() | Subscription::selected;
        }

        void observe_selected(kmc::event::Record const&) override
        {
            auto const& simulation{ *simulation_ };
            auto const delta{ simulation.time() - last_time_ };
            last_time_ = simulation.time();

            auto const& classes{ simulation.kinetics().kinetic_classes() };
            auto const& counts{ simulation.class_counts() };
            kmc::Rate total{};

            for (std::size_t i{ 1 }; i < classes.size(); ++i)
                if (counts[i] != 0)
                    total += counts[i] * classes[i].rate;

            if (total <= kmc::Rate{})
                return;

            for (std::size_t i{ 1 }; i < classes.size(); ++i)
            {
                auto const& kinetic_class{ classes[i] };
                if (counts[i] == 0 or not kinetic_class.is_initialized())
                    continue;

                auto const key{ static_cast<kmc::RateClassKey>(i) };
                auto const fraction{ counts[i] * kinetic_class.rate / total };
                auto& value{ accumulation_.classes[model_->kinetics().descriptor(key)] };
                value.activity_fraction += fraction;
                value.activity_time += fraction * delta;
            }

            accumulation_.residence_time += delta;
            ++accumulation_.samples;
        }

        void capture_snapshot(kmc::Simulation const&) override
        {
            last_data_ = std::exchange(accumulation_, {});
        }

        static std::string header(kmc::Simulation const& simulation, ModelT const& model)
        {
            std::string text{ Writer::default_header };
            auto const& classes{ simulation.kinetics().kinetic_classes() };
            for (std::size_t key{ 1 }; key < classes.size(); ++key)
            {
                if (classes[key].is_initialized())
                    std::format_to(std::back_inserter(text), ",{}",
                        DescriptorFormat<Descriptor>::label(
                            model.kinetics().descriptor(
                                static_cast<kmc::RateClassKey>(key))));
                else
                    std::format_to(std::back_inserter(text), ",not_encountered_{}", key);
            }
            return text;
        }

        void write_init(kmc::Simulation const& simulation) override
        {
            auto make_header = [this] { return header(*simulation_, *model_); };

            auto row = [this, &simulation](bool weighted) {
                std::string text{ std::format("{},{:v:}", simulation.step(), simulation.time()) };
                auto const samples{ static_cast<double>(last_data_.samples) };

                for (std::size_t i{ 1 }; i < simulation.kinetics().kinetic_classes().size(); ++i)
                {
                    if (not simulation.kinetics().kinetic_classes()[i].is_initialized())
                    {
                        text += ',';
                        continue;
                    }

                    Accumulation value{};
                    auto const& descriptor{ model_->kinetics().descriptor(
                        static_cast<kmc::RateClassKey>(i)) };
                    if (auto it{ last_data_.classes.find(descriptor) };
                        it != last_data_.classes.end())
                        value = it->second;

                    if (weighted)
                        std::format_to(std::back_inserter(text), ",{}",
                            value.activity_time / last_data_.residence_time);
                    else
                        std::format_to(std::back_inserter(text), ",{}",
                            value.activity_fraction / samples);
                }
                return text;
            };

            activity_.initialize(writer_path() / "activity", make_header,
                [row] { return row(false); });
            weighted_activity_.initialize(writer_path() / "time_weighted_activity",
                std::move(make_header), [row] { return row(true); });
        }

        void write_snapshot(kmc::Simulation const& simulation) override
        {
            if (last_data_.samples == 0 or
                last_data_.residence_time <= physics::quantity::TimeDifference{})
                return;

            auto const columns{ simulation.kinetics().kinetic_classes().size() - 1 };
            activity_.append(columns);
            weighted_activity_.append(columns);
        }

        void write_final(kmc::Simulation const&) override
        {
            activity_.finalize();
            weighted_activity_.finalize();
        }

        kmc::Simulation const* simulation_{};
        ModelT const* model_{};
        kmc::Time last_time_{};
        Data last_data_{}, accumulation_{};
        utility::csv::Writer activity_, weighted_activity_;
    };
}
