#pragma once
#include "config.h"

#include <array>
#include <charconv>
#include <limits>
#include <string>
#include <string_view>
#include <utility>

namespace physics
{
    namespace exponent
    {
        constexpr std::string to_string(Exponent exponent)
        {
            std::array<char, std::numeric_limits<Exponent>::digits10 + 2> buffer;

            const auto [end, error]{
                std::to_chars(buffer.data(), buffer.data() + buffer.size(), exponent)
            };

            if (error != std::errc{})
                std::unreachable();

            return { buffer.data(), end };
        }

        namespace detail
        {
            constexpr std::string_view digit_to_superscript(char digit)
            {
                switch (digit)
                {
                    case '-': return "\u207B"; // superscript minus
                    case '0': return "\u2070"; // superscript zero
                    case '1': return "\u00B9"; // superscript one
                    case '2': return "\u00B2"; // superscript two
                    case '3': return "\u00B3"; // superscript three
                    case '4': return "\u2074"; // superscript four
                    case '5': return "\u2075"; // superscript five
                    case '6': return "\u2076"; // superscript six
                    case '7': return "\u2077"; // superscript seven
                    case '8': return "\u2078"; // superscript eight
                    case '9': return "\u2079"; // superscript nine
                    default: std::unreachable();
                }
            }
        }

        constexpr std::string to_superscript(Exponent exponent)
        {
            std::string result;

            for (auto digit : to_string(exponent))
                result += detail::digit_to_superscript(digit);

            return result;
        }
    }
}
