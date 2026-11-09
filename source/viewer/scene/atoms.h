#pragma once
#include "point.h"
#include "shaders.h"

class viewer::scene::Atoms
{
public:
    explicit Atoms(Lattice&, Bridge&);

    void draw(Camera3D&, bool index = false);

    void set(ViewMode viewMode);
    void setShowBulk(bool showBulk);

    void updateAll();
    void updatePoint(lattice::Index, SiteState const&);

    bool synchronizeHighlight(lattice::Index index, bool highlighted);

    void resetHighlights();
    void clearHolograms();
    void rebuildHolograms();

    void markDirty() noexcept { pointsDirty_ = true; }

    auto& lattice() const { return lattice_; }
    auto viewMode() const { return viewMode_; }
    auto multiSampling() const { return multiSampling_; }
    auto showBulk() const { return showBulk_; }
    auto& showBulk() { return showBulk_; }
    auto& points() { return points_; }

private:
    Task task_{ Task::Pause };
    ViewMode viewMode_{ ViewMode::Coordination };
    bool showBulk_{ false }, multiSampling_{ false };
    bool pointsDirty_{ true }, hologramsDirty_{ true };

    Lattice& lattice_;
    Bridge& visualBridge_;

    std::vector<scene::Point> points_;
    std::vector<lattice::Index> highlightedSites_;

    atoms::Shaders shaders_;
    Mesh pointsMesh_;
    Buffer pointsBuffer_;
};
