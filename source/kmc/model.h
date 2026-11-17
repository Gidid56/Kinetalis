#pragma once
#include "event.h"
#include "kinetics.h"

#include <utility>

namespace kmc
{
    // A model contains the kinetic model and its statically typed transformations.
    template <class KineticModel, class TransformationSet>
    class Model
    {
    public:
        using KineticsType = KineticModel;
        using TransformationsType = TransformationSet;

        static constexpr std::size_t slots_per_site{ TransformationSet::slot_count };

        explicit Model(KineticModel kinetics = KineticModel{}, TransformationSet transformations = TransformationSet{})
            : kinetics_{ std::move(kinetics) },
              transformations_{ std::move(transformations) }
        { }

        auto& kinetics(this auto& self) noexcept { return self.kinetics_; }
        auto& transformations(this auto& self) noexcept { return self.transformations_; }

    private:
        KineticModel kinetics_;
        TransformationSet transformations_;
    };
}
