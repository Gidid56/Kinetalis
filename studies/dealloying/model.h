#pragma once
#include "events/dissolution.h"
#include "events/diffusion.h"
#include "kinetics/linear_bond_breaking.h"
#include "kmc/model.h"
#include "kmc/setup.h"
#include "batch/campaign.h"

namespace studies::dealloying
{
    using Mechanisms = kmc::event::Mechanisms<events::Dissolution, events::Diffusion>;
    using Model = kmc::Model<kinetics::LinearBondBreaking, Mechanisms>;
    using Setup = kmc::Setup<Model>;
    using Campaign = batch::Campaign<Model>;
}
