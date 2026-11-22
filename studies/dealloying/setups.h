#pragma once
#include "kmc/setup.h"
#include "lattice/lattice.h"
#include "dealloying/kinetics/linear_bond_breaking.h"
#include "parameters.h"
#include "model.h"
#include "simulation/engine.h"

#include <stdexcept>

namespace studies::dealloying::setups
{
    using namespace lattice;

    template <Bravais bravais>
    Lattice cube(chemistry::Element element, Length side = {}, Indices surface_orientation = Indices::unit_z())
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
    bulk_with_vacancy(chemistry::Composition::Quantity x0 = 10, Length side = 30_nm)
    {
        Lattice lattice{ cube<Bravais::cF>(chemistry::Element::Au, side) };
        lattice.set_fill_function([x0](Lattice& lattice, simulation::Engine& rng) {
            using enum chemistry::Element;
            chemistry::Composition composition{ { Ag, 100 - x0 }, { Au, x0 } };
            lattice.set_label("bulk with vacancy");
            lattice.fill(rng, composition);
            lattice.center_site().set_element(lattice, none);
        });
        return lattice;
    }

    // A freestanding Ag-Au film supported by a pure Au base layer, with an
    // unoccupied region above it. Adapted from the archived slab construction.
    //
    // Only x and y are periodic. The nominal box dimensions are rounded up
    // to whole supercell periods by Lattice::scale_shape(), so the layer
    // positions are calculated from the actual resulting box height.
    static inline Lattice thin_film(
        chemistry::Composition::Quantity x0 = 10,
        Length side = 60_nm,
        Length box_height = 90_nm,
        Length substrate_thickness = 7.5_nm,
        Indices surface_orientation = Indices::unit_z())
    {
        if (x0 > 100)
            throw std::invalid_argument{ "Thin-film Au content must be between 0 and 100." };
        if (side <= 0_nm or box_height <= 0_nm or substrate_thickness <= 0_nm or
            2 * substrate_thickness >= box_height)
            throw std::invalid_argument{
                "Thin-film dimensions must be positive, with two surface layers "
                "fitting inside the box height." };

        using enum chemistry::Element;

        UnitCell unit_cell{
            UnitCell::conventional<Bravais::cF>(chemistry::Atom{ Au })
        };
        Lattice lattice{ unit_cell, surface_orientation };
        lattice.scale_shape(Parameters{ side, side, box_height });
        lattice.set_periodic_boundaries(math::Axis::x | math::Axis::y);
        lattice.construct_sites();
        lattice.construct_neighbor_list();

        lattice.set_fill_function([x0, substrate_thickness](
            Lattice& lattice, simulation::Engine& rng) {
            chemistry::Composition alloy{ { Ag, 100 - x0 }, { Au, x0 } };
            lattice.set_label("Ag-Au thin film on Au substrate");
            lattice.fill(rng, alloy);

            // At +/-half_height each sheet extends one substrate thickness
            // into the simulation box, reproducing the archived layout.
            const auto half_height{ lattice.shape().parameters().z() / 2 };
            lattice.fill(none, shape::sheet(2 * substrate_thickness).move_by(math::Axis::z, half_height));
            lattice.fill(Au, shape::sheet(2 * substrate_thickness).move_by(math::Axis::z, -half_height));
        });
        return lattice;
    }

    static inline Lattice
    cut_sphere(Length box_side, physics::quantity::Radius radius, bool inverted)
    {
        Lattice lattice{ cube<Bravais::cF>(chemistry::Element::Au, box_side) };
        lattice.set_fill_function([radius, inverted](Lattice& lattice, simulation::Engine& rng) {
            using enum chemistry::Element;
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
        Lattice lattice{ cube<Bravais::cF>(chemistry::Element::Au, 1.5_nm) };
        lattice.set_fill_function([=](Lattice& lattice, simulation::Engine& rng) {
            lattice.set_label("passivation schematic");
            using enum chemistry::Element;

            auto layer{ lattice.shape() };
            layer.scale(math::Vector<3>{ 1.0, 1.0, 1.0 / total_layers });

            const auto full_z{ lattice.shape().vectors().z() };
            const auto layer_z{ layer.vectors().z() };
            layer.move_by((layer_z - full_z) / 2);

            const auto layer_sites{ lattice.sites().size() / total_layers };
            const auto noble_sites{ layer_sites / noble_factor };

            chemistry::Composition alloy{ { Au, noble_sites }, { Ag, layer_sites - noble_sites } };
            chemistry::Composition dealloyed{ { Au, dissolved_layers * noble_sites },
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
        chemistry::Composition::Quantity x0 = 10,
        ElectricPotential overpotential = 0_V,
        kmc::Temperature temperature = 300_K,
        std::optional<kinetics::LinearBondBreaking::Modulation> modulation = {})
    {
        Setup setup;

        setup.make_lattice = [x0, campaign_type] {
            return bulk_with_vacancy(x0, box_size(x0, campaign_type));
        };
        auto potential{ critical_potential(x0, temperature) + overpotential };

        auto& kinetics{ setup.model.kinetics() };
        kinetics.set_potential(potential).set_temperature(temperature);
        //if (modulation)
        //    kinetics.set_modulation(*modulation);
        setup.name = std::format("T = {:K};x_Au = {};E = {:V}", temperature, x0, potential);
        if (modulation)
            setup.name += std::format("{}_{}_{} = {}", enum_name(modulation->descriptor.event),
            modulation->descriptor.element, static_cast<unsigned>(modulation->descriptor.coordination),
            modulation->ratio);

        return setup;
    }

    static inline Setup thin_film_dealloying(
        chemistry::Composition::Quantity x0 = 10,
        ElectricPotential overpotential = 0_V,
        kmc::Temperature temperature = 300_K,
        Length side = 60_nm,
        Length box_height = 90_nm,
        Length substrate_thickness = 7.5_nm,
        Indices surface_orientation = Indices::unit_z())
    {
        Setup setup;
        setup.make_lattice = [=] {
            return thin_film(x0, side, box_height, substrate_thickness, surface_orientation);
        };

        // Same potential convention as pore_dealloying. The reference critical
        // potentials are calibrated for bulk pores, not for thin-film geometry.
        const auto potential{ critical_potential(x0, temperature) + overpotential };
        setup.model.kinetics().set_potential(potential).set_temperature(temperature);
        setup.name = std::format(
            "thin film; x_Au = {}; side = {:nm}; box height = {:nm}; "
            "Au substrate = {:nm}; orientation = ({},{},{}); T = {:K}; E = {:V}",
            x0, side, box_height, substrate_thickness,
            surface_orientation.x(), surface_orientation.y(), surface_orientation.z(),
            temperature, potential);
        return setup;
    }

    static inline Setup geometry_analysis(physics::quantity::Radius radius, bool inverted)
    {
        Setup setup;

        setup.make_lattice = [radius, inverted] {
            return cut_sphere(2 * radius + 1_nm, radius, inverted);
        };
        setup.name = std::format("nano{};r = {:nm}", inverted ? "void" : "particle", radius);

        return setup;
    }

    static inline Setup schematic(int layer)
    {
        Setup setup;

        setup.make_lattice = [layer] {
            return passivation_schematic(layer);
        };
        setup.name = std::format("schematic");
        setup.seed = simulation::Engine::parse_seed(seeds[0]);

        return setup;
    }
}
