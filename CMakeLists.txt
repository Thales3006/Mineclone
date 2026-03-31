cmake_minimum_required(VERSION 3.16)
project(mineclone)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    src/*.c
)

add_executable(mineclone ${SOURCES})

target_include_directories(mineclone PRIVATE src)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(mineclone
    glfw
    OpenGL::GL
)

install(TARGETS mineclone
        RUNTIME DESTINATION bin)

install(DIRECTORY shaders
        DESTINATION bin)

install(DIRECTORY textures
        DESTINATION bin)
