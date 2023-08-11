#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_QUATERNION_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_QUATERNION_H

#include "ccraft/engine/gl_includes.h"
#include "ccraft/engine/vec3.h"

union CCRAFTE_Quaternion {
    struct {
        GLfloat w;
        GLfloat x;
        GLfloat y;
        GLfloat z;
    };
    GLfloat data[4];
};

union CCRAFTE_Quaternion
CCRAFTE_quaternion_normalize(union CCRAFTE_Quaternion q);

union CCRAFTE_Quaternion
CCRAFTE_quaternion_multiply_quaternion(union CCRAFTE_Quaternion a,
                                       union CCRAFTE_Quaternion b);

union CCRAFTE_Quaternion
CCRAFTE_quaternion_rotation(float a, union CCRAFTE_Vec3 direction);

union CCRAFTE_Quaternion
CCRAFTE_quaternion_rotation_from_euler(float roll, float pitch, float yaw);

union CCRAFTE_Quaternion
CCRAFTE_quaternion_rotate(union CCRAFTE_Quaternion q, float a,
                          union CCRAFTE_Vec3 direction);

void CCRAFTE_quaternion_print(union CCRAFTE_Quaternion q);

#endif
