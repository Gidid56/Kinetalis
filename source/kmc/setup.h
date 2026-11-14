#pragma once
#include "lattice/types.h"
#include "simulation/types.h"
#include "kmc/types.h"

template <class Rules>
struct kmc::Setup
{
    std::string name{};
    lattice::Generator make_lattice;
    model::Generator make_model;
    [[no_unique_address]] Rules rules{};
    std::optional<simulation::Seed> seed;
};
