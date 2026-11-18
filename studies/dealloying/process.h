#pragma once
#include "kmc/event.h"
#include "lattice/lattice.h"

#include <format>
#include <optional>

namespace studies::dealloying::process
{
    struct Dissolution
    {
        static constexpr auto event = kmc::event::Type::dissolution;
        static constexpr std::size_t slot_count{ 1 };

        chemistry::Element element{ chemistry::Element::Ag };
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
            location.source().set_element(lattice, chemistry::Element::none);
        }

    };

    struct Diffusion
    {
        static constexpr auto event = kmc::event::Type::diffusion;
        static constexpr std::size_t slot_count{ lattice::max_coordination };

        lattice::Coordination max_source_coordination{ 9 }, min_target_coordination{ 2 };

        std::optional<kmc::event::Location> location(
            lattice::Site& source, std::size_t slot, lattice::Lattice& lattice) const
        {
            if (slot >= source.max_neighbors()) return std::nullopt;
            return kmc::event::Location{ lattice::Cluster<2>{
                &source,
                lattice.neighbor(source.neighbor_index() + static_cast<lattice::Index>(slot))
            } };
        }

        bool is_source_candidate(lattice::Site const& source) const
        {
            return source.is_occupied() and source.coordination() <= max_source_coordination;
        }

        bool is_candidate_after_source_check(kmc::event::Location const& location) const
        {
            return location.sites().size() == 2 and location.sites()[1]->is_vacant();
        }

        bool is_candidate(kmc::event::Location const& location) const
        {
            return is_source_candidate(location.source()) and
                   is_candidate_after_source_check(location);
        }

        // A selected candidate can still be rejected by the target constraint.
        bool is_rejected(kmc::event::Location const& location) const
        {
            assert(location.sites().size() == 2);
            return location.sites()[1]->coordination() <= min_target_coordination and
                   location.source().coordination() != 0;
        }

        void execute(kmc::event::Location const& location, lattice::Lattice& lattice) const
        {
            assert(location.sites().size() == 2);
            const auto element{ location.source().element() };
            location.sites()[1]->set_element(lattice, element);
            location.source().set_element(lattice, chemistry::Element::none);
        }


        std::string settings() const
        {
            return std::format(
                "max_source_coordination = {}\n"
                "min_target_coordination = {}",
                static_cast<unsigned>(max_source_coordination),
                static_cast<unsigned>(min_target_coordination));
        }
    };
}
