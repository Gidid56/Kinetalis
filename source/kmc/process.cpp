#include "pch.h"
#include "process.h"

using namespace kmc;

process::Record::Record(event::Location location, event::Type event, bool rejected)
    : type{ event },
      outcome{ rejected ? process::Outcome::rejected : process::Outcome::accepted },
      location_{ std::move(location) }
{ }
