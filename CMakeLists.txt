cmake_minimum_required(VERSION 3.0)
project(voxelplanet)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(VoxelPlanet)
add_subdirectory(VoxelPlanet.TestMod)
