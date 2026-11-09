#include "pch.h"

#include "site.h"

#include "lattice/lattice.h"

using namespace lattice;
using kmc::Event;

void Site::set_element(Lattice& lattice, Element element)
{
    this->set_dirty(true);

    if (element_ == element)
        return;

    if (element_ != Element::none)
        update_neighbors(lattice, -1);

    element_ = element;

    if (element_ != Element::none)
        update_neighbors(lattice, 1);
}

void Site::insert(Site* neighbor, Index neighbor_index)
{
    if constexpr (elementSpecificNeighbors)
        if (neighbor->element() != Element::none)
            ++element_neighbors_[elementKey(neighbor->element())];

    if (neighbor->is_occupied())
        ++coordination_;

    if (max_neighbors_ == 0)
        neighbor_index_ = neighbor_index;

    ++max_neighbors_;
}

bool Site::is_active() const
{
    if constexpr (not std::ranges::contains(kmc::enabledEvents, Event::deposition))
        if (element_ == Element::none)
            return false;

    if (element_ == Element::Au and vacant_neighbors() <= 2)
        return false;

    return not is_bulk();
}

bool Site::is_allowed() const { return is_active(); }

void Site::execute_process(Lattice& lattice)
{
    if (is_occupied())
        set_element(lattice, Element::none);
    else
        set_element(lattice, Element::Ag);
}

void Site::update_neighbors(Lattice& lattice, std::int_fast8_t delta)
{
    for (auto neighbor : lattice.neighbors(*this))
    {
        neighbor->set_dirty(true);
        neighbor->coordination_ += delta;
        if constexpr (elementSpecificNeighbors)
            if (element_ != Element::none)
                neighbor->element_neighbors_[elementKey(element_)] += delta;
    }
    set_pristine(false);
}

Event Site::event() const
{
    if (not is_active())
        return Event::none;

    if constexpr (std::ranges::contains(kmc::enabledEvents, Event::dissolution))
        if (element_ == Element::Ag)
            return Event::dissolution;

    if constexpr (std::ranges::contains(kmc::enabledEvents, Event::deposition))
        if (element_ == Element::none)
            return Event::deposition;

    return Event::none;
}

physics::quantity::Displacement Site::layer_position(math::Axis axis) const
{
    if (enum_flags_test(axis, math::Axis::r2))
        return sqrt(position_.r_squared<2>());

    if (enum_flags_test(axis, math::Axis::r3))
        return sqrt(position_.r_squared<3>());

    if (math::is_dim<3>(axis))
        return position_[axis];

    throw std::invalid_argument{ "Invalid layer axis." };
}

Coordination Site::coordination(Element element) const
{
    return element_neighbors_[elementKey(element)];
}
