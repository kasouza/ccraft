cmake_minimum_required(VERSION 3.20.0)

project(ccraft)


# Compile commands.json 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Needed to make the language server work correctly (clangd)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Dependencies
set(DEPS_PATH ${PROJECT_SOURCE_DIR}/libs)

add_subdirectory(${DEPS_PATH}/glfw)
add_subdirectory(${DEPS_PATH}/stb_image)
add_subdirectory(${DEPS_PATH}/glad)

# Executable
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

