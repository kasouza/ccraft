#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_VEC2_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_VEC2_H

#include "ccraft/engine/gl_includes.h"

union CCRAFTE_Vec2 {
    struct {
        GLfloat x;
        GLfloat y;
    };

    GLfloat data[2];
};

#endif
