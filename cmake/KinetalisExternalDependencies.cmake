include_guard(GLOBAL)

function(kinetalis_require_submodule name path marker)
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/${path}/${marker}")
        message(
            FATAL_ERROR
            "Missing ${name} submodule at ${path}.\n"
            "Initialize the external dependencies with:\n"
            "    git submodule update --init --recursive\n"
            "For a repository converted from an archive, run one of:\n"
            "    tools/setup_submodules.ps1\n"
            "    tools/setup_submodules.sh"
        )
    endif()
endfunction()

macro(kinetalis_add_external_dependencies)
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/magic_enum/magic_enum.hpp")
        message(FATAL_ERROR "Missing vendored magic_enum headers at external/magic_enum.")
    endif()

    add_library(kinetalis_magic_enum INTERFACE)
    add_library(Kinetalis::magic_enum ALIAS kinetalis_magic_enum)

    target_include_directories(
        kinetalis_magic_enum
        SYSTEM INTERFACE
            "${PROJECT_SOURCE_DIR}/external"
    )

    if(KINETALIS_BUILD_VIEWER)
        kinetalis_require_submodule(Corrade external/corrade CMakeLists.txt)
        kinetalis_require_submodule(Magnum external/magnum CMakeLists.txt)
        kinetalis_require_submodule(
            "Magnum Integration"
            external/magnum-integration
            CMakeLists.txt
        )
        kinetalis_require_submodule(SDL2 external/SDL CMakeLists.txt)
        kinetalis_require_submodule(Dear-ImGui external/imgui imgui.h)
        kinetalis_require_submodule(ImPlot external/implot implot.h)

        list(
            PREPEND CMAKE_MODULE_PATH
            "${PROJECT_SOURCE_DIR}/external/corrade/modules"
            "${PROJECT_SOURCE_DIR}/external/magnum/modules"
            "${PROJECT_SOURCE_DIR}/external/magnum-integration/modules"
        )

        set(CORRADE_BUILD_STATIC ON CACHE BOOL "" FORCE)
        set(CORRADE_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(CORRADE_WITH_INTERCONNECT OFF CACHE BOOL "" FORCE)
        set(CORRADE_WITH_PLUGINMANAGER ON CACHE BOOL "" FORCE)
        set(CORRADE_WITH_TESTSUITE OFF CACHE BOOL "" FORCE)
        set(CORRADE_WITH_UTILITY ON CACHE BOOL "" FORCE)

        add_subdirectory(
            "${PROJECT_SOURCE_DIR}/external/corrade"
            "${PROJECT_BINARY_DIR}/external/corrade"
            EXCLUDE_FROM_ALL
            SYSTEM
        )

        set(SDL_ATOMIC OFF CACHE BOOL "" FORCE)
        set(SDL_AUDIO OFF CACHE BOOL "" FORCE)
        set(SDL_CPUINFO OFF CACHE BOOL "" FORCE)
        set(SDL_FILE OFF CACHE BOOL "" FORCE)
        set(SDL_FILESYSTEM OFF CACHE BOOL "" FORCE)
        set(SDL_HAPTIC OFF CACHE BOOL "" FORCE)
        set(SDL_LOCALE OFF CACHE BOOL "" FORCE)
        set(SDL_POWER OFF CACHE BOOL "" FORCE)
        set(SDL_RENDER OFF CACHE BOOL "" FORCE)
        set(SDL_SENSOR OFF CACHE BOOL "" FORCE)
        set(SDL_SHARED OFF CACHE BOOL "" FORCE)
        set(SDL_STATIC ON CACHE BOOL "" FORCE)

        add_subdirectory(
            "${PROJECT_SOURCE_DIR}/external/SDL"
            "${PROJECT_BINARY_DIR}/external/SDL"
            EXCLUDE_FROM_ALL
            SYSTEM
        )

        set(MAGNUM_BUILD_STATIC ON CACHE BOOL "" FORCE)
        set(MAGNUM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(MAGNUM_BUILD_GL_TESTS OFF CACHE BOOL "" FORCE)
        set(MAGNUM_WITH_GL ON CACHE BOOL "" FORCE)
        set(MAGNUM_WITH_MESHTOOLS ON CACHE BOOL "" FORCE)
        set(MAGNUM_WITH_PRIMITIVES ON CACHE BOOL "" FORCE)
        set(MAGNUM_WITH_SCENEGRAPH ON CACHE BOOL "" FORCE)
        set(MAGNUM_WITH_SDL2APPLICATION ON CACHE BOOL "" FORCE)
        set(MAGNUM_WITH_SHADERS ON CACHE BOOL "" FORCE)
        set(MAGNUM_WITH_TEXT ON CACHE BOOL "" FORCE)
        set(MAGNUM_WITH_TRADE ON CACHE BOOL "" FORCE)

        add_subdirectory(
            "${PROJECT_SOURCE_DIR}/external/magnum"
            "${PROJECT_BINARY_DIR}/external/magnum"
            EXCLUDE_FROM_ALL
            SYSTEM
        )

        set(IMGUI_DIR "${PROJECT_SOURCE_DIR}/external/imgui" CACHE PATH "" FORCE)
        set(MAGNUM_WITH_IMGUIINTEGRATION ON CACHE BOOL "" FORCE)

        add_subdirectory(
            "${PROJECT_SOURCE_DIR}/external/magnum-integration"
            "${PROJECT_BINARY_DIR}/external/magnum-integration"
            EXCLUDE_FROM_ALL
            SYSTEM
        )
    endif()
endmacro()
