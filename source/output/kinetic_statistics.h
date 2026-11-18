#pragma once
#include "writer.h"
#include "types.h"
#include "kmc/simulation.h"
#include "utility/file.h"

#include <algorithm>
#include <concepts>
#include <map>
#include <stdexcept>
#include <type_traits>

namespace output
{
    // The model owns its descriptor and kinetic equivalence relation. The writer
    // aggregates event observations and active kinetic-class populations.
    template <class ModelType>
    class KineticStatistics : public Writer
    {
    public:
        using Writer::Writer;
        using Descriptor = typename ModelType::KineticsType::Descriptor;
        using CountMap = std::map<Descriptor, std::size_t>;

        auto& set_output_transitions(bool enabled)
        {
            output_transitions_ = enabled;
            return *this;
        }

        auto const& counts() const noexcept { return counts_; }
        auto const& transitions() const noexcept { return transitions_; }

    private:
        std::string_view name() const override { return "kinetic_statistics"; }

        std::string settings() const override
        {
            return std::format("output_transitions = {}", output_transitions_);
        }

        void capture_init(kmc::Simulation const& simulation) override
        {
            model_ = &dynamic_cast<kmc::RejectionTreeSimulation<ModelType> const&>(
                simulation).model();

            last_descriptor_.reset();
            counts_.clear();
            transitions_.clear();
        }

        void observe_selected(kmc::event::Record const& record) override
        {
            // The selected event already carries its registered kinetic class.
            auto const key{ record.class_key };
            Descriptor descriptor{ model_->kinetics().descriptor(key) };
            ++counts_[record.outcome][descriptor];

            if (record.outcome == kmc::event::Outcome::rejected)
                return;

            if (last_descriptor_)
                ++transitions_[*last_descriptor_][descriptor];

            last_descriptor_ = std::move(descriptor);
        }

        void capture_snapshot(kmc::Simulation const& simulation) override
        {
            auto& active{ counts_[kmc::event::Outcome::active] };
            active.clear();
            auto const& classes{ simulation.classes() };
            for (std::size_t i{ 1 }; i < classes.size(); ++i)
            {
                if (classes[i].count == 0)
                    continue;
                auto key{ static_cast<kmc::RateClassKey>(i) };
                Descriptor descriptor{ model_->kinetics().descriptor(key) };
                active[descriptor] += classes[i].count;
            }
        }

        void write_snapshot(kmc::Simulation const& simulation) override
        {
            auto row = [](Descriptor const& descriptor) {
                return DescriptorFormat<Descriptor>::row(descriptor);
            };

            for (auto const& [outcome, counts] : counts_)
            {
                std::string csv{ std::format("{},count\n", DescriptorFormat<Descriptor>::header()) };
                for (auto const& [descriptor, count] : counts)
                    std::format_to(std::back_inserter(csv), "{},{}\n", row(descriptor), count);
                utility::file::write<utility::file::Type::csv>(
                    writer_path() / enum_name(outcome) / snapshot_name(simulation.step()), csv);
            }

            if (transitions_.empty() or not output_transitions_)
                return;

            auto const header{ DescriptorFormat<Descriptor>::header() };
            auto const columns{ std::ranges::count(header, ',') + 1 };
            std::string csv{ "from" };
            csv.append(columns, ',');
            csv += "to";
            csv.append(columns, ',');
            csv += std::format("\n{0},{0},count\n", header);
            for (auto const& [from, destinations] : transitions_)
                for (auto const& [to, count] : destinations)
                    std::format_to(std::back_inserter(csv), "{},{},{}\n",
                        row(from), row(to), count);
            utility::file::write<utility::file::Type::csv>(
                writer_path() / "transitions" / snapshot_name(simulation.step()), csv);
        }

        bool output_transitions_{ false };
        ModelType const* model_{};
        std::optional<Descriptor> last_descriptor_{};
        std::map<kmc::event::Outcome, CountMap> counts_;
        std::map<Descriptor, CountMap> transitions_;
    };
}
