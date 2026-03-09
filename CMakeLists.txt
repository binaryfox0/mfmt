cmake_minimum_required(VERSION 3.10)

project(mfmt)

add_subdirectory(deps/aparse)
set(BUILD_TESTING OFF)
add_subdirectory(deps/zlib-ng)
add_subdirectory(deps/minizip-ng)

function(target_add_asan target)
    if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE
            $<$<CONFIG:Debug>:-fsanitize=address -fno-omit-frame-pointer>
            $<$<CONFIG:RelWithDebInfo>:-fsanitize=address -fno-omit-frame-pointer>
        )
        target_link_options(${target} PRIVATE
            $<$<CONFIG:Debug>:-fsanitize=address>
            $<$<CONFIG:RelWithDebInfo>:-fsanitize=address>
        )
        message(STATUS "ASan configured for Debug/RelWithDebInfo on ${target}")
    endif()
endfunction()

add_library(mfmtio
    mfmtio/src/mfmtio.c
)
target_include_directories(mfmtio PUBLIC
    mfmtio/include
)

add_library(msav 
    msav/src/msav_arr.c
    msav/src/msav_object.c
    msav/src/msav.c
)
# temporarily public for development purposes
target_link_libraries(msav PUBLIC 
    aparse
    zlib
    mfmtio
)
target_include_directories(msav PUBLIC
    msav/include
)
target_add_asan(msav)

add_library(aatls
    aatls/src/aatls.c
    aatls/src/aatls_arr.c
)
target_include_directories(aatls PUBLIC 
    aatls/include
)
target_link_libraries(aatls PRIVATE
    mfmtio
)
target_add_asan(aatls)

add_subdirectory(tools)
