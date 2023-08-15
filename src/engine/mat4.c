#include "ccraft/engine/mat4.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

union CCRAFTE_Mat4 CCRAFTE_mat4_translation(union CCRAFTE_Vec3 translation) {
    union CCRAFTE_Mat4 mat = {
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, translation.x, translation.y,
         translation.z, 1},
    };

    return mat;
}

union CCRAFTE_Mat4 CCRAFTE_mat4_scale(union CCRAFTE_Vec3 scale) {
    union CCRAFTE_Mat4 mat = {
        {scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, 1},
    };

    return mat;
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

union CCRAFTE_Mat4 CCRAFTE_mat4_perspective(float fov, float aspect_ratio,
                                            float near, float far) {
    float tan_half_alpha = tanf(fov / 2.0f);
    float n = 1 / tan_half_alpha;

    union CCRAFTE_Mat4 mat = {{n / aspect_ratio, 0, 0, 0,

                               0, n, 0, 0,

                               0, 0, (-near - far) / (near - far), 1,

                               0, 0, (2 * far * near) / (near - far), 0}};

    return mat;
}

union CCRAFTE_Mat4
CCRAFTE_mat4_model_from_transform(struct CCRAFTE_Transform *transform) {
    union CCRAFTE_Mat4 translation =
        CCRAFTE_mat4_translation(transform->translation);
    union CCRAFTE_Mat4 rotation =
        CCRAFTE_mat4_rotation_from_quaternion(transform->rotation);
    union CCRAFTE_Mat4 scale = CCRAFTE_mat4_scale(transform->scale);

    union CCRAFTE_Mat4 rot_scale =
        CCRAFTE_mat4_multiply_mat4(&rotation, &scale);
    return CCRAFTE_mat4_multiply_mat4(&translation, &rot_scale);
}

union CCRAFTE_Mat4
CCRAFTE_mat4_rotation_from_quaternion(union CCRAFTE_Quaternion q) {
    float w_squared = q.w * q.w;
    float x_squared = q.x * q.x;
    float y_squared = q.y * q.y;
    float z_squared = q.z * q.z;

    union CCRAFTE_Mat4 mat = {
        {2 * (w_squared + x_squared) - 1, 2 * (q.x * q.y + q.w * q.z),
         2 * (q.x * q.z - q.w * q.y), 0,

         2 * (q.x * q.y - q.w * q.z), 2 * (w_squared + y_squared) - 1,
         2 * (q.y * q.z + q.w * q.x), 0,

         2 * (q.x * q.z + q.w * q.y), 2 * (q.y * q.z - q.w * q.x),
         2 * (w_squared + z_squared) - 1, 0,

         0, 0, 0, 1}};

    return mat;
}

union CCRAFTE_Mat4
CCRAFTE_mat4_view_from_camera(struct CCRAFTE_Camera *camera) {
    union CCRAFTE_Vec3 target = CCRAFTE_vec3_direction(camera->yaw, camera->pitch);

    union CCRAFTE_Vec3 D = CCRAFTE_vec3_normalize(target);
    union CCRAFTE_Vec3 R = CCRAFTE_vec3_normalize(CCRAFTE_vec3_cross(target, camera->world_up));
    union CCRAFTE_Vec3 U = CCRAFTE_vec3_normalize(CCRAFTE_vec3_cross(R, D));

    union CCRAFTE_Mat4 translation = {{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
                                       -camera->position.x, -camera->position.y,
                                       -camera->position.z, 1}};

    union CCRAFTE_Mat4 mat = {
        {
            R.x, U.x, D.x, 0,
            R.y, U.y, D.y, 0,
            R.z, U.z, D.z, 0,
            0, 0, 0, 1
        },
    };

    return CCRAFTE_mat4_multiply_mat4(&mat, &translation);
}
