#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_VERTEX_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_VERTEX_H

#include "ccraft/engine/vec2.h"
#include "ccraft/engine/vec3.h"

struct CCRAFTE_Vertex {
    union CCRAFTE_Vec3 position;
    union CCRAFTE_Vec2 tex_coords;

    GLfloat texture_array_index;
};

#endif
