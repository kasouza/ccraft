#include "ccraft/engine/mat4.h"
#include "tests.h"

bool is_identity(union CCRAFTE_Mat4 *a) {
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            size_t idx = i + (4 * j);
            if ((i == j && a->data[idx] != 1) ||
                (i != j && a->data[idx] != 0)) {
                return false;
            }
        }
    }

    return true;
}

bool test_mat4_identity() {
    union CCRAFTE_Mat4 a = CCRAFTE_mat4_identity();
    if (!is_identity(&a)) {
        TEST_FAIL("MATRIX IDENTITY - invalid value");
    }

    return true;
}

bool test_mat4_multiply_identity_x_identity() {
    union CCRAFTE_Mat4 a = CCRAFTE_mat4_identity();
    union CCRAFTE_Mat4 b = CCRAFTE_mat4_identity();
    union CCRAFTE_Mat4 c = CCRAFTE_mat4_multiply_mat4(&a, &b);

    if (!is_identity(&c)) {
        TEST_FAIL("MATRIX MULTIPLY - identity x identity != identity");
    }

    return true;
}

bool test_mat4_multiply() {
    TEST(test_mat4_multiply_identity_x_identity);

    union CCRAFTE_Mat4 a = {
        {3, 9, 51, 0, 8, 2, 41, 12, 2, 22, 9, 1, 4, 34, 7, 3},
    };

    union CCRAFTE_Mat4 b = {
        { 43, 63, 9, 239, 41, 92, 74, 4, 8, 4, 0, 75, 7, 0, 8, 4 },
    };


    union CCRAFTE_Mat4 correct_result = {
        { 1607, 8837, 6530, 1482, 1023, 2317, 6557, 1190, 356, 2630, 1097, 273, 53, 375, 457, 20 }
    };

    union CCRAFTE_Mat4 c = CCRAFTE_mat4_multiply_mat4(&a, &b);

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            size_t idx = i + (4 * j);
            if (c.data[idx] != correct_result.data[idx]) {
                TEST_FAIL_ARGS("MATRIX_MULTIPLY - mat x mat wrong results i: %ld, j: %ld", i, j);
            }
        }
    }

    return true;
}

bool test_mat4() {
    TEST(test_mat4_identity);
    TEST(test_mat4_multiply);

    return true;
}
