#pragma once
#include "lattice/path.h"
#include "types.h"

#include <concepts>
#include <cstdint>
#include <format>
#include <string>

namespace kmc::event
{
    enum class Type : std::uint8_t
    {
        none,
        diffusion,
        dissolution,
        deposition,
    };

    template <class T>
    concept Location = std::same_as<T, lattice::Site> or std::same_as<T, lattice::Path>;

    // The source site and the full event location are evaluated before
    // selection; rejection is evaluated only for a selected candidate.
    template <typename T, class Location_>
    concept Rule = Location<Location_> and requires(
                                               T const& rule,
                                               lattice::Site const& source,
                                               Location_ const& location,
                                               Location_& mutable_location,
                                               lattice::Lattice& lattice,
                                               kmc::Process const& process) {
        { T::event } -> std::convertible_to<kmc::event::Type>;
        { rule.is_source_candidate(source) } -> std::convertible_to<bool>;
        { rule.is_candidate(location) } -> std::convertible_to<bool>;
        { rule.is_rejected(location) } -> std::convertible_to<bool>;
        { rule.execute(mutable_location, lattice) } -> std::same_as<void>;
        { rule.supports(process) } -> std::convertible_to<bool>;
        { rule.settings() } -> std::convertible_to<std::string>;
    };

    template <class Site, class Path>
    requires Rule<Site, lattice::Site> && Rule<Path, lattice::Path>
    struct Rules
    {
        [[no_unique_address]] Site site;
        [[no_unique_address]] Path path;

        std::string settings() const
        {
            return std::format(
                "[site: {}]\n{}\n\n"
                "[path: {}]\n{}\n",
                enum_name(Site::event),
                site.settings(),
                enum_name(Path::event),
                path.settings());
        }
    };
}
