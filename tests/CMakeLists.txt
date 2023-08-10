file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/engine/mat4.c
    test.c
    test_mat4.c
)

set(TEST_EXECUTABLE_NAME ${PROJECT_NAME}_test)

add_executable(${TEST_EXECUTABLE_NAME} ${SOURCES})
target_link_libraries(${TEST_EXECUTABLE_NAME} glfw glad)
target_include_directories(${TEST_EXECUTABLE_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include include)
target_compile_options(${TEST_EXECUTABLE_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_compile_features(${TEST_EXECUTABLE_NAME} PRIVATE c_std_17)

