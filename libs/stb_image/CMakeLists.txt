cmake_minimum_required(VERSION 3.20.0)
project(stb_image VERSION 2.27)

add_library(stb_image stb_image.cpp)
target_include_directories(stb_image PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_features(stb_image PRIVATE cxx_std_17)
