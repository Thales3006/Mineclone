cmake_minimum_required(VERSION 3.16)
project(mineclone)

set(CMAKE_CXX_STANDARD 17)

find_package(PalSigslot REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad STATIC lib/glad/glad.c)
target_include_directories(glad PUBLIC lib)

add_library(fastnoise INTERFACE)
target_include_directories(fastnoise INTERFACE lib)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

add_executable(mineclone ${SOURCES})

target_include_directories(mineclone PRIVATE src)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(mineclone
    glad
    glfw
    OpenGL::GL
    fastnoise
    Pal::Sigslot
)

install(TARGETS mineclone
        RUNTIME DESTINATION bin)

install(DIRECTORY shaders
        DESTINATION bin)

install(DIRECTORY textures
        DESTINATION bin)
