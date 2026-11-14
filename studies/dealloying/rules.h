#pragma once
#include "batch/campaign.h"
#include "kmc/process.h"
#include "kmc/setup.h"
#include "lattice/path.h"
#include "lattice/site.h"

namespace studies::dealloying::rules
{
    struct Dissolution
    {
        static constexpr auto event = kmc::Event::dissolution;

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

        bool active(lattice::Site const& site) const
        {
            return site.element() == element and not site.is_bulk();
        }

        bool allowed(lattice::Site const& site) const { return active(site); }

        bool supports(kmc::Process const& process) const
        {
            return process.event() == event and process.element() == element;
        }
    };

    struct Diffusion
    {
        static constexpr auto event = kmc::Event::diffusion;

        lattice::Coordination max_source_coordination{ 9 }, min_target_coordination{ 2 };

        bool source_active(lattice::Site const& site) const
        {
            return site.is_occupied() and site.coordination() <= max_source_coordination;
        }

        bool active(lattice::Path const& path) const
        {
            return source_active(path.front()) and path.back().is_vacant();
        }

        bool allowed(lattice::Path const& path) const
        {
            return active(path) and (path.back().coordination() > min_target_coordination or
                                        path.front().coordination() == 0);
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
    using Rules = kmc::EventRules<rules::Dissolution, rules::Diffusion>;
    using Setup = kmc::Setup<Rules>;
    using Campaign = batch::Campaign<Rules>;
}
