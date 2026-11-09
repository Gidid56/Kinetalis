#include "pch.h"

#include "model.h"

#include "physics/equations/kinetics.h"

using namespace kmc;

Frequency Model::rate(Process const& process) const
{
    if (not process.is_allowed())
        return {};

    auto tst_rate = physics::equations::kinetics::tst_rate(
        transmission_coefficient(process),
        std::max<Energy>(0_eV, activation_gibbs_energy(process)),
        temperature_);

    if (modulation_ and process == modulation_->process)
        return tst_rate * modulation_->ratio;

    return tst_rate;
}
