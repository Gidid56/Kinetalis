#include "pch.h"

#include "core.h"

#include "gui.h"

#include "lattice/lattice.h"
#include "utility/file.h"
#include "viewer/bridge.h"
#include "viewer/scene/atoms.h"
#include "viewer/viewer.h"

using namespace viewer;
using namespace lattice;
using namespace kmc;
using namespace Magnum::GL;

GUI::GUI(Viewer& app, ImGuiContext* context)
    : Magnum::ImGuiIntegration::Context{ *context,
          Vector2{ app.windowSize() } / app.dpiScaling(),
          app.windowSize(),
          app.framebufferSize() },
      viewer_{ app }
{ }

void GUI::reset()
{
    status_ = {};
    selectedSite_.reset();
    pendingSamples_.clear();

    times_.clear();
    phaseDiameters_.clear();
    phaseFractions_.clear();
    event_fluxes_.clear();
}

void GUI::synchronize()
{
    viewer_.bridge().consumeGui(status_, pendingSamples_);

    for (auto const& sample : pendingSamples_)
    {
        if (sample.snapshot.time == Time{})
            continue;

        times_.emplace_back(sample.snapshot.time / 1_s);

        if (sample.phasesData)
            for (auto phase : output::Phases::active_phases)
            {
                phaseDiameters_[phase].emplace_back(
                    sample.phasesData->correlation_lengths.at(phase) / 1_nm);

                phaseFractions_[phase].emplace_back(sample.phasesData->fractions.at(phase));
            }

        if (sample.eventFluxData)
            for (auto& [event, fluxData] : *sample.eventFluxData)
            {
                if (fluxData.value)
                    event_fluxes_[event].emplace_back(*fluxData.value / 1_Hz);
            }
    }
}

