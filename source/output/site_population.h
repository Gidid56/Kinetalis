#pragma once
#include "output/writer.h"
#include "kmc/simulation.h"
#include "utility/file.h"

#include <concepts>
#include <map>
#include <set>

namespace output
{
    // Site populations inferred from FCC coordination and eligible event-path
    // multiplicities. These assumptions are specific to the dealloying study,
    // not part of generic kinetic-class analysis.
    template <class ModelType>
    requires std::derived_from<ModelType, kmc::Model>
    class SitePopulation : public Writer
    {
    public:
        using Writer::Writer;
        using Descriptor = typename ModelType::Descriptor;
        using SiteKey = std::pair<lattice::Element, lattice::Coordination>;

        struct Accumulation
        {
            double count{};
            double population_fraction{};
            physics::quantity::TimeDifference count_time{};
            physics::quantity::TimeDifference population_time{};
        };

        struct Data
        {
            std::map<SiteKey, Accumulation> sites;
            physics::quantity::TimeDifference residence_time{};
            std::size_t samples{};
        };

        auto const& last_data() const noexcept { return last_data_; }

    private:
        std::string_view name() const override { return "site_population"; }

        static std::map<SiteKey, std::size_t> site_counts(
            kmc::Simulation const& simulation, ModelType const& model)
        {
            std::map<SiteKey, std::size_t> counts;
            std::set<SiteKey> direct_counts;
            auto const& class_counts{ simulation.class_counts() };

            for (std::size_t index{ 1 }; index < class_counts.size(); ++index)
            {
                auto const count{ class_counts[index] };
                if (count == 0)
                    continue;

                auto const key{ static_cast<kmc::RateClassKey>(index) };
                auto const& descriptor{ model.descriptor(key) };
                if (descriptor.event == kmc::event::Type::none or
                    descriptor.element == lattice::Element::none)
                    continue;

                SiteKey site{ descriptor.element, descriptor.coordination };
                if (descriptor.event != kmc::event::Type::diffusion)
                {
                    if (direct_counts.contains(site) and counts[site] != count)
                        throw std::logic_error{ "Conflicting direct site-class counts." };
                    counts[site] = count;
                    direct_counts.insert(site);
                    continue;
                }

                if (direct_counts.contains(site))
                    continue;

                auto const paths{ static_cast<std::size_t>(lattice::max_coordination) -
                                  static_cast<std::size_t>(descriptor.coordination) };
                if (paths == 0)
                    continue;
                if (count % paths != 0)
                    throw std::logic_error{
                        "Diffusion count is inconsistent with site-path multiplicity."
                    };
                counts[site] = count / paths;
            }
            return counts;
        }

        void capture_init(kmc::Simulation const& simulation) override
        {
            simulation_ = &simulation;
            model_ = dynamic_cast<ModelType*>(simulation.model().get());
            if (not model_)
                throw std::logic_error{ "SitePopulation requires the selected model type." };
            last_time_ = simulation.time();
            last_data_ = {};
            accumulation_ = {};
        }

        void observe_selected(kmc::event::Record const&) override
        {
            auto const delta{ simulation_->time() - last_time_ };
            last_time_ = simulation_->time();

            auto const counts{ site_counts(*simulation_, *model_) };
            std::size_t total{};
            for (auto const& [_, count] : counts)
                total += count;
            if (total == 0)
                return;

            for (auto const& [site, count] : counts)
            {
                auto const population{ static_cast<double>(count) / total };
                auto& value{ accumulation_.sites[site] };
                value.count += count;
                value.population_fraction += population;
                value.count_time += static_cast<double>(count) * delta;
                value.population_time += population * delta;
            }

            accumulation_.residence_time += delta;
            ++accumulation_.samples;
        }

        void capture_snapshot(kmc::Simulation const&) override
        {
            last_data_ = std::exchange(accumulation_, {});
        }

        void write_init(kmc::Simulation const&) override
        {
            std::string header{ Writer::default_header };
            for (auto element : lattice::active_elements)
                for (lattice::Coordination coordination{}; coordination < lattice::max_coordination;
                     ++coordination)
                    std::format_to(std::back_inserter(header), ",{}_{}",
                        enum_name(element), static_cast<unsigned>(coordination));
            header += '\n';

            for (auto const* file : { "site_count", "population",
                "time_weighted_site_count", "time_weighted_population" })
                utility::file::write<utility::file::Type::csv>(writer_path() / file, header);
        }

        void write_snapshot(kmc::Simulation const& simulation) override
        {
            if (last_data_.samples == 0 or
                last_data_.residence_time <= physics::quantity::TimeDifference{})
                return;

            auto const prefix{ std::format("{},{:v:}", simulation.step(), simulation.time()) };
            auto const samples{ static_cast<double>(last_data_.samples) };

            std::string site_count{ prefix };
            std::string population{ prefix };
            std::string weighted_count{ prefix };
            std::string weighted_population{ prefix };

            for (auto element : lattice::active_elements)
                for (lattice::Coordination coordination{}; coordination < lattice::max_coordination;
                     ++coordination)
                {
                    Accumulation value{};
                    if (auto it{ last_data_.sites.find(SiteKey{ element, coordination }) };
                        it != last_data_.sites.end())
                        value = it->second;

                    std::format_to(std::back_inserter(site_count), ",{}", value.count / samples);
                    std::format_to(std::back_inserter(population), ",{}",
                        value.population_fraction / samples);
                    std::format_to(std::back_inserter(weighted_count), ",{}",
                        value.count_time / last_data_.residence_time);
                    std::format_to(std::back_inserter(weighted_population), ",{}",
                        value.population_time / last_data_.residence_time);
                }

            utility::file::append<utility::file::Type::csv>(
                writer_path() / "site_count", site_count + '\n');
            utility::file::append<utility::file::Type::csv>(
                writer_path() / "population", population + '\n');
            utility::file::append<utility::file::Type::csv>(
                writer_path() / "time_weighted_site_count", weighted_count + '\n');
            utility::file::append<utility::file::Type::csv>(
                writer_path() / "time_weighted_population", weighted_population + '\n');
        }

        kmc::Simulation const* simulation_{};
        ModelType* model_{};
        kmc::Time last_time_{};
        Data last_data_{}, accumulation_{};
    };
}
