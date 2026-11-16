#pragma once
#include "batch/campaign.h"
#include "kmc/event.h"
#include "kmc/process.h"
#include "kmc/setup.h"
#include "lattice/path.h"
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

        bool is_source_candidate(lattice::Site const& site) const
        {
            return site.element() == element and site.coordination() <= max_coordination;
        }

        bool is_candidate(lattice::Site const& site) const
        {
            return is_source_candidate(site);
        }

        // No postselection rejection in the dissolution model.
        bool is_rejected(lattice::Site const&) const { return false; }

        void execute(lattice::Site& site, lattice::Lattice& lattice) const
        {
            site.set_element(lattice, kmc::Element::none);
        }

        bool supports(kmc::Process const& process) const
        {
            return process.event() == event and process.element() == element;
        }
    };

    struct Diffusion
    {
        static constexpr auto event = kmc::event::Type::diffusion;

        lattice::Coordination max_source_coordination{ 9 }, min_target_coordination{ 2 };

        bool is_source_candidate(lattice::Site const& site) const
        {
            return site.is_occupied() and site.coordination() <= max_source_coordination;
        }

        bool is_candidate(lattice::Path const& path) const
        {
            return is_source_candidate(path.front()) and path.back().is_vacant();
        }

        // A selected candidate can still be rejected by the target constraint.
        bool is_rejected(lattice::Path const& path) const
        {
            return path.back().coordination() <= min_target_coordination and
                   path.front().coordination() != 0;
        }

        void execute(lattice::Path& path, lattice::Lattice& lattice) const
        {
            const auto element{ path.front().element() };
            path.back().set_element(lattice, element);
            path.front().set_element(lattice, kmc::Element::none);
        }

        bool supports(kmc::Process const& process) const
        {
            return process.event() == event and process.element() != kmc::Element::none;
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
    using Rules = kmc::EventRules<events::Dissolution, events::Diffusion>;
    using Setup = kmc::Setup<Rules>;
    using Campaign = batch::Campaign<Rules>;
}
