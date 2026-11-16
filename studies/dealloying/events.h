#pragma once
#include "batch/campaign.h"
#include "kmc/event.h"
#include "kmc/setup.h"
#include "lattice/site.h"

namespace studies::dealloying::events
{
    struct Dissolution
    {
        static constexpr auto event = kmc::event::Type::dissolution;

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

        bool is_source_candidate(lattice::Site const& source) const
        {
            return source.element() == element and source.coordination() <= max_coordination;
        }

        bool is_candidate(kmc::event::Location const& location) const
        {
            return location.sites().size() == 1 and is_source_candidate(location.source());
        }

        // No postselection rejection in the dissolution model.
        bool is_rejected(kmc::event::Location const&) const { return false; }

        void execute(kmc::event::Location const& location, lattice::Lattice& lattice) const
        {
            assert(location.sites().size() == 1);
            location.source().set_element(lattice, kmc::Element::none);
        }

    };

    struct Diffusion
    {
        static constexpr auto event = kmc::event::Type::diffusion;

        lattice::Coordination max_source_coordination{ 9 }, min_target_coordination{ 2 };

        bool is_source_candidate(lattice::Site const& source) const
        {
            return source.is_occupied() and source.coordination() <= max_source_coordination;
        }

        bool is_candidate(kmc::event::Location const& location) const
        {
            return location.sites().size() == 2 and is_source_candidate(location.source()) and
                   location.sites()[1]->is_vacant();
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
            location.source().set_element(lattice, kmc::Element::none);
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

namespace studies::dealloying
{
    using Rules = kmc::event::Rules<events::Dissolution, events::Diffusion>;
    using Setup = kmc::Setup<Rules>;
    using Campaign = batch::Campaign<Rules>;
}
