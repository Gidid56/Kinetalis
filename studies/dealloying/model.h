#pragma once
#include "process.h"
#include "kinetics/linear_bond_breaking.h"
#include "kinetics/composition_dependent.h"
#include "kinetics/power_law_bond_energy.h"
#include "kinetics/gtc.h"
#include "kmc/model.h"
#include "kmc/setup.h"
#include "batch/campaign.h"

namespace studies::dealloying
{
    using Processes = kmc::Processes<process::Dissolution, process::Diffusion>;
    using Model = kmc::Model<kinetics::LinearBondBreaking, Processes>;
    using Setup = kmc::Setup<Model>;
    using Campaign = batch::Campaign<Model>;
}
