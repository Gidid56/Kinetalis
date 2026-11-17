#pragma once
#include "transformations.h"
#include "kinetics.h"

#include <utility>

namespace kmc
{
    // A model contains the kinetic model and its statically typed transformations.
    template <class KineticsT, class TransformationsT>
    class Model
    {
    public:
        using KineticsType = KineticsT;
        using TransformationsType = TransformationsT;

        static constexpr std::size_t slots_per_site{ TransformationsT::slot_count };

        explicit Model(KineticsT kinetics = KineticsT{}, TransformationsT transformations = TransformationsT{})
            : kinetics_{ std::move(kinetics) },
              transformations_{ std::move(transformations) }
        { }

        auto& kinetics(this auto& self) noexcept { return self.kinetics_; }
        auto& transformations(this auto& self) noexcept { return self.transformations_; }

    private:
        KineticsT kinetics_;
        TransformationsT transformations_;
    };
}