void GUI::draw()
{
    synchronize();
    newFrame();

    ImGui::SetNextWindowPos(ImVec2{ 4, 8 });
    ImGui::Begin("Kinetalis", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    showSimulationControls();
    showSimulationStatus();
    showViewerControls();

    if (viewer_.lattice())
        showLatticeStats(*viewer_.lattice());

    showProcessStatistics();
    showDealloyingAnalysis();

    ImGui::End();

    if (selectedSite_ and viewer_.lattice())
        showSelection(*selectedSite_, *viewer_.lattice());

    updateApplicationCursor(viewer_);

    drawFrame();
}

constexpr auto opened{ ImGuiTreeNodeFlags_DefaultOpen };
constexpr auto table{ ImGuiTableFlags_BordersInner | ImGuiTableFlags_SizingFixedFit };
constexpr auto logarithmic{ ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat };
constexpr auto columns{ 2 }, leftWidth{ 110 }, rightWidth{ 120 };
using namespace ImGui;

void GUI::showSimulationControls()
{
    if (CollapsingHeader("Simulation", opened))
    {
        auto& bridge{ viewer_.bridge() };

        auto selectedTask{ static_cast<int>(bridge.task()) };

        RadioButton("Run", &selectedTask, static_cast<int>(Task::Run));
        SameLine();
        RadioButton("Pause", &selectedTask, static_cast<int>(Task::Pause));
        SameLine();
        RadioButton("Replay", &selectedTask, static_cast<int>(Task::Replay));

        const auto requestedTask{ static_cast<Task>(selectedTask) };

        if (requestedTask != bridge.task())
            bridge.requestTask(requestedTask);

        if (requestedTask == Task::Replay)
        {
            auto stepRate{ bridge.replayRate() };

            PushItemWidth(leftWidth + rightWidth);

            if (SliderInt("", &stepRate, -10'000'000, 10'000'000, "%i steps/s", logarithmic))
            {
                bridge.setReplayRate(stepRate);
            }
        }
    }
}

void GUI::showSimulationStatus()
{
    if (BeginTable("KMC Simulation", columns, table))
    {
        TableSetupColumn({}, {}, leftWidth);
        TableSetupColumn({}, {}, rightWidth);

        TableNextColumn();
        TextUnformatted("Step");
        TableNextColumn();
        TextUnformatted(std::format(locale_, "{:L}", status_.step).c_str());

        TableNextColumn();
        TextUnformatted("Rejections");
        TableNextColumn();
        Text("%.1f %%", 100.0 * status_.rejectionFraction);

        if (status_.step != 0)
        {
            TableNextColumn();
            TextUnformatted("Performance");
            TableNextColumn();
            TextUnformatted(std::format("{:ns;6g}/step", status_.runTime / status_.step).c_str());
        }

        TableNextColumn();
        TextUnformatted("Regime");
        TableNextColumn();
        TextUnformatted(enum_flags_name(status_.reachedConditions, '\n').c_str());

        EndTable();
    }

    if (CollapsingHeader("Physics", opened))
    {
        if (BeginTable("Physics", columns, table))
        {
            TableSetupColumn({}, {}, leftWidth);
            TableSetupColumn({}, {}, rightWidth);

            TableNextColumn();
            TextUnformatted("Time");
            TableNextColumn();
            TextUnformatted(std::format("{:s;.6g}", status_.physicalTime).c_str());

            TableNextColumn();
            TextUnformatted("Temperature");
            TableNextColumn();
            TextUnformatted(std::format("{:;.6g}", status_.temperature).c_str());

            //TableNextColumn(); TextUnformatted("Potential");
            //TableNextColumn(); TextUnformatted(std::format("{:V;.6g}", status_.model.potential()).c_str());

            //TableNextColumn(); TextUnformatted("Concentration");
            //TableNextColumn(); TextUnformatted(std::format("{:mol/l;.6g}", status_.model.concentration()).c_str());

            EndTable();
        }
    }
}

void GUI::showSelection(lattice::Index index, Lattice const& lattice)
{
    auto const& site{ lattice.sites().at(index) };
    auto const state{ viewer_.bridge().state(index) };
    const auto paused{ viewer_.bridge().task() == Task::Pause };

    ImGui::SetNextWindowPos(ImVec2{ viewer_.windowSize().x() - 230.f, 8 });
    ImGui::Begin("Selection", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (CollapsingHeader("Site", opened))
    {
        if (BeginTable("Site", columns, table))
        {
            TableSetupColumn({}, {}, leftWidth - 20);
            TableSetupColumn({}, {}, rightWidth + 20);

            TableNextColumn();
            TextUnformatted("Element");
            TableNextColumn();
            Text("%s (%s)", name(state.element).data(), symbol(state.element).data());

            TableNextColumn();
            TextUnformatted("Coordination");
            TableNextColumn();
            Text("%i / %i", state.coordination, state.maxNeighbors);

            if constexpr (elementSpecificNeighbors)
                if (paused)
                    for (auto element : lattice::activeElements)
                        Text("%s: %i", symbol(element).data(), site.coordination(element));

            TableNextColumn();
            TextUnformatted("Position");
            TableNextColumn();
            TextUnformatted(
                std::format(
                    "x: {:nm;.6g}\ny: {:nm;.6g}\nz: {:nm;.6g}",
                    site.position().x(),
                    site.position().y(),
                    site.position().z())
                    .c_str());

            TableNextColumn();
            TextUnformatted("Index");
            TableNextColumn();
            TextUnformatted(std::format(locale_, "{:L}", index).c_str());

            if constexpr (useLayers)
            {
                TableNextColumn();
                TextUnformatted("Monolayer");
                TableNextColumn();
                TextUnformatted(std::format(locale_, "{:L}", state.layer).c_str());
            }

            EndTable();
        }
    }
    ImGui::End();
}

void GUI::showViewerControls()
{
    if (CollapsingHeader("Viewer", opened))
    {
        auto& atoms{ viewer_.atoms() };

        auto viewMode{ enum_integer(atoms.viewMode()) };
        RadioButton("Elements", &viewMode, std::to_underlying(ViewMode::Elements));
        SameLine();
        RadioButton("Coordination", &viewMode, std::to_underlying(ViewMode::Coordination));

        RadioButton("ID", &viewMode, std::to_underlying(ViewMode::ID));
        SameLine();
        RadioButton("Pristine", &viewMode, std::to_underlying(ViewMode::Pristine));
        if constexpr (useLayers)
        {
            SameLine();
            RadioButton("Layers", &viewMode, std::to_underlying(ViewMode::Layers));
        }

        Separator();
        atoms.set(ViewMode{ viewMode });

        auto showBulk{ atoms.showBulk() };
        Checkbox("Axes", &showAxes_);
        SameLine();
        Checkbox("Bulk", &showBulk);
        atoms.setShowBulk(showBulk);
        SameLine();
        Checkbox("Frame", &showFrame_);

        Separator();
        Text("Camera");
        SameLine();
        if (Button("-x"))
            viewer_.alignCameraTo(-math::Vector<3>::unit_x());
        SameLine();
        if (Button("x"))
            viewer_.alignCameraTo(math::Vector<3>::unit_x());
        SameLine();
        if (Button("-y"))
            viewer_.alignCameraTo(-math::Vector<3>::unit_y());
        SameLine();
        if (Button("y"))
            viewer_.alignCameraTo(math::Vector<3>::unit_y());
        SameLine();
        if (Button("-z"))
            viewer_.alignCameraTo(-math::Vector<3>::unit_z());
        SameLine();
        if (Button("z"))
            viewer_.alignCameraTo(math::Vector<3>::unit_z());
        Separator();

        if (BeginTable("Viewer", columns, table))
        {
            TableSetupColumn({}, {}, leftWidth);
            TableSetupColumn({}, {}, rightWidth);

            TableNextColumn();
            Text("Frame rate");
            TableNextColumn();
            Text("%.0f fps", GetIO().Framerate);

            EndTable();
        }
    }
}

void GUI::showDealloyingAnalysis()
{
    if (CollapsingHeader("Analysis", opened))
    {
        if (TreeNode("Event Fluxes"))
        {
            if (ImPlot::BeginPlot("Event Fluxes", {}))
            {
                ImPlot::SetupAxis(ImAxis_X1, "time (s)", ImPlotAxisFlags_AutoFit);
                ImPlot::SetupAxis(ImAxis_Y1, "rate (Hz)", ImPlotAxisFlags_AutoFit);
                ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Log10);
                ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Log10);
                ImPlot::SetupLegend(ImPlotLocation_NorthWest);

                for (auto&& [event, flux] : event_fluxes_)
                {
                    const auto count{ std::min(times_.size(), flux.size()) };

                    if (count > 1)
                    {
                        ImPlot::PlotLine(
                            enum_name(event).data(),
                            times_.data() + 1,
                            flux.data() + 1,
                            static_cast<int>(count - 1));
                    }
                }
                ImPlot::EndPlot();
            }
            TreePop();
        }

        if (TreeNode("Phase fractions"))
        {
            if (ImPlot::BeginPlot("Phase fractions", {}))
            {
                ImPlot::SetupAxis(ImAxis_X1, "time (s)", ImPlotAxisFlags_AutoFit);
                ImPlot::SetupAxis(ImAxis_Y1, "fraction");
                ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);
                ImPlot::SetupLegend(ImPlotLocation_NorthWest);

                for (auto&& [phase, fractions] : phaseFractions_)
                {
                    ImPlot::PlotLine(
                        enum_name(phase).data(),
                        times_.data(),
                        fractions.data(),
                        static_cast<int>(std::size(times_)));
                }
                ImPlot::EndPlot();
            }
            TreePop();
        }

        if (TreeNode("Phase diameters"))
        {
            if (ImPlot::BeginPlot("Phase diameters", {}))
            {
                ImPlot::SetupAxis(ImAxis_X1, "time (s)", ImPlotAxisFlags_AutoFit);
                ImPlot::SetupAxis(ImAxis_Y1, "diameter (nm)", ImPlotAxisFlags_AutoFit);
                ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Log10);
                ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Log10);
                ImPlot::SetupLegend(ImPlotLocation_NorthWest);

                for (auto&& [phase, diameters] : phaseDiameters_)
                {
                    ImPlot::PlotLine(
                        enum_name(phase).data(),
                        times_.data(),
                        diameters.data(),
                        static_cast<int>(std::size(times_)));
                }
                ImPlot::EndPlot();
            }
            TreePop();
        }
    }
}

