cmake_minimum_required(VERSION 3.10)

project(mfmt-tools)

# if(WIN32)
#     find_package(PDCurses CONFIG REQUIRED)
# else()
#     find_package(Curses REQUIRED)
# endif()

add_executable(aatls-cli 
    ${CMAKE_SOURCE_DIR}/deps/stb_image/src/stb_image.c
    ${CMAKE_SOURCE_DIR}/deps/stb_image_write/src/stb_image_write.c
    aatls-cli/src/main.c
)

target_link_libraries(aatls-cli PRIVATE 
    aparse
    aatls
)
# if(WIN32)
#     target_link_libraries(aatls-cli PRIVATE PDCurses::PDCurses)
# else()
#     target_include_directories(aatls-cli PRIVATE ${CURSES_INCLUDE_DIR})
#     target_link_libraries(aatls-cli PRIVATE ${CURSES_LIBRARIES})
# endif()

target_include_directories(aatls-cli PRIVATE 
    ${CMAKE_SOURCE_DIR}/deps/stb_image/include
    ${CMAKE_SOURCE_DIR}/deps/stb_image_write/include
)
target_add_asan(aatls-cli)

add_executable(msav-cli msav-cli/src/main.c)
target_link_libraries(msav-cli PRIVATE 
    minizip
    msav
)
target_add_asan(msav-cli)
