#pragma once
#include "kmc/event.h"
#include "lattice/lattice.h"

#include <format>

namespace studies::dealloying::process
{
    struct Dissolution
    {
        static constexpr auto event = kmc::event::Type::dissolution;
        static constexpr std::size_t cluster_size{ 1 };
        static constexpr std::size_t slot_count{ 1 };

        chemistry::Element element{ chemistry::Element::Ag };
        lattice::Coordination max_coordination{ 11 };

        std::string settings() const
        {
            return std::format(
                "element = {}\n"
                "max_coordination = {}",
                element,
                max_coordination);
        }

        bool is_candidate(lattice::Site const& source) const
        {
            return source.element() == element and source.coordination() <= max_coordination;
        }

        void execute(kmc::event::Location const& location, lattice::Lattice& lattice) const
        {
            location.source().set_element(lattice, chemistry::Element::none);
        }
    };

    struct Diffusion
    {
        static constexpr auto event = kmc::event::Type::diffusion;
        static constexpr std::size_t cluster_size{ 2 };
        static constexpr std::size_t slot_count{ lattice::max_coordination };

        static constexpr bool allow_isolated_atom_diffusion{ true };
        lattice::Coordination max_source_coordination{ 9 }, min_target_coordination{ 2 };

        bool has_slot(lattice::Site const& source, std::size_t slot) const
        {
            return slot < source.max_neighbors();
        }

        kmc::event::Location
        location(lattice::Site& source, std::size_t slot, lattice::Lattice& lattice) const
        {
            return kmc::event::Location{ &source,
                lattice.neighbor(source.neighbor_index() + static_cast<lattice::Index>(slot)) };
        }

        bool is_candidate(lattice::Site const& source) const
        {
            return source.is_occupied() and source.coordination() <= max_source_coordination;
        }

        bool is_candidate(kmc::event::Location const& location) const
        {
            return location.sites()[1]->is_vacant();
        }

        // A selected candidate can still be rejected by additional constraints.
        bool is_rejected(kmc::event::Location const& location) const
        {
            if constexpr (allow_isolated_atom_diffusion)
                if (location.source().coordination() == 0)
                    return false;
            return location.sites()[1]->coordination() <= min_target_coordination;
        }

        void execute(kmc::event::Location const& location, lattice::Lattice& lattice) const
        {
            const auto element{ location.source().element() };
            location.sites()[1]->set_element(lattice, element);
            location.source().set_element(lattice, chemistry::Element::none);
        }

        std::string settings() const
        {
            return std::format(
                "allow_isolated_atom_diffusion = {}\n"
                "max_source_coordination = {}\n"
                "min_target_coordination = {}",
                allow_isolated_atom_diffusion,
                max_source_coordination,
                min_target_coordination);
        }
    };
}
