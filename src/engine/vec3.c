#include "ccraft/engine/vec3.h"
#include "ccraft/engine/quaternion.h"

#include "math.h"
#include <stdio.h>

union CCRAFTE_Vec3 CCRAFTE_vec3_rotate(union CCRAFTE_Vec3 vec, float a,
                                       union CCRAFTE_Vec3 direction) {
    union CCRAFTE_Quaternion P = {
        {0, vec.x, vec.y, vec.z},
    };

    float cos_half_alpha = cosf(a / 2.0);
    float sin_half_alpha = sinf(a / 2.0);

    union CCRAFTE_Quaternion R = {{cos_half_alpha, sin_half_alpha * direction.x,
                                   sin_half_alpha * direction.y,
                                   sin_half_alpha * direction.z}};

    union CCRAFTE_Quaternion R_2 = {{R.w, -R.x, -R.y, -R.z}};

    union CCRAFTE_Quaternion rotated = CCRAFTE_quaternion_multiply_quaternion(
        CCRAFTE_quaternion_multiply_quaternion(R, P), R_2);

    union CCRAFTE_Vec3 rotated_vec = {{rotated.x, rotated.y, rotated.z}};

    return rotated_vec;
}

void CCRAFTE_vec3_print(union CCRAFTE_Vec3 vec) {
    printf("[ %f, %f, %f ]\n", vec.x, vec.y, vec.z);
}

bool CCRAFTE_vec3_equals(union CCRAFTE_Vec3 a, union CCRAFTE_Vec3 b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

union CCRAFTE_Vec3 CCRAFTE_vec3_add(union CCRAFTE_Vec3 a,
                                    union CCRAFTE_Vec3 b) {
    union CCRAFTE_Vec3 c = {{
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
    }};

    return c;
}

union CCRAFTE_Vec3 CCRAFTE_vec3_subtract(union CCRAFTE_Vec3 a,
                                    union CCRAFTE_Vec3 b) {
    union CCRAFTE_Vec3 c = {{
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
    }};

    return c;
}

union CCRAFTE_Vec3 CCRAFTE_vec3_normalize(union CCRAFTE_Vec3 vec) {
    float magnitude = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

    union CCRAFTE_Vec3 normalized = {
        {vec.x / magnitude, vec.y / magnitude, vec.z / magnitude},
    };

    return normalized;
}

union CCRAFTE_Vec3 CCRAFTE_vec3_cross(union CCRAFTE_Vec3 a,
                                      union CCRAFTE_Vec3 b) {
    union CCRAFTE_Vec3 c = {
        {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}};

    return c;
}

union CCRAFTE_Vec3 CCRAFTE_vec3_scale(union CCRAFTE_Vec3 vec, float scalar) {

    union CCRAFTE_Vec3 scaled = {{
        vec.x * scalar,
        vec.y * scalar,
        vec.z * scalar,
    }};

    return scaled;
}

union CCRAFTE_Vec3 CCRAFTE_vec3_direction(float yaw, float pitch) {
    float cos_yaw = cosf(yaw);
    float sin_yaw = sinf(yaw);

    float cos_pitch = cosf(pitch);
    float sin_pitch = sinf(pitch);

    union CCRAFTE_Vec3 dir = {
        {cos_yaw * cos_pitch, sin_pitch, sin_yaw * cos_pitch}};

    return CCRAFTE_vec3_normalize(dir);
}
