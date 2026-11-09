#pragma once
#include "pch.h"

#include "viewer/types.h"

#include <Corrade/Containers/ArrayViewStl.h>
#include <Corrade/Containers/Reference.h>
#include <Corrade/Containers/StridedArrayView.h>
#include <Corrade/PluginManager/Manager.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/GL/RenderbufferFormat.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/Version.h>
#include <Magnum/ImGuiIntegration/Context.hpp>
#include <Magnum/Image.h>
#include <Magnum/Magnum.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/DualQuaternion.h>
#include <Magnum/Math/Functions.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Math/Time.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Shaders/Vector.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/DistanceFieldGlyphCacheGL.h>
#include <Magnum/Text/Renderer.h>
#include <Magnum/Trade/MeshData.h>
#include <implot.h>

using Magnum::SceneGraph::AbstractTranslationRotation3D;
using Magnum::SceneGraph::Camera3D;
using Magnum::SceneGraph::Drawable3D;
using Magnum::SceneGraph::DrawableGroup3D;
using Magnum::SceneGraph::MatrixTransformation3D;
using Magnum::SceneGraph::Object;
using Magnum::SceneGraph::Scene;

using Magnum::Shaders::FlatGL3D;
using Magnum::Shaders::GenericGL3D;

using Magnum::GL::Buffer;
using Magnum::GL::Framebuffer;
using Magnum::GL::Mesh;
using Magnum::GL::Renderbuffer;
using Magnum::GL::Renderer;

using Magnum::Color3;
using Magnum::Color4;
using Magnum::Matrix3;
using Magnum::Matrix4;
using Magnum::Vector2;
using Magnum::Vector2i;
using Magnum::Vector3;

namespace viewer
{
    constexpr Magnum::Color3 toMagnum(utility::Color const& color)
    {
        auto scaled{ color / 255.f };
        return { scaled.x(), scaled.y(), scaled.z() };
    }

    constexpr Magnum::Vector3 toMagnum(lattice::Position const& position)
    {
        using namespace physics::unit::literals;

        auto vector{ (position / 1_nm).to<float>() };

        return { vector.x(), vector.y(), vector.z() };
    }

    constexpr Magnum::Vector3 toMagnum(math::Vector<3> const& direction)
    {
        auto vector{ direction.to<float>() };

        return { vector.x(), vector.y(), vector.z() };
    }

    template <typename T>
    constexpr Magnum::Matrix3 toMagnum(math::Matrix<3, 3, T> const& matrix)
    {
        return { toMagnum(matrix.x()), toMagnum(matrix.y()), toMagnum(matrix.z()) };
    }
}
