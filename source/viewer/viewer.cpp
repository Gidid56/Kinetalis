#include "pch.h"

#include "core.h"

#include "viewer.h"

#include "bridge.h"
#include "gui.h"
#include "lattice/lattice.h"
#include "scene/atoms.h"
#include "scene/axes.h"
#include "scene/camera.h"
#include "scene/frame.h"
#include "trajectory.h"

int Viewer::argc{ 0 };
char** Viewer::argv{ nullptr };

using namespace viewer;
using namespace kmc;
using namespace Magnum::Math::Literals;
using namespace Magnum::GL;

Viewer::Viewer(const Arguments& arguments)
    : Magnum::Platform::Application{
          arguments,
          Configuration{}
              .setWindowFlags(Configuration::WindowFlag::Resizable)
              .setTitle("Kinetalis")
              .setSize({ 1440, 1080 }),
          GLConfiguration{}.addFlags(GLConfiguration::Flag::QuietLog)
          //.setSampleCount(16)
      },
      indexBuffer_{ defaultFramebuffer.viewport() },
      frame_{ nullptr }
{
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImPlot::CreateContext();
    ImPlot::StyleColorsDark();

    constexpr auto fontFile{ "resources/fonts/Inter Regular.ttf" };
    ImGui::GetIO().Fonts->AddFontFromFileTTF(fontFile, 18.0f);

    auto size{ defaultFramebuffer.viewport().size() };
    colorBuffer_.setStorage(RenderbufferFormat::RGBA8, size);
    depthBuffer_.setStorage(RenderbufferFormat::DepthComponent, size);

    indexBuffer_.attachRenderbuffer(Framebuffer::ColorAttachment{ 0 }, colorBuffer_)
        .attachRenderbuffer(Framebuffer::BufferAttachment::Depth, depthBuffer_)
        .mapForDraw({ { GenericGL3D::ColorOutput, Framebuffer::ColorAttachment{ 0 } } })
        .mapForRead(Framebuffer::ColorAttachment{ 0 });

    Renderer::enable(Renderer::Feature::Blending);
    Renderer::enable(Renderer::Feature::DepthTest);
    Renderer::enable(Renderer::Feature::ProgramPointSize);
    Renderer::setBlendFunction(
        Renderer::BlendFunction::SourceAlpha,
        Renderer::BlendFunction::OneMinusSourceAlpha);
    Renderer::setBlendEquation(Renderer::BlendEquation::Add, Renderer::BlendEquation::Add);

    /* Loop at 60 Hz max */
    setSwapInterval(1);
    setMinimalLoopPeriod(16.67_msec);

    camera_ = std::make_unique<camera::ArcBall>(
        Vector3::xAxis(),
        Vector3{},
        Vector3::zAxis(),
        30.0_degf,
        1.E-1f,
        1.E+3f,
        windowSize(),
        framebufferSize());

    axes_ = std::make_unique<scene::Axes>();
    gui_ = std::make_unique<GUI>(*this, ImGui::GetCurrentContext());
}