void GUI::showProcessStatistics()
{
    if (CollapsingHeader("Process stats", opened))
    {
        constexpr auto maxCoord{ 11 };
        constexpr auto width{ 0.25 };

        for (auto&& [state, processes] : status_.processCounts)
        {
            auto title{ std::format("{} processes", enum_name(state)) };

            if (TreeNode(title.c_str()))
            {
                std::map<Element, std::vector<int>> countMap;

                for (auto& [process, count] : processes)
                {
                    if (not countMap.contains(process.element()))
                        countMap[process.element()].resize(1 + maxCoord);
                    countMap[process.element()][process.coordination()] += (int)count;
                }

                if (ImPlot::BeginPlot(title.c_str(), {}))
                {
                    ImPlot::SetupAxis(ImAxis_X1, "coordination");
                    ImPlot::SetupAxis(ImAxis_Y1, "frequency");

                    const double y_baseline = 1e-1;
                    ImPlot::SetupAxisLimits(ImAxis_Y1, y_baseline, 1e9, ImGuiCond_Always);
                    ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Log10);
                    ImPlot::SetupAxisLimits(ImAxis_X1, -0.5, maxCoord + 0.5, ImGuiCond_Always);
                    ImPlot::SetupAxisTicks(ImAxis_X1, 0, maxCoord, 1 + maxCoord);
                    ImPlot::SetupLegend(ImPlotLocation_NorthEast);

                    for (auto offset{ -width / 2 }; auto& [element, counts] : countMap)
                    {
                        ImPlot::PlotBars(
                            name(element).data(),
                            counts.data(),
                            (int)std::size(counts),
                            width,
                            offset);
                        offset += width;
                    }
                    ImPlot::EndPlot();
                }
                TreePop();
            }
        }
    }
}

