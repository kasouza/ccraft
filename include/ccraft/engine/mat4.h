#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_MAT4_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_MAT4_H

#include "ccraft/engine/gl_includes.h"

union CCRAFTE_Mat4 {
    struct {
        GLfloat a11;
        GLfloat a21;
        GLfloat a31;
        GLfloat a41;

        GLfloat a12;
        GLfloat a22;
        GLfloat a32;
        GLfloat a42;

        GLfloat a13;
        GLfloat a23;
        GLfloat a33;
        GLfloat a43;

        GLfloat a14;
        GLfloat a24;
        GLfloat a34;
        GLfloat a44;
    };

    GLfloat data[16];
};

union CCRAFTE_Mat4 CCRAFTE_mat4_identity();

union CCRAFTE_Mat4 CCRAFTE_mat4_multiply_mat4(union CCRAFTE_Mat4 *a,
                                              union CCRAFTE_Mat4 *b);

union CCRAFTE_Mat4 CCRAFTE_mat4_rotation(float x, float y, float z);
union CCRAFTE_Mat4 CCRAFTE_mat4_perspective(float fov, float aspect_ratio, float near, float far);

void CCRAFTE_mat4_print(union CCRAFTE_Mat4 *mat);


#endif
