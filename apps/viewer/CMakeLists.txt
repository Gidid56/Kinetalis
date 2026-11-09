add_executable(kinetalis_viewer main.cpp)

target_link_libraries(
    kinetalis_viewer
    PRIVATE
        Kinetalis::viewer
        Kinetalis::dealloying
)

kinetalis_target_defaults(kinetalis_viewer)

set_target_properties(
    kinetalis_viewer
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:kinetalis_viewer>"
)

add_custom_command(
    TARGET kinetalis_viewer
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_FILE_DIR:kinetalis_viewer>/resources/shaders"
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_FILE_DIR:kinetalis_viewer>/resources/fonts"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${PROJECT_SOURCE_DIR}/resources/shaders/shader.vert"
        "${PROJECT_SOURCE_DIR}/resources/shaders/shader.frag"
        "$<TARGET_FILE_DIR:kinetalis_viewer>/resources/shaders"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${PROJECT_SOURCE_DIR}/resources/fonts/Inter Regular.ttf"
        "${PROJECT_SOURCE_DIR}/resources/fonts/IBM Plex Sans Regular.ttf"
        "$<TARGET_FILE_DIR:kinetalis_viewer>/resources/fonts/"
    VERBATIM
)
