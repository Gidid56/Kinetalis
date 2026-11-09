#pragma once

class viewer::scene::Axes
{
public:
    explicit Axes();

    void draw(Camera3D&);

private:
    Magnum::Matrix4 transformation_;
    FlatGL3D shader_{};
    Mesh mesh_;
};
