#pragma once
#include "chemistry/element.h"
#include "physics/physics.h"

#include <filesystem>
#include <functional>

namespace kmc
{
    enum class Event : std::uint8_t
    {
        none,
        diffusion,
        dissolution,
        deposition,
    };

    using chemistry::Element;

    namespace process
    {
        enum class Outcome : std::uint8_t
        {
            active,
            accepted,
            rejected,
        };
    }

    inline constexpr bool staticProcessKeys{ true }, stochasticResidenceTime{ true };
}

namespace kmc
{
    using Step = std::uint64_t;
    using Steps = std::vector<Step>;

    using namespace physics::unit::literals;

    using Temperature = physics::quantity::AbsoluteTemperature;

    using physics::quantity::Frequency;
    using physics::quantity::Time;

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

    template <class Rules>
    struct Setup;

    template <class Rules>
    class RejectionTreeSimulation;

    namespace model { using Generator = std::function<std::unique_ptr<Model>()>; }

    class Observer;

    class AnalysisPlan;

    using PlanFactory = std::function<kmc::AnalysisPlan(std::filesystem::path const&)>;
}
