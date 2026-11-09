#include "pch.h"

#include "path.h"

#include "lattice.h"
#include "site.h"

using namespace lattice;
using kmc::Event;

Event Path::event() const
{
    if (not is_active())
        return Event::none;

    return Event::diffusion;
}

bool Path::is_open() const { return front().is_occupied() and back().is_vacant(); }

bool Path::is_active() const { return front().is_active() and is_open(); }

bool Path::is_allowed() const
{
    return this->is_active() and front().is_allowed() and
           (back().coordination() >= 3 or front().coordination() == 0);
}

void Path::execute_process(Lattice& lattice)
{
    if (back().is_occupied() and front().is_vacant())
        std::swap(sites_[0], sites_[1]);

    if (front().is_occupied() and back().is_vacant())
    {
        back().set_element(lattice, front().element());
        front().set_element(lattice, Element::none);
        //auto backOriginal{ back().original() };
        //back().setOriginal(front().original());
        //front().setOriginal(backOriginal);
    }

    if (front().is_occupied() and back().is_vacant())
        std::swap(sites_[0], sites_[1]);
}

bool Path::is_same_layer() const { return front().layer() == back().layer(); }

math::Vector<3, Displacement> Path::path_vector(Lattice const& lattice) const
{
    return lattice.displacement(front(), back());
}

std::vector<Position> Path::transition_states(Lattice const& lattice) const { return {}; }

