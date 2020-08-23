add_library(disintegration MODULE mod.cpp)

target_compile_features(disintegration PRIVATE cxx_std_17)
target_compile_options(disintegration PRIVATE -Wall -Wextra -pedantic)

target_include_directories(disintegration PRIVATE ../VoxelPlanet/src)
target_link_libraries(disintegration voxelplanet CONAN_PKG::glm)
