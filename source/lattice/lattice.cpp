#include "pch.h"

#include "lattice.h"

#include "cells.h"
#include "simulation/engine.h"

using namespace lattice;

Lattice::Lattice(UnitCell const& unit_cell, Indices miller_indices)
    : unit_cell_{ unit_cell },
      miller_indices_{ miller_indices },
      shape_{ unit_cell.oriented_periodic_supercell(miller_indices) },
      max_path_length_{ unit_cell.next_neighbor_distance() }
{ }

// The constructSites() method first calculates the number of unit cells necessary
// to cover the shape by calling unitCell_.multiplesToCover(shape_).
// It then iterates through each unit cell that would cover the shape
// and each site in the unit cell to determine the position of the site in the lattice.
// If the position of the site is within the shape, a new Site object is created
// with the position and atom, and added to the sites_ container.
void Lattice::construct_sites()
{
    if (not sites_.empty())
        throw std::logic_error{ "Lattice sites have already been constructed." };

    Coordinate const dim{ unit_cell_.multiples_to_cover(shape_) };
    Coordinate const center{ -0.5 * dim };

    sites_.reserve(static_cast<std::size_t>((dim + 1).product() * unit_cell_.sites().size()));

    for (int x{}; x <= dim.x(); ++x)
        for (int y{}; y <= dim.y(); ++y)
            for (int z{}; z <= dim.z(); ++z)
                for (auto&& [w, site] : unit_cell_.sites() | std::views::enumerate)
                {
                    auto position{ unit_cell_.position(
                        site.coordinate + center + Coordinate{ x, y, z }) };

                    if (shape_.contains(position))
                    {
                        sites_.emplace_back(position, site.atom.element, Index{});
                    }
                }
    if (sites_.size() >= std::numeric_limits<Index>::max())
        throw std::overflow_error{ "Increase lattice::Index size." };

    rotate_up();
    sort_sites();
}

void Lattice::rotate_up()
{
    //if (not orthogonal(unitCell_.system()))
    if (not shape_.is_rectangular())
        return;

    auto transform{ shape_.reciprocal_basis().normalized_rows() };

    for (auto& site : sites_)
        site.set_position(shape_.center() + transform.dot(site.position()));

    shape_.set_vectors(math::Matrix<3>::identity() * shape_.parameters());
}

void Lattice::sort_sites()
{
    std::ranges::sort(sites_, std::ranges::less{}, &Site::position);

    for (Index index{}; auto& site : sites_)
    {
        site.set_index(index++);
        //site.resetOriginal();
    }
}

// This function implements the periodic boundary conditions for a given set of axes.
// It first selects the axes to apply the periodic boundary conditions to
// and then creates a list of translated sites that are no further than maxDistance
// from the edges of the parallelepiped in the selected directions.
// It then appends the translated sites to the list of sites in the lattice.
void Lattice::make_periodic()
{
    if (periodic_boundaries_ != math::Axis::none and not shape_.is_rectangular())
        throw std::domain_error{ std::format(
            "Periodic boundaries currently require a rectangular lattice shape."
            "\nShape: {}.",
            shape_) };

    for (auto axis :
        enums::selected_flags(periodic_boundaries_) | std::views::filter(math::is_dim<3>))
    {
        if (shape_.parameters()[axis] <= 2 * max_path_length_)
            throw std::domain_error{ "Periodic dimensions must exceed twice the neighbor cutoff." };

        // For each site in the lattice, the function computes the periodicityFraction,
        // which is the fraction of the lattice unit cell size that the maxDistance represents.
        // It then computes the coordinate of the site along the current axis,
        // and checks if it is inside the periodic boundary.
        // If the site is inside the boundary, it creates a translated site
        // by copying the current site and adding a translation vector equal
        // to the size of the unit cell in the current axis, in the opposite direction of the coordinate.

        std::vector<Site> translated;

        for (auto& site : sites_)
        {
            auto periodicity_fraction{ max_path_length_ / shape_.parameters()[axis] };
            auto coordinate{ (shape_.coordinate(site.position()) - site::center)[axis] -
                             epsilon::coordinate };

            if (abs(coordinate) >= 0.5 - periodicity_fraction)
            {
                translated.emplace_back(
                    site,
                    -math::binary_sign(coordinate) * shape_.vectors()[axis]);
            }
        }
        std::ranges::move(translated, std::back_inserter(sites_));
    }
}

