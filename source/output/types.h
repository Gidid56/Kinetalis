#pragma once

#include <cstddef>
#include <map>

namespace control
{
    class StepLimit;
    class DurationLimit;
}

namespace output
{
	class Writer;

	class Config;
	class Time;
	class Lattice;
	class Performance;

	class Composition;
	class Phases;
    class Surface;
    class EventFlux;

    // Study-owned specializations describe kinetic descriptors for CSV and labels.
    // The KMC model and simulation do not depend on this presentation contract.
    template <class Descriptor>
    struct DescriptorFormat;
}
