#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_TEXTURE_ARRAY_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_TEXTURE_ARRAY_H

#include "ccraft/engine/gl_includes.h"

#include <stdbool.h>

struct CCRAFTE_TextureArray
{
    unsigned int texture;
    GLsizei width;
    GLsizei height;

    int texture_count;
    int max_textures;
};

struct CCRAFTE_TextureArray* CCRAFTE_create_texture_array(GLsizei width, GLsizei height, GLsizei texture_count);
void CCRAFTE_free_texture_array(struct CCRAFTE_TextureArray *texture);

int CCRAFTE_texture_array_add_texture(
    struct CCRAFTE_TextureArray *texture_array, const char *file);

#endif
