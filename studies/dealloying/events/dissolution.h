#pragma once
#include "kmc/event/location.h"
#include "kmc/event/types.h"
#include "lattice/lattice.h"

#include <format>
#include <optional>

namespace studies::dealloying::events
{
    struct Dissolution
    {
        static constexpr auto event = kmc::event::Type::dissolution;
        static constexpr std::size_t slot_count{ 1 };

        kmc::Element element{ kmc::Element::Ag };
        lattice::Coordination max_coordination{ 11 };

        std::string settings() const
        {
            return std::format(
                "element = {}\n"
                "max_coordination = {}",
                element,
                static_cast<unsigned>(max_coordination));
        }

        std::optional<kmc::event::Location> location(
            lattice::Site& source, std::size_t slot, lattice::Lattice&) const
        {
            if (slot != 0) return std::nullopt;
            return kmc::event::Location{ source };
        }

        bool is_source_candidate(lattice::Site const& source) const
        {
            return source.element() == element and source.coordination() <= max_coordination;
        }

        bool is_candidate_after_source_check(kmc::event::Location const& location) const
        {
            return location.sites().size() == 1;
        }

        bool is_candidate(kmc::event::Location const& location) const
        {
            return is_source_candidate(location.source()) and
                   is_candidate_after_source_check(location);
        }

        // No postselection rejection in the dissolution model.
        bool is_rejected(kmc::event::Location const&) const { return false; }

        void execute(kmc::event::Location const& location, lattice::Lattice& lattice) const
        {
            assert(location.sites().size() == 1);
            location.source().set_element(lattice, kmc::Element::none);
        }

    };
}
