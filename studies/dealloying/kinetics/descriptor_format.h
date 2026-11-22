#pragma once
#include "composition_dependent.h"
#include "gtc.h"
#include "gtc_path_cache.h"
#include "linear_bond_breaking.h"
#include "output/types.h"

#include <algorithm>
#include <format>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>

namespace studies::dealloying::kinetics::formatting
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
    struct DescriptorFormat<studies::dealloying::kinetics::GTC::Descriptor>
    {
        using Descriptor = studies::dealloying::kinetics::GTC::Descriptor;

        static constexpr std::string_view header()
        {
            return studies::dealloying::kinetics::formatting::csv_header;
        }

        static std::string details(Descriptor const& descriptor)
        {
            if (descriptor.event != kmc::event::Type::diffusion)
                return {};
            // The occupancy mask is part of the kinetic class. Without it,
            // different GTC classes produce indistinguishable CSV rows/labels.
            const double saddle{descriptor.transition_state_coordination
                ? std::min(*descriptor.transition_state_coordination,
                    static_cast<double>(studies::dealloying::kinetics::gtc::final_coordination(
                        descriptor.environment)))
                : studies::dealloying::kinetics::gtc::saddle_coordination(
                    descriptor.environment)};
            return std::format("mask=0x{:05X};nTS={:.6g}{}",
                descriptor.environment, saddle,
                descriptor.transition_state_coordination ? ";override" : "");
        }

        static std::string label(Descriptor const& descriptor)
        {
            auto result{ studies::dealloying::kinetics::formatting::base_label(descriptor) };
            if (auto const info{ details(descriptor) }; not info.empty())
                result += "_" + info;
            return result;
        }

        static std::string row(Descriptor const& descriptor)
        {
            return studies::dealloying::kinetics::formatting::base_row(
                descriptor, details(descriptor));
        }
    };

    template <>
    struct DescriptorFormat<studies::dealloying::kinetics::LinearBondBreaking::Descriptor>
    {
        using Descriptor = studies::dealloying::kinetics::LinearBondBreaking::Descriptor;

        static constexpr std::string_view header() { return studies::dealloying::kinetics::formatting::csv_header; }

        static std::string label(Descriptor const& descriptor)
        {
            return studies::dealloying::kinetics::formatting::base_label(descriptor);
        }

        static std::string row(Descriptor const& descriptor)
        {
            return studies::dealloying::kinetics::formatting::base_row(descriptor, {});
        }
    };

    template <>
    struct DescriptorFormat<studies::dealloying::kinetics::CompositionDependent::Descriptor>
    {
        using Descriptor = studies::dealloying::kinetics::CompositionDependent::Descriptor;

        static constexpr std::string_view header() { return studies::dealloying::kinetics::formatting::csv_header; }

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
            auto text{ studies::dealloying::kinetics::formatting::base_label(descriptor) };
            if (auto const suffix{ details(descriptor) }; not suffix.empty())
                text += "_" + suffix;
            return text;
        }

        static std::string row(Descriptor const& descriptor)
        {
            return studies::dealloying::kinetics::formatting::base_row(descriptor, details(descriptor));
        }
    };
}
