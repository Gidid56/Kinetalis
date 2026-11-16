#pragma once
#include "event.h"

#include <utility>

class kmc::process::Record
{
public:
    Record(event::Location, event::Type, bool rejected);

    event::Location const& location() const noexcept { return location_; }
    auto sites() const noexcept { return location_.sites(); }

    event::Type type;
    Outcome outcome;

private:
    event::Location location_;
};