//std::vector<Position> Path::transition_states(Lattice const& lattice) const
//{
//    //	// TODO: Replace this direct midpoint placeholder with
//    //	// the geometric transition-state coordination model.
//    //	return{ front().position() + pathVector(lattice) / 2 };
//
//    /*
//		Temporary dynamic construction of minimal GTC path proposals.
//
//		The returned positions are the geometric midpoints of all admissible
//		one- or two-contact arc segments.  For a two-segment path, both segment
//		midpoints are inserted.  Exact path grouping is deliberately not retained
//		by this temporary std::vector<Position> interface.
//	*/
//    if (lattice.unit_cell().bravais() != Bravais::cF or not is_open())
//        return {};
//
//    constexpr std::size_t samples_per_arc{ 64 };
//    constexpr double coordinate_tolerance{ 8 * epsilon::coordinate };
//    const auto length_tolerance{ 8 * epsilon::length };
//    const auto area_tolerance{ math::square(length_tolerance) };
//
//    const Position initial{};
//    const auto final{ path_vector(lattice) };
//    const Length nearest_neighbor_distance{ final.norm() };
//
//    if (nearest_neighbor_distance <= length_tolerance or
//        math::abs(nearest_neighbor_distance - lattice.unit_cell().next_neighbor_distance()) >
//            length_tolerance)
//        return {};
//
//    const auto nearest_neighbor_distance_squared{ math::square(nearest_neighbor_distance) };
//
//    struct LocalNeighbor
//    {
//        Site const* site{};
//        Position position{};
//        bool initial_neighbor{};
//        bool final_neighbor{};
//    };
//
//    std::vector<LocalNeighbor> local_neighbors;
//    local_neighbors.reserve(2 * maxCoordination);
//
//    auto contact_residual =
//        [&](Position const& position, bool initial_neighbor, bool final_neighbor) {
//            Length residual{};
//            if (initial_neighbor)
//                residual += math::abs(position.norm() - nearest_neighbor_distance);
//            if (final_neighbor)
//                residual += math::abs((position - final).norm() - nearest_neighbor_distance);
//            return residual;
//        };
//
//    auto insert_neighbor = [&](Site const& endpoint, bool initial_endpoint) {
//        for (auto neighbor : lattice.neighbors(endpoint))
//        {
//            if (neighbor == &front() or neighbor == &back())
//                continue;
//
//            const Position candidate_position{
//                initial_endpoint ? lattice.displacement(front(), *neighbor)
//                                 : final + lattice.displacement(back(), *neighbor)
//            };
//
//            auto entry{ std::ranges::find(local_neighbors, neighbor, &LocalNeighbor::site) };
//            if (entry == local_neighbors.end())
//            {
//                local_neighbors.emplace_back(
//                    LocalNeighbor{ neighbor,
//                        candidate_position,
//                        initial_endpoint,
//                        not initial_endpoint });
//                continue;
//            }
//
//            const bool initial_neighbor{ entry->initial_neighbor or initial_endpoint };
//            const bool final_neighbor{ entry->final_neighbor or not initial_endpoint };
//
//            if (contact_residual(candidate_position, initial_neighbor, final_neighbor) <
//                contact_residual(entry->position, initial_neighbor, final_neighbor))
//                entry->position = candidate_position;
//
//            entry->initial_neighbor = initial_neighbor;
//            entry->final_neighbor = final_neighbor;
//        }
//    };
//
//    insert_neighbor(front(), true);
//    insert_neighbor(back(), false);
//
//    std::ranges::sort(local_neighbors, {}, [](LocalNeighbor const& neighbor) {
//        return neighbor.site->index();
//    });
//
//    auto occupied = [](LocalNeighbor const& neighbor) {
//        return neighbor.site->is_occupied();
//    };
//
//    auto is_contact = [&](Position const& point, Position const& support) {
//        return math::abs((point - support).norm() - nearest_neighbor_distance) <= length_tolerance;
//    };
//
//    auto overlaps_occupied_neighbor = [&](Position const& point) {
//        for (auto const& neighbor : local_neighbors)
//        {
//            if (not occupied(neighbor))
//                continue;
//
//            if ((point - neighbor.position).norm() + length_tolerance < nearest_neighbor_distance)
//                return true;
//        }
//        return false;
//    };
//
//    auto projected_coordinate = [&](Position const& point) {
//        return point.dot(final) / nearest_neighbor_distance_squared;
//    };
//
//    auto ends_closer_to_final = [&](Position const& begin, Position const& end) {
//        return (end - final).norm() + length_tolerance < (begin - final).norm();
//    };
//
//    struct Arc
//    {
//        Position midpoint{};
//    };
//
//    auto admissible_arcs = [&](Position const& begin,
//                               Position const& end,
//                               Position const& support_a,
//                               Position const& support_b) {
//        std::vector<Arc> arcs;
//        arcs.reserve(2);
//
//        if (not ends_closer_to_final(begin, end) or not is_contact(begin, support_a) or
//            not is_contact(begin, support_b) or not is_contact(end, support_a) or
//            not is_contact(end, support_b))
//            return arcs;
//
//        const Position support_vector{ support_b - support_a };
//        const Length support_distance{ support_vector.norm() };
//        if (support_distance <= length_tolerance or
//            support_distance + length_tolerance >= 2 * nearest_neighbor_distance)
//            return arcs;
//
//        const Position center{ (support_a + support_b) / 2 };
//        const Position radial_begin{ begin - center };
//        const Position radial_end{ end - center };
//        const Length radius{ radial_begin.norm() };
//
//        if (radius <= length_tolerance or math::abs(radial_end.norm() - radius) > length_tolerance)
//            return arcs;
//
//        const Position tangent_begin{ support_vector.cross(radial_begin) / support_distance };
//        double cosine{ radial_begin.dot(radial_end) / math::square(radius) };
//        double sine{ support_vector.dot(radial_begin.cross(radial_end)) /
//                     (support_distance * math::square(radius)) };
//        cosine = std::clamp(cosine, -1., 1.);
//        sine = std::clamp(sine, -1., 1.);
//
//        std::array<double, 2> angles{};
//        if (std::abs(sine) <= coordinate_tolerance and cosine < 0.)
//        {
//            angles = { std::numbers::pi, -std::numbers::pi };
//        }
//        else
//        {
//            const double short_angle{ std::atan2(sine, cosine) };
//            if (std::abs(short_angle) <= coordinate_tolerance)
//                return arcs;
//
//            angles = { short_angle,
//                short_angle > 0. ? short_angle - 2 * std::numbers::pi
//                                 : short_angle + 2 * std::numbers::pi };
//        }
//
//        auto point_on_arc = [&](double angle, double fraction) {
//            const double value{ fraction * angle };
//            return center + radial_begin * std::cos(value) + tangent_begin * std::sin(value);
//        };
//
//        for (double angle : angles)
//        {
//            double previous_coordinate{ projected_coordinate(begin) };
//            bool admissible{ true };
//
//            for (std::size_t sample{ 1 }; sample <= samples_per_arc; ++sample)
//            {
//                const double fraction{ static_cast<double>(sample) / samples_per_arc };
//                const Position point{ point_on_arc(angle, fraction) };
//                const double coordinate{ projected_coordinate(point) };
//
//                if (coordinate <= previous_coordinate + coordinate_tolerance or
//                    overlaps_occupied_neighbor(point))
//                {
//                    admissible = false;
//                    break;
//                }
//                previous_coordinate = coordinate;
//            }
//
//            if (admissible)
//                arcs.emplace_back(Arc{ point_on_arc(angle, .5) });
//        }
//        return arcs;
//    };
//
//    auto three_sphere_intersections =
//        [&](Position const& center_a, Position const& center_b, Position const& center_c) {
//            std::vector<Position> intersections;
//            intersections.reserve(2);
//
//            const Position ab{ center_b - center_a };
//            const Length ab_length{ ab.norm() };
//            if (ab_length <= length_tolerance)
//                return intersections;
//
//            const auto axis_x{ ab / ab_length };
//            const Position ac{ center_c - center_a };
//            const Displacement projection{ axis_x.dot(ac) };
//            const Position normal_component{ ac - axis_x * projection };
//            const Displacement normal_length{ normal_component.norm() };
//            if (normal_length <= length_tolerance)
//                return intersections;
//
//            const auto axis_y{ normal_component / normal_length };
//            const auto axis_z{ axis_x.cross(axis_y) };
//            const Displacement x{ ab_length / 2 };
//            const Displacement y{ (math::square(projection) + math::square(normal_length) -
//                                      2 * projection * x) /
//                                  (2 * normal_length) };
//            const auto z_squared{ nearest_neighbor_distance_squared - math::square(x) -
//                                  math::square(y) };
//
//            if (z_squared < -area_tolerance)
//                return intersections;
//
//            const Position base{ center_a + axis_x * x + axis_y * y };
//            if (z_squared <= area_tolerance)
//            {
//                intersections.emplace_back(base);
//                return intersections;
//            }
//
//            const Length z{ sqrt(z_squared) };
//            intersections.emplace_back(base + axis_z * z);
//            intersections.emplace_back(base - axis_z * z);
//            return intersections;
//        };
//
//    std::vector<Position> transition_states;
//
//    auto append_unique = [&](Position const& point) {
//        if (std::ranges::none_of(transition_states, [&](Position const& existing) {
//                return (existing - point).norm() <= length_tolerance;
//            }))
//            transition_states.emplace_back(point);
//    };
//
//    std::vector<LocalNeighbor const*> common_neighbors;
//    std::vector<LocalNeighbor const*> initial_neighbors;
//    std::vector<LocalNeighbor const*> final_neighbors;
//
//    for (auto const& neighbor : local_neighbors)
//    {
//        if (not occupied(neighbor))
//            continue;
//
//        if (neighbor.initial_neighbor)
//            initial_neighbors.emplace_back(&neighbor);
//        if (neighbor.final_neighbor)
//            final_neighbors.emplace_back(&neighbor);
//        if (neighbor.initial_neighbor and neighbor.final_neighbor)
//            common_neighbors.emplace_back(&neighbor);
//    }
//
//    // Direct one-segment paths supported by two occupied common neighbors.
//    for (std::size_t left{}; left < common_neighbors.size(); ++left)
//        for (std::size_t right{ left + 1 }; right < common_neighbors.size(); ++right)
//            for (auto const& arc : admissible_arcs(
//                     initial,
//                     final,
//                     common_neighbors[left]->position,
//                     common_neighbors[right]->position))
//                append_unique(arc.midpoint);
//
//    // Hollow-mediated two-segment paths.  One occupied common neighbor is
//    // retained as a support while the second support changes at the junction.
//    for (auto persistent : common_neighbors)
//        for (auto first_support : initial_neighbors)
//        {
//            if (first_support == persistent)
//                continue;
//
//            for (auto second_support : final_neighbors)
//            {
//                if (second_support == persistent or second_support == first_support)
//                    continue;
//
//                for (Position const& junction : three_sphere_intersections(
//                         persistent->position,
//                         first_support->position,
//                         second_support->position))
//                {
//                    if ((junction - initial).norm() <= length_tolerance or
//                        (junction - final).norm() <= length_tolerance)
//                        continue;
//
//                    auto first_arcs{ admissible_arcs(
//                        initial,
//                        junction,
//                        persistent->position,
//                        first_support->position) };
//                    if (first_arcs.empty())
//                        continue;
//
//                    auto second_arcs{ admissible_arcs(
//                        junction,
//                        final,
//                        persistent->position,
//                        second_support->position) };
//                    if (second_arcs.empty())
//                        continue;
//
//                    for (auto const& first_arc : first_arcs)
//                        for (auto const& second_arc : second_arcs)
//                        {
//                            append_unique(first_arc.midpoint);
//                            append_unique(second_arc.midpoint);
//                        }
//                }
//            }
//        }
//
//    for (Position& transition_state : transition_states)
//        transition_state += front().position();
//
//    return transition_states;
//}
