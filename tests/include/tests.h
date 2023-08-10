#ifndef KASOUZA_CCRAFT_TESTS_INCLUDE_TESTS_INCLUDE_TESTS_H
#define KASOUZA_CCRAFT_TESTS_INCLUDE_TESTS_INCLUDE_TESTS_H

#include <stdbool.h>
#include <stdio.h>

bool test_mat4();

#define TEST_FAIL_ARGS(message, ...) fprintf(stderr, "\033[31m [%s, %d]" message "\033[0m\n", __FILE__, __LINE__, __VA_ARGS__); return false;
#define TEST_FAIL(message) fprintf(stderr, "\033[31m [%s, %d] " message "\033[0m\n", __FILE__, __LINE__); return false;

#define TEST(test_name) if (!test_name()) { return false; }

#endif
