#pragma once
#include "writer.h"
#include "lattice/types.h"

#include <map>

class output::Composition : public output::Writer
{
public:
    static constexpr auto active_elements{ lattice::active_elements };

    using Writer::Writer;

private:
    std::string_view name() const override { return "composition"; }
    std::string settings() const override;

    void capture_init(kmc::Simulation const&) override;
    Subscription subscriptions() const override
    {
        return Writer::subscriptions() | Subscription::selected;
    }
    void observe_selected(kmc::event::Record const&) override;
    void write_init(kmc::Simulation const&) override;
    void write_snapshot(kmc::Simulation const&) override;

    std::map<chemistry::Element, std::size_t> element_counts_;
};
