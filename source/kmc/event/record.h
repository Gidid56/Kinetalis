#pragma once
#include "location.h"
#include "types.h"

#include <utility>

namespace kmc::event
{
    class Record
    {
    public:
        Record(Location location, Type event, bool rejected, RateClassKey class_key) noexcept
            : type{ event },
              outcome{ rejected ? Outcome::rejected : Outcome::accepted },
              class_key{ class_key },
              location_{ std::move(location) }
        { }

        Location const& location() const noexcept { return location_; }
        auto sites() const noexcept { return location_.sites(); }

        Type type;
        Outcome outcome;
        RateClassKey class_key;

    private:
        Location location_;
    };
}
