#pragma once
#include "../states.h"

class viewer::scene::Point
{
public:
    Point() = default;

    enum class State
    {
        Initial,
        Transition,
        Final
    };

    explicit Point(lattice::Site const&);
    explicit Point(lattice::Position const&, State);

    void setPosition(lattice::Position const& position) { position_ = toMagnum(position); }
    void setRadius(chemistry::atom::Radius radius)
    {
        using namespace physics::unit::literals;

        radius_ = static_cast<Magnum::Float>((radius / 1_nm).value());
    }
    void setColor(lattice::Index, SiteState const&, ViewMode, bool highlighted);

private:
    Magnum::Vector3 position_{};
    Magnum::Float radius_{};
    Magnum::Color4 color_{};
    Magnum::Int index_{ -1 };
};
