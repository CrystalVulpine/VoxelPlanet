add_library(politicalcompass MODULE mod.cpp)

target_compile_features(politicalcompass PRIVATE cxx_std_17)
target_compile_options(politicalcompass PRIVATE -Wall -Wextra -pedantic)

target_include_directories(politicalcompass PRIVATE ../VoxelPlanet/src)
target_link_libraries(politicalcompass voxelplanet CONAN_PKG::glm)
