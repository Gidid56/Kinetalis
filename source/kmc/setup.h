#pragma once
#include "lattice/types.h"
#include "random/engine.h"

template <class Rules>
struct kmc::Setup
{
    using Seed = random_::Engine::seed_type;

    std::string name{};
    lattice::Generator make_lattice;
    model::Generator make_model;
    [[no_unique_address]] Rules rules{};
    std::optional<Seed> seed;
};
