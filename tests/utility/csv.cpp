#include "utility/csv.h"

#include <boost/ut.hpp>
#include <chrono>
#include <filesystem>
#include <format>
#include <system_error>
#include <stdexcept>
#include <string>
#include <vector>

namespace ut = boost::ut;
using namespace boost::ut::literals;

namespace
{
    struct TempDirectory
    {
        std::filesystem::path root{
            std::filesystem::temp_directory_path() /
            std::format("kinetalis-csv-{}",
                std::chrono::steady_clock::now().time_since_epoch().count())
        };

        ~TempDirectory()
        {
            std::error_code error;
            std::filesystem::remove_all(root, error);
        }
    };

    ut::suite<"utility.csv.writer"> csv_writer_tests = [] {
        "growing columns are padded at the correct row boundaries"_test = [] {
            for (auto padding : { utility::csv::Padding::empty, utility::csv::Padding::zero })
            {
                TempDirectory temp;
                auto const base{ temp.root / "composition" };
                auto const data{ temp.root / "composition.data.csv" };
                auto const schema{ temp.root / "composition.schema.csv" };
                auto const result{ temp.root / "composition.csv" };

                std::string header{ "key:step,time/s" };
                std::string row{ "0,0" };
                utility::csv::Writer writer{ padding };
                writer.initialize(base, [&] { return header; }, [&] { return row; });

                writer.append(0);
                header += ",Ag";
                row = "1,1,0.8";
                writer.append(1);
                row = "2,2,0.7";
                writer.append(1);
                header += ",Au,Cu";
                row = "3,3,0.6,0.3,0.1";
                writer.append(3);

                auto const changes{ utility::csv::Table{}.read(schema) };
                ut::expect(ut::eq(changes.size(), std::size_t{ 3 }));
                ut::expect(changes[0] == utility::csv::Table::Row{ "0", "0", "key:step,time/s" });
                ut::expect(changes[1] == utility::csv::Table::Row{ "1", "1", "key:step,time/s,Ag" });
                ut::expect(changes[2] == utility::csv::Table::Row{
                    "3", "3", "key:step,time/s,Ag,Au,Cu" });
                ut::expect(std::filesystem::exists(data));
                ut::expect(not std::filesystem::exists(result));

                writer.finalize();

                ut::expect(std::filesystem::exists(result));
                ut::expect(not std::filesystem::exists(data));
                ut::expect(not std::filesystem::exists(schema));

                auto const final{ utility::csv::Table{}.read(result) };
                ut::expect(ut::eq(final.size(), std::size_t{ 5 }));
                ut::expect(final.rows_match_header());
                ut::expect(final[0] == utility::csv::Table::Row{
                    "key:step", "time/s", "Ag", "Au", "Cu" });
                auto const missing{ padding == utility::csv::Padding::zero ? "0" : "" };
                ut::expect(final[1] == utility::csv::Table::Row{
                    "0", "0", missing, missing, missing });
                ut::expect(final[2] == utility::csv::Table::Row{
                    "1", "1", "0.8", missing, missing });
                ut::expect(final[3] == utility::csv::Table::Row{
                    "2", "2", "0.7", missing, missing });
                ut::expect(final[4] == utility::csv::Table::Row{
                    "3", "3", "0.6", "0.3", "0.1" });
            }
        };

        "zero-row output retains a complete header"_test = [] {
            TempDirectory temp;
            auto const base{ temp.root / "empty" };
            utility::csv::Writer writer;
            writer.initialize(base,
                [] { return std::string{ "key:step,time/s" }; },
                [] { return std::string{ "0,0" }; });
            writer.finalize();

            auto const final{ utility::csv::Table{}.read(base) };
            ut::expect(ut::eq(final.size(), std::size_t{ 1 }));
            ut::expect(final[0] == utility::csv::Table::Row{ "key:step", "time/s" });
        };

        "column counts may not decrease and finalized writers cannot append"_test = [] {
            TempDirectory temp;
            utility::csv::Writer writer;
            writer.initialize(temp.root / "counts",
                [] { return std::string{ "key:step,time/s,A,B" }; },
                [] { return std::string{ "0,0,1,2" }; });
            writer.append(2);
            ut::expect(ut::throws<std::logic_error>([&] { writer.append(1); }));
            writer.finalize();
            ut::expect(ut::throws<std::logic_error>([&] { writer.append(2); }));
        };
    };
}
