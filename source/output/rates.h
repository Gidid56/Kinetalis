#pragma once
#include "writer.h"
#include "types.h"
#include "kmc/simulation.h"
#include "utility/file.h"

#include <concepts>
#include <stdexcept>

namespace output
{
    template <class ModelType>
    class Rates : public Writer
    {
    public:
        using Writer::Writer;
        using Descriptor = typename ModelType::KineticsType::Descriptor;

    private:
        std::string_view name() const override { return "rates"; }
        bool uses_snapshot_resolution() const override { return false; }

        void write_final(kmc::Simulation const& simulation) override
        {
            auto const* model{ &dynamic_cast<kmc::RejectionTreeSimulation<ModelType> const&>(simulation).model() };
            if (not model)
                throw std::logic_error{ "Rates requires the selected model type." };

            std::string csv{ std::format("key:class,{},rate/Hz\n",
                DescriptorFormat<Descriptor>::header()) };
            for (std::size_t i{ 1 }; i < simulation.class_rates().size(); ++i)
            {
                auto const key{ static_cast<kmc::RateClassKey>(i) };
                if (not simulation.class_initialized(key))
                    continue;

                std::format_to(std::back_inserter(csv), "{},{},{:v:Hz}\n",
                    i, DescriptorFormat<Descriptor>::row(model->kinetics().descriptor(key)),
                    simulation.class_rates()[i]);
            }
            utility::file::write<utility::file::Type::csv>(writer_path(), csv);
        }
    };
}
