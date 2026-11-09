if(KINETALIS_BUILD_TESTS)
    add_executable(
        physics_literals_generator
        EXCLUDE_FROM_ALL
        literals_generator.cpp
    )

    target_include_directories(
        physics_literals_generator
        PRIVATE
            "${PROJECT_SOURCE_DIR}/source"
    )

    kinetalis_target_defaults(physics_literals_generator)

    set_target_properties(
        physics_literals_generator
        PROPERTIES
            DISABLE_PRECOMPILE_HEADERS ON
            VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/source"
    )

    add_custom_target(
        generate_physics_literals
        COMMAND
            $<TARGET_FILE:physics_literals_generator>
        WORKING_DIRECTORY
            "${PROJECT_SOURCE_DIR}/source"
        DEPENDS
            physics_literals_generator
    )
endif()
