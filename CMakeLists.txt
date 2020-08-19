cmake_minimum_required(VERSION 3.0)
project(voxelplanet)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# todo: find a better way to do this
add_definitions("-std=c++17")

file(GLOB sources
    "VoxelPlanet/src/*.cpp"
    "VoxelPlanet/src/*.hpp")

add_executable(voxelplanet ${sources})

target_link_libraries(voxelplanet
    CONAN_PKG::opengl
    CONAN_PKG::glew
    CONAN_PKG::glfw
    CONAN_PKG::glm)

target_compile_options(voxelplanet PRIVATE -Wall -Wextra -pedantic)
