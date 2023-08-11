#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_VEC3_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_VEC3_H

#include "ccraft/engine/gl_includes.h"

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

union CCRAFTE_Vec3 CCRAFTE_vec3_normalize(union CCRAFTE_Vec3 vec);
union CCRAFTE_Vec3 CCRAFTE_vec3_cross(union CCRAFTE_Vec3 a, union CCRAFTE_Vec3 b);

#endif
