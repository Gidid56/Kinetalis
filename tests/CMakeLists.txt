# Compile each module once, and link its objects into both the module test
# executable and the combined test_all executable.
function(kinetalis_add_test_suite suite)
    set(objects kinetalis_test_${suite}_objects)
    set(executable kinetalis_test_${suite})

    add_library(${objects} OBJECT ${ARGN})
    target_link_libraries(${objects} PRIVATE Kinetalis::core)
    kinetalis_target_defaults(${objects})

    add_executable(${executable} "${PROJECT_SOURCE_DIR}/tests/main.cpp")
    target_link_libraries(${executable} PRIVATE Kinetalis::core ${objects})
    kinetalis_target_defaults(${executable})

    add_test(NAME ${suite} COMMAND ${executable})
    set_property(GLOBAL APPEND PROPERTY KINETALIS_TEST_OBJECTS ${objects})
    set_property(GLOBAL APPEND PROPERTY KINETALIS_TEST_EXECUTABLES ${executable})
endfunction()

kinetalis_add_test_suite(kmc
    kmc/rate_tree.cpp
    kmc/simulation.cpp
)
kinetalis_add_test_suite(lattice
    lattice/unitcell.cpp
    lattice/geometry.cpp
    lattice/ordering.cpp
    lattice/site.cpp
)
add_subdirectory(chemistry)
add_subdirectory(models)
add_subdirectory(simulation)
add_subdirectory(math)
add_subdirectory(physics)

# A real executable, visible as a startup target in Visual Studio.
# It contains exactly the same suites as the individual CTest executables.
get_property(test_objects GLOBAL PROPERTY KINETALIS_TEST_OBJECTS)
add_executable(test_all "${PROJECT_SOURCE_DIR}/tests/main.cpp")
target_link_libraries(test_all PRIVATE Kinetalis::core ${test_objects})
kinetalis_target_defaults(test_all)

# Build everything; executing tests remains an explicit user action.
get_property(test_executables GLOBAL PROPERTY KINETALIS_TEST_EXECUTABLES)
add_custom_target(kinetalis_tests DEPENDS test_all ${test_executables})
