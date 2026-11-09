cmake_minimum_required(VERSION 3.22)

project(
    Kinetalis
    VERSION 0.5.1
    DESCRIPTION "Kinetic Monte Carlo simulation framework"
    LANGUAGES C CXX
)

option(KINETALIS_BUILD_BATCH "Build the batch application" ON)
option(KINETALIS_BUILD_VIEWER "Build the interactive Magnum viewer" OFF)
option(KINETALIS_BUILD_TESTS "Build the Kinetalis test executable" ON)
option(KINETALIS_ENABLE_PCH "Use precompiled headers" ON)
option(KINETALIS_ENABLE_CLANG_TIDY "Run clang-tidy on Kinetalis sources" OFF)
option(KINETALIS_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/$<CONFIG>/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/$<CONFIG>/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/$<CONFIG>/lib")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(KinetalisExternalDependencies)
include(KinetalisTargetDefaults)

if(KINETALIS_ENABLE_CLANG_TIDY)
    find_program(
        KINETALIS_CLANG_TIDY
        NAMES clang-tidy
        REQUIRED
    )

    message(STATUS "Using clang-tidy: ${KINETALIS_CLANG_TIDY}")
endif()

kinetalis_add_external_dependencies()

add_subdirectory(apps)
add_subdirectory(source)
add_subdirectory(studies)
add_subdirectory(tools)

if(TARGET kinetalis_batch)
    find_program(SBATCH_EXECUTABLE sbatch)

    if(SBATCH_EXECUTABLE)
        add_custom_target(submit_batch
            COMMAND
                ${CMAKE_COMMAND} -E make_directory "$ENV{HOME}/slurm"
            COMMAND
                ${SBATCH_EXECUTABLE}
                --chdir=$ENV{HOME}
                ${CMAKE_SOURCE_DIR}/cmake/slurm_batch.sh
                $<TARGET_FILE:kinetalis_batch>
            COMMENT "Submitting batch job to Slurm"
            VERBATIM
        )
    endif()
endif()

if(KINETALIS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
