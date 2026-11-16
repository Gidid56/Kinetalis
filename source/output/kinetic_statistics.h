#pragma once
#include "writer.h"
#include "kmc/simulation.h"
#include "utility/file.h"

#include <concepts>
#include <map>
#include <stdexcept>
#include <type_traits>

namespace output
{
    // The model owns its descriptor and kinetic equivalence relation. Only
    // descriptor-independent event observations are handled by this writer.
    template <class ModelType>
    requires std::derived_from<ModelType, kmc::Model>
    class KineticStatistics : public Writer
    {
    public:
        using Writer::Writer;
        using Descriptor = typename ModelType::Descriptor;
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
            simulation_ = &simulation;
            model_ = dynamic_cast<ModelType*>(simulation.model().get());
            if (not model_)
                throw std::logic_error{ "KineticStatistics requires the selected model type." };

            last_descriptor_.reset();
            counts_.clear();
            transitions_.clear();
            keys_.clear();
        }

        void observe_selected(kmc::event::Record const& record) override
        {
            // Observe before event execution, while the local configuration is unchanged.
            auto key{ model_->classify(record.type, record.location(), simulation_->lattice()) };
            Descriptor descriptor{ model_->descriptor(key) };
            keys_[descriptor] = key;
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
            auto const& counts{ simulation.process_counts() };
            for (std::size_t i{ 1 }; i < counts.size(); ++i)
            {
                if (counts[i] == 0)
                    continue;
                auto key{ static_cast<kmc::RateClassKey>(i) };
                Descriptor descriptor{ model_->descriptor(key) };
                keys_[descriptor] = key;
                active[descriptor] += counts[i];
            }
        }

        void write_snapshot(kmc::Simulation const& simulation) override
        {
            auto info = [&](Descriptor const& descriptor) -> kmc::ClassInfo const& {
                return model_->class_info(keys_.at(descriptor));
            };

            for (auto const& [outcome, counts] : counts_)
            {
                std::string csv{ kmc::ClassInfo::header };
                csv += ",count\n";
                for (auto const& [descriptor, count] : counts)
                    std::format_to(std::back_inserter(csv), "{},{}\n", info(descriptor), count);
                utility::file::write<utility::file::Type::csv>(
                    writer_path() / enum_name(outcome) / snapshot_name(simulation.step()), csv);
            }

            if (transitions_.empty() or not output_transitions_)
                return;

            std::string csv{ std::format("from,,,,to,,,,\n{0},{0},count\n", kmc::ClassInfo::header) };
            for (auto const& [from, destinations] : transitions_)
                for (auto const& [to, count] : destinations)
                    std::format_to(std::back_inserter(csv), "{},{},{}\n",
                        info(from), info(to), count);
            utility::file::write<utility::file::Type::csv>(
                writer_path() / "transitions" / snapshot_name(simulation.step()), csv);
        }

        bool output_transitions_{ false };
        kmc::Simulation const* simulation_{};
        ModelType* model_{};
        std::optional<Descriptor> last_descriptor_{};
        std::map<Descriptor, kmc::RateClassKey> keys_;
        std::map<kmc::event::Outcome, CountMap> counts_;
        std::map<Descriptor, CountMap> transitions_;
    };
}
