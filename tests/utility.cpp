#include "utility.h"

#include <boost/ut.hpp>

using boost::ut::suite;
using namespace boost::ut::literals;

namespace utility
{
    suite<"utility"> utility_tests = [] {
        "rng"_test = [] {
        };
    };
}
