#include "kmc/simulation.h"
#include "kmc/process.h"
#include "dealloying/kinetics/linear_bond_breaking.h"
#include "dealloying/kinetics/composition_dependent.h"
#include "output/basics.h"
#include "physics/equations/kinetics.h"
#include "kmc/analysis_plan.h"
#include "lattice/lattice.h"
#include "output/kinetic_activity.h"
#include "output/kinetic_statistics.h"
#include "output/site_population.h"
#include "dealloying/kinetics/descriptor_format.h"
#include "output/event_flux.h"

#include <algorithm>
#include <array>
#include <bit>
#include <boost/ut.hpp>
#include <cmath>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using namespace kmc;
    using chemistry::Element;
    using lattice::Site;

    struct SiteRule
    {
        static constexpr event::Type event{ event::Type::dissolution };
        static constexpr std::size_t slot_count{ 1 };
        static constexpr std::size_t cluster_size{ 1 };
        bool enabled{ true };
        bool allow_gold{ false };
        std::vector<std::size_t>* source_checks{};

        event::Location location(
            Site& source, SlotId, lattice::Lattice&) const
        {
            return event::Location{ &source };
        }

        bool is_candidate(Site const& site) const
        {
            if (source_checks and site.index() < source_checks->size())
                ++(*source_checks)[site.index()];

            return enabled and (site.element() == Element::Ag or
                   (allow_gold and site.element() == Element::Au)) and
                   site.coordination() < lattice::max_coordination;
        }

        bool is_candidate(event::Location const& location) const
        {
            return location.sites().size() == 1 and is_candidate(location.source());
        }

        bool is_rejected(event::Location const& location) const
        {
            return location.source().coordination() > 8;
        }

        void execute(event::Location const& location, lattice::Lattice& lattice) const
        {
            location.source().set_element(lattice, Element::none);
        }


        std::string settings() const { return "test site rule"; }
    };

    struct ClusterRule
    {
        static constexpr event::Type event{ event::Type::diffusion };
        static constexpr std::size_t slot_count{ lattice::max_coordination };
        static constexpr std::size_t cluster_size{ 2 };
        bool enabled{ true };

        bool has_slot(Site const& source, SlotId slot) const
        {
            return slot < source.max_neighbors();
        }

        event::Location location(
            Site& source, SlotId slot, lattice::Lattice& lattice) const
        {
            return event::Location{ &source,
                lattice.neighbor(source.neighbor_index() + static_cast<lattice::Index>(slot)) };
        }

        bool is_candidate(Site const& site) const
        {
            return enabled and site.is_occupied() and
                   site.coordination() < lattice::max_coordination;
        }

        bool is_candidate(event::Location const& location) const
        {
            return location.sites().size() == 2 and is_candidate(location.source()) and
                   location.sites()[1]->is_vacant();
        }

        bool is_rejected(event::Location const& location) const
        {
            return location.sites()[1]->coordination() < 9;
        }

        void execute(event::Location const& location, lattice::Lattice& lattice) const
        {
            const auto element{ location.source().element() };
            location.sites()[1]->set_element(lattice, element);
            location.source().set_element(lattice, Element::none);
        }


        std::string settings() const { return "test cluster rule"; }
    };

    static_assert(kmc::Process<SiteRule>);
    static_assert(kmc::Process<ClusterRule>);

    using Processes = kmc::Processes<SiteRule, ClusterRule>;

    using Descriptor = studies::dealloying::kinetics::LinearBondBreaking::Descriptor;
    using ModelBase = studies::dealloying::kinetics::LinearBondBreaking;

    class TestKinetics final : public ModelBase
    {
    public:
        explicit TestKinetics(std::size_t radius = 1) : radius_{ radius } { }

        std::size_t invalidation_radius() const noexcept { return radius_; }

        Rate rate(RateClassKey key) const
        {
            auto const& kinetic_class{ descriptor(key) };
            const auto base{ kinetic_class.event == event::Type::dissolution ? 0.24_eV : 0.30_eV };
            const auto barrier{ base + 0.005_eV * static_cast<double>(kinetic_class.coordination) };
            return physics::equations::kinetics::tst_rate(Ratio{ 1 }, barrier, temperature());
        }

    private:
        std::size_t radius_;
    };

    using TestModel = kmc::Model<TestKinetics, Processes>;
    using TestSimulation = RejectionTreeSimulation<TestModel>;

    Setup<TestModel> make_setup(std::size_t radius = 1)
    {
        Setup<TestModel> setup;
        setup.name = "KMC incremental update integration test";
        setup.seed = simulation::Seed{ 1, 2, 3, 4, 5, 6, 7, 8 };
        setup.model = TestModel{ TestKinetics{ radius } };
        setup.make_lattice = [] {
            auto cell{ lattice::UnitCell::conventional<lattice::Bravais::cF>(400_pm)
                .fill(chemistry::Atom{ Element::Au }) };
            lattice::Lattice lattice{ cell };
            lattice.scale_shape(lattice::Parameters::all(1600_pm));
            using enum math::Axis;
            lattice.set_periodic_boundaries(x | y | z);
            lattice.construct_sites();
            lattice.construct_neighbor_list();
            lattice.set_fill_function([](lattice::Lattice& lattice, simulation::Engine&) {
                for (auto& site : lattice.sites())
                {
                    if (site.index() % 9 == 0)
                        site.set_element(lattice, Element::none);
                    else if (site.index() % 3 == 1)
                        site.set_element(lattice, Element::Ag);
                }
            });
            return lattice;
        };
        return setup;
    }

    void expect_reference_state(TestSimulation const& simulation, Processes const& processes, Step step)
    {
        auto const& lattice{ simulation.lattice() };
        auto const& classes{ simulation.kinetic_classes() };
        constexpr auto block_size{ std::size_t{ 1 } << rate_tree::baseLevel };
        std::vector<Rate> block_sums((lattice.sites().size() + block_size - 1) / block_size);
        std::vector<std::size_t> expected_counts(classes.size());

        auto add = [&](Rate& site_sum, Descriptor const& descriptor) {
            auto const key{ ModelBase::static_key(descriptor) };
            ut::expect(ut::that % key > 0) << "active class has nonzero key, step " << step;
            ut::expect(ut::that % key < classes.size()) << "class key within table, step " << step;
            if (key == 0 or key >= classes.size())
                return;
            auto const* model{ &simulation.model().kinetics() };
            ut::expect(model != nullptr and model->descriptor(key) == descriptor)
                << "model descriptor matches rebuilt class, step " << step;
            ut::expect(classes[key].is_initialized())
                << "kinetic class initialized, step " << step;
            ut::expect(ut::eq(classes[key].rate.value(), simulation.model().kinetics().rate(key).value()))
                << "cached model rate, step " << step;
            ++expected_counts[key];
            site_sum += simulation.model().kinetics().rate(key);
        };

        for (auto const& site : lattice.sites())
        {
            Rate site_sum{};
            auto has_events{ false };
            const event::Location source{ const_cast<Site*>(&site) };
            if (processes.get<SiteRule>().is_candidate(site) and kmc::is_location_candidate(processes.get<SiteRule>(), source))
            {
                add(site_sum, Descriptor{ SiteRule::event, source.source().element(), source.source().coordination() });
                has_events = true;
            }
            if (processes.get<ClusterRule>().is_candidate(site))
                for (auto const* target : lattice.neighbors(site))
                {
                    const event::Location location{ const_cast<Site*>(&site), const_cast<Site*>(target) };
                    if (kmc::is_location_candidate(processes.get<ClusterRule>(), location))
                    {
                        add(site_sum, Descriptor{ ClusterRule::event, location.source().element(), location.source().coordination() });
                        has_events = true;
                    }
                }

            block_sums[site.index() / block_size] += site_sum;
            ut::expect(ut::eq(site.is_active(), has_events))
                << "site active flag, step " << step << ", site " << site.index();
            ut::expect(not site.is_dirty())
                << "site clean after rate update, step " << step << ", site " << site.index();
            auto const actual_occupied{ std::ranges::count_if(lattice.neighbors(site),
                [](auto const* other) { return other->is_occupied(); }) };
            ut::expect(ut::eq(static_cast<std::size_t>(site.coordination()),
                static_cast<std::size_t>(actual_occupied)))
                << "coordination after event, step " << step;
        }

        ut::expect(ut::eq(classes.size(), expected_classes.size()));
        for (std::size_t key{}; key < classes.size(); ++key)
            ut::expect(ut::eq(classes[key].count, expected_counts[key]))
                << "kinetic class count key " << key << ", step " << step;

        auto const& tree{ *simulation.rate_tree() };
        for (std::size_t i{}; i < block_sums.size(); ++i)
            ut::expect(ut::eq(tree.block_rate(static_cast<lattice::Index>(i * block_size)).value(),
                block_sums[i].value()))
                << "block rate, block " << i << ", step " << step;

        // Independent full bottom-up reduction using the production tree's
        // summation order, rather than its recursive update implementation.
        std::vector<Rate> level(std::bit_ceil(block_sums.size()));
        std::ranges::copy(block_sums, level.begin());
        for (auto count{ level.size() }; count > 1; count /= 2)
            for (std::size_t i{}; i < count / 2; ++i)
                level[i] = level[2 * i] + level[2 * i + 1];
        ut::expect(ut::eq(tree.rate_sum().value(), level.front().value()))
            << "root rate matches full reconstruction at step " << step;
    }

    class CheckingObserver final : public Observer
    {
    public:
        explicit CheckingObserver(Step target, Processes processes = {}, Step initial_step = 0)
            : Observer{ Resolution{ 1, 1.0 } }, target_{ target },
              last_step_{ initial_step }, processes_{ processes }
        { }

        std::string_view name() const override { return "kmc-integration-test"; }
        bool stop_requested() const override { return last_step_ >= target_; }

        void observe_started(Simulation const& simulation) override
        {
            expect_reference_state(dynamic_cast<TestSimulation const&>(simulation), processes_, simulation.step());
        }

        void observe_selected(event::Record const& record) override
        {
            ++selections_;
            bool rejected{};
            auto const& source{ record.location().source() };
            if (record.sites().size() == 1)
            {
                ++site_selections_;
                ut::expect(processes_.get<SiteRule>().is_candidate(source))
                    << "selected site has valid source";
                ut::expect(kmc::is_location_candidate(processes_.get<SiteRule>(), record.location()))
                    << "selected site is a candidate";
                rejected = kmc::is_rejected(processes_.get<SiteRule>(), record.location());
                ut::expect(record.type == SiteRule::event);
            }
            else
            {
                ++cluster_selections_;
                ut::expect(processes_.get<ClusterRule>().is_candidate(source))
                    << "selected cluster has valid source";
                ut::expect(kmc::is_location_candidate(processes_.get<ClusterRule>(), record.location()))
                    << "selected cluster is a candidate";
                rejected = kmc::is_rejected(processes_.get<ClusterRule>(), record.location());
                ut::expect(record.type == ClusterRule::event);
            }
            auto const expected_key{ ModelBase::static_key(Descriptor{
                record.type, source.element(), source.coordination() }) };
            ut::expect(ut::eq(record.class_key, expected_key))
                << "selected event carries the registered kinetic class";
            ut::expect(ut::eq(record.outcome == event::Outcome::rejected, rejected))
                << "postselection rejection matches rule";
            if (record.outcome == event::Outcome::rejected)
                ++rejections_;
        }

        void observe_processed(event::Record const& record) override
        {
            ut::expect(record.outcome == event::Outcome::accepted);
            ++processed_;
            if (record.sites().size() == 1)
                ++accepted_site_events_;
            else
                ++accepted_cluster_events_;
        }

        void take_snapshot(Simulation const& simulation) override
        {
            if (simulation.step() == last_step_)
                return;
            ut::expect(ut::eq(simulation.step(), last_step_ + 1))
                << "one accepted event per validated snapshot";
            last_step_ = simulation.step();
            expect_reference_state(dynamic_cast<TestSimulation const&>(simulation),
                processes_, last_step_);
        }

        Step last_step() const { return last_step_; }
        std::size_t selected_count() const { return selections_; }
        std::size_t rejected_count() const { return rejections_; }
        std::size_t processed_count() const { return processed_; }
        std::size_t site_selections() const { return site_selections_; }
        std::size_t accepted_site_events() const { return accepted_site_events_; }
        std::size_t accepted_cluster_events() const { return accepted_cluster_events_; }
        std::size_t cluster_selections() const { return cluster_selections_; }

    private:
        Step target_{};
        Step last_step_{};
        Processes processes_;
        std::size_t selections_{};
        std::size_t rejections_{};
        std::size_t processed_{};
        std::size_t site_selections_{};
        std::size_t cluster_selections_{};
        std::size_t accepted_site_events_{};
        std::size_t accepted_cluster_events_{};
    };

    class FirstAcceptedObserver final : public Observer
    {
    public:
        std::string_view name() const override { return "first-accepted-event"; }
        bool stop_requested() const override { return accepted_; }

        void observe_processed(event::Record const& record) override
        {
            accepted_ = true;
            source_index_ = record.sites()[0]->index();
        }

        lattice::Index source_index() const { return source_index_; }
        bool accepted() const { return accepted_; }

    private:
        bool accepted_{};
        lattice::Index source_index_{};
    };

    void expect_same_state(TestSimulation const& a, TestSimulation const& b)
    {
        ut::expect(ut::eq(a.step(), b.step())) << "step after resume";
        ut::expect(ut::eq(a.time().value(), b.time().value())) << "time after resume";
        ut::expect(a.rng().state() == b.rng().state()) << "RNG state after resume";
        ut::expect(ut::eq(a.rejection_fraction(), b.rejection_fraction()))
            << "rejection fraction after resume";
        ut::expect(ut::eq(a.rate_tree()->rate_sum().value(), b.rate_tree()->rate_sum().value()))
            << "total rate after resume";
        ut::expect(a.kinetic_classes() == b.kinetic_classes()) << "kinetic class counts after resume";
        for (std::size_t i{}; i < a.lattice().sites().size(); ++i)
        {
            ut::expect(a.lattice().sites()[i].element() ==
                b.lattice().sites()[i].element()) << "occupancy after resume, site " << i;
            ut::expect(ut::eq(a.lattice().sites()[i].coordination(),
                b.lattice().sites()[i].coordination())) << "coordination after resume, site " << i;
        }
    }

    ut::suite<"kmc.simulation"> simulation_tests = [] {
        "kinetic-class initialization is independent of rate and population"_test = [] {
            KineticClass uninitialized{};
            ut::expect(not uninitialized.is_initialized());

            KineticClass initialized{ .rate = Rate{}, .count = 0,
                                      .type = event::Type::diffusion };
            ut::expect(initialized.is_initialized());
            ut::expect(initialized.rate == Rate{});
            ut::expect(ut::eq(initialized.count, std::size_t{}));
        };

        "event records own their site-pointer storage"_test = [] {
            auto setup{ make_setup() };
            TestSimulation simulation{ setup };
            auto& lattice{ simulation.mutable_lattice() };
            auto& first{ lattice.sites().at(0) };
            auto& second{ lattice.sites().at(1) };

            event::Record site_record{ event::Location{ &first }, SiteRule::event, false, 65537 };
            ut::expect(ut::eq(site_record.sites().size(), std::size_t{ 1 }));
            ut::expect(site_record.sites()[0] == &first);
            ut::expect(site_record.type == SiteRule::event);
            ut::expect(ut::eq(site_record.class_key, RateClassKey{ 65537 }));

            auto site_copy{ site_record };
            ut::expect(site_copy.sites().data() != site_record.sites().data())
                << "a record copy owns its own pointer storage";
            ut::expect(ut::eq(site_copy.sites().size(), std::size_t{ 1 }));
            ut::expect(site_copy.sites()[0] == &first);
            ut::expect(ut::eq(site_copy.class_key, site_record.class_key));

            std::array<Site*, 2> const cluster{ &first, &second };
            event::Record cluster_record{ event::Location{ cluster[0], cluster[1] }, ClusterRule::event, false, 65538 };
            ut::expect(ut::eq(cluster_record.sites().size(), std::size_t{ 2 }));
            ut::expect(cluster_record.sites()[0] == &first);
            ut::expect(cluster_record.sites()[1] == &second);
            ut::expect(cluster_record.type == ClusterRule::event);
            ut::expect(ut::eq(cluster_record.class_key, RateClassKey{ 65538 }));
            ut::expect(std::ranges::equal(cluster_record.sites(), cluster));

            auto cluster_copy{ cluster_record };
            ut::expect(cluster_copy.sites().data() != cluster_record.sites().data())
                << "a cluster record copy owns its own pointer storage";
            ut::expect(std::ranges::equal(cluster_copy.sites(), cluster));
            ut::expect(ut::eq(cluster_copy.class_key, cluster_record.class_key));

            auto cluster_move{ std::move(cluster_copy) };
            ut::expect(ut::eq(cluster_move.sites().size(), std::size_t{ 2 }));
            ut::expect(cluster_move.sites().data() != cluster_record.sites().data());
            ut::expect(cluster_move.sites()[0] == &first);
            ut::expect(cluster_move.sites()[1] == &second);
            ut::expect(ut::eq(cluster_move.class_key, cluster_record.class_key));
        };

        "site and cluster processes execute lattice changes"_test = [] {
            auto setup{ make_setup() };
            TestSimulation simulation{ setup };
            auto& lattice{ simulation.mutable_lattice() };
            SiteRule site_rule;
            ClusterRule cluster_rule;

            auto source_it{ std::ranges::find_if(lattice.sites(), [&](Site const& site) {
                return site_rule.is_candidate(site);
            }) };
            ut::expect(source_it != lattice.sites().end()) << "site candidate exists";
            if (source_it == lattice.sites().end())
                return;

            auto const atoms_before{ lattice.number_atoms() };
            site_rule.execute(event::Location{ &*source_it }, lattice);
            ut::expect(source_it->is_vacant()) << "site rule removes selected atom";
            ut::expect(ut::eq(lattice.number_atoms(), atoms_before - 1));

            Site* source{};
            Site* destination{};
            for (auto& site : lattice.sites())
            {
                for (auto* neighbor : lattice.neighbors(site))
                {
                    std::array<Site*, 2> candidate{ &site, neighbor };
                    if (kmc::is_location_candidate(cluster_rule, event::Location{ candidate[0], candidate[1] }))
                    {
                        source = &site;
                        destination = neighbor;
                        break;
                    }
                }
                if (source)
                    break;
            }

            ut::expect(source != nullptr) << "cluster candidate exists";
            if (not source)
                return;

            auto const element{ source->element() };
            auto const atoms_before_move{ lattice.number_atoms() };
            std::array<Site*, 2> cluster{ source, destination };
            cluster_rule.execute(event::Location{ cluster[0], cluster[1] }, lattice);
            ut::expect(source->is_vacant()) << "source becomes vacant";
            ut::expect(destination->element() == element) << "destination receives atom";
            ut::expect(ut::eq(lattice.number_atoms(), atoms_before_move))
                << "cluster execution conserves atom count";
        };

        "radius-two invalidation recalculates exactly two neighbor shells"_test = [] {
            std::vector<std::size_t> source_checks;
            auto setup{ make_setup(2) };
            setup.model.processes().get<ClusterRule>().enabled = false;
            setup.model.processes().get<SiteRule>().source_checks = &source_checks;

            TestSimulation simulation{ setup };
            auto const& lattice{ simulation.lattice() };
            source_checks.resize(lattice.sites().size());
            ut::expect(not simulation.model().processes().get<ClusterRule>().enabled)
                << "disabled diffusion remains disabled in copied model";
            ut::expect(ut::eq(simulation.model().kinetics().invalidation_radius(), std::size_t{ 2 }))
                << "radius from statically typed kinetics";

            AnalysisPlan plan;
            auto& observer{ plan.add<FirstAcceptedObserver>() };
            auto const status{ simulation.run(plan) };

            ut::expect(status == simulation::Status::analysed);
            ut::expect(observer.accepted());
            if (not observer.accepted())
                return;
            ut::expect(ut::eq(simulation.step(), Step{ 1 }));
            std::vector<bool> expected(lattice.sites().size());
            auto& source{ lattice.sites().at(observer.source_index()) };
            expected[source.index()] = true;
            for (auto* first : lattice.neighbors(source))
            {
                expected[first->index()] = true;
                for (auto* second : lattice.neighbors(*first))
                    expected[second->index()] = true;
            }

            auto beyond_first_shell{ false };
            for (std::size_t index{}; index < expected.size(); ++index)
            {
                const bool visited{ source_checks[index] != 0 };
                ut::expect(ut::eq(visited, expected[index]))
                    << "radius-two visit at source " << index;
                ut::expect(not lattice.sites()[index].is_dirty())
                    << "dirty flag cleared after second pass at source " << index;

                if (expected[index] and index != source.index())
                {
                    const auto is_first_neighbor{ std::ranges::any_of(
                        lattice.neighbors(source),
                        [index](auto* site) { return site->index() == index; }) };
                    beyond_first_shell |= not is_first_neighbor;
                }
            }
            ut::expect(beyond_first_shell) << "second shell contains additional sites";
            expect_reference_state(simulation, setup.model.processes(), simulation.step());
        };

        "radius-two incremental updates match full reconstruction"_test = [] {
            auto setup{ make_setup(2) };
            TestSimulation simulation{ setup };
            AnalysisPlan plan;
            auto& checker{ plan.add<CheckingObserver>(40, setup.model.processes()) };
            const auto status{ simulation.run(plan) };
            ut::expect(status == simulation::Status::analysed);
            ut::expect(ut::eq(simulation.step(), Step{ 40 }));
            ut::expect(ut::eq(checker.last_step(), simulation.step()));
        };

        "static kinetic-class cache is lazy and only contains encountered classes"_test = [] {
            auto setup{ make_setup() };
            setup.model.processes().get<SiteRule>().enabled = false;
            setup.model.processes().get<ClusterRule>().enabled = false;
            TestSimulation simulation{ setup };

            auto const& classes{ simulation.kinetic_classes() };
            ut::expect(ut::eq(classes.size(), simulation.model().kinetics().number_keys()));
            for (auto const& kinetic_class : classes)
            {
                ut::expect(not kinetic_class.is_initialized())
                    << "unused class stays uninitialized";
                ut::expect(kinetic_class.rate == Rate{}) << "unused class has zero rate";
                ut::expect(ut::eq(kinetic_class.count, std::size_t{}))
                    << "unused class has zero count";
            }
        };

        "static kinetic-class cache initializes encountered events only once"_test = [] {
            auto setup{ make_setup() };
            TestSimulation simulation{ setup };
            auto const& classes{ simulation.kinetic_classes() };

            std::size_t initialized{};
            auto const* model{ &simulation.model().kinetics() };
            ut::expect(model != nullptr);
            for (std::size_t i{}; i < classes.size(); ++i)
                if (classes[i].is_initialized())
                {
                    ++initialized;
                    ut::expect(ut::eq(static_cast<std::size_t>(
                        ModelBase::static_key(model->descriptor(static_cast<RateClassKey>(i)))), i))
                        << "cached class owns its key";
                    ut::expect(classes[i].rate == simulation.model().kinetics().rate(static_cast<RateClassKey>(i)))
                        << "cached rate matches model";
                }
                else
                    ut::expect(classes[i].rate == Rate{})
                        << "uninitialized rate remains zero";
            ut::expect(ut::that % initialized > 0) << "at least one class created";

            expect_reference_state(simulation, setup.model.processes(), 0);
        };

        "static kinetic-class keys must distinguish different kinetic classes"_test = [] {
            Descriptor const silver{ event::Type::dissolution, Element::Ag, 5 };
            Descriptor const gold{ event::Type::dissolution, Element::Au, 5 };
            ut::expect(silver != gold);
            ut::expect(ut::eq(ModelBase::static_key(silver), ModelBase::static_key(gold)))
                << "current dissolution key intentionally omits element";
        };

        "distinct kinetic classes sharing a static key are rejected"_test = [] {
            auto setup{ make_setup() };
            setup.model.processes().get<SiteRule>().allow_gold = true;
            setup.model.processes().get<ClusterRule>().enabled = false;

            // Give both Au and Ag source sites coordination 11, forcing
            // their dissolution events to collide on the same static key.
            auto make_lattice{ setup.make_lattice };
            setup.make_lattice = [make_lattice] {
                auto lattice{ make_lattice() };
                lattice.set_fill_function([](lattice::Lattice& lattice, simulation::Engine&) {
                    auto& vacancy{ lattice.sites().at(0) };
                    auto* silver{ *lattice.neighbors(vacancy).begin() };
                    vacancy.set_element(lattice, Element::none);
                    silver->set_element(lattice, Element::Ag);
                });
                return lattice;
            };

            ut::expect(ut::throws<std::logic_error>([&] {
                TestSimulation simulation{ setup };
            })) << "dissolution Au and Ag must not reuse the same static key";
        };

        "kinetics and flux CSV columns stay stable as new kinetic classes appear"_test = [] {
            auto setup{ make_setup() };
            TestSimulation simulation{ setup };
            const auto root{ std::filesystem::temp_directory_path() /
                std::format("kinetalis-csv-test-{}",
                    std::chrono::steady_clock::now().time_since_epoch().count()) };

            struct Cleanup
            {
                std::filesystem::path path;
                ~Cleanup() { std::error_code error; std::filesystem::remove_all(path, error); }
            } cleanup{ root };

            AnalysisPlan plan;
            plan.add<output::KineticActivity<TestModel>>(root)
                .set_resolution({ .min_snapshot_step = 1, .snapshot_scale_factor = 1.0 });
            auto& statistics{ plan.add<output::KineticStatistics<TestModel>>(root)
                .set_output_transitions(true)
                .set_resolution({ .min_snapshot_step = 1, .snapshot_scale_factor = 1.0 }) };
            auto& site_population{ plan.add<output::SitePopulation>(root)
                .set_resolution({ .min_snapshot_step = 1, .snapshot_scale_factor = 1.0 }) };
            plan.add<output::EventFlux>(root)
                .set_resolution({ .min_snapshot_step = 1, .snapshot_scale_factor = 1.0 });
            plan.add<CheckingObserver>(32, setup.model.processes());

            ut::expect(simulation.run(plan) == simulation::Status::analysed);
            ut::expect(statistics.counts().contains(event::Outcome::accepted))
                << "descriptor-based statistics record accepted kinetic events";
            ut::expect(not statistics.transitions().empty())
                << "descriptor-based transitions are recorded";

            std::map<output::SitePopulation::SiteKey, std::size_t> expected_sites;
            for (auto const& site : simulation.lattice().sites())
                if (site.is_occupied() and site.is_surface())
                    ++expected_sites[{ site.element(), site.coordination() }];
            for (auto const& [site, count] : expected_sites)
                ut::expect(ut::eq(site_population.counts().at(site), count))
                    << "physical site population matches complete lattice scan";
            for (auto const& [site, count] : site_population.counts())
                ut::expect(ut::eq(count, expected_sites[site]))
                    << "incremental site population contains no spurious sites";

            auto read_csv = [&](std::string const& filename, std::size_t expected_columns) {
                auto path{ root / filename };
                path += ".csv";
                std::ifstream stream{ path };
                ut::expect(stream.is_open()) << filename << " exists";
                std::vector<std::string> lines;
                for (std::string line; std::getline(stream, line);)
                {
                    ut::expect(ut::eq(static_cast<std::size_t>(
                        std::ranges::count(line, ',')) + 1, expected_columns))
                        << filename << " has a fixed number of CSV columns";
                    lines.push_back(std::move(line));
                }
                ut::expect(ut::that % lines.size() > 2) << filename << " has snapshot rows";
                return lines;
            };

            constexpr auto site_columns{
                std::size(lattice::active_elements) * lattice::max_coordination + 2
            };
            for (auto const* file : {
                "site_population/site_count",
                "site_population/population",
                "site_population/time_weighted_site_count",
                "site_population/time_weighted_population"
            })
                read_csv(file, site_columns);

            auto const& classes{ simulation.kinetic_classes() };
            auto lines{ read_csv("kinetic_activity/activity", classes.size() + 1) };
            read_csv("kinetic_activity/time_weighted_activity", classes.size() + 1);

            if (not lines.empty())
                for (std::size_t key{ 1 }; key < classes.size(); ++key)
                {
                    auto const label{ classes[key].is_initialized()
                        ? output::DescriptorFormat<Descriptor>::label(
                            simulation.model().kinetics().descriptor(
                                static_cast<RateClassKey>(key)))
                        : std::format("not_encountered_{}", key) };
                    ut::expect(lines.front().find("," + label) != std::string::npos)
                        << "activity header includes final key label " << key;
                }

            // Three declared event types plus step and time, two fields per type.
            auto const flux_columns{ std::size_t{ 2 } +
                2 * (enum_values<event::Type>().size() - 1) };
            read_csv("event_flux", flux_columns);
        };

        "site populations are independent of composition-dependent kinetic classes"_test = [] {
            auto base_setup{ make_setup() };
            using CompositionModel = kmc::Model<
                studies::dealloying::kinetics::CompositionDependent, Processes>;
            Setup<CompositionModel> setup;
            setup.name = base_setup.name;
            setup.seed = base_setup.seed;
            setup.make_lattice = base_setup.make_lattice;
            RejectionTreeSimulation<CompositionModel> simulation{ setup };
            AnalysisPlan plan;
            auto& site_population{ plan.add<output::SitePopulation>() };
            plan.add<control::StepLimit>(25);

            ut::expect(simulation.run(plan) == simulation::Status::analysed);
            std::map<output::SitePopulation::SiteKey, std::size_t> expected_sites;
            for (auto const& site : simulation.lattice().sites())
                if (site.is_occupied() and site.is_surface())
                    ++expected_sites[{ site.element(), site.coordination() }];

            for (auto const& [site, count] : expected_sites)
                ut::expect(ut::eq(site_population.counts().at(site), count))
                    << "composition-dependent classes do not change physical populations";
            for (auto const& [site, count] : site_population.counts())
                ut::expect(ut::eq(count, expected_sites[site]))
                    << "all physical classes match direct enumeration";
        };

        "no active events exhausts without selecting an event"_test = [] {
            auto setup{ make_setup() };
            setup.model.processes().get<SiteRule>().enabled = false;
            setup.model.processes().get<ClusterRule>().enabled = false;
            TestSimulation simulation{ setup };
            expect_reference_state(simulation, setup.model.processes(), 0);
            ut::expect(ut::eq(simulation.rate_tree()->rate_sum().value(), 0.0));
            AnalysisPlan plan;
            ut::expect(simulation.run(plan) == simulation::Status::exhausted);
            ut::expect(ut::eq(simulation.step(), Step{}));
            ut::expect(ut::eq(simulation.rejection_fraction(), 0.0));
        };

        "initial static kinetic-class tables and block rates match a full reconstruction"_test = [] {
            auto setup{ make_setup() };
            TestSimulation simulation{ setup };
            expect_reference_state(simulation, setup.model.processes(), 0);
            ut::expect(ut::that % simulation.rate_tree()->rate_sum().value() > 0.0)
                << "simulation has selectable events";
        };

        "reconstruction matches every accepted event and validates rejected selections"_test = [] {
            auto setup{ make_setup() };
            TestSimulation simulation{ setup };
            AnalysisPlan plan;
            auto& checker{ plan.add<CheckingObserver>(120, setup.model.processes()) };
            auto const status{ simulation.run(plan) };
            ut::expect(status == simulation::Status::analysed) << "stopped at target step";
            ut::expect(ut::eq(simulation.step(), Step{ 120 }));
            ut::expect(ut::eq(checker.last_step(), simulation.step()));
            ut::expect(ut::eq(checker.processed_count(), std::size_t{ 120 }));
            ut::expect(ut::eq(checker.selected_count(),
                checker.processed_count() + checker.rejected_count()));
            ut::expect(ut::that % checker.site_selections() > 0) << "site events selected";
            ut::expect(ut::that % checker.cluster_selections() > 0) << "cluster events selected";
            ut::expect(ut::that % checker.rejected_count() > 0) << "rejection path exercised";
            ut::expect(ut::that % checker.accepted_site_events() > 0)
                << "site update path exercised";
            ut::expect(ut::that % checker.accepted_cluster_events() > 0)
                << "diffusion update path exercised";
        };

        "split and uninterrupted runs yield exactly the same final state"_test = [] {
            auto setup{ make_setup() };
            TestSimulation uninterrupted{ setup }, resumed{ setup };
            AnalysisPlan one_pass;
            auto& full_check{ one_pass.add<CheckingObserver>(160, setup.model.processes()) };
            auto const full_status{ uninterrupted.run(one_pass) };
            ut::expect(full_status == simulation::Status::analysed);
            ut::expect(ut::eq(full_check.last_step(), Step{ 160 }));

            AnalysisPlan first_phase;
            auto& first_check{ first_phase.add<CheckingObserver>(50, setup.model.processes()) };
            auto const first_status{ resumed.run(first_phase) };
            ut::expect(first_status == simulation::Status::analysed);
            ut::expect(ut::eq(first_check.last_step(), Step{ 50 }));

            AnalysisPlan second_phase;
            auto& second_check{ second_phase.add<CheckingObserver>(160, setup.model.processes(), Step{ 50 }) };
            auto const second_status{ resumed.run(second_phase) };
            ut::expect(second_status == simulation::Status::analysed);
            ut::expect(ut::eq(second_check.last_step(), Step{ 160 }));
            ut::expect(ut::eq(second_check.processed_count(), std::size_t{ 110 }));
            expect_same_state(uninterrupted, resumed);
        };
    };
}
