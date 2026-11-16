#pragma once
#include "composition_dependent.h"
#include "linear_bond_breaking.h"
#include "output/descriptor_format.h"

#include <format>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>

namespace studies::dealloying::models::formatting
{
    inline constexpr std::string_view csv_header{
        "key:event,key:element,key:coordination,key:details"
    };

    template <class Descriptor>
    std::string base_label(Descriptor const& descriptor)
    {
        return std::format("{}_{}_{}", enum_name(descriptor.event),
            descriptor.element, static_cast<unsigned>(descriptor.coordination));
    }

    template <class Descriptor>
    std::string base_row(Descriptor const& descriptor, std::string_view details)
    {
        return std::format("{},{},{},{}", enum_name(descriptor.event),
            descriptor.element, static_cast<unsigned>(descriptor.coordination), details);
    }
}

namespace output
{
    template <>
    struct DescriptorFormat<studies::dealloying::models::LinearBondBreaking::Descriptor>
    {
        using Descriptor = studies::dealloying::models::LinearBondBreaking::Descriptor;

        static constexpr std::string_view header() { return studies::dealloying::models::formatting::csv_header; }

        static std::string label(Descriptor const& descriptor)
        {
            return studies::dealloying::models::formatting::base_label(descriptor);
        }

        static std::string row(Descriptor const& descriptor)
        {
            return studies::dealloying::models::formatting::base_row(descriptor, {});
        }
    };

    template <>
    struct DescriptorFormat<studies::dealloying::models::CompositionDependent::Descriptor>
    {
        using Descriptor = studies::dealloying::models::CompositionDependent::Descriptor;

        static constexpr std::string_view header() { return studies::dealloying::models::formatting::csv_header; }

        static std::string details(Descriptor const& descriptor)
        {
            std::string details;
            for (auto&& [element, count] : std::views::zip(
                lattice::active_elements, descriptor.element_neighbors))
                if (count)
                    std::format_to(std::back_inserter(details), "{}x{};",
                        static_cast<unsigned>(count), element);
            return details;
        }

        static std::string label(Descriptor const& descriptor)
        {
            auto text{ studies::dealloying::models::formatting::base_label(descriptor) };
            if (auto const suffix{ details(descriptor) }; not suffix.empty())
                text += "_" + suffix;
            return text;
        }

        static std::string row(Descriptor const& descriptor)
        {
            return studies::dealloying::models::formatting::base_row(descriptor, details(descriptor));
        }
    };
}
