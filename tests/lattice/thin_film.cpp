#include "dealloying/setups.h"
#include "lattice/lattice.h"
#include "simulation/engine.h"

#include <boost/ut.hpp>
#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using chemistry::Element;
    namespace setups = studies::dealloying::setups;

    constexpr auto test_seed = "00000001-00000002-00000003-00000004-"
                               "00000005-00000006-00000007-00000008";

    void check_film(lattice::Indices orientation)
    {
        constexpr auto side{ 4_nm };
        constexpr auto box_height{ 6_nm };
        constexpr auto substrate{ 1_nm };

        auto film{ setups::thin_film(25, side, box_height, substrate, orientation) };
        ut::expect(film.periodic_boundaries() == (math::Axis::x | math::Axis::y));
        ut::expect(film.shape().parameters().z().value() >= box_height.value());

        simulation::Engine rng{ test_seed };
        film.apply_fill_function(rng);

        const double half_height{ film.shape().parameters().z().value() / 2 };
        const double thickness{ substrate.value() };
        constexpr double margin{ 1e-12 };

        std::size_t bottom{}, alloy{}, top{}, ag{}, au{};
        for (auto const& site : film.sites())
        {
            const double z{ site.position().z().value() };
            if (z < -half_height + thickness - margin)
            {
                ut::expect(site.element() == Element::Au);
                ++bottom;
            }
            else if (z > half_height - thickness + margin)
            {
                ut::expect(site.element() == Element::none);
                ++top;
            }
            else if (z > -half_height + thickness + margin and
                     z < half_height - thickness - margin)
            {
                ut::expect(site.element() == Element::Ag or site.element() == Element::Au);
                ++alloy;
                if (site.element() == Element::Ag) ++ag;
                if (site.element() == Element::Au) ++au;
            }
        }

        ut::expect(bottom > 0);
        ut::expect(alloy > 0);
        ut::expect(top > 0);
        ut::expect(ag > 0);
        ut::expect(au > 0);
        ut::expect(film.label() == "Ag-Au thin film on Au substrate");
    }

    ut::suite<"lattice.thin_film"> thin_film_tests = [] {
        "film has bottom Au substrate and open upper vacuum along [001]"_test = [] {
            check_film(lattice::Indices::unit_z());
        };

        "film geometry supports the [111] surface orientation"_test = [] {
            check_film(lattice::Indices{ 1, 1, 1 });
        };

        "film dimensions and composition are validated"_test = [] {
            ut::expect(ut::throws<std::invalid_argument>([] {
                (void)setups::thin_film(101, 4_nm, 6_nm, 1_nm);
            }));
            ut::expect(ut::throws<std::invalid_argument>([] {
                (void)setups::thin_film(25, 4_nm, 6_nm, 3_nm);
            }));
            ut::expect(ut::throws<std::invalid_argument>([] {
                (void)setups::thin_film(25, 0_nm, 6_nm, 1_nm);
            }));
        };

        "thin film setup attaches generator and configures kinetics"_test = [] {
            auto setup{ setups::thin_film_dealloying(
                10, 0.1_V, 300_K, 4_nm, 6_nm, 1_nm) };
            ut::expect(setup.model.kinetics().potential() ==
                critical_potential(10, 300_K) + 0.1_V);
            ut::expect(setup.model.kinetics().temperature() == 300_K);
            ut::expect(setup.name.find("thin film") != std::string::npos);
            auto film{ setup.make_lattice() };
            ut::expect(film.periodic_boundaries() == (math::Axis::x | math::Axis::y));
        };
    };
}