void Lattice::construct_neighbor_list()
{
    if (not neighbors_.empty())
        throw std::logic_error{ "The neighbor list has already been constructed." };

    auto const site_count{ sites_.size() };

    neighbors_.reserve(site_count * max_coordination);

    {
        utility::SizeGuard original_sites{ sites_ };

        make_periodic();

        if (sites_.size() >= std::numeric_limits<Index>::max())
            throw std::overflow_error{ "Increase lattice::Index size." };

        Cells const cells{ sites_, max_path_length_ };

        auto const distance_squared{ math::square(max_path_length_ + epsilon::length) };

        for (auto& site : sites_ | std::views::take(site_count))
        {
            auto const cell{ cells.indices(site) };

            for (Indices const& shift : Cells::index_shifts)
                for (Index const temporary_index : cells.sites(cell + shift))
                {
                    auto& neighbor{ sites_[temporary_index] };

                    if (site.index() == neighbor.index())
                        continue;

                    if (distance_squared < (site.position() - neighbor.position()).norm_squared())
                        continue;

                    if (neighbors_.size() >= std::numeric_limits<Index>::max())
                        throw std::overflow_error{ "Increase lattice::Index size." };

                    auto const neighbor_index{ static_cast<Index>(neighbors_.size()) };

                    neighbors_.emplace_back(neighbor.index());
                    site.insert(&neighbor, neighbor_index);
                }
        }
    }

    sites_.shrink_to_fit();
    neighbors_.shrink_to_fit();
}

constexpr auto compPosition = [](physics::quantity::Displacement const& left,
                                  physics::quantity::Displacement const& right) {
    return abs(left - right) <= epsilon::length;
};

Layers::iterator Lattice::get_layer(physics::quantity::Displacement position)
{
    auto upper{ layers_.lower_bound(position) };
    auto closest{ layers_.end() };

    auto consider = [&](auto candidate) {
        if (candidate == layers_.end() or not compPosition(position, candidate->first))
            return;

        if (closest == layers_.end() or
            math::abs(position - candidate->first) < math::abs(position - closest->first))
            closest = candidate;
    };

    consider(upper);
    if (upper != layers_.begin())
        consider(std::prev(upper));

    if (closest != layers_.end())
        return closest;
    return layers_.try_emplace(position).first;
}

void Lattice::construct_layers(math::Axis axis)
{
    if constexpr (not useLayers)
        return;

    layer_axis_ = axis;
    layers_.clear();

    for (Index index{}; auto& site : sites_)
        get_layer(site.layer_position(axis))->second.emplace_back(index++);

    for (Layer layer{}; auto const& indices : layers_ | std::views::values)
    {
        for (auto const index : indices)
            sites_.at(index).set_layer(layer);

        ++layer;
    }
}

void Lattice::fill(Element element)
{
    master_alloy_ = Composition{ element };

    for (auto& site : sites_)
        site.set_element(*this, element);
}

void Lattice::fill(simulation::Engine& rng, Composition const& composition)
{
    master_alloy_ = composition;

    for (auto& site : sites_)
        site.set_element(*this, rng.element(composition));
}

void Lattice::fill(Element element, Shape const& shape, bool place_inside)
{
    for (auto& site : sites_)
    {
        if (place_inside == shape.contains(site.position()))
        {
            site.set_element(*this, element);
        }
    }
}

void Lattice::fill(
    simulation::Engine& rng,
    Composition const& composition,
    Shape const& shape,
    bool place_inside)
{
    for (auto& site : sites_)
    {
        if (place_inside == shape.contains(site.position()))
        {
            site.set_element(*this, rng.element(composition));
        }
    }
}

void Lattice::fill_exact(
    simulation::Engine& rng,
    Composition const& composition,
    Shape const& shape,
    bool place_inside)
{
    const auto selected = std::ranges::count_if(sites_, [&](auto const& site) {
        return place_inside == shape.contains(site.position());
    });
    if (composition.total_quantity() != selected)
        throw std::invalid_argument{
            "Composition total quantity does not match the number of sites in the specified shape."
        };

    auto remaining{ composition };
    for (auto& site : sites_)
    {
        if (place_inside == shape.contains(site.position()))
        {
            const auto element{ rng.element(remaining) };
            site.set_element(*this, element);
            remaining -= element;
        }
    }
}

