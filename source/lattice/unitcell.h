#pragma once
#include "bravais.h"
#include "shape.h"
/*
   This header file defines a class 'UnitCell' within the 'lattice::unitcell' namespace,
   inheriting from 'shape::Parallelepiped'. The 'UnitCell' class represents a fundamental
   unit cell within a crystal lattice and includes various static functions to generate
   and manipulate different types of unit cells based on Bravais lattice types and crystal
   systems.

   - Constructors allow the creation of unit cells with specified Bravais types, lattice vectors,
	 and optional sites.
   - Static functions generate conventional and primitive unit cells for various Bravais lattice
	 types and crystal systems, using specific parameters and angles.
   - Additional static functions create unit cells for specific lattice types, such as hexagonal
	 close-packed (hcp).
   - Functions enable modification and manipulation of the unit cell, including insertion of sites,
	 filling with atoms, setting atoms at specific indices, and computing properties like Miller
	 planes, periodic rotated shapes, covering multiples for shapes, next neighbor distance, metrics,
	 packing fraction, and more.
   - Overridden functions provide iterators, access to the unit cell's shape, its Bravais type,
	 crystal system, and sites.

   This class encapsulates versatile functionality to handle crystal lattice structures, allowing for
   the creation, manipulation, and computation of various properties of a unit cell, catering to
   different lattice types and crystal systems.
*/
class lattice::unitcell::UnitCell : public shape::Parallelepiped
{
public:
    constexpr UnitCell(
        Bravais bravais,
        lattice::Basis const& vectors,
        unitcell::Sites const& sites = { { site::origin } })
        : shape::Parallelepiped{ vectors },
          bravais_{ bravais },
          sites_{ sites }
    { }

    template <Bravais bravais>
    requires(system(bravais) == System::triclinic)
    static constexpr UnitCell conventional(Parameters const& parameters, Angles const& angles)
    {
        return { bravais,
            vectors::conventional::triclinic(angles) * parameters,
            sites(centering(bravais)) };
    }

    template <Bravais bravais>
    requires(system(bravais) == System::monoclinic)
    static constexpr UnitCell conventional(Parameters const& parameters, Angle angle)
    {
        return { bravais,
            vectors::conventional::monoclinic(angle) * parameters,
            sites(centering(bravais)) };
    }

    template <Bravais bravais>
    requires(system(bravais) == System::monoclinic)
    static constexpr UnitCell primitive(Parameters const& parameters, Angle angle)
    {
        return { bravais,
            vectors::to::primitive(centering(bravais))
                    .dot(vectors::conventional::monoclinic(angle)) *
                parameters };
    }

    template <Bravais bravais>
    requires(system(bravais) == System::orthorhombic)
    static constexpr UnitCell conventional(Parameters const& parameters)
    {
        return { bravais, vectors::basis * parameters, sites(centering(bravais)) };
    }

    template <Bravais bravais>
    requires(system(bravais) == System::orthorhombic)
    static constexpr UnitCell primitive(Parameters const& parameters)
    {
        return { bravais,
            vectors::to::primitive(centering(bravais)).dot(vectors::basis * parameters) };
    }

    template <Bravais bravais>
    requires(system(bravais) == System::tetragonal)
    static constexpr UnitCell conventional(math::Vector<2, Parameter> const& p)
    {
        return { bravais,
            vectors::basis * Parameters{ p.x(), p.x(), p.y() },
            sites(centering(bravais)) };
    }

    template <Bravais bravais>
    requires(system(bravais) == System::tetragonal)
    static constexpr UnitCell primitive(math::Vector<2, Parameter> const& p)
    {
        return { bravais,
            vectors::to::primitive(centering(bravais))
                .dot(vectors::basis * Parameters{ p.x(), p.x(), p.y() }) };
    }

    template <Bravais bravais>
    requires(system(bravais) == System::rhombohedral)
    static constexpr UnitCell conventional(Parameter parameter, Angle angle)
    {
        return { bravais,
            vectors::conventional::rhombohedral(angle) * parameter,
            sites(centering(bravais)) };
    }

    template <Bravais bravais>
    requires(system(bravais) == System::hexagonal)
    static constexpr UnitCell conventional(math::Vector<2, Parameter> const& p)
    {
        return { bravais,
            vectors::conventional::hexagonal * Parameters{ p.x(), p.x(), p.y() },
            sites(centering(bravais)) };
    }

