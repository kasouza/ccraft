#include "ccraft/engine/mat4.h"
#include <assert.h>
#include <stdio.h>

union CCRAFTE_Mat4 CCRAFTE_mat4_identity() {
    union CCRAFTE_Mat4 mat = {
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    };

    return mat;
}

union CCRAFTE_Mat4 CCRAFTE_mat4_multiply_mat4(union CCRAFTE_Mat4 *a,
                                              union CCRAFTE_Mat4 *b) {
    assert(a != NULL && "Matrix cannot be null");
    assert(b != NULL && "Matrix cannot be null");

    union CCRAFTE_Mat4 c = {0};

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            size_t c_idx = i + (4 * j);
            for (size_t k = 0; k < 4; k++) {
                size_t a_idx = i + (4 * k);
                size_t b_idx = k + (4 * j);

                c.data[c_idx] += a->data[a_idx] * b->data[b_idx];
            }
        }
    }

    CCRAFTE_mat4_print(&c);

    return c;
}

void CCRAFTE_mat4_print(union CCRAFTE_Mat4 *mat) {
    assert(mat != NULL && "Matrix cannot be null");

    printf("[\n");

    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 4; j++) {
            size_t idx = i + (4 * j);
            printf("    %f ", mat->data[idx]);
        }

        printf("\n");
    }

    printf("]\n");
}
