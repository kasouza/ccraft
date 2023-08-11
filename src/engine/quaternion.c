#include "ccraft/engine/quaternion.h"

#include <math.h>
#include <stdio.h>

union CCRAFTE_Quaternion
CCRAFTE_quaternion_normalize(union CCRAFTE_Quaternion q) {
    float magnitude = sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
    union CCRAFTE_Quaternion normalized = {
        {q.w / magnitude, q.x / magnitude, q.y / magnitude, q.z / magnitude}};

    return normalized;
}

union CCRAFTE_Quaternion
CCRAFTE_quaternion_multiply_quaternion(union CCRAFTE_Quaternion a,
                                       union CCRAFTE_Quaternion b) {
    union CCRAFTE_Quaternion c = {{
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
    }};

    return c;
}

void CCRAFTE_quaternion_print(union CCRAFTE_Quaternion q) {
    printf("[ %f, %f, %f, %f ]\n", q.w, q.x, q.y, q.z);
}

union CCRAFTE_Quaternion
CCRAFTE_quaternion_rotation(float a, union CCRAFTE_Vec3 direction) {
    float cos_half_alpha = cosf(a / 2.0);
    float sin_half_alpha = sinf(a / 2.0);

    union CCRAFTE_Quaternion R = {{cos_half_alpha, sin_half_alpha * direction.x,
                                   sin_half_alpha * direction.y,
                                   sin_half_alpha * direction.z}};

    return R;
}

union CCRAFTE_Quaternion
CCRAFTE_quaternion_rotation_from_euler(float roll, float pitch, float yaw) {
    float cr = cosf(roll * 0.5);
    float sr = sinf(roll * 0.5);
    float cp = cosf(pitch * 0.5);
    float sp = sinf(pitch * 0.5);
    float cy = cosf(yaw * 0.5);
    float sy = sinf(yaw * 0.5);

    union CCRAFTE_Quaternion q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}

union CCRAFTE_Quaternion
CCRAFTE_quaternion_rotate(union CCRAFTE_Quaternion q, float a, union CCRAFTE_Vec3 direction) {
    union CCRAFTE_Quaternion rotation = CCRAFTE_quaternion_rotation(a, direction);

    return CCRAFTE_quaternion_multiply_quaternion(q, rotation);
}
