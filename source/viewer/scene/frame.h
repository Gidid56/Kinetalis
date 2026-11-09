#pragma once

class viewer::scene::Frame
{
public:
    explicit Frame(lattice::Shape const&);

    void draw(Camera3D&);

private:
    Mesh innerMesh_, outerMesh_;
    Matrix4 innerTransform_, outerTransform_;
    FlatGL3D shader_{};
};