void Lattice::gradient_fill(
    simulation::Engine& rng,
    Composition const& bottom,
    Displacement bottom_pos,
    Composition const& top,
    Displacement top_pos,
    std::function<double(double)> function,
    math::Axis axis)
{
    auto height{ shape_.parameters()[axis] };
    auto direction{ shape_.vectors()[axis].normalized() };
    constexpr auto resolution{ 1'000 };

    for (auto& site : sites_)
    {
        auto distance{ direction.dot(site.position()) };

        if (distance < bottom_pos or distance >= top_pos)
            continue;

        double fraction{ function((distance - bottom_pos) / (top_pos - bottom_pos)) };

        Composition composition;

        for (auto&& [element, quantity] : bottom)
        {
            composition += { element,
                math::round<std::size_t>(
                    (1 - fraction) * quantity * top.total_quantity() * resolution) };
        }
        for (auto&& [element, quantity] : top)
        {
            composition += { element,
                math::round<std::size_t>(
                    fraction * quantity * bottom.total_quantity() * resolution) };
        }
        site.set_element(*this, rng.element(composition));
    }
}

math::Vector<3, Displacement> Lattice::displacement(Site const& from, Site const& to) const
{
    auto coordinate{ shape_.reciprocal_basis().dot(to.position() - from.position()) };

    for (auto axis :
        enums::selected_flags(periodic_boundaries_) | std::views::filter(math::is_dim<3>))
    {
        coordinate[axis] -= round(coordinate[axis]);
    }
    return shape_.vectors().transpose_dot(coordinate);
}

//std::span<Site* const> Lattice::neighbors(Site const& site) const
//{
//    if (site.max_neighbors() == 0)
//        return {};
//    return { &neighbors_.at(site.neighbor_index()),
//        static_cast<std::size_t>(site.max_neighbors()) };
//}

Site& Lattice::center_site()
{
    if (sites_.empty())
        throw std::logic_error{ "An empty lattice has no center site." };

    return *std::ranges::min_element(sites_, {}, [this](auto& site) {
        return (site.position() - shape_.center()).norm_squared();
    });
}

std::size_t Lattice::number_atoms() const
{
    return std::ranges::count_if(sites_, &Site::is_occupied);
}

double Lattice::solid_fraction() const
{
    if (sites_.empty())
        return 0.;
    return number_atoms() / static_cast<double>(sites_.size());
}

double Lattice::fraction(Element element) const
{
    if (sites_.empty())
        return 0.;
    return std::ranges::count(sites_, element, &Site::element) / static_cast<double>(sites_.size());
}

std::size_t Lattice::broken_bonds() const
{
    return utility::sum(
        sites_ | std::views::filter(&Site::is_occupied) |
        std::views::transform(&Site::vacant_neighbors));
}

std::size_t Lattice::surface_atoms(Coordination vacancies) const
{
    auto is_surface{ [vacancies](
                         Site const& site) { return site.vacant_neighbors() >= vacancies; } };
    return std::ranges::distance(
        sites_ | std::views::filter(&Site::is_occupied) | std::views::filter(is_surface));
}

std::string Lattice::to_xyz_file(bool const bulk, std::size_t const precision) const
{
    if (precision < 1 or precision > std::numeric_limits<double>::max_digits10)
        throw std::invalid_argument{ "Invalid XYZ precision." };

    constexpr auto unit{ 0.1_nm };

    auto const& vectors{ shape_.vectors() };
    auto const origin{ shape_.center() - vectors.sum_columns() / 2 };

    auto const include = [bulk](Site const& site) {
        return site.is_occupied() and (bulk or site.is_surface());
    };
    auto const periodic = [this](math::Axis const axis) {
        return enum_flags_test(periodic_boundaries_, axis) ? 'T' : 'F';
    };

    auto const site_count{ std::ranges::count_if(sites_, include) };

    std::string text;
    text.reserve(256 + 64 * site_count);

    auto output{ std::back_inserter(text) };
    auto append_number = [&](std::floating_point auto const value) {
        std::format_to(output, "{:.{}g} ", value, precision);
    };

    std::format_to(output, "{}\nLattice=\"", site_count);

    for (auto const& vector : vectors / unit)
        for (const double coordinate : vector)
            append_number(coordinate);

    std::format_to(output, "\" Origin=\"");

    for (const double coordinate : origin / unit)
        append_number(coordinate);

    std::format_to(
        output,
        "\" Properties=species:S:1:pos:R:3 pbc=\"{} {} {}\"",
        periodic(math::Axis::x),
        periodic(math::Axis::y),
        periodic(math::Axis::z));

    if (not label_.empty())
        std::format_to(output, " Label={}", label_);

    text += '\n';

    for (auto const& site : sites_)
    {
        if (not include(site))
            continue;

        std::format_to(output, "{}\t", symbol(site.element()));

        for (const double coordinate : site.position() / unit)
            append_number(coordinate);

        text += '\n';
    }

    return text;
}

std::format_context::iterator
std::formatter<Lattice>::format(Lattice const& lattice, std::format_context& context) const
{
    auto out{ context.out() };

    out = std::format_to(
        out,
        "\tUnit cell\n\t{:nm}\n\tshape {:nm}\n\n"
        "{} sites, {} atoms.\n"
        "Top surface miller indices {}.\n"
        "Periodic boundaries for {}-axes.\n"
        "Max path length {:nm}.\n",
        lattice.unit_cell(),
        lattice.shape(),
        lattice.sites().size(),
        lattice.number_atoms(),
        lattice.miller_indices(),
        enum_flags_name(lattice.periodic_boundaries()),
        lattice.max_path_length());

    if constexpr (useLayers)
        out = std::format_to(out, "Layers perpendicular to {}-axis.\n", lattice.layer_axis());

    out = std::format_to(out, "\nSite composition\n");

    for (auto& element : activeElements)
    {
        out = std::format_to(out, "{} {:%;.3}\n", element, Fraction{ lattice.fraction(element) });
    }

    std::map<Coordination, std::size_t> distribution;

    for (auto const& site : lattice.sites())
        ++distribution[site.max_neighbors()];

    for (auto const& [neighbors, number] : distribution)
        out = std::format_to(out, "{} neighbors/site ({}x).\n", neighbors, number);

    return out;
}

site::Type Lattice::site_type(Site const& site) const
{
    if (site.coordination() == 0)
        return site::Type::isolated;
    if (site.vacant_neighbors() == 0)
        return site::Type::bulk;

    std::array<Coordination, 8> n_i{}, v_i{};

    auto neighbors{ this->neighbors(site) | std::views::filter(&Site::is_occupied) };
    for (auto neighbor : neighbors)
    {
        ++n_i[std::ranges::count_if(
            this->neighbors(*neighbor) | std::views::filter(&Site::is_occupied),
            [&](auto site) { return std::ranges::contains(neighbors, site); })];
    }
    auto vacancies{ this->neighbors(site) | std::views::filter(&Site::is_vacant) };
    for (auto vacancy : vacancies)
    {
        ++v_i[std::ranges::count_if(
            this->neighbors(*vacancy) | std::views::filter(&Site::is_vacant),
            [&](auto site) { return std::ranges::contains(vacancies, site); })];
    }

    //if (site.coordination() == 4 and nI[2] == 4)
    //    return site::Type::_100__ad;
    //if (site.coordination() == 3 and nI[2] == 3)
    //    return site::Type::_111__ad;
    //if (site.coordination() == 4 and nI[1] == 1 and nI[2] == 2 and nI[3] == 1)
    //    return site::Type::_111__dimer_ad;
    //if (site.coordination() == 5 and nI[2] == 3 and nI[3] == 2)
    //    return site::Type::_111__step_A_ad;
    //if (site.coordination() == 5 and nI[2] == 4 and nI[4] == 1)
    //    return site::Type::_111__step_B_ad;

    //if (site.coordination() == 6 and nI[2] == 3 and nI[3] == 2 and nI[4] == 1)
    //    return site::Type::Kink;

    //if (site.vacantNeighbors() == 5 and vI[2] == 3 and vI[3] == 2)
    //    return site::Type::_111__step_edge_A;
    //if (site.vacantNeighbors() == 5 and vI[2] == 4 and vI[4] == 1)
    //    return site::Type::_111__step_edge_B;
    //if (site.vacantNeighbors() == 4 and vI[1] == 1 and vI[2] == 2 and vI[3] == 1)
    //    return site::Type::_111__step_corner;
    //if (site.vacantNeighbors() == 3 and vI[2] == 3)
    //    return site::Type::_111__surface;
    //if (site.vacantNeighbors() == 4 and vI[2] == 4)
    //    return site::Type::_100__surface;

    return site::Type::none;
}
