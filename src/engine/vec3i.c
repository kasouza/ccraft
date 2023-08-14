#include "ccraft/engine/vec3i.h"
#include <stdio.h>

void CCRAFTE_vec3i_print(union CCRAFTE_Vec3i vec) {
    printf("[ %d, %d, %d ]\n", vec.x, vec.y, vec.z);
}

bool CCRAFTE_vec3i_equals(union CCRAFTE_Vec3i a, union CCRAFTE_Vec3i b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

union CCRAFTE_Vec3i CCRAFTE_vec3i_add(union CCRAFTE_Vec3i a,
                                    union CCRAFTE_Vec3i b) {
    union CCRAFTE_Vec3i c = {{
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
    }};

    return c;
}

union CCRAFTE_Vec3i CCRAFTE_vec3i_subtract(union CCRAFTE_Vec3i a,
                                    union CCRAFTE_Vec3i b) {
    union CCRAFTE_Vec3i c = {{
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
    }};

    return c;
}

union CCRAFTE_Vec3i CCRAFTE_vec3i_scale(union CCRAFTE_Vec3i vec, int scalar) {

    union CCRAFTE_Vec3i scaled = {{
        vec.x * scalar,
        vec.y * scalar,
        vec.z * scalar,
    }};

    return scaled;
}
