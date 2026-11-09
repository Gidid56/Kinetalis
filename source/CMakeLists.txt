find_package(Threads REQUIRED)

add_library(kinetalis_core STATIC)
add_library(Kinetalis::core ALIAS kinetalis_core)

target_sources(
    kinetalis_core
    PRIVATE
        lattice/lattice.cpp
        lattice/path.cpp
        lattice/shape.cpp
        lattice/site.cpp
        lattice/unitcell.cpp

        kmc/model.cpp
        kmc/process.cpp
        kmc/simulation.cpp
        kmc/observer.cpp

        models/electrochemical_interface.cpp
        models/composition_dependent.cpp
        models/linear_bond_breaking.cpp
        models/power_law_bond_energy.cpp

        output/writer.cpp
        output/basics.cpp
        output/phases.cpp
        output/surface.cpp
        output/event_flux.cpp
        output/process_stats.cpp
        output/process_kinetics.cpp
        utility/csv.cpp

        random/engine.cpp
)

target_include_directories(
    kinetalis_core
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_link_libraries(
    kinetalis_core
    PUBLIC
        Kinetalis::magic_enum
        Threads::Threads
)

kinetalis_target_defaults(kinetalis_core)

add_library(kinetalis_batch_runtime STATIC)
add_library(Kinetalis::batch ALIAS kinetalis_batch_runtime)

target_sources(
    kinetalis_batch_runtime
    PRIVATE
        batch/statistics/aggregate.cpp
        batch/statistics/merge.cpp
        batch/terminal.cpp
        batch/runner.cpp
)

target_link_libraries(
    kinetalis_batch_runtime
    PUBLIC
        Kinetalis::core
)

kinetalis_target_defaults(kinetalis_batch_runtime)

if(KINETALIS_BUILD_VIEWER)
    find_package(Corrade REQUIRED Main PluginManager)

    find_package(
        Magnum REQUIRED
        GL
        MeshTools
        Primitives
        SceneGraph
        Sdl2Application
        Shaders
        Text
        Trade
    )

    find_package(MagnumIntegration REQUIRED ImGui)

    add_library(
        kinetalis_implot STATIC
        "${PROJECT_SOURCE_DIR}/external/implot/implot.cpp"
        "${PROJECT_SOURCE_DIR}/external/implot/implot_items.cpp"
    )
    add_library(Kinetalis::implot ALIAS kinetalis_implot)

    target_include_directories(
        kinetalis_implot
        SYSTEM PUBLIC
            "${PROJECT_SOURCE_DIR}/external/implot"
    )

    target_link_libraries(
        kinetalis_implot
        PUBLIC
            MagnumIntegration::ImGui
    )

    add_library(kinetalis_viewer_runtime STATIC)
    add_library(Kinetalis::viewer ALIAS kinetalis_viewer_runtime)

    target_sources(
        kinetalis_viewer_runtime
        PRIVATE
            viewer/bridge.cpp
            viewer/gui.cpp
            viewer/sample.cpp
            viewer/trajectory.cpp
            viewer/viewer.cpp

            viewer/scene/atoms.cpp
            viewer/scene/axes.cpp
            viewer/scene/camera.cpp
            viewer/scene/frame.cpp
            viewer/scene/point.cpp
            viewer/scene/shaders.cpp

            "${PROJECT_SOURCE_DIR}/resources/shaders/shader.frag"
            "${PROJECT_SOURCE_DIR}/resources/shaders/shader.vert"
    )

    if(WIN32)
        target_sources(
            kinetalis_viewer_runtime
            PRIVATE
                viewer/dialog/windows.cpp
        )
    endif()

    set_source_files_properties(
        "${PROJECT_SOURCE_DIR}/resources/shaders/shader.frag"
        "${PROJECT_SOURCE_DIR}/resources/shaders/shader.vert"
        PROPERTIES HEADER_FILE_ONLY TRUE
    )

    target_link_libraries(
        kinetalis_viewer_runtime
        PUBLIC
            Kinetalis::core
            Kinetalis::implot
            Corrade::Main
            Corrade::PluginManager
            Magnum::Magnum
            Magnum::GL
            Magnum::MeshTools
            Magnum::Primitives
            Magnum::SceneGraph
            Magnum::Sdl2Application
            Magnum::Shaders
            Magnum::Text
            Magnum::Trade
            MagnumIntegration::ImGui
    )

    kinetalis_target_defaults(kinetalis_viewer_runtime)
endif()
