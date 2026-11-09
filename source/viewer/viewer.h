#pragma once
#include "kmc/analysis_plan.h"
#include "kmc/setup.h"
#include "kmc/simulation.h"
#include "states.h"

class viewer::Viewer : public Magnum::Platform::Application
{
    static int argc;
    static char** argv;

public:
    explicit Viewer(const Arguments& = { argc, argv });
    ~Viewer();

    kmc::Simulation& createSimulation(kmc::Setup const&, kmc::PlanFactory);

    Viewer& load(Lattice&);
    Viewer& setTask(Task);
    Viewer& run() { return setTask(Task::Run); }
    Viewer& alignCameraTo(math::Vector<3> axis);

    auto& atoms() const { return *atoms_; }
    auto& bridge() const { return *visualBridge_; }
    auto& gui() const { return *gui_; }

    auto& lattice() const { return lattice_; }
    auto& simulation() const { return simulation_; }
    auto& trajectory() const { return trajectory_; }
    auto& analysisPlan() { return analysisPlan_; }

private:
    void stopWorker();
    void consumeControlRequest();

    void drawEvent() override;
    void viewportEvent(ViewportEvent&) override;
    void pointerPressEvent(PointerEvent&) override;
    void pointerMoveEvent(PointerMoveEvent&) override;
    void pointerReleaseEvent(PointerEvent&) override;
    void scrollEvent(ScrollEvent&) override;
    void keyPressEvent(KeyEvent&) override;
    void keyReleaseEvent(KeyEvent&) override;
    void textInputEvent(TextInputEvent&) override;

    void doubleClickEvent(PointerEvent&);

    std::unique_ptr<scene::Atoms> atoms_;
    std::unique_ptr<scene::Axes> axes_;
    std::unique_ptr<scene::Frame> frame_;
    std::unique_ptr<camera::ArcBall> camera_;
    std::unique_ptr<GUI> gui_;

    std::chrono::steady_clock::time_point lastMouseReleaseTime_{};
    Vector2 lastMouseClickPosition_{};
    Framebuffer indexBuffer_;
    Renderbuffer colorBuffer_, depthBuffer_;

    std::unique_ptr<kmc::Simulation> simulation_{};

    kmc::AnalysisPlan analysisPlan_;
    Bridge* visualBridge_{};
    Trajectory* trajectory_{};

    std::jthread worker_;
    Lattice* lattice_{ nullptr };
};
