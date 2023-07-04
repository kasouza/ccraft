project(glad VERSION 1.0)

add_library(glad ${PROJECT_SOURCE_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/include)
