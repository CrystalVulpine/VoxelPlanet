cmake_minimum_required(VERSION 3.0)
project(voxelplanet)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set(CMAKE_CXX_EXTENSIONS false)
set_property(GLOBAL PROPERTY USE_FOLDERS true)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(VoxelPlanet)
add_subdirectory(VoxelPlanet.ColorChanger)
add_subdirectory(VoxelPlanet.Disintegration)
add_subdirectory(VoxelPlanet.PoliticalCompass)
add_subdirectory(VoxelPlanet.TestMod)