#include "dialog/windows.h"

void GUI::showLatticeStats(Lattice const& lattice)
{
    if (CollapsingHeader("Lattice", opened))
    {
        if (BeginTable("Lattice", columns, table))
        {
            TableSetupColumn({}, {}, leftWidth);
            TableSetupColumn({}, {}, rightWidth);

            TableNextColumn();
            Text("Lattice sites");
            TableNextColumn();
            Text(std::format(locale_, "{:L}", lattice.sites().size()).c_str());

            TableNextColumn();
            Text("Outer surface");
            TableNextColumn();
            Text(std::format("{:nm;.6g}", lattice.shape().surface_area()).c_str());

            TableNextColumn();
            Text("Total volume");
            TableNextColumn();
            Text(std::format("{:nm;.6g}", lattice.shape().volume()).c_str());

            TableNextColumn();
            Text("Periodic bound.");
            TableNextColumn();
            Text(enum_flags_name(lattice.periodic_boundaries(), ',').c_str());

            EndTable();
        }
        BeginDisabled(viewer_.bridge().task() != Task::Pause);

        if (Button("Save Snapshot"))
        {
            // Define filter list
            const COMDLG_FILTERSPEC filters[] = {
                { L"XYZ Files (*.xyz)", L"*.xyz" },
                { L"All Files (*.*)", L"*.*" },
            };

            if (auto pathString{ ShowSaveFileDialog_Windows(
                    L"Save Lattice",
                    L"lattice",
                    L"xyz",
                    filters,
                    (UINT)std::size(filters)) })
            {
                std::filesystem::path path{ *pathString };
                utility::file::write<utility::file::Type::xyz>(
                    path,
                    lattice.to_xyz_file(viewer_.atoms().showBulk()));
            }
        }
        EndDisabled();
    }
}
