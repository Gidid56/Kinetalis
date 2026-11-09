#include "pch.h"

#include "viewer/core.h"

#include "axes.h"

#include <Magnum/Primitives/Axis.h>

using namespace viewer::scene;

Axes::Axes()
{
    mesh_ = Magnum::MeshTools::compile(Magnum::Primitives::axis3D());

    transformation_ = Matrix4{ Matrix3{ Magnum::Math::IdentityInit } * 16 };
}

void Axes::draw(Camera3D& camera)
{
    Renderer::setLineWidth(6);

    shader_
        .setTransformationProjectionMatrix(
            camera.projectionMatrix() * camera.cameraMatrix() * transformation_)
        .setColor(Color4{ .8f, .8f })
        .draw(mesh_);
}
