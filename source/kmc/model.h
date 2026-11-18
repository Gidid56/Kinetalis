#pragma once
#include "event/mechanisms.h"
#include "event/kinetics.h"

#include <utility>

namespace kmc
{
    // A model contains the kinetic model and its statically typed mechanisms.
    template <class KineticsT, class MechanismsT>
    class Model
    {
    public:
        using KineticsType = KineticsT;
        using MechanismsType = MechanismsT;

        static constexpr std::size_t slots_per_site{ MechanismsT::slot_count };

        explicit Model(KineticsT kinetics = KineticsT{}, MechanismsT mechanisms = MechanismsT{})
            : kinetics_{ std::move(kinetics) },
              mechanisms_{ std::move(mechanisms) }
        { }

        auto& kinetics(this auto& self) noexcept { return self.kinetics_; }
        auto& mechanisms(this auto& self) noexcept { return self.mechanisms_; }

    private:
        KineticsT kinetics_;
        MechanismsT mechanisms_;
    };
}
