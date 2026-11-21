#include "dealloying/kinetics/gtc.h"
#include "dealloying/kinetics/gtc_geometry.h"
#include "dealloying/kinetics/gtc_path_cache.h"
#include "dealloying/kinetics/descriptor_format.h"
#include "lattice/lattice.h"

#include <boost/ut.hpp>
#include <array>
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

            expect_rate(model.rate({ event::Type::dissolution, Ag, 9, std::nullopt }),
                kinetics::PowerLawBondEnergy{}.rate({ event::Type::dissolution, Ag, 9 }));
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

        "nonfinite saddle coordinate is rejected before caching"_test = [] {
            kinetics::GTC model;
            kinetics::GTC::Descriptor desc{event::Type::diffusion, Au, 6,
                std::numeric_limits<double>::quiet_NaN()};
            ut::expect(ut::throws<std::invalid_argument>([&] {(void)model.key(desc);}));
        };
    };
}
