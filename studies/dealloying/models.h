#pragma once
#include "kmc/types.h"
#include "kmc/setup.h"
#include "kmc/process.h"
#include "batch/campaign.h"

namespace studies::dealloying::rules
{
    struct Dissolution
    {
        static constexpr auto event = kmc::Event::dissolution;

        bool active(lattice::Site const&) const;
        bool allowed(lattice::Site const&) const;
        bool supports(kmc::Process const&) const;
    };

    struct Diffusion
    {
        static constexpr auto event = kmc::Event::diffusion;

        bool active(lattice::Path const&) const;
        bool allowed(lattice::Path const&) const;
        bool supports(kmc::Process const&) const;
    };
}

namespace studies::dealloying
{
    using Rules = kmc::EventRules<
        rules::Dissolution,
        rules::Diffusion
    >;

    using Setup = kmc::Setup<Rules>;
    using Campaign = batch::Campaign<Rules>;
}
