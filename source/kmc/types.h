#pragma once
#include "chemistry/element.h"
#include "enums.h"
#include "physics/physics.h"

#include <functional>
#include <filesystem>

namespace kmc
{
    using chemistry::Element;

    // Enable or disable events here if desired
    inline constexpr std::array enabledEvents{
        Event::diffusion,
        Event::dissolution,
        //Event::deposition,
    };

    inline constexpr bool staticProcessKeys{ true }, stochasticResidenceTime{ true };
}

namespace kmc
{
    using Step = std::uint64_t;
    using Steps = std::vector<Step>;

    using namespace physics::unit::literals;
    
    using Temperature = physics::quantity::AbsoluteTemperature;

    using physics::quantity::Time;
    using physics::quantity::Frequency;

    using Rate = Frequency;
    using Rates = std::vector<Rate>;
    

    namespace rate_tree { struct Interface; }

    namespace process
    {
        using Key = std::uint8_t;
        using Keys = std::vector<Key>;

        class Record;

        class Statistics;
    }

    class Model;
    class Process;
    class Simulation;
    struct Setup;

    namespace model { using Generator = std::function<std::unique_ptr<Model>()>; }

    class Observer;

    class AnalysisPlan;

    using PlanFactory = std::function<kmc::AnalysisPlan(std::filesystem::path const&)>;
}
