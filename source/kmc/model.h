#pragma once
#include "types.h"
#include "process.h"

#include <cstddef>

class kmc::Model
{
public:
    static constexpr Temperature standard_temperature{ 25_degC };

    struct Modulation
    {
        Process process;
        Ratio ratio;
    };

    virtual ~Model() = default;

    // Maximum nearest-neighbor graph distance from a modified event site
    // to a source whose outgoing event rates may need recomputation.
    // Radius one also covers the present rules' eligibility dependencies.
    virtual std::size_t invalidation_radius() const noexcept { return 1; }

    auto& set_temperature(this auto& self, Temperature temperature)
    {
        self.temperature_ = temperature;
        return self;
    }

    auto& set_modulation(this auto& self, Modulation const& modulation)
    {
        self.modulation_ = modulation;
        return self;
    }

    Frequency rate(Process const&) const;

    virtual std::string output_text() const;

    auto temperature() const { return temperature_; }

protected:
    virtual Ratio transmission_coefficient(Process const&) const = 0;
    virtual Energy activation_gibbs_energy(Process const&) const = 0;

private:
    Temperature temperature_{ standard_temperature };

    std::optional<Modulation> modulation_{};
};
