#include "kmc/simulation.h"
#include "kmc/analysis_plan.h"
#include "output/basics.h"
#include "lattice/lattice.h"

#include <boost/ut.hpp>
#include <array>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using namespace kmc;
    using lattice::Site;

    // Test-only transformations: execution leaves the lattice unchanged so that
    // every selected slot can be checked against the original homogeneous layout.
    struct SiteProbe
    {
        static constexpr event::Type event{ event::Type::dissolution };
        static constexpr std::size_t slot_count{ 2 };

        std::optional<event::Location> location(Site& source, SlotId slot, lattice::Lattice&) const
        {
            if (slot != 0) return std::nullopt;
            return event::Location{ source };
        }

        bool is_source_candidate(Site const&) const { return true; }
        bool is_candidate(event::Location const&) const { return true; }
        bool is_rejected(event::Location const&) const { return false; }
        void execute(event::Location const&, lattice::Lattice&) const {}
        std::string settings() const { return "site probe"; }
    };

    struct NeighborProbe
    {
        static constexpr event::Type event{ event::Type::diffusion };
        static constexpr std::size_t slot_count{ 3 };

        std::optional<event::Location> location(
            Site& source, SlotId slot, lattice::Lattice& lattice) const
        {
            if (slot >= 2 or slot >= source.max_neighbors()) return std::nullopt;
            return event::Location{ lattice::Cluster<2>{
                &source,
                lattice.neighbor(source.neighbor_index() + static_cast<lattice::Index>(slot))
            } };
        }

        bool is_source_candidate(Site const&) const { return true; }
        bool is_candidate(event::Location const&) const { return true; }
        bool is_rejected(event::Location const&) const { return false; }
        void execute(event::Location const&, lattice::Lattice&) const {}
        std::string settings() const { return "neighbor probe"; }
    };

    struct SparseProbe
    {
        static constexpr event::Type event{ event::Type::deposition };
        static constexpr std::size_t slot_count{ 5 };

        std::optional<event::Location> location(Site& source, SlotId slot, lattice::Lattice&) const
        {
            // Slots 0, 1 and 2 never exist. Sites of different parity have
            // different valid local slots; the overall slot layout stays fixed.
            if (slot != (source.index() % 2 == 0 ? 3u : 4u)) return std::nullopt;
            return event::Location{ source };
        }

        bool is_source_candidate(Site const&) const { return true; }
        bool is_candidate(event::Location const&) const { return true; }
        bool is_rejected(event::Location const&) const { return false; }
        void execute(event::Location const&, lattice::Lattice&) const {}
        std::string settings() const { return "sparse probe"; }
    };

    static_assert(event::Transformation<SiteProbe>);
    static_assert(event::Transformation<NeighborProbe>);
    static_assert(event::Transformation<SparseProbe>);

    using Probes = event::Transformations<SiteProbe, NeighborProbe, SparseProbe>;
    static_assert(Probes::slot_count == 10);

    struct ProbeKinetics : Kinetics
    {
        RateClassKey classify(event::Type type, event::Location const&, lattice::Lattice const&)
        {
            return static_cast<RateClassKey>(std::to_underlying(type));
        }

        std::size_t number_keys() const noexcept { return 4; }

        event::Type class_event(RateClassKey key) const
        {
            return static_cast<event::Type>(key);
        }

        Rate rate(RateClassKey key) const { return static_cast<double>(key) * 1_Hz; }
        void refresh_rates() {}
        std::string output_text() const { return "slot-layout probe"; }
    };

    using ProbeModel = Model<ProbeKinetics, Probes>;
    using ProbeSimulation = RejectionTreeSimulation<ProbeModel>;

    Setup<ProbeModel> make_probe_setup()
    {
        Setup<ProbeModel> setup;
        setup.seed = simulation::Seed{ 1, 2, 3, 4, 5, 6, 7, 8 };
        setup.make_lattice = [] {
            auto cell{ lattice::UnitCell::conventional<lattice::Bravais::cF>(400_pm)
                .fill(chemistry::Atom{ Element::Au }) };
            lattice::Lattice lattice{ cell };
            lattice.scale_shape(lattice::Parameters::all(1600_pm));
            using enum math::Axis;
            lattice.set_periodic_boundaries(x | y | z);
            lattice.construct_sites();
            lattice.construct_neighbor_list();
            return lattice;
        };
        return setup;
    }

    struct SelectedSlots final : Observer
    {
        std::array<std::size_t, 4> selected{};
        std::array<std::size_t, 2> neighbor_slots{};
        std::array<std::size_t, 2> sparse_slots{};

        std::string_view name() const override { return "selected-slots"; }

        void observe_started(Simulation const& simulation) override
        {
            lattice_ = &simulation.lattice();
        }

        void observe_selected(event::Record const& record) override
        {
            const auto key{ static_cast<std::size_t>(record.class_key) };
            ut::expect(key < selected.size());
            if (key >= selected.size()) return;

            ut::expect(record.type == static_cast<event::Type>(key));
            ut::expect(record.outcome == event::Outcome::accepted);
            ut::expect(record.sites().front() == &record.location().source());
            ++selected[key];

            if (record.type == SiteProbe::event)
                ut::expect(ut::eq(record.sites().size(), std::size_t{ 1 }));
            else if (record.type == NeighborProbe::event)
            {
                ut::expect(ut::eq(record.sites().size(), std::size_t{ 2 }));
                if (record.sites().size() != 2) return;
                auto neighbors{ lattice_->neighbors(record.location().source()) };
                auto it{ neighbors.begin() };
                auto const* first{ *it++ };
                auto const* second{ *it };

                if (record.sites()[1] == first)
                    ++neighbor_slots[0];
                else if (record.sites()[1] == second)
                    ++neighbor_slots[1];
                else
                    ut::expect(false) << "selected neighbor is not a decoded slot";
            }
            else if (record.type == SparseProbe::event)
            {
                ut::expect(ut::eq(record.sites().size(), std::size_t{ 1 }));
                ++sparse_slots[record.location().source().index() % 2];
            }
        }

    private:
        lattice::Lattice const* lattice_{};
    };

    ut::suite<"kmc.transformation_slots"> transformation_slot_tests = [] {
        "three transformations have contiguous, correctly decoded local slot ranges"_test = [] {
            Probes transformations;
            std::vector<std::size_t> offsets;
            std::vector<event::Type> types;

            transformations.for_each([&](auto const& transformation, std::size_t offset) {
                offsets.push_back(offset);
                types.push_back(std::remove_cvref_t<decltype(transformation)>::event);
            });

            ut::expect(offsets == std::vector<std::size_t>{ 0, 2, 5 });
            ut::expect(types == std::vector<event::Type>{
                SiteProbe::event, NeighborProbe::event, SparseProbe::event });

            for (SlotId slot{}; slot < Probes::slot_count; ++slot)
            {
                const auto expected_type{ slot < 2 ? SiteProbe::event
                    : slot < 5 ? NeighborProbe::event : SparseProbe::event };
                const auto expected_local{ slot < 2 ? slot : slot < 5 ? slot - 2 : slot - 5 };

                std::size_t visited{};
                transformations.visit(slot, [&](auto const& transformation, SlotId local_slot) {
                    ++visited;
                    ut::expect(std::remove_cvref_t<decltype(transformation)>::event == expected_type);
                    ut::expect(ut::eq(local_slot, expected_local));
                });
                ut::expect(ut::eq(visited, std::size_t{ 1 }));
            }

            ut::expect(ut::throws<std::out_of_range>([&] {
                transformations.visit(Probes::slot_count, [](auto const&, SlotId) {});
            }));
        };

        "homogeneous class-key slots preserve permanently absent geometries"_test = [] {
            ProbeSimulation simulation{ make_probe_setup() };
            auto const& lattice{ simulation.lattice() };
            auto const& keys{ simulation.event_class_keys() };
            auto const site_count{ lattice.sites().size() };

            ut::expect(ut::eq(keys.size(), site_count * Probes::slot_count));
            for (auto const& site : lattice.sites())
            {
                ut::expect(site.max_neighbors() >= 2);
                const std::array<RateClassKey, Probes::slot_count> expected{
                    2, 0, 1, 1, 0, 0, 0, 0,
                    site.index() % 2 == 0 ? 3u : 0u,
                    site.index() % 2 == 1 ? 3u : 0u
                };

                for (SlotId slot{}; slot < expected.size(); ++slot)
                    ut::expect(ut::eq(
                        keys.at(static_cast<std::size_t>(site.index()) * expected.size() + slot),
                        expected[slot])) << "site " << site.index() << ", slot " << slot;
            }

            auto const& counts{ simulation.class_counts() };
            ut::expect(ut::eq(counts.at(0), std::size_t{}));
            ut::expect(ut::eq(counts.at(1), 2 * site_count));
            ut::expect(ut::eq(counts.at(2), site_count));
            ut::expect(ut::eq(counts.at(3), site_count));

            // Type::diffusion = 1 Hz, dissolution = 2 Hz, deposition = 3 Hz.
            // Two diffusion slots plus one of each other type: 7 Hz per site.
            ut::expect(ut::eq(simulation.rate_tree()->rate_sum().value(),
                7.0 * static_cast<double>(site_count)));

            constexpr auto block_size{ std::size_t{ 1 } << rate_tree::baseLevel };
            for (std::size_t index{}; index < site_count; index += block_size)
                ut::expect(ut::eq(
                    simulation.rate_tree()->block_rate(static_cast<lattice::Index>(index)).value(),
                    7.0 * static_cast<double>(std::min(block_size, site_count - index))));
        };

        "selection never chooses absent slots and decodes all active transformations"_test = [] {
            ProbeSimulation simulation{ make_probe_setup() };
            auto const initial_keys{ simulation.event_class_keys() };
            auto const initial_counts{ simulation.class_counts() };
            AnalysisPlan plan;
            auto& selected{ plan.add<SelectedSlots>() };
            plan.add<control::StepLimit>(320);

            ut::expect(simulation.run(plan) == simulation::Status::analysed);
            ut::expect(ut::eq(simulation.step(), Step{ 320 }));
            ut::expect(simulation.event_class_keys() == initial_keys);
            ut::expect(simulation.class_counts() == initial_counts);
            ut::expect(ut::eq(simulation.rejection_fraction(), 0.0));

            for (std::size_t key{ 1 }; key < selected.selected.size(); ++key)
                ut::expect(selected.selected[key] > 0) << "event class selected: " << key;
            ut::expect(selected.neighbor_slots[0] > 0);
            ut::expect(selected.neighbor_slots[1] > 0);
            ut::expect(selected.sparse_slots[0] > 0);
            ut::expect(selected.sparse_slots[1] > 0);
        };
    };
}
