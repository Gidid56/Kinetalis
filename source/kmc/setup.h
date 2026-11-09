#pragma once
#include "lattice/types.h"
#include "random/engine.h"

struct kmc::Setup
{
    using Seed = random_::Engine::seed_type;

    std::string name{};
    lattice::Generator make_lattice;
    model::Generator make_model;
    std::optional<Seed> seed;
};