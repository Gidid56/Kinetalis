#include "pch.h"

#include "viewer/core.h"

#include "point.h"

#include "lattice/site.h"
#include "viewer/bridge.h"

using namespace viewer;
using namespace viewer::scene;

namespace utility::color
{
    constexpr std::array colors{
        white,
        red,
        green,
        blue,
        yellow,
        maroon,
        cyan,
        gray,
        teal,
        olive,
        navy,
        magenta,
        silver,
        lime,
        black,
        purple,
        darkBlue,
        firebrick,
        darkOrange,
        paleVioletRed,
        slateGray,
        hotPink,
        slateBlue,
        darkMagenta,
        darkTurquoise,
        ghostWhite,
        chocolate,
        lightBlue,
        mediumOrchid,
        blanchedAlmond,
        darkOrchid,
        deepPink,
        lightGoldenrodYellow,
        turquoise,
        thistle,
        lawnGreen,
        orangeRed,
        papayaWhip,
        aquamarine,
        darkSalmon,
        rosyBrown,
        mediumAquamarine,
        gold,
        moccasin,
        dimGray,
        lightCyan,
        lemonChiffon,
        mediumSlateBlue,
        salmon,
        cadetBlue,
        oldLace,
        limeGreen,
        steelBlue,
        peru,
        lightSlateGray,
        tomato,
        brown,
        plum,
        lightCoral,
        lightYellow,
        lightGreen,
        orange,
        mediumSeaGreen,
        blueViolet,
        indigo,
        cornsilk,
        oliveDrab,
        tan,
        azure,
        greenYellow,
        chartreuse,
        navajoWhite,
        darkViolet,
        khaki,
        indianRed,
        bisque,
        lightSkyBlue,
        linen,
        darkOliveGreen,
        sandyBrown,
        paleGreen,
        mediumVioletRed,
        cornflowerBlue,
        paleTurquoise,
        lightPink,
        darkSlateGray,
        lightSalmon,
        dodgerBlue,
        seaGreen,
        sienna,
        darkSlateBlue,
        lightGray,
        lightSteelBlue,
        royalBlue,
        lightSeaGreen,
        deepSkyBlue,
        mediumBlue,
        floralWhite,
        darkGray,
        darkKhaki,
        saddleBrown,
        lavenderBlush,
        darkGoldenrod,
        darkRed,
        aliceBlue,
        crimson,
        peachPuff,
        midnightBlue,
        violet,
        paleGoldenrod,
        seashell,
        springGreen,
        goldenrod,
        mediumPurple,
        mediumSpringGreen,
        powderBlue,
        coral,
        yellowGreen,
        skyBlue,
        gainsboro,
        snow,
        antiqueWhite,
        burlywood,
        forestGreen,
        mediumTurquoise,
        lavender,
        darkSeaGreen,
        beige,
        mistyRose,
        mintCream,
        whiteSmoke,
        darkGreen,
        orchid,
        honeydew,
        wheat,
        pink,
        darkCyan,
        ivory,
    };
}

constexpr auto scale(auto max) { return 4.f / max; };
constexpr auto ad(auto max) { return max * (1 / 4.f); };
constexpr auto kink(auto max) { return max * (2 / 4.f); };
constexpr auto surface(auto max) { return max * (3 / 4.f); };
constexpr auto bulk(auto max) { return max; };

constexpr auto red = [](lattice::Coordination coordination, auto max) -> Magnum::Color3::Type {
    if (coordination <= kink(max))
        return 1;
    if (coordination >= surface(max))
        return 0;
    return 1 - (coordination - kink(max)) * scale(max);
};

constexpr auto green = [](lattice::Coordination coordination, auto max) -> Magnum::Color3::Type {
    if (coordination <= ad(max))
        return 1;
    if (coordination <= kink(max))
        return 1 - (coordination - ad(max)) * scale(max);
    if (coordination <= surface(max))
        return (coordination - kink(max)) * scale(max);
    return 1 - (coordination - surface(max)) * scale(max);
};

constexpr auto blue = [](lattice::Coordination coordination, auto max) -> Magnum::Color3::Type {
    if (coordination <= ad(max))
        return 1 - coordination * scale(max);
    if (coordination <= surface(max))
        return 0;
    return (coordination - surface(max)) * scale(max);
};

Magnum::Color3 colorNeighbors(lattice::Coordination coordination, auto max)
{
    if (max == 0)
        return { 1.0f, 1.0f, 1.0f };
    return { red(coordination, max), green(coordination, max), blue(coordination, max) };
}

Magnum::Color3 colorElement(chemistry::Element element) { return toMagnum(color(element)); }

Magnum::Color3 colorID(std::uint64_t id)
{
    using utility::color::colors;
    return toMagnum(colors.at(id % std::size(colors)));
}

Magnum::Color3 toColor(lattice::Index index)
{
    return {
        (((index + 1) & 0x00FF0000) >> 16) / 255.0f,
        (((index + 1) & 0x0000FF00) >> 8) / 255.0f,
        (((index + 1) & 0x000000FF) >> 0) / 255.0f,
    };
}

Magnum::Color3 colorPhase(output::Phases::Phase phase)
{
    switch (phase)
    {
        case output::Phases::Phase::solid: return toMagnum(utility::color::coral);
        case output::Phases::Phase::pristine: return toMagnum(utility::color::indigo);

        default: return toMagnum(utility::color::black);
    }
}

constexpr auto hologramAlpha{ 0.75f };

Point::Point(lattice::Site const& site)
    : position_{ toMagnum(site.position()) },
      index_{ static_cast<Magnum::Int>(site.index()) }
{ }

Point::Point(lattice::Position const& position, State state)
    : position_{ toMagnum(position) },
      index_{ -1 }
{
    switch (state)
    {
        case State::Transition:
            color_ = { toMagnum(utility::color::lightSalmon), hologramAlpha };
            break;
        case State::Final:
            color_ = { toMagnum(utility::color::lightSteelBlue), hologramAlpha };
            break;
    }
    setRadius(chemistry::Atom{ chemistry::Element::Au }.display_radius());
}

void
Point::setColor(lattice::Index index, SiteState const& site, ViewMode viewMode, bool highlighted)
{
    switch (viewMode)
    {
        case ViewMode::Elements: color_ = colorElement(site.element); break;

        case ViewMode::Coordination:
            color_ = colorNeighbors(site.coordination, site.maxNeighbors);
            break;

        case ViewMode::ID:
            color_ = toColor(index);
            break;

            //case ViewMode::Type: color_ = colorID(site.type()); break;    // TODO

        case ViewMode::Layers: color_ = colorID(site.layer); break;

        case ViewMode::Pristine:
            color_ = colorPhase(
                site.pristine ? output::Phases::Phase::pristine : output::Phases::Phase::solid);
            break;

        default: color_ = Magnum::Color3{ 1, 1, 1 }; break;
    }
    if (highlighted)
        color_ *= 1.5;
}