Viewer::~Viewer()
{
    stopWorker();
    gui_.reset();

    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

template <class Rules>
kmc::Simulation& Viewer::createSimulation(kmc::Setup<Rules> const& setup, kmc::PlanFactory planFactory)
{
    stopWorker();

    simulation_ = std::make_unique<kmc::KmcSimulation<Rules>>(setup);
    analysisPlan_ = planFactory({});

    auto& lattice{ simulation_->mutable_lattice() };

    trajectory_ = &analysisPlan_.add<Trajectory>(lattice);
    visualBridge_ = &analysisPlan_.add<Bridge>(lattice, analysisPlan_);

    trajectory_->setViewerBridge(visualBridge_);

    analysisPlan_.set_resolution({ .min_snapshot_step = 100, .snapshot_scale_factor = 1.05 });

    load(lattice);

    std::print(
        "\nCreated KMC simulation.\n\n{}\n{}\n{}\n",
        simulation_->lattice(),
        simulation_->model()->output_text(),
        simulation_->rng());

    return *simulation_;
}

Viewer& Viewer::load(Lattice& lattice)
{
    lattice_ = &lattice;

    alignCameraTo(math::Vector<3>::unit_x());

    atoms_ = std::make_unique<scene::Atoms>(lattice, *visualBridge_);
    frame_ = std::make_unique<scene::Frame>(lattice.shape());

    gui_->reset();

    return *this;
}

Viewer& Viewer::setTask(Task task)
{
    if (not simulation_ or not visualBridge_)
        return *this;

    if (task == Task::Pause)
    {
        stopWorker();
        visualBridge_->publishTask(Task::Pause);
        return *this;
    }

    if (task == visualBridge_->task() and worker_.joinable())
    {
        return *this;
    }

    stopWorker();

    switch (task)
    {
        case Task::Run: {
            auto* simulation{ simulation_.get() };

            visualBridge_->publishTask(Task::Run);

            worker_ = std::jthread{ [this, simulation](std::stop_token token) {
                try
                {
                    trajectory_->replayAll(token);

                    if (not token.stop_requested())
                        simulation->run(analysisPlan_, token);
                }
                catch (const std::exception& exception)
                {
                    std::print("\nError: {}\n\n", exception.what());
                }
                visualBridge_->publishTask(Task::Pause);
            } };
            break;
        }

        case Task::Replay: {
            visualBridge_->publishTask(Task::Replay);

            worker_ = std::jthread{ [this](std::stop_token token) {
                trajectory_->replay(token);
                visualBridge_->publishTask(Task::Pause);
            } };
            break;
        }

        case Task::Pause: break;
    }

    return *this;
}

void Viewer::stopWorker()
{
    if (not worker_.joinable())
        return;

    worker_.request_stop();
    worker_.join();
}

void Viewer::consumeControlRequest()
{
    if (not visualBridge_)
        return;

    if (const auto task{ visualBridge_->consumeTaskRequest() })
    {
        setTask(*task);
    }
}

Viewer& Viewer::alignCameraTo(math::Vector<3> axis)
{
    auto upAxis{ axis.cross(math::Vector<3>::unit_z()) == math::Vector<3>{}
                     ? math::Vector<3>::unit_y()
                     : math::Vector<3>::unit_z() };
    auto topSurface{ lattice_->shape().vectors().dot(axis).normalized() *
                     lattice_->shape().vectors().dot(upAxis).norm() /
                     Magnum::Math::tan(camera_->fieldOfView() / 2) };
    const auto center{ toMagnum(lattice_->shape().center()) };

    camera_->setViewParameters(center + toMagnum(topSurface), center, toMagnum(upAxis));

    return *this;
}

void Viewer::drawEvent()
{
    if (visualBridge_ and atoms_)
    {
        const auto changed{ visualBridge_->consume(
            [this](lattice::Index index, SiteState state) { atoms_->updatePoint(index, state); }) };
        if (changed)
            atoms_->markDirty();
    }
    camera_->update();

    defaultFramebuffer.clear(FramebufferClear::Color | FramebufferClear::Depth).bind();
    Renderer::enable(Renderer::Feature::Blending);

    if (gui_->showAtoms() and atoms_)
        atoms_->draw(camera_->camera());
    if (gui_->showFrame() and frame_)
        frame_->draw(camera_->camera());
    if (gui_->showAxes() and axes_)
        axes_->draw(camera_->camera());

    Renderer::disable(Renderer::Feature::DepthTest);
    //Renderer::enable(Renderer::Feature::ScissorTest);

    gui_->draw();

    consumeControlRequest();

    //Renderer::disable(Renderer::Feature::ScissorTest);
    Renderer::enable(Renderer::Feature::DepthTest);

    swapBuffers();
    redraw();
}

void Viewer::viewportEvent(ViewportEvent& event)
{
    defaultFramebuffer.setViewport({ {}, event.framebufferSize() });
    indexBuffer_.setViewport({ {}, event.framebufferSize() });
    colorBuffer_.setStorage(RenderbufferFormat::RGBA8, event.framebufferSize());
    depthBuffer_.setStorage(RenderbufferFormat::DepthComponent, event.framebufferSize());

    gui_->relayout(
        Vector2{ event.windowSize() } / event.dpiScaling(),
        event.windowSize(),
        event.framebufferSize());

    camera_->reshape(event.windowSize(), event.framebufferSize());
}

using Pointer = Magnum::Platform::Sdl2Application::Pointer;

void Viewer::pointerPressEvent(PointerEvent& event)
{
    if (gui_->handlePointerPressEvent(event))
        return;

    if (event.pointer() != Pointer::MouseLeft)
        return;

    camera_->initTransformation(event.position());
}

void Viewer::pointerMoveEvent(PointerMoveEvent& event)
{
    if (gui_->handlePointerMoveEvent(event))
        return;

    if (not(event.pointers() & Pointer::MouseLeft))
        return;

    if (event.modifiers() & PointerMoveEvent::Modifier::Shift)
        camera_->translate(event.position());
    else
        camera_->rotate(event.position());
}

void Viewer::pointerReleaseEvent(PointerEvent& event)
{
    if (gui_->handlePointerReleaseEvent(event))
        return;

    if (event.pointer() == Pointer::MouseRight)
    {
        atoms_->resetHighlights();
        gui_->clearHighlight();
    }
    else if (event.pointer() == Pointer::MouseLeft)
    {
        auto time{ std::chrono::steady_clock::now() };

        if (time - lastMouseReleaseTime_ <= std::chrono::milliseconds{ 500 } and
            (event.position() - lastMouseClickPosition_).length() <= 10)
        {
            doubleClickEvent(event);

            lastMouseReleaseTime_ = {};
            lastMouseClickPosition_ = {};
        }
        else
        {
            lastMouseReleaseTime_ = time;
            lastMouseClickPosition_ = event.position();
        }
    }
}

void Viewer::doubleClickEvent(PointerEvent& event)
{
    indexBuffer_.clearColor(0, Magnum::Vector4ui{}).clear(FramebufferClear::Depth).bind();
    atoms_->draw(camera_->camera(), true);

    Vector2 const framebufferPosition =
        event.position() * Vector2{ framebufferSize() } / Vector2{ windowSize() };

    /* Rundung auf Pixelkoordinate */
    Vector2i roundedPosition = Vector2i{ Magnum::Math::round(framebufferPosition) };

    /* Y-Achse flippen (Framebuffer origin unten) */
    roundedPosition.y() = indexBuffer_.viewport().sizeY() - roundedPosition.y() - 1;

    auto color{ indexBuffer_
            .read(
                Magnum::Range2Di::fromSize(roundedPosition, { 1, 1 }),
                { Magnum::PixelFormat::RGBA8Unorm })
            .pixels<Magnum::Color4ub>()[0][0]
            .rgb() };
    auto index{ (color.r() << 16) + (color.g() << 8) + (color.b() << 0) - 1 };

    if (index >= 0 and index < lattice_->sites().size())
    {
        auto const siteIndex{ static_cast<lattice::Index>(index) };
        visualBridge_->toggleHighlighted(siteIndex);
        gui_->highlight(siteIndex);
    }
}

void Viewer::keyPressEvent(KeyEvent& event)
{
    if (gui_->handleKeyPressEvent(event))
        return;

    switch (event.key())
    {
        break;
        case KeyEvent::Key::Home: camera_->reset(); break;
        case KeyEvent::Key::X: alignCameraTo(math::Vector<3>::unit_x()); break;
        case KeyEvent::Key::Y: alignCameraTo(math::Vector<3>::unit_y()); break;
        case KeyEvent::Key::Z: alignCameraTo(math::Vector<3>::unit_z());
    }
}

void Viewer::keyReleaseEvent(KeyEvent& event)
{
    if (gui_->handleKeyReleaseEvent(event))
        return;
}

void Viewer::textInputEvent(TextInputEvent& event)
{
    if (gui_->handleTextInputEvent(event))
        return;
}

void Viewer::scrollEvent(ScrollEvent& event)
{
    if (gui_->handleScrollEvent(event))
        return;

    const auto delta = event.offset().y();
    if (abs(delta) < 1.0e-2f)
        return;

    camera_->zoom(1.0f - 0.1f * delta);
}
