#include "dealloying/kinetics/gtc.h"
#include "dealloying/kinetics/gtc_geometry.h"
#include "dealloying/kinetics/gtc_path_cache.h"
#include "dealloying/kinetics/descriptor_format.h"
#include "lattice/lattice.h"
#include "dealloying/model.h"
#include "dealloying/process.h"
#include "kmc/simulation.h"
#include "kmc/analysis_plan.h"
#include "output/grouped_kinetic_activity.h"
#include "output/kinetic_activity.h"
#include "physics/equations/kinetics.h"

#include <boost/ut.hpp>
#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <map>
#include <numeric>
#include <ranges>
#include <type_traits>
#include <vector>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using namespace kmc;
    namespace kinetics = studies::dealloying::kinetics;
    using enum chemistry::Element;

    void expect_rate(Rate actual, Rate expected)
    {
        ut::expect(std::abs(actual.value() / expected.value() - 1.0) < 1e-12);
    }


    using GtcModel = studies::dealloying::Model;
    using GtcSimulation = kmc::RejectionTreeSimulation<GtcModel>;

    kmc::Setup<GtcModel> gtc_setup()
    {
        kmc::Setup<GtcModel> setup;
        setup.seed = simulation::Seed{1,2,3,4,5,6,7,8};
        setup.model.kinetics().set_temperature(300_K).set_potential(0.85_V);
        setup.make_lattice = [] {
            auto cell{lattice::UnitCell::conventional<lattice::Bravais::cF>(400_pm)
                .fill(chemistry::Atom{Au})};
            lattice::Lattice lattice{cell};
            lattice.scale_shape(lattice::Parameters::all(1200_pm));
            using enum math::Axis;
            lattice.set_periodic_boundaries(x | y | z);
            lattice.construct_sites();
            lattice.construct_neighbor_list();
            lattice.set_fill_function([](lattice::Lattice& lattice, simulation::Engine&) {
                for (auto& site:lattice.sites())
                    if (site.index()%7==0)
                        site.set_element(lattice, chemistry::Element::none);
                    else if (site.index()%3==1)
                        site.set_element(lattice, Ag);
            });
            return lattice;
        };
        return setup;
    }

    // Rebuild every event from current site occupancy, independently of the
    // cached slot key and incrementally updated site/block sums.
    void expect_gtc_reference_state(GtcSimulation const& simulation)
    {
        auto& lattice{const_cast<lattice::Lattice&>(simulation.lattice())};
        auto const& model{simulation.model()};
        auto const& kinetics{model.kinetics()};
        auto const& classes{kinetics.kinetic_classes()};
        auto const& stored{simulation.slot_class_keys()};
        auto const& processes{model.processes()};
        constexpr std::size_t block_size{std::size_t{1}<<kmc::rate_tree::baseLevel};
        std::vector<Rate> blocks((lattice.sites().size()+block_size-1)/block_size);
        std::vector<std::size_t> counts(classes.size());

        for (auto& site:lattice.sites())
        {
            Rate site_total{};
            bool candidate{};
            auto const first{static_cast<std::size_t>(site.index())*GtcModel::slots_per_site};
            processes.for_each([&](auto const& process,std::size_t offset) {
                using ProcessT = std::remove_cvref_t<decltype(process)>;
                const bool eligible{process.is_candidate(site)};
                candidate |= eligible;
                Rate process_total{};
                for (std::size_t slot{};slot<ProcessT::slot_count;++slot)
                {
                    const auto position{first+offset+slot};
                    kmc::RateClassKey expected{};
                    if (eligible and kmc::has_slot(process,site,slot))
                    {
                        auto location{kmc::make_location(process,site,slot,lattice)};
                        if (kmc::is_location_candidate(process,location))
                        {
                            const auto descriptor{
                                kinetics.classify(ProcessT::event,location,lattice)};
                            expected=stored[position];
                            ut::expect(expected != kmc::RateClassKey{})
                                << "nonzero GTC class for eligible slot";
                            if (expected != kmc::RateClassKey{} and expected < classes.size())
                            {
                                ut::expect(kinetics.descriptor(expected)==descriptor)
                                    << "cached GTC descriptor, step " << simulation.step();
                                const auto actual_rate{kinetics.rate(descriptor)};
                                ut::expect(classes[expected].is_initialized());
                                ut::expect(ut::eq(classes[expected].rate.value(),
                                    actual_rate.value())) << "cached GTC rate";
                                ++counts[expected];
                                process_total += actual_rate;
                            }
                        }
                    }
                    ut::expect(ut::eq(stored[position],expected))
                        << "slot key, step " << simulation.step();
                }
                site_total += process_total;
            });
            blocks[site.index()/block_size] += site_total;
            ut::expect(site.is_source_eligible()==candidate)
                << "source eligibility, step " << simulation.step();
            ut::expect(not site.is_site_update_queued());
            ut::expect(not site.is_block_update_queued());
        }

        ut::expect(simulation.class_counts()==counts)
            << "all GTC class populations after step " << simulation.step();
        for (std::size_t i{};i<blocks.size();++i)
            ut::expect(ut::eq(simulation.rate_tree()->block_rate(
                static_cast<lattice::Index>(i*block_size)).value(),blocks[i].value()))
                << "incremental block rate after step " << simulation.step();

        std::vector<Rate> levels(std::bit_ceil(blocks.size()));
        std::ranges::copy(blocks,levels.begin());
        for (auto count{levels.size()};count>1;count/=2)
            for (std::size_t i{};i<count/2;++i)
                levels[i]=levels[2*i]+levels[2*i+1];
        ut::expect(ut::eq(simulation.rate_tree()->rate_sum().value(),levels.front().value()))
            << "full GTC rate-tree reconstruction after step " << simulation.step();
    }

    class GtcReferenceObserver : public kmc::Observer
    {
    public:
        explicit GtcReferenceObserver(kmc::Step target)
            : Observer{Resolution{1,1.0}},target_{target} {}

        std::string_view name() const override { return "gtc-full-reference"; }
        bool requires_class_counts() const override { return true; }
        bool stop_requested(kmc::Simulation const& simulation) const override
        {
            return simulation.step()>=target_;
        }
        void observe_started(kmc::Simulation const& simulation) override
        {
            expect_gtc_reference_state(dynamic_cast<GtcSimulation const&>(simulation));
        }
        void take_snapshot(kmc::Simulation const& simulation) override
        {
            expect_gtc_reference_state(dynamic_cast<GtcSimulation const&>(simulation));
        }
    private:
        kmc::Step target_{};
    };

    ut::suite<"kinetics.gtc"> gtc_tests = [] {
        "dissolution retains power-law rates and an empty environment uses a direct midpoint"_test = [] {
            kinetics::GTC model;
            kinetics::PowerLawBondEnergy baseline;
            static_assert(kinetics::GTC::invalidation_hops == 2);
            model.set_temperature(300_K).set_potential(0.85_V);
            baseline.set_temperature(300_K).set_potential(0.85_V);
            for (auto element : { Ag, Au })
                for (lattice::Coordination n : { 3, 6, 9 })
                {
                    expect_rate(model.rate({ event::Type::dissolution, element, n, std::nullopt }),
                        baseline.rate({ event::Type::dissolution, element, n }));
                    // With no occupied spectator, neither a support arc nor
                    // final-site bonding is available: n_TS=0, not the n=2
                    // PowerLaw fallback.
                    ut::expect(model.rate({ event::Type::diffusion, element, n, std::nullopt }) <
                        baseline.rate({ event::Type::diffusion, element, n }));
                }
            ut::expect(model.output_text().find("direct hop") != std::string::npos);
        };

        "an explicitly specified saddle changes the diffusion rate only"_test = [] {
            kinetics::GTC model;
            model.set_temperature(300_K).set_potential(0.85_V);
            kinetics::GTC::Descriptor baseline{ event::Type::diffusion, Au, 6, 2.0,
                kinetics::gtc::FccEnvironment::full_mask };
            auto refined{baseline};
            refined.transition_state_coordination = 3.1;
            ut::expect(model.rate(refined) > model.rate(baseline));
            ut::expect(model.key(refined) != model.key(baseline));
            ut::expect(model.number_keys() == std::size_t{3});
            ut::expect(model.descriptor(model.key(refined)) == refined);
            ut::expect(output::DescriptorFormat<kinetics::GTC::Descriptor>::row(refined).find(
                "nTS=3.1") != std::string::npos);

            kinetics::PowerLawBondEnergy reference;
            reference.set_temperature(model.temperature()).set_potential(model.potential());
            expect_rate(model.rate({ event::Type::dissolution, Ag, 9, std::nullopt }),
                reference.rate({ event::Type::dissolution, Ag, 9 }));
        };

        "classify does not yet invent a path-specific saddle"_test = [] {
            auto cell{lattice::UnitCell::conventional<lattice::Bravais::cF>(400_pm)
                .fill(chemistry::Atom{Au})};
            lattice::Lattice lattice{cell};
            lattice.scale_shape(lattice::Parameters::all(1600_pm));
            using enum math::Axis;
            lattice.set_periodic_boundaries(x | y | z);
            lattice.construct_sites();
            lattice.construct_neighbor_list();
            auto& source{lattice.sites().front()};
            auto* target{*lattice.neighbors(source).begin()};
            target->set_element(lattice, chemistry::Element::none);

            kinetics::GTC model;
            const auto desc{model.classify(event::Type::diffusion,
                event::Location{&source, target}, lattice)};
            ut::expect(desc.event == event::Type::diffusion);
            ut::expect(desc.element == Au);
            ut::expect(desc.coordination == source.coordination());
            ut::expect(not desc.transition_state_coordination.has_value());
        };

        "canonical eighteen-site mask is invariant across FCC hop directions"_test = [] {
            auto cell{lattice::UnitCell::conventional<lattice::Bravais::cF>(400_pm)
                .fill(chemistry::Atom{Au})};
            lattice::Lattice lattice{cell};
            lattice.scale_shape(lattice::Parameters::all(1600_pm));
            using enum math::Axis;
            lattice.set_periodic_boundaries(x | y | z);
            lattice.construct_sites();
            lattice.construct_neighbor_list();

            kinetics::gtc::FccEnvironment const mapping{lattice};
            auto& source{lattice.sites().front()};
            std::size_t directions{};
            for (auto* target:lattice.neighbors(source))
            {
                if (target==&source)
                    continue;
                target->set_element(lattice,chemistry::Element::none);
                ut::expect(mapping.classify(source,*target) ==
                    kinetics::gtc::FccEnvironment::full_mask);
                target->set_element(lattice,Au);
                ++directions;
            }
            ut::expect(directions == 12)
                << "full periodic FCC source has twelve crystallographic directions";
        };

        "path cache requires occupied supports and reuses the same mask"_test = [] {
            using kinetics::gtc::saddle_coordination;
            ut::expect(saddle_coordination(0) == 0.0);
            ut::expect(kinetics::gtc::final_coordination(0) == 0u);

            // Bit 14 is a target-only neighbor in the fixed lexicographic
            // FCC layout. A single support cannot form an admissible arc.
            // The straight-hop midpoint has a finite Gaussian weight.
            const std::uint32_t target_only{1u<<14};
            const double distance_ratio{std::sqrt(1.25)};
            const double expected{std::exp(-std::pow(6.0*(distance_ratio-1.0),2))};
            ut::expect(kinetics::gtc::final_coordination(target_only) == 1u);
            ut::expect(std::abs(saddle_coordination(target_only)-expected) < 1e-6);

            // Bit 9 is source-only, so the final site has zero bonds.
            // Even with a positive midpoint weight, E_final is the maximum.
            ut::expect(kinetics::gtc::final_coordination(1u<<9) == 0u);
            ut::expect(saddle_coordination(1u<<9) == 0.0);

            // Bits 6 and 7 form a direct one-segment arc supported by two
            // shared FCC neighbors. Its effective coordination and final
            // lattice coordination are both two, providing an exact reference.
            const std::uint32_t direct_arc{(1u<<6)|(1u<<7)};
            ut::expect(kinetics::gtc::final_coordination(direct_arc) == 2u);
            ut::expect(std::abs(saddle_coordination(direct_arc)-2.0) < 1e-5);

            // At least one pair among the four common FCC neighbors must
            // support an admissible one-segment path.
            constexpr std::array<unsigned,4> common{6,7,10,11};
            bool found{};
            for (std::size_t i{};i<common.size();++i)
                for (std::size_t j{i+1};j<common.size();++j)
                {
                    auto const mask{(1u<<common[i])|(1u<<common[j])};
                    auto const saddle{saddle_coordination(mask)};
                    if (saddle > 0.0)
                    {
                        ut::expect(saddle_coordination(mask) == saddle);
                        ut::expect(saddle <= static_cast<double>(
                            kinetics::gtc::final_coordination(mask)));
                        found=true;
                    }
                }
            ut::expect(found);
            ut::expect(ut::throws<std::invalid_argument>([] {
                (void)saddle_coordination(1u<<18);
            }));
        };

        "cached admissible path affects the kinetic diffusion rate"_test = [] {
            kinetics::GTC model;
            model.set_temperature(300_K).set_potential(0.85_V);

            constexpr std::array<unsigned,4> common{6,7,10,11};
            bool found{};
            for (std::size_t i{};i<common.size();++i)
                for (std::size_t j{i+1};j<common.size();++j)
                    for (std::size_t k{j+1};k<common.size();++k)
                {
                    // A third occupied common neighbor contributes to the
                    // Gaussian energy while another pair supports the arc.
                    std::uint32_t const mask{
                        (1u<<common[i])|(1u<<common[j])|(1u<<common[k])};
                    auto const saddle{kinetics::gtc::saddle_coordination(mask)};
                    if (std::abs(saddle-2.0)<1e-3)
                        continue;

                    kinetics::GTC::Descriptor with_path{
                        event::Type::diffusion,Au,6,std::nullopt,mask};
                    kinetics::GTC::Descriptor baseline{
                        event::Type::diffusion,Au,6,2.0,mask};
                    ut::expect(model.rate(with_path) != model.rate(baseline));
                    ut::expect(model.key(with_path) != model.key(baseline));
                    found=true;
                    break;
                }
            ut::expect(found);
        };

        "explicit transition state is capped by the final lattice-site energy"_test = [] {
            kinetics::GTC model;
            model.set_temperature(300_K).set_potential(0.85_V);
            const std::uint32_t target_only{1u<<14};
            // Only one target neighbor remains: any larger path coordination
            // must give the same rate as n_TS=1.
            expect_rate(
                model.rate({event::Type::diffusion,Au,6,100.0,target_only}),
                model.rate({event::Type::diffusion,Au,6,1.0,target_only}));
        };

        "Gaussian support weight uses the distance descriptor of Eq. 1.44"_test = [] {
            using kinetics::gtc::distance_weight;
            ut::expect(std::abs(distance_weight(1_nm, 1_nm) - 1.0) < 1e-12);
            ut::expect(std::abs(distance_weight(1_nm + 1_nm / 6, 1_nm) -
                std::exp(-1.0)) < 1e-12);
            ut::expect(ut::throws<std::invalid_argument>([] {
                (void)distance_weight(1_nm, 1_nm, 0.0);
            }));
        };


        "sparse canonical masks are bijective for every hop and rotated boundaries"_test = [] {
            using kinetics::gtc::FccEnvironment;
            for (auto indices : {lattice::Indices{0,0,1}, lattice::Indices{1,1,1}})
            {
                auto cell{lattice::UnitCell::conventional<lattice::Bravais::cF>(400_pm)
                    .fill(chemistry::Atom{Au})};
                lattice::Lattice lattice{cell,indices};
                lattice.scale_shape(lattice::Parameters::all(2400_pm));
                using enum math::Axis;
                lattice.set_periodic_boundaries(x | y | z);
                lattice.construct_sites();
                lattice.construct_neighbor_list();
                lattice.fill(chemistry::Element::none);
                auto& source{lattice.sites().front()};
                source.set_element(lattice,Au);
                FccEnvironment mapping{lattice};
                std::size_t directions{};

                for (auto* target:lattice.neighbors(source))
                {
                    std::vector<lattice::Site*> spectators;
                    auto insert=[&](lattice::Site* neighbor) {
                        if (neighbor==&source or neighbor==target)
                            return;
                        if (std::ranges::find(spectators,neighbor)==spectators.end())
                            spectators.push_back(neighbor);
                    };
                    for (auto* neighbor:lattice.neighbors(source))
                        insert(neighbor);
                    for (auto* neighbor:lattice.neighbors(*target))
                        insert(neighbor);
                    ut::expect(spectators.size()==18u);

                    std::uint32_t seen{};
                    for (auto* spectator:spectators)
                    {
                        spectator->set_element(lattice,Au);
                        auto const mask{mapping.classify(source,*target)};
                        ut::expect(std::popcount(mask)==1)
                            << "one occupied spectator gives one bit";
                        ut::expect((seen & mask)==0u)
                            << "different physical spectators have different bits";
                        seen |= mask;
                        spectator->set_element(lattice,chemistry::Element::none);
                    }
                    ut::expect(seen==FccEnvironment::full_mask)
                        << "all eighteen canonical bits are represented";
                    ++directions;
                }
                ut::expect(directions==12u);
            }
        };

        "reverse FCC diffusion satisfies detailed balance for asymmetric environments"_test = [] {
            auto cell{lattice::UnitCell::conventional<lattice::Bravais::cF>(400_pm)
                .fill(chemistry::Atom{Au})};
            lattice::Lattice lattice{cell};
            lattice.scale_shape(lattice::Parameters::all(1600_pm));
            using enum math::Axis;
            lattice.set_periodic_boundaries(x | y | z);
            lattice.construct_sites();
            lattice.construct_neighbor_list();
            auto& source{lattice.sites().front()};
            kinetics::GTC model;
            kinetics::PowerLawBondEnergy binding;
            model.set_temperature(300_K);
            const auto thermal{physics::equations::kinetics::thermal_energy(300_K)};

            for (auto* target:lattice.neighbors(source))
                for (unsigned pattern{};pattern<4;++pattern)
                {
                    lattice.fill(chemistry::Element::none);
                    source.set_element(lattice,Au);
                    std::vector<lattice::Site*> spectators;
                    auto insert=[&](lattice::Site* neighbor) {
                        if (neighbor==&source or neighbor==target or
                            std::ranges::find(spectators,neighbor)!=spectators.end())
                            return;
                        spectators.push_back(neighbor);
                    };
                    for (auto* neighbor:lattice.neighbors(source))
                        insert(neighbor);
                    for (auto* neighbor:lattice.neighbors(*target))
                        insert(neighbor);
                    std::ranges::sort(spectators,{},&lattice::Site::index);
                    for (std::size_t i{};i<spectators.size();++i)
                        if ((i*7+pattern*3)%11 < 6)
                            spectators[i]->set_element(lattice,Au);

                    const auto forward{model.classify(event::Type::diffusion,
                        event::Location{&source,target},lattice)};
                    const auto r_forward{model.rate(forward).value()};
                    const auto initial_binding{binding.lattice_energy_barrier(
                        {event::Type::dissolution,Au,source.coordination()})};

                    source.set_element(lattice,chemistry::Element::none);
                    target->set_element(lattice,Au);
                    const auto reverse{model.classify(event::Type::diffusion,
                        event::Location{target,&source},lattice)};
                    const auto r_reverse{model.rate(reverse).value()};
                    const auto final_binding{binding.lattice_energy_barrier(
                        {event::Type::dissolution,Au,target->coordination()})};

                    const double expected{(final_binding-initial_binding)/thermal};
                    const double actual{std::log(r_forward)-std::log(r_reverse)};
                    ut::expect(std::abs(actual-expected)<1e-5)
                        << "detailed balance for directed hop and occupancy pattern";
                }
        };

        "GTC incremental rates agree with full reconstruction after every event"_test = [] {
            GtcSimulation simulation{gtc_setup()};
            kmc::AnalysisPlan plan;
            plan.add<GtcReferenceObserver>(12);
            ut::expect(simulation.run(plan)==simulation::Status::analysed);
            ut::expect(simulation.step()==12u);
        };

        "grouped kinetic activity equals exact class-by-class integration"_test = [] {
            GtcSimulation simulation{gtc_setup()};
            kmc::AnalysisPlan plan;
            plan.add<GtcReferenceObserver>(8);
            auto& grouped{plan.add<output::GroupedKineticActivity<GtcModel>>()};
            auto& detailed{plan.add<output::KineticActivity<GtcModel>>()};
            grouped.set_resolution({1,1.0});
            detailed.set_resolution({1,1.0});
            ut::expect(simulation.run(plan)==simulation::Status::analysed);

            auto const& grouped_data{grouped.last_data()};
            auto const& detailed_data{detailed.last_data()};
            ut::expect(grouped_data.samples==detailed_data.samples);
            ut::expect(std::abs(grouped_data.residence_time.value()-
                detailed_data.residence_time.value())<1e-12);

            using Group = output::GroupedKineticActivity<GtcModel>::Group;
            std::map<Group,std::pair<double,double>> expected;
            for (auto const& [descriptor,activity]:detailed_data.classes)
            {
                auto& sum{expected[Group{
                    descriptor.event,descriptor.element,descriptor.coordination}]};
                sum.first += activity.activity_fraction;
                sum.second += activity.activity_time.value();
            }
            for (auto const& [key, sum]:expected)
            {
                auto it{grouped_data.groups.find(key)};
                ut::expect(it!=grouped_data.groups.end());
                if (it==grouped_data.groups.end())
                    continue;
                const double frac_error{std::abs(it->second.activity_fraction-sum.first)};
                const double time_error{std::abs(it->second.activity_time-sum.second)};
                ut::expect(frac_error<1e-8) << "grouped activity fraction";
                ut::expect(time_error<1e-8*std::max(1.0,std::abs(sum.second)))
                    << "grouped residence time";
            }
        };

        "GTC CSV labels distinguish occupancy masks and expose the effective saddle"_test = [] {
            using Descriptor=kinetics::GTC::Descriptor;
            Descriptor first{event::Type::diffusion,Au,6,std::nullopt,1u<<14};
            auto second{first};
            second.environment=1u<<9;
            const auto label_a{output::DescriptorFormat<Descriptor>::label(first)};
            const auto label_b{output::DescriptorFormat<Descriptor>::label(second)};
            ut::expect(label_a!=label_b);
            ut::expect(label_a.find("mask=0x")!=std::string::npos);
            ut::expect(label_a.find("nTS=")!=std::string::npos);
        };

        "nonfinite saddle coordinate is rejected before caching"_test = [] {
            kinetics::GTC model;
            kinetics::GTC::Descriptor desc{event::Type::diffusion, Au, 6,
                std::numeric_limits<double>::quiet_NaN()};
            ut::expect(ut::throws<std::invalid_argument>([&] {(void)model.key(desc);}));
        };
    };
}
