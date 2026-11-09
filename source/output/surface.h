#pragma once
#include "writer.h"

class output::Surface : public output::Writer
{
public:
    using Writer::Writer;

    void capture_init(kmc::Simulation const&) override;

    void observe_selected(kmc::process::Record const&) override;
    void observe_processed(kmc::process::Record const&) override;

    void write_init(kmc::Simulation const&) override;
    void write_snapshot(kmc::Simulation const&) override;

    auto sites() const { return sites_; }
    auto bonds() const { return bonds_; }

private:
    std::string_view name() const override { return "surface"; }

    void update(lattice::Site const& site, int change);

    lattice::Lattice const* lattice_{};

    std::size_t sites_{}, bonds_{};
};
