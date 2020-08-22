file(GLOB sources
  "src/*.cpp"
  "src/*.hpp")

add_executable(voxelplanet ${sources})

set_property(TARGET voxelplanet PROPERTY ENABLE_EXPORTS true)

target_compile_features(voxelplanet PRIVATE cxx_std_17)
target_compile_options(voxelplanet PRIVATE -Wall -Wextra -pedantic)

target_link_libraries(voxelplanet
  CONAN_PKG::opengl
  CONAN_PKG::glew
  CONAN_PKG::glfw
  CONAN_PKG::glm)
