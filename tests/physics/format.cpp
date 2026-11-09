#include "physics/physics.h"

#include <boost/ut.hpp>

#include <exception>
#include <format>
#include <string>
#include <string_view>
#include <utility>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    using namespace physics;
    using namespace physics::quantity;
    using namespace physics::unit::literals;

    template <class Function>
    std::string format_error_message(Function&& function)
    {
        try
        {
            std::forward<Function>(function)();
            return "<no exception>";
        }
        catch (std::format_error const& error)
        {
            return error.what();
        }
        catch (std::exception const& error)
        {
            return std::string{ "<wrong std::exception: " } + error.what() + '>';
        }
        catch (...)
        {
            return "<wrong non-std exception>";
        }
    }

    ut::suite<"physics.format"> format_tests = [] {
        "default formatting uses coherent base units"_test = [] {
            ut::expect(ut::eq(std::string{ "1 m" }, std::format("{}", 1_m)));
            ut::expect(ut::eq(std::string{ "1 kg" }, std::format("{}", 1_kg)));
            ut::expect(
                ut::eq(std::string{ "1 m²" }, std::format("{}", 1_m * 1_m)));
            ut::expect(
                ut::eq(std::string{ "1 m s⁻¹" }, std::format("{}", 1_m / 1_s)));
            ut::expect(
                ut::eq(std::string{ "2" }, std::format("{}", Factor{ 2.0 })));
        };

        "requested units convert the displayed value"_test = [] {
            ut::expect(
                ut::eq(std::string{ "1.5 km" }, std::format("{:km}", 1500_m)));
            ut::expect(
                ut::eq(std::string{ "2.5 kJ" }, std::format("{:kJ}", Energy{ 2500.0 })));
            ut::expect(
                ut::eq(std::string{ "25 %" }, std::format("{:%}", Fraction{ 0.25 })));
            ut::expect(
                ut::eq(std::string{ "1 ppm" }, std::format("{:ppm}", Fraction{ 1E-6 })));
        };

        "compound units accept spaces asterisks and fraction slashes"_test = [] {
            const auto speed{ 120_km / 2_h };
            const ThermalConductivity conductivity{ 1.0 };

            ut::expect(
                ut::eq(std::string{ "60 km/h" }, std::format("{:km/h}", speed)));
            ut::expect(
                ut::eq(
                    std::string{ "1 W/(K⋅m)" },
                    std::format("{:W/m/K}", conductivity)));

            const auto star{ std::format("{:kg*m/s}", Force{ 1.0 }) };
            const auto space{ std::format("{:kg m/s}", Force{ 1.0 }) };
            ut::expect(ut::eq(space, star))
                << "'*' and space should be equivalent unit separators";
        };

        "value-only unit-only and numeric formatting work together"_test = [] {
            const auto speed{ 120_km / 2_h };

            ut::expect(
                ut::eq(
                    std::string{ "60.00" },
                    std::format("{:v:km/h;.2f}", speed)));
            ut::expect(
                ut::eq(
                    std::string{ "km/h" },
                    std::format("{:u:km/h}", speed)));
            ut::expect(
                ut::eq(
                    std::string{ "1.200 m" },
                    std::format("{:m;.3f}", 1.2_m)));
        };

        "ASCII aliases are accepted but canonical symbols are emitted"_test = [] {
            ut::expect(
                ut::eq(
                    std::string{ "2 Ω" },
                    std::format("{:Ohm}", ElectricalResistance{ 2.0 })));
            ut::expect(
                ut::eq(
                    std::string{ "180 °" },
                    std::format("{:deg}", 180_deg)));
        };

        "format errors report the actual user mistake"_test = [] {
            const auto wrong_dimension{ format_error_message(
                [] { (void)std::format("{:s}", 1_m); }) };
            ut::expect(
                ut::eq(
                    std::string{ "Unit is incompatible with quantity dimensions." },
                    wrong_dimension))
                << "length formatted as seconds";

            const auto percent_length{ format_error_message(
                [] { (void)std::format("{:%}", 1_m); }) };
            ut::expect(
                ut::eq(
                    std::string{ "Dimensionless unit requires a dimensionless quantity." },
                    percent_length))
                << "length formatted as percent";

            const auto unknown_unit{ format_error_message(
                [] { (void)std::format("{:not-a-unit}", 1_m); }) };
            ut::expect(
                ut::eq(std::string{ "Invalid unit." }, unknown_unit))
                << "unknown unit token";

            const auto duplicate_dimensionless{ format_error_message(
                [] { (void)std::format("{:% ppm}", Factor{ 0.5 }); }) };
            ut::expect(
                ut::eq(
                    std::string{ "Only one dimensionless unit may be specified." },
                    duplicate_dimensionless))
                << "two dimensionless representations in one format specification";
        };
    };
}
