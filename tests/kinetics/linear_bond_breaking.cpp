#include "dealloying/kinetics/linear_bond_breaking.h"
#include "dealloying/kinetics/descriptor_format.h"

#include <boost/ut.hpp>
#include <limits>
#include <stdexcept>
#include <string>

namespace ut = boost::ut;
using namespace boost::ut::literals;
using namespace physics::unit::literals;

namespace
{
    using namespace kmc;
    namespace models = studies::dealloying::models;

    ut::suite<"kinetics.linear_bond_breaking"> linear_bond_breaking_tests = [] {
        "default bond energy and setter retain the configured value"_test = [] {
            kinetics::LinearBondBreaking model;
            ut::expect(model.constants().bond_energy == 0.15_eV);

            model.set_bond_energy(0.20_eV);
            ut::expect(model.constants().bond_energy == 0.20_eV);
            ut::expect(model.output_text().find("Linear bond breaking") != std::string::npos);
            ut::expect(output::DescriptorFormat<kinetics::LinearBondBreaking::Descriptor>::header() ==
                std::string_view{ "key:event,key:element,key:coordination,key:details" });
        };

        "nonfinite bond energies are rejected"_test = [] {
            ut::expect(ut::throws<std::invalid_argument>([] {
                kinetics::LinearBondBreaking::ensure_range(
                    Energy{ std::numeric_limits<double>::infinity() });
            }));
            ut::expect(ut::throws<std::invalid_argument>([] {
                kinetics::LinearBondBreaking::ensure_range(
                    Energy{ std::numeric_limits<double>::quiet_NaN() });
            }));
        };
    };
}
