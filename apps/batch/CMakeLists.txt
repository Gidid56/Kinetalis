add_executable(kinetalis_batch main.cpp)

target_link_libraries(
    kinetalis_batch
    PRIVATE
        Kinetalis::batch
        Kinetalis::dealloying
)

kinetalis_target_defaults(kinetalis_batch)
