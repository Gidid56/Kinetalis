#pragma once
#include "kmc/setup.h"
#include "lattice/lattice.h"
#include "models/linear_bond_breaking.h"
#include "parameters.h"
#include "rules.h"
#include "simulation/engine.h"

namespace studies::dealloying::setups
{
    using namespace lattice;

    template <Bravais bravais>
    Lattice cube(Element element, Length side = {}, Indices surface_orientation = Indices::unit_z())
    {
        using enum math::Axis;
        UnitCell unit_cell{ UnitCell::conventional<bravais>(chemistry::Atom{ element }) };
        Parameters min_dimensions{ side, side, side };

        Lattice lattice{ unit_cell, surface_orientation };
        lattice.scale_shape(min_dimensions);
        lattice.set_periodic_boundaries(x | y | z);
        lattice.construct_sites();
        lattice.construct_neighbor_list();
        return lattice;
    }

    static inline Lattice
    bulk_with_vacancy(Composition::Quantity x0 = 10, Length side = utility::is_debug ? 5_nm : 30_nm)
    {
        Lattice lattice{ cube<Bravais::cF>(Element::Au, side) };
        lattice.set_fill_function([x0](Lattice& lattice, simulation::Engine& rng) {
            using enum Element;
            Composition composition{ { Ag, 100 - x0 }, { Au, x0 } };
            lattice.set_label("bulk with vacancy");
            lattice.fill(rng, composition);
            lattice.center_site().set_element(lattice, none);
        });
        return lattice;
    }

    static inline Lattice
    cut_sphere(Length box_side, physics::quantity::Radius radius, bool inverted)
    {
        Lattice lattice{ cube<Bravais::cF>(Element::Au, box_side) };
        lattice.set_fill_function([radius, inverted](Lattice& lattice, simulation::Engine& rng) {
            using enum Element;
            lattice.set_label("nanopore");
            lattice.fill(inverted ? Au : none);
            if (radius > 0_nm)
                lattice.fill(
                    inverted ? none : Au,
                    shape::sphere(radius).move_by(rng.position(lattice.unit_cell())));
        });
        return lattice;
    }

    static inline Lattice
    passivation_schematic(int dissolved_layers, int total_layers = 8, int noble_factor = 4)
    {
        Lattice lattice{ cube<Bravais::cF>(Element::Au, 1.5_nm) };
        lattice.set_fill_function([=](Lattice& lattice, simulation::Engine& rng) {
            lattice.set_label("passivation schematic");
            using enum Element;

            auto layer{ lattice.shape() };
            layer.scale(math::Vector<3>{ 1.0, 1.0, 1.0 / total_layers });

            const auto full_z{ lattice.shape().vectors().z() };
            const auto layer_z{ layer.vectors().z() };
            layer.move_by((layer_z - full_z) / 2);

            const auto layer_sites{ lattice.sites().size() / total_layers };
            const auto noble_sites{ layer_sites / noble_factor };

            Composition alloy{ { Au, noble_sites }, { Ag, layer_sites - noble_sites } };
            Composition dealloyed{ { Au, dissolved_layers * noble_sites },
                { none, layer_sites - dissolved_layers * noble_sites } };

            for (int i = 0; i < total_layers; ++i)
            {
                if (i < total_layers - dissolved_layers)
                {
                    lattice.fill_exact(rng, alloy, layer);
                }
                else if (i == total_layers - dissolved_layers)
                {
                    lattice.fill_exact(rng, dealloyed, layer);
                }
                else
                {
                    lattice.fill(none, layer);
                }

                layer.move_by(layer_z);
            }
        });
        return lattice;
    }

    static inline Setup pore_dealloying(
        CampaignType campaign_type,
        lattice::Composition::Quantity x0 = 10,
        ElectricPotential overpotential = 0_V,
        kmc::Temperature temperature = 300_K,
        std::optional<kmc::Model::Modulation> modulation = {})
    {
        Setup setup;

        setup.make_lattice = [x0, campaign_type] {
            return bulk_with_vacancy(x0, box_size(x0, campaign_type));
        };
        auto potential{ critical_potential(x0, temperature) + overpotential };

        setup.make_model = [potential, temperature, modulation] {
            auto model{ std::make_unique<kmc::models::LinearBondBreaking>() };
            (*model).set_potential(potential).set_temperature(temperature);
            if (modulation)
                (*model).set_modulation(*modulation);
            return model;
        };
        setup.name = std::format("T = {:K};x_Au = {};E = {:V}", temperature, x0, potential);
        if (modulation)
            setup.name += std::format("{:_} = {}", modulation->process, modulation->ratio);

        return setup;
    }

    static inline Setup geometry_analysis(physics::quantity::Radius radius, bool inverted)
    {
        Setup setup;

        setup.make_lattice = [radius, inverted] {
            return cut_sphere(2 * radius + 1_nm, radius, inverted);
        };
        setup.make_model = &std::make_unique<kmc::models::LinearBondBreaking>;
        setup.name = std::format("nano{};r = {:nm}", inverted ? "void" : "particle", radius);

        return setup;
    }

    static inline Setup schematic(int layer)
    {
        Setup setup;

        setup.make_lattice = [layer] {
            return passivation_schematic(layer);
        };
        setup.make_model = &std::make_unique<kmc::models::LinearBondBreaking>;
        setup.name = std::format("schematic");
        setup.seed = simulation::Engine::parse_seed(seeds[0]);

        return setup;
    }
}
