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
	class Rates;
	class Performance;

	class Composition;
	class Phases;
    class Surface;

	namespace process
	{
		using Count = std::size_t;
		using CountMap = std::map<kmc::process::Key, Count>;

        class EventFlux;
		class Statistics;
        class Kinetics;
	}
}
