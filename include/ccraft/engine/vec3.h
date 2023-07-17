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

#endif
