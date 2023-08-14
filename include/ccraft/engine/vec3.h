#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_VEC3_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_VEC3_H

#include "ccraft/engine/gl_includes.h"

#include <stdbool.h>

union CCRAFTE_Vec3 {
    struct {
        GLfloat x;
        GLfloat y;
        GLfloat z;
    };
    GLfloat data[3];
};

union CCRAFTE_Vec3 CCRAFTE_vec3_rotate(union CCRAFTE_Vec3 vec, float a, union CCRAFTE_Vec3 direction);
void CCRAFTE_vec3_print(union CCRAFTE_Vec3 vec);

bool CCRAFTE_vec3_equals(union CCRAFTE_Vec3 a, union CCRAFTE_Vec3 b);

union CCRAFTE_Vec3 CCRAFTE_vec3_add(union CCRAFTE_Vec3 a, union CCRAFTE_Vec3 b);
union CCRAFTE_Vec3 CCRAFTE_vec3_subtract(union CCRAFTE_Vec3 a,
                                    union CCRAFTE_Vec3 b);
union CCRAFTE_Vec3 CCRAFTE_vec3_normalize(union CCRAFTE_Vec3 vec);
union CCRAFTE_Vec3 CCRAFTE_vec3_cross(union CCRAFTE_Vec3 a, union CCRAFTE_Vec3 b);
union CCRAFTE_Vec3 CCRAFTE_vec3_scale(union CCRAFTE_Vec3 vec, float scalar);

union CCRAFTE_Vec3 CCRAFTE_vec3_direction(float yaw, float pitch);

#endif
