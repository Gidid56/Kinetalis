#pragma once
#include "lattice/types.h"
#include "simulation/types.h"
#include "kmc/types.h"
#include "kmc/model.h"

template <class ModelType>
struct kmc::Setup
{
    std::string name{};
    lattice::Generator make_lattice;
    ModelType model{};
    std::optional<simulation::Seed> seed;
};
