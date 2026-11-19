#pragma once
#include "writer.h"
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
            class_header_.clear();
            class_columns_ = {};
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

        static void replace_header(std::filesystem::path path, std::string_view new_header,
                                   std::size_t extra_columns)
        {
            path.replace_extension(enum_name(utility::file::Type::csv));
            auto temporary{ path };
            temporary += ".tmp";
            {
                auto input{ utility::file::wait_for_read(path) };
                std::string old_header;
                if (not std::getline(input, old_header))
                    throw std::runtime_error{ "Cannot replace empty kinetic activity CSV header." };
                auto output{ utility::file::wait_for_open<std::ofstream>(
                    temporary, std::ios_base::out | std::ios_base::trunc) };
                output << new_header << '\n';
                for (std::string line; std::getline(input, line);)
                {
                    output << line;
                    for (std::size_t i{}; i < extra_columns; ++i)
                        output.put(',');
                    output.put('\n');
                }
                if (input.bad() or not output)
                    throw std::runtime_error{ "Failed to rewrite kinetic activity CSV." };
            }
            std::filesystem::rename(temporary, path);
        }

        void write_init(kmc::Simulation const& simulation) override
        {
            class_header_ = header(simulation, *model_);
            class_columns_ = simulation.kinetics().kinetic_classes().size() - 1;
            utility::file::write<utility::file::Type::csv>(
                writer_path() / "activity", class_header_ + '\n');
            utility::file::write<utility::file::Type::csv>(
                writer_path() / "time_weighted_activity", class_header_ + '\n');
        }

        void write_snapshot(kmc::Simulation const& simulation) override
        {
            if (last_data_.samples == 0 or
                last_data_.residence_time <= physics::quantity::TimeDifference{})
                return;

            auto const new_header{ header(simulation, *model_) };
            if (new_header != class_header_)
            {
                auto const new_columns{ simulation.kinetics().kinetic_classes().size() - 1 };
                if (new_columns < class_columns_)
                    throw std::logic_error{ "Kinetic class count shrank during output." };
                auto const extra{ new_columns - class_columns_ };
                replace_header(writer_path() / "activity", new_header, extra);
                replace_header(writer_path() / "time_weighted_activity", new_header, extra);
                class_header_ = new_header;
                class_columns_ = new_columns;
            }

            auto const prefix{ std::format("{},{:v:}", simulation.step(), simulation.time()) };
            auto const samples{ static_cast<double>(last_data_.samples) };
            std::string activity{ prefix };
            std::string weighted{ prefix };

            for (std::size_t i{ 1 }; i < simulation.kinetics().kinetic_classes().size(); ++i)
            {
                if (not simulation.kinetics().kinetic_classes()[i].is_initialized())
                {
                    activity += ',';
                    weighted += ',';
                    continue;
                }

                Accumulation value{};
                auto const& descriptor{ model_->kinetics().descriptor(static_cast<kmc::RateClassKey>(i)) };
                if (auto it{ last_data_.classes.find(descriptor) };
                    it != last_data_.classes.end())
                    value = it->second;

                std::format_to(std::back_inserter(activity), ",{}",
                    value.activity_fraction / samples);
                std::format_to(std::back_inserter(weighted), ",{}",
                    value.activity_time / last_data_.residence_time);
            }
            utility::file::append<utility::file::Type::csv>(
                writer_path() / "activity", activity + '\n');
            utility::file::append<utility::file::Type::csv>(
                writer_path() / "time_weighted_activity", weighted + '\n');
        }

        kmc::Simulation const* simulation_{};
        ModelT const* model_{};
        kmc::Time last_time_{};
        Data last_data_{}, accumulation_{};
        std::string class_header_;
        std::size_t class_columns_{};
    };
}
