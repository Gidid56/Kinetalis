#include "pch.h"

#include "viewer/core.h"

#include "frame.h"

#include "lattice/shape.h"

#include <Magnum/MeshTools/Transform.h>
#include <Magnum/Primitives/Cone.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Primitives/Cylinder.h>
#include <Magnum/Primitives/UVSphere.h>

using namespace viewer::scene;
using namespace lattice::shape;
using namespace Magnum::Primitives;
using namespace Magnum::MeshTools;
using namespace Magnum::Math::Literals;
using Magnum::Trade::MeshData;

MeshData mesh(Parallelepiped const&) { return cubeWireframe(); }

MeshData mesh(Ellipsoid const&) { return uvSphereWireframe(32, 64); }

MeshData mesh(Cylindroid const&)
{
    return transform3D(cylinderWireframe(2, 64, 1), Matrix4::rotationX(90.0_degf));
}

MeshData mesh(EllipticCone const&)
{
    return transform3D(coneWireframe(64, 1), Matrix4::rotationX(90.0_degf));
}

MeshData mesh(lattice::Shape const& shape)
{
    if (auto const* derived = dynamic_cast<Parallelepiped const*>(&shape))
        return mesh(*derived);

    if (auto const* derived = dynamic_cast<Ellipsoid const*>(&shape))
        return mesh(*derived);

    if (auto const* derived = dynamic_cast<Cylindroid const*>(&shape))
        return mesh(*derived);

    if (auto const* derived = dynamic_cast<EllipticCone const*>(&shape))
        return mesh(*derived);

    throw std::invalid_argument{ "No viewer mesh defined for this Shape type" };
}

Frame::Frame(lattice::Shape const& shape)
{
    innerMesh_ = compile(mesh(shape));
    outerMesh_ = compile(cubeWireframe());

    innerTransform_ =
        Matrix4::translation(toMagnum(shape.center())) * Matrix4{ toMagnum(shape.vectors() / 2) };
    outerTransform_ = Matrix4{ toMagnum(math::Matrix<3>::identity() * shape.outer_cuboid() / 2) };
}

void Frame::draw(Camera3D& camera)
{
    Renderer::setLineWidth(8);

    shader_
        .setTransformationProjectionMatrix(
            camera.projectionMatrix() * camera.cameraMatrix() * innerTransform_)
        .setColor(Color4{ .5f, .5f })
        .draw(innerMesh_);
}