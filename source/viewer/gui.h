#pragma once
#include "sample.h"
#include "output/phases.h"

class viewer::GUI : public Magnum::ImGuiIntegration::Context
{
public:
    explicit GUI(Viewer&, ImGuiContext*);

    void synchronize();
    void draw();

    void reset();

    void showViewerControls();
    void showSimulationControls();
    void showSimulationStatus();
    void showProcessStatistics();
    void showDealloyingAnalysis();
    void showLatticeStats(Lattice const&);
    void showSelection(lattice::Index, Lattice const&);

    void highlight(lattice::Index index) { selectedSite_ = index; }
    void clearHighlight() { selectedSite_.reset(); }

    auto showAtoms() const { return showAtoms_; }
    auto showFrame() const { return showFrame_; }
    auto showPaths() const { return showPaths_; }
    auto showAxes() const { return showAxes_; }

    auto& viewer() { return viewer_; }

private:
    Viewer& viewer_;

    gui::Status status_{};
    std::vector<gui::Sample> pendingSamples_;

    bool showAtoms_{ true }, showFrame_{ true }, showPaths_{ false }, showAxes_{ false };

    std::optional<lattice::Index> selectedSite_;

    std::vector<double> times_;

    std::map<output::Phases::Phase, std::vector<double>> phaseFractions_, phaseDiameters_;
    std::map<kmc::Event, std::vector<double>> event_fluxes_;

    std::locale locale_{ "" };
};
