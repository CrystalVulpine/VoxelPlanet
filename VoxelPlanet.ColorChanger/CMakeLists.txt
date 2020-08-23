add_library(colorchanger MODULE mod.cpp)

target_compile_features(colorchanger PRIVATE cxx_std_17)
target_compile_options(colorchanger PRIVATE -Wall -Wextra -pedantic)

target_include_directories(colorchanger PRIVATE ../VoxelPlanet/src)
target_link_libraries(colorchanger voxelplanet CONAN_PKG::glm)
