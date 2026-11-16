#pragma once
#include "writer.h"
#include "kmc/simulation.h"
#include "lattice/lattice.h"
#include "utility/file.h"

#include <algorithm>
#include <map>
#include <optional>
#include <vector>

namespace output
{
    // Physical populations of occupied surface sites, independent of the
    // kinetic equivalence classes assigned by the model.
    class SitePopulation : public Writer
    {
    public:
        using Writer::Writer;
        using SiteKey = std::pair<lattice::Element, lattice::Coordination>;
        using Counts = std::map<SiteKey, std::size_t>;

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
        auto const& counts() const noexcept { return counts_; }

    private:
        std::string_view name() const override { return "site_population"; }

        static std::optional<SiteKey> classify_site(lattice::Site const& site)
        {
            if (not site.is_occupied() or not site.is_surface())
                return std::nullopt;
            return SiteKey{ site.element(), site.coordination() };
        }

        void capture_init(kmc::Simulation const& simulation) override
        {
            simulation_ = &simulation;
            last_time_ = simulation.time();
            last_data_ = {};
            accumulation_ = {};
            affected_.clear();
            counts_.clear();

            for (auto const& site : simulation.lattice().sites())
                if (auto const key{ classify_site(site) })
                    ++counts_[*key];
        }

        void observe_selected(kmc::event::Record const& record) override
        {
            auto const delta{ simulation_->time() - last_time_ };
            last_time_ = simulation_->time();

            std::size_t total{};
            for (auto const& [_, count] : counts_)
                total += count;

            for (auto const& [site, count] : counts_)
            {
                if (count == 0)
                    continue;

                auto const population{ static_cast<double>(count) / total };
                auto& value{ accumulation_.sites[site] };
                value.count += count;
                value.population_fraction += population;
                value.count_time += static_cast<double>(count) * delta;
                value.population_time += population * delta;
            }
            accumulation_.residence_time += delta;
            ++accumulation_.samples;

            affected_.clear();
            if (record.outcome != kmc::event::Outcome::accepted)
                return;

            auto remember = [this](lattice::Site const& site) {
                if (std::ranges::none_of(affected_, [&](auto const& entry) {
                        return entry.first == &site;
                    }))
                    affected_.emplace_back(&site, classify_site(site));
            };

            // Execution can change the occupancy of event sites and the
            // coordination of their direct neighbors, but not distant sites.
            for (auto* site : record.sites())
            {
                remember(*site);
                for (auto* neighbor : simulation_->lattice().neighbors(*site))
                    remember(*neighbor);
            }
        }

        void observe_processed(kmc::event::Record const&) override
        {
            for (auto const& [site, before] : affected_)
            {
                auto const after{ classify_site(*site) };
                if (before == after)
                    continue;

                if (before)
                {
                    auto& count{ counts_.at(*before) };
                    if (count == 0)
                        throw std::logic_error{ "Site population counter underflow." };
                    --count;
                }
                if (after)
                    ++counts_[*after];
            }
            affected_.clear();
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
        kmc::Time last_time_{};
        Counts counts_;
        std::vector<std::pair<lattice::Site const*, std::optional<SiteKey>>> affected_;
        Data last_data_{}, accumulation_{};
    };
}
