#pragma once
#include "chemistry/element.h"
#include "physics/physics.h"

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <vector>

namespace kmc
{
    using Step = std::uint64_t;
    using Steps = std::vector<Step>;

    using namespace physics::unit::literals;

    using Temperature = physics::quantity::AbsoluteTemperature;

    using physics::quantity::Time;

    using Rate = physics::quantity::Frequency;
    using Rates = std::vector<Rate>;

    namespace rate_tree { struct Interface; }

    using RateClassKey = std::uint32_t;
    using RateClassKeys = std::vector<RateClassKey>;

    namespace event { class Record; }

    class Kinetics;
    template <class KineticsT, class ProcessesT> class Model;
    class Simulation;

    using SlotId = std::size_t;

    template <class ModelT>
    struct Setup;

    template <class ModelT>
    class RejectionTreeSimulation;

    class Observer;

    class AnalysisPlan;

    using PlanFactory = std::function<kmc::AnalysisPlan(std::filesystem::path const&)>;
}
