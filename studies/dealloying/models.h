#pragma once
#include "kmc/types.h"

namespace studies::dealloying::rules
{
    struct Dissolution
    {
        static constexpr auto event = kmc::Event::dissolution;

        bool allowed(lattice::Site const&) const;
        bool supports(kmc::Process const&) const;
    };

    struct Diffusion
    {
        static constexpr auto event = kmc::Event::diffusion;

        bool allowed(lattice::Path const&) const;
        bool supports(kmc::Process const&) const;
    };
}
