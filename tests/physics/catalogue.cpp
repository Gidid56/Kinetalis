#include "physics/literals.h"
#include "physics/prefixed_unit.h"
#include "physics/unit_dimensions.h"

#include <boost/ut.hpp>

#include <cmath>
#include <numbers>
#include <string>
#include <string_view>
#include <utility>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace physics;
    using namespace physics::unit::literals;

    void expect_prefixed_unit(
        std::string_view text,
        PrefixedUnit expected,
        std::string_view reason)
    {
        const auto actual{ prefixed_unit::parse(text) };

        ut::expect(
            ut::eq(
                std::to_underlying(expected.unit),
                std::to_underlying(actual.unit)))
            << reason << "; token='" << text << "'; unit";

        ut::expect(
            ut::eq(
                std::to_underlying(expected.prefix),
                std::to_underlying(actual.prefix)))
            << reason << "; token='" << text << "'; prefix";
    }

    ut::suite<"physics.catalogue"> catalogue_tests = [] {
        "representative literals convert to canonical storage"_test = [] {
            ut::expect(ut::eq(1E-3, (1_ms).value()));
            ut::expect(ut::eq(1E3, (1_km).value()));
            ut::expect(ut::eq(1E-3, (1_g).value()));
            ut::expect(ut::eq(60.0, (1_min).value()));

            const auto degree_error{
                std::abs((1_deg).value() - std::numbers::pi / 180)
            };
            ut::expect(ut::that % degree_error < 1E-15)
                << "1 degree: actual radians " << (1_deg).value();

            ut::expect(ut::eq(1E-3, (1_l).value()));
            ut::expect(ut::eq(1E5, (1_bar).value()));
            ut::expect(ut::eq(4.184, (1_cal).value()));
            ut::expect(ut::eq(1.602'176'634E-19, (1_eV).value()));
            ut::expect(ut::eq(149'597'870'700.0, (1_au).value()));

            const auto knot_expected{ 1852.0 / 3600.0 };
            ut::expect(
                ut::that % std::abs((1_kn).value() - knot_expected) < 1E-15)
                << "1 knot: expected " << knot_expected
                << ", actual " << (1_kn).value();

            ut::expect(ut::eq(0.25, (25_percent).value()));
            ut::expect(ut::eq(1E-6, (1_ppm).value()));
        };

        "prefix metadata follows decimal powers"_test = [] {
            ut::expect(
                ut::eq(std::string_view{ "µ" }, prefix::symbol(Prefix::micro)));
            ut::expect(
                ut::eq(std::string_view{ "u" }, prefix::ascii_symbol(Prefix::micro)));

            for (const auto current : prefix::values)
            {
                const auto actual{ prefix::factor(current) };
                const auto expected{
                    std::pow(10.0L, std::to_underlying(current))
                };
                const auto error{ std::abs(actual - expected) };

                ut::expect(ut::that % error < 1E-15L)
                    << "prefix exponent " << std::to_underlying(current)
                    << ": expected factor " << expected
                    << ", actual " << actual;

                ut::expect(ut::that % prefix::symbol(current).empty() == false)
                    << "prefix exponent " << std::to_underlying(current)
                    << " has no canonical symbol";
            }
        };

        "every concrete unit has usable metadata"_test = [] {
            for (auto i{ unit::index::first }; i <= unit::index::last; ++i)
            {
                const auto current{ static_cast<Unit>(i) };
                const auto symbol{ unit::symbol(current) };
                const auto factor{ unit::factor(current) };

                ut::expect(ut::that % symbol.empty() == false)
                    << "unit enum " << i << " has no canonical symbol";

                ut::expect(ut::that % std::isfinite(factor) == true)
                    << "unit '" << symbol << "' has non-finite factor " << factor;

                ut::expect(ut::that % factor > 0.0L)
                    << "unit '" << symbol << "' has non-positive factor " << factor;

                bool mapped{};
                try
                {
                    const auto representation{
                        unit::dimensions::representation(current)
                    };
                    mapped = representation.size() == Dimensions<>::size;
                }
                catch (...)
                {
                    mapped = false;
                }

                ut::expect(ut::that % mapped == true)
                    << "unit '" << symbol << "' has no runtime dimension mapping";
            }

            ut::expect(ut::eq(1.0L, unit::factor(Unit::none)));
            ut::expect(ut::eq(0.0L, unit::offset(Unit::none)));
            ut::expect(ut::that % unit::symbol(Unit::none).empty() == true);
        };

        "canonical unit symbols parse back to the same unit"_test = [] {
            for (auto i{ unit::index::first }; i <= unit::index::last; ++i)
            {
                const auto current{ static_cast<Unit>(i) };
                const auto token{ unit::symbol(current) };

                expect_prefixed_unit(
                    token,
                    { current, Prefix::none },
                    "canonical unit symbol should round-trip");
            }
        };

        "all legal canonical prefix combinations parse"_test = [] {
            for (auto i{ unit::index::first }; i <= unit::index::last; ++i)
            {
                const auto current{ static_cast<Unit>(i) };

                if (not unit::is_prefixable(current))
                    continue;

                for (const auto current_prefix : prefix::values)
                {
                    std::string token{ prefix::symbol(current_prefix) };
                    token += unit::symbol(current);

                    expect_prefixed_unit(
                        token,
                        { current, current_prefix },
                        "prefixed unit should round-trip");
                }
            }
        };

        "common ASCII spellings and ambiguous tokens parse as expected"_test = [] {
            expect_prefixed_unit(
                "um", { Unit::meter, Prefix::micro }, "ASCII micrometre");
            expect_prefixed_unit(
                "Ohm", { Unit::ohm, Prefix::none }, "ASCII ohm");
            expect_prefixed_unit(
                "kOhm", { Unit::ohm, Prefix::kilo }, "prefixed ASCII ohm");
            expect_prefixed_unit(
                "degC", { Unit::degree_celsius, Prefix::none }, "ASCII Celsius");
            expect_prefixed_unit(
                "min", { Unit::minute, Prefix::none }, "minute");
            expect_prefixed_unit(
                "mm", { Unit::meter, Prefix::milli }, "millimetre");
            expect_prefixed_unit(
                "dam", { Unit::meter, Prefix::deca }, "decametre");
            expect_prefixed_unit(
                "dB", { Unit::bel, Prefix::deci }, "decibel");
        };

        "invalid unit text fails cleanly"_test = [] {
            expect_prefixed_unit("", {}, "empty token");
            expect_prefixed_unit("not-a-unit", {}, "unknown token");
            expect_prefixed_unit("kmin", {}, "prefix on non-prefixable unit");
        };

        //"special units expose their persistent quantity properties"_test = [] {
        //    using quantity::Property;

        //    ut::expect(
        //        ut::eq(
        //            std::to_underlying(Property::absolute),
        //            std::to_underlying(unit::default_constraints(Unit::kelvin))));
        //    ut::expect(
        //        ut::eq(
        //            std::to_underlying(Property::absolute),
        //            std::to_underlying(
        //                unit::default_property(Unit::degree_celsius))));
        //    ut::expect(
        //        ut::eq(
        //            std::to_underlying(Property::logarithmic),
        //            std::to_underlying(unit::default_property(Unit::neper))));
        //    ut::expect(
        //        ut::eq(
        //            std::to_underlying(Property::logarithmic),
        //            std::to_underlying(unit::default_property(Unit::bel))));
        //    ut::expect(
        //        ut::eq(
        //            std::to_underlying(Property::none),
        //            std::to_underlying(unit::default_property(Unit::percent))));
        //};
    };
}
