#pragma once
#include "shape.h"
#include "site.h"
#include "unitcell.h"
/*
   Represents a lattice structure.

   Public Interface:
   - Constructors gather necessary building blocks (unit cell, optional Miller indices).
   - Methods for lattice construction: construct sites, paths, and layers.

   Private Attributes:
   - 'UnitCell unitCell_': Stores the unit cell, the fundamental repeating unit of the lattice.
   - 'Indices millerIndices_': Specifies the top surface orientation via Miller indices.
   - 'shape::Parallelepiped shape_': Defines the lattice's outer shape for periodic boundaries.

   - 'Axis periodicBoundaries_{}': Specifies periodic boundary configurations for the lattice.
   - 'Length maxPathLength_{}': Sets the maximum length for diffusion paths in the lattice.
   - 'Axis layerAxis_{ Axis::Z }': Identifies the axis for layer construction within the lattice.

   - 'Sites sites_': Contains lattice sites created during construction.
   - 'Paths paths_': Stores paths between lattice sites.
   - 'Layers layers_': Manages layers within the lattice for analysis and manipulation.

   Note:
   - 'lattice::Lattice' collects essential components (unit cell, Miller indices) during construction,
     while dedicated methods ('constructSites', 'constructNeighborList', 'constructLayers') handle actual lattice creation.
   - 'millerIndices_' define the top surface orientation, and 'shape_' sets the outer shape of the lattice,
     enabling periodic boundaries in all dimensions.
*/
class lattice::Lattice
{
public:
    explicit Lattice(UnitCell const&, Indices miller_indices = { 0, 0, 1 });
    Lattice(Lattice const&) = default;
    Lattice(Lattice&&) = default;

    void construct_sites();
    void construct_neighbor_list();
    void construct_layers(math::Axis);

private:
    void rotate_up();
    void sort_sites();
    void make_periodic();
    Layers::iterator get_layer(physics::quantity::Displacement);

public:
    void fill(Element);
    void fill(simulation::Engine&, Composition const&);
    void fill(Element, Shape const&, bool place_inside = true);
    void fill(simulation::Engine&, Composition const&, Shape const&, bool place_inside = true);
    void fill_exact(simulation::Engine&, Composition const&, Shape const&, bool place_inside = true);

    void set_fill_function(FillFunction&& function) { fill_function_ = std::move(function); }
    void apply_fill_function(simulation::Engine& rng)
    {
        if (fill_function_)
            fill_function_(*this, rng);
    }
    void set_label(std::string const& label) { label_ = label; }

    void gradient_fill(
        simulation::Engine&,
        Composition const& bottom,
        Displacement,
        Composition const& top,
        Displacement,
        std::function<double(double)> function = math::identity<double>,
        math::Axis axis = math::Axis::z);

    math::Vector<3, Displacement> displacement(Site const& from, Site const& to) const;

    auto& scale_shape(math::Vector<3> const& scale)
    {
        shape_.scale(scale);
        return *this;
    }
    auto& scale_shape(Parameters const& parameters)
    {
        shape_.scale(parameters);
        return *this;
    }
    auto& set_periodic_boundaries(math::Axis axis)
    {
        periodic_boundaries_ = axis;
        return *this;
    }
    auto& set_max_path_length(Length length)
    {
        max_path_length_ = length;
        return *this;
    }

    Index site_index(Site const& site) const { return static_cast<Index>(&site - sites_.data()); }

    Site* neighbor(Index index) { return &sites_.at(neighbors_.at(index)); }

    auto neighbors(this auto& self, Site const& site)
    {
        auto indices =
            std::span{ &self.neighbors_.at(site.neighbor_index()), site.max_neighbors() };

        return indices | std::views::transform(
                             [sites = self.sites_.data()](Index index) { return sites + index; });
    }
    Site& center_site();

    site::Type site_type(Site const&) const;

    std::size_t number_atoms() const;
    double solid_fraction() const;
    double fraction(Element) const;
    std::size_t broken_bonds() const;
    std::size_t surface_atoms(Coordination vacancies = 1) const;

    std::string to_xyz_file(bool const bulk, std::size_t const precision = 6) const;

    auto& unit_cell() const { return unit_cell_; }
    auto& miller_indices() const { return miller_indices_; }
    auto& shape() const { return shape_; }

    auto periodic_boundaries() const { return periodic_boundaries_; }
    auto max_path_length() const { return max_path_length_; }
    auto layer_axis() const { return layer_axis_; }

    auto& sites() { return sites_; }
    auto& sites() const { return sites_; }
    auto& neighbors() const { return neighbors_; }
    auto& layers() const { return layers_; }
    auto& fill_function() const { return fill_function_; }
    auto& master_alloy() const { return master_alloy_; }
    auto& label() const { return label_; }

private:
    UnitCell unit_cell_;
    Indices miller_indices_;
    shape::Parallelepiped shape_;

    Distance max_path_length_{};
    math::Axis periodic_boundaries_{};
    math::Axis layer_axis_{ math::Axis::z };

    Sites sites_;
    Neighbors neighbors_;
    Layers layers_;

    lattice::FillFunction fill_function_;
    Composition master_alloy_{};
    std::string label_{};
};

template <>
struct std::formatter<Lattice> : public std::formatter<std::string_view>
{
    std::format_context::iterator format(Lattice const&, std::format_context&) const;
};
