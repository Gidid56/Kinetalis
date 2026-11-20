#pragma once
#include "writer.h"
#include "utility/csv.h"

#include <map>
#include <vector>

class output::Composition : public output::Writer
{
public:
    using Writer::Writer;

private:
    std::string_view name() const override { return "composition"; }

    void capture_init(kmc::Simulation const&) override;
    Subscription subscriptions() const override
    {
        return Writer::subscriptions() | Subscription::selected;
    }
    void observe_selected(kmc::event::Record const&) override;
    void write_init(kmc::Simulation const&) override;
    void write_snapshot(kmc::Simulation const&) override { csv_.append(columns_.size()); }
    void write_final(kmc::Simulation const&) override { csv_.finalize(); }

    std::map<chemistry::Element, std::size_t> element_counts_;
    std::vector<chemistry::Element> columns_;
    utility::csv::Writer csv_{ utility::csv::Padding::zero };
};
