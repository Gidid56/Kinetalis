# Independent test executables provide per-module CTest results and targeted runs.
function(kinetalis_add_test_suite suite)
    set(target kinetalis_test_${suite})
    add_executable(${target} "${PROJECT_SOURCE_DIR}/tests/main.cpp" ${ARGN})
    target_link_libraries(${target} PRIVATE Kinetalis::core)
    kinetalis_target_defaults(${target})
    add_test(NAME ${suite} COMMAND ${target})
    set_property(GLOBAL APPEND PROPERTY KINETALIS_TEST_EXECUTABLES ${target})
endfunction()

kinetalis_add_test_suite(kmc
    kmc.cpp
    kmc/rate_tree.cpp
    kmc/simulation.cpp
)
kinetalis_add_test_suite(lattice
    lattice.cpp
    lattice/geometry.cpp
    lattice/ordering.cpp
    lattice/site.cpp
)
add_subdirectory(chemistry)
add_subdirectory(math)
add_subdirectory(physics)

get_property(test_executables GLOBAL PROPERTY KINETALIS_TEST_EXECUTABLES)
add_custom_target(kinetalis_tests DEPENDS ${test_executables})
