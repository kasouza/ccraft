#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_VEC3I_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_VEC3I_H

#include "ccraft/engine/gl_includes.h"
#include <stdbool.h>

union CCRAFTE_Vec3i {
    struct {
        int x;
        int y;
        int z;
    };
    int data[3];
};

void CCRAFTE_vec3i_print(union CCRAFTE_Vec3i vec);

bool CCRAFTE_vec3i_equals(union CCRAFTE_Vec3i a, union CCRAFTE_Vec3i b);

union CCRAFTE_Vec3i CCRAFTE_vec3i_add(union CCRAFTE_Vec3i a, union CCRAFTE_Vec3i b);
union CCRAFTE_Vec3i CCRAFTE_vec3i_subtract(union CCRAFTE_Vec3i a,
                                    union CCRAFTE_Vec3i b);
union CCRAFTE_Vec3i CCRAFTE_vec3i_scale(union CCRAFTE_Vec3i vec, int scalar);

#endif
