add_library(kinetalis_dealloying STATIC)
add_library(Kinetalis::dealloying ALIAS kinetalis_dealloying)

target_sources(
    kinetalis_dealloying
    PRIVATE        
        dealloying/models/electrochemical_interface.cpp
        dealloying/models/linear_bond_breaking.cpp
        dealloying/models/composition_dependent.cpp
        dealloying/models/power_law_bond_energy.cpp
        dealloying/analysis/electrochemistry.cpp
        dealloying/models/gtc.cpp
        dealloying/analysis/regime.cpp
)

target_include_directories(
    kinetalis_dealloying
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

target_link_libraries(
    kinetalis_dealloying
    PUBLIC
        Kinetalis::core
)

kinetalis_target_defaults(kinetalis_dealloying)
