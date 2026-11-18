#pragma once
#include "process.h"
#include "kinetics.h"

#include <utility>

namespace kmc
{
    // A model contains the kinetic model and its statically typed processes.
    template <class KineticsT, class ProcessesT>
    class Model
    {
    public:
        using KineticsType = KineticsT;
        using ProcessesType = ProcessesT;

        static constexpr std::size_t slots_per_site{ ProcessesT::slot_count };

        explicit Model(KineticsT kinetics = KineticsT{}, ProcessesT processes = ProcessesT{})
            : kinetics_{ std::move(kinetics) },
              processes_{ std::move(processes) }
        { }

        auto& kinetics(this auto& self) noexcept { return self.kinetics_; }
        auto& processes(this auto& self) noexcept { return self.processes_; }

    private:
        KineticsT kinetics_;
        ProcessesT processes_;
    };
}
