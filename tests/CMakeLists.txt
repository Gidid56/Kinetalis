add_executable(
    kinetalis_tests
    main.cpp
    kmc.cpp
    kmc/simulation.cpp
    kmc/rate_tree.cpp
    lattice.cpp
    lattice/geometry.cpp
    lattice/ordering.cpp
    lattice/site.cpp
)

add_subdirectory(chemistry)
add_subdirectory(math)
add_subdirectory(physics)

target_link_libraries(
    kinetalis_tests
    PRIVATE
        Kinetalis::core
)

kinetalis_target_defaults(kinetalis_tests)

add_test(NAME kinetalis_tests COMMAND kinetalis_tests)
