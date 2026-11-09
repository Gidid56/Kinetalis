#include "pch.h"

#include "viewer/core.h"

#include "atoms.h"

#include "lattice/lattice.h"
#include "lattice/path.h"
#include "viewer/bridge.h"

using namespace viewer::scene;
using namespace Magnum::GL;

Atoms::Atoms(Lattice& lattice, Bridge& bridge)
    : lattice_{ lattice },
      visualBridge_{ bridge },
      pointsMesh_{ MeshPrimitive::Points }
{
    pointsMesh_.addVertexBuffer(
        pointsBuffer_,
        0,
        GenericGL3D::Position{},
        Attribute<1, Magnum::Float>{},
        GenericGL3D::Color4{},
        Attribute<3, Magnum::Int>{});

    points_.reserve(std::size(lattice.sites()));

    for (auto& site : lattice.sites())
        points_.emplace_back(site);
    updateAll();

    pointsBuffer_.setData(points_, BufferUsage::StreamDraw);
    pointsMesh_.setCount((Magnum::Int)points_.size());

    shaders_.setAmbientColor(Color3{ 0.15f })
        .setSpecularColor(Color3{ 0.15f })
        .setLightIntensity(0.75f)
        .setShininess(10.0f);
}

void Atoms::draw(Camera3D& camera, bool index)
{
    const auto task{ visualBridge_.task() };

    if (task != task_)
    {
        task_ = task;

        if (task_ == Task::Pause)
        {
            hologramsDirty_ = true;
        }
        else
        {
            clearHolograms();
            hologramsDirty_ = false;
        }
    }

    if (task_ == Task::Pause and hologramsDirty_)
        rebuildHolograms();

    if (pointsDirty_)
    {
        pointsBuffer_.setData(points_, BufferUsage::StreamDraw);
        pointsMesh_.setCount(static_cast<Magnum::Int>(points_.size()));
        pointsDirty_ = false;
    }

    shaders_.setTransformationMatrix(camera.cameraMatrix())
        .setProjectionMatrix(camera.projectionMatrix())
        .setScale(
            camera.viewport().y() * camera.projectionMatrix()[1][1] * (multiSampling_ ? 2 : 1))
        .setShowId(index)
        .draw(pointsMesh_);
}

void Atoms::set(ViewMode viewMode)
{
    std::swap(viewMode_, viewMode);

    if (viewMode_ != viewMode)
        updateAll();
}

void Atoms::setShowBulk(bool showBulk)
{
    std::swap(showBulk_, showBulk);

    if (showBulk_ != showBulk)
        updateAll();
}

void Atoms::updateAll()
{
    for (lattice::Index index{}; index < lattice_.sites().size(); ++index)
    {
        updatePoint(index, visualBridge_.state(index));
    }
    pointsDirty_ = true;
}

void Atoms::updatePoint(lattice::Index index, SiteState const& site)
{
    const auto highlightChanged{ synchronizeHighlight(index, site.highlighted) };

    auto& point{ points_.at(index) };

    if (not showBulk_ and site.isBulk())
        point.setRadius({});
    else
        point.setRadius(chemistry::Atom{ site.element }.display_radius());

    point.setColor(index, site, viewMode_, site.highlighted);

    if (highlightChanged and task_ == Task::Pause)
        hologramsDirty_ = true;

    pointsDirty_ = true;
}

bool Atoms::synchronizeHighlight(lattice::Index index, bool highlighted)
{
    auto iterator{ std::ranges::find(highlightedSites_, index) };

    if (highlighted)
    {
        if (iterator != highlightedSites_.end())
            return false;

        highlightedSites_.emplace_back(index);
        return true;
    }

    if (iterator == highlightedSites_.end())
        return false;

    highlightedSites_.erase(iterator);
    return true;
}

void Atoms::resetHighlights()
{
    const auto highlighted{ highlightedSites_ };

    for (auto index : highlighted)
        visualBridge_.setHighlighted(index, false);

    points_.resize(lattice_.sites().size());
    pointsMesh_.setCount((Magnum::Int)points_.size());
    hologramsDirty_ = false;
}

void Atoms::clearHolograms()
{
    const auto siteCount{ lattice_.sites().size() };

    if (points_.size() == siteCount)
        return;

    points_.resize(siteCount);
    pointsDirty_ = true;
}

void Atoms::rebuildHolograms()
{
    assert(task_ == Task::Pause);

    clearHolograms();
    hologramsDirty_ = false;

    auto forEachAllowedPath = [&](auto&& function) {
        for (auto sourceIndex : highlightedSites_)
        {
            auto& source{ lattice_.sites().at(sourceIndex) };

            for (auto destination : lattice_.neighbors(source))
            {
                lattice::Path path{ &source, destination };

                if (path.is_allowed())
                    std::invoke(function, path);
            }
        }
    };

    // All transition holograms come first.
    forEachAllowedPath([&](lattice::Path const& path) {
        for (auto const& position : path.transition_states(lattice_))
            points_.emplace_back(position, scene::Point::State::Transition);
    });

    // All final-site holograms come afterwards.
    forEachAllowedPath([&](lattice::Path const& path) {
        const auto position{ path.front().position() + path.path_vector(lattice_) };
        points_.emplace_back(position, scene::Point::State::Final);
    });

    pointsDirty_ = true;
}