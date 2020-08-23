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

# Install the mods in the `mods/` subdirectory of the application whether we are in release or debug mode
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/mods)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

add_subdirectory(VoxelPlanet)
add_subdirectory(VoxelPlanet.ColorChanger)
add_subdirectory(VoxelPlanet.Disintegration)
add_subdirectory(VoxelPlanet.PoliticalCompass)
add_subdirectory(VoxelPlanet.TestMod)
