#pragma once

namespace viewer
{
    class Viewer;

    namespace scene
    {
        class Atoms;
        class Axes;
        class Camera;
        class Point;
        class Frame;
    }

    class Bridge;
    class Trajectory;

    using PackedState = std::uint64_t;
    struct SiteState;

    class GUI;

    namespace gui
    {
        struct Status;
        struct Sample;
    }

    namespace atoms { class Shaders; }
    namespace camera { class ArcBall; }
}
using viewer::Viewer;