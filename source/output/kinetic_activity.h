#pragma once
#include "writer.h"
#include "kmc/simulation.h"
#include "utility/file.h"

#include <concepts>
#include <fstream>
#include <map>
#include <stdexcept>

namespace output
{
    template <class ModelType>
    requires std::derived_from<ModelType, kmc::Model>
    class KineticActivity : public Writer
    {
    public:
        using Writer::Writer;
        using Descriptor = typename ModelType::Descriptor;

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

        void capture_init(kmc::Simulation const& simulation) override
        {
            simulation_ = &simulation;
            model_ = dynamic_cast<ModelType const*>(simulation.model().get());
            if (not model_)
                throw std::logic_error{ "KineticActivity requires the selected model type." };
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

            auto const& counts{ simulation.class_counts() };
            auto const& rates{ simulation.class_rates() };
            kmc::Rate total{};

            for (std::size_t i{ 1 }; i < counts.size(); ++i)
                if (rates[i].first.is_active())
                    total += counts[i] * rates[i].second;

            if (total <= kmc::Rate{})
                return;

            for (std::size_t i{ 1 }; i < counts.size(); ++i)
            {
                if (counts[i] == 0 or not rates[i].first.is_active())
                    continue;

                auto const key{ static_cast<kmc::RateClassKey>(i) };
                auto const fraction{ counts[i] * rates[i].second / total };
                auto& value{ accumulation_.classes[model_->descriptor(key)] };
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

        static std::string header(kmc::Simulation const& simulation)
        {
            std::string text{ Writer::default_header };
            auto const& rates{ simulation.class_rates() };
            for (std::size_t key{ 1 }; key < rates.size(); ++key)
            {
                auto const& info{ rates[key].first };
                if (info.is_active())
                    std::format_to(std::back_inserter(text), ",{}", info.label());
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
            class_header_ = header(simulation);
            class_columns_ = simulation.class_rates().size() - 1;
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

            auto const new_header{ header(simulation) };
            if (new_header != class_header_)
            {
                auto const new_columns{ simulation.class_rates().size() - 1 };
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

            for (std::size_t i{ 1 }; i < simulation.class_rates().size(); ++i)
            {
                auto const& info{ simulation.class_rates()[i].first };
                if (not info.is_active())
                {
                    activity += ',';
                    weighted += ',';
                    continue;
                }

                Accumulation value{};
                auto const& descriptor{ model_->descriptor(static_cast<kmc::RateClassKey>(i)) };
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
        ModelType const* model_{};
        kmc::Time last_time_{};
        Data last_data_{}, accumulation_{};
        std::string class_header_;
        std::size_t class_columns_{};
    };
}
