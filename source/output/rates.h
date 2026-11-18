#pragma once
#include "writer.h"
#include "types.h"
#include "kmc/simulation.h"
#include "utility/file.h"

#include <concepts>

namespace output
{
    template <class ModelT>
    class Rates : public Writer
    {
    public:
        using Writer::Writer;
        using Descriptor = typename ModelT::KineticsType::Descriptor;

    private:
        std::string_view name() const override { return "rates"; }
        bool uses_snapshot_resolution() const override { return false; }

        void write_final(kmc::Simulation const& simulation) override
        {
            auto const& kinetics{
                dynamic_cast<kmc::RejectionTreeSimulation<ModelT> const&>(simulation)
                    .model().kinetics()
            };

            std::string csv{ std::format("key:class,{},rate/Hz\n",
                DescriptorFormat<Descriptor>::header()) };
            for (std::size_t i{ 1 }; i < simulation.kinetic_classes().size(); ++i)
            {
                auto const key{ static_cast<kmc::RateClassKey>(i) };
                if (not simulation.kinetic_classes()[i].is_initialized())
                    continue;

                std::format_to(std::back_inserter(csv), "{},{},{:v:Hz}\n",
                    i, DescriptorFormat<Descriptor>::row(kinetics.descriptor(key)),
                    simulation.kinetic_classes()[i].rate);
            }
            utility::file::write<utility::file::Type::csv>(writer_path(), csv);
        }
    };
}