    template <Bravais bravais>
    requires(system(bravais) == System::cubic)
    static constexpr UnitCell conventional(Parameter parameter)
    {
        return { bravais, vectors::basis * parameter, sites(centering(bravais)) };
    }

    template <Bravais bravais>
    requires(system(bravais) == System::cubic)
    static constexpr UnitCell primitive(Parameter parameter)
    {
        return { bravais,
            vectors::to::primitive(centering(bravais)).dot(vectors::basis) * parameter };
    }

    static constexpr UnitCell hcp(Parameter parameter)
    {
        return { Bravais::hP,
            vectors::conventional::hexagonal * parameters::hcp * parameter,
            unitcell::Sites{ { site::origin }, { site::hcp } } };
    }

    template <Bravais bravais>
    requires(system(bravais) == System::cubic)
    static constexpr UnitCell conventional(chemistry::Atom atom)
    {
        return conventional<bravais>(
            parameters::smallest(centering(bravais)) * 2 * atom.display_radius())
            .fill(atom);
    }

    template <Bravais bravais>
    requires(system(bravais) == System::cubic)
    static constexpr UnitCell primitive(chemistry::Atom atom)
    {
        return primitive<bravais>(
            parameters::smallest(centering(bravais)) * 2 * atom.display_radius())
            .fill(atom);
    }

    static constexpr UnitCell hcp(chemistry::Atom atom)
    {
        return hcp(2 * atom.display_radius()).fill(atom);
    }

    static constexpr unitcell::Sites sites(Centering centering)
    {
        using namespace site;

        switch (centering)
        {
            case Centering::none: return { { origin } };

            case Centering::base: return { { origin }, { base } };

            case Centering::body: return { { origin }, { site::center } };

            case Centering::face: return { { origin }, { base }, { front }, { side } };

            default: return {};
        }
    }

    constexpr UnitCell& insert(unitcell::Site site)
    {
        sites_.emplace_back(site);
        return *this;
    }

    constexpr UnitCell& fill(chemistry::Atom atom)
    {
        for (auto& [site_, atom_] : sites_)
            atom_ = atom;
        return *this;
    }

    constexpr UnitCell& set(Index index, chemistry::Atom atom)
    {
        if (index < sites_.size())
            sites_[index].atom = atom;
        return *this;
    }

    constexpr ReciprocalVector miller_plane(Indices const& miller_indices) const
    {
        return reciprocal_basis_.transpose_dot(miller_indices);
    }

    shape::Parallelepiped oriented_periodic_supercell(
        Indices const& new_z,
        Indices const& old_z = Indices::unit_z()) const;

    // Calculates the minimum number of unit cell multiples
    // needed to fully cover a given shape.
    Coordinate multiples_to_cover(Shape const& shape) const;

    Distance next_neighbor_distance() const;

    constexpr auto metric() const { return vectors_.dot(vectors_.transposed()); }

    constexpr auto reciprocal_metric() const
    {
        return reciprocal_basis_.dot(reciprocal_basis_.transposed());
    }

    constexpr Factor packing_fraction() const
    {
        return utility::sum(
                   sites_ | std::views::transform(&Site::atom) |
                   std::views::transform(&chemistry::Atom::display_volume)) /
               this->volume();
    }

    constexpr auto center_of_sites() const
    {
        return utility::sum(sites_ | std::views::transform(&Site::coordinate)) / std::size(sites_);
    }

    constexpr auto begin() { return sites_.begin(); }
    constexpr auto end() { return sites_.end(); }
    constexpr auto begin() const { return sites_.begin(); }
    constexpr auto end() const { return sites_.end(); }
    constexpr auto size() const { return sites_.size(); }

    constexpr shape::Parallelepiped shape() const { return *this; }
    constexpr auto bravais() const { return bravais_; }
    constexpr auto system() const { return lattice::system(bravais_); }
    constexpr auto& sites() const { return sites_; }

private:
    Bravais bravais_;
    unitcell::Sites sites_;
};

template <>
struct std::formatter<lattice::UnitCell> : public std::formatter<lattice::Basis>
{
    std::format_context::iterator format(lattice::UnitCell const&, std::format_context&) const;
};
