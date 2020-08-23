add_library(testmod MODULE mod.cpp)

target_compile_features(testmod PRIVATE cxx_std_17)
target_compile_options(testmod PRIVATE -Wall -Wextra -pedantic)
