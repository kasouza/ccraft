#include "ccraft/engine/mat4.h"
#include <assert.h>
#include <math.h>
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

union CCRAFTE_Mat4 CCRAFTE_mat4_rotation(float x, float y, float z) {
    float sin_alpha = sinf(x);
    float cos_alpha = cosf(x);

    float sin_betha = sinf(y);
    float cos_betha = cosf(y);

    float sin_gamma = sinf(z);
    float cos_gamma = cosf(z);

    union CCRAFTE_Mat4 mat = {
        {cos_betha * cos_gamma, cos_betha * sin_gamma, -sin_betha, 0,

         sin_alpha * sin_betha * cos_gamma - cos_alpha * sin_gamma,
         sin_alpha * sin_betha * sin_gamma + cos_alpha * cos_gamma,
         sin_alpha * cos_betha, 0,

         cos_alpha * sin_betha * cos_gamma + sin_alpha * sin_gamma,
         cos_alpha * sin_betha * sin_gamma - sin_alpha * cos_gamma,
         cos_alpha * cos_betha, 0,

         0, 0, 0, 1}};

    return mat;
}

union CCRAFTE_Mat4 CCRAFTE_mat4_perspective(float fov, float aspect_ratio, float near, float far) {
    float tan_half_alpha = tanf(fov / 2.0f);
    float n = 1 / tan_half_alpha;

    union CCRAFTE_Mat4 mat = {{n / aspect_ratio, 0, 0, 0,

                               0, n, 0, 0,

                               0, 0, (-near - far) / (near - far), 1,

                               0, 0, (2 * far * near) / (near - far), 0}};

    return mat;
}
