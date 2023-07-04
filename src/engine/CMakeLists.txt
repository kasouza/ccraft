set(SOURCES
	error.c
	input.c
	renderer.c
	shaders.c
	texture.c
	utils.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC c_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad stb_image)
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTIONS})
