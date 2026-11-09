include_guard(GLOBAL)

function(kinetalis_target_defaults target)
    target_compile_features(${target} PUBLIC cxx_std_23)

    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8)

        if(KINETALIS_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)

        if(KINETALIS_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()

    if(KINETALIS_ENABLE_PCH)
        target_precompile_headers(
            ${target}
            PRIVATE
                "${PROJECT_SOURCE_DIR}/source/pch.h"
        )
    endif()

    if(KINETALIS_ENABLE_CLANG_TIDY)
        set_property(
            TARGET ${target}
            PROPERTY CXX_CLANG_TIDY
                "${KINETALIS_CLANG_TIDY}"
                "--extra-arg=/EHsc"
        )
    endif()
endfunction()
