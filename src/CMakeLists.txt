# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    engine/camera.c
    engine/mesh.c
	engine/error.c
	engine/input.c
	engine/renderer.c
	engine/shaders.c
	engine/utils.c
    engine/mat4.c
    engine/vec3.c
    engine/vec3i.c
    engine/quaternion.c
	engine/texture.c
	engine/texture_array.c
    engine/utils.c
    voxel_type.c
    chunk.c
    meshing.c
    main.c
    world.c
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} glfw glad stb_image klib)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_compile_features(${PROJECT_NAME} PRIVATE c_std_17)

