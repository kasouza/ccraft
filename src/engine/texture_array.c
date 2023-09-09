#include "ccraft/engine/texture_array.h"
#include "ccraft/engine/debug.h"

#include <stb_image.h>

#include <assert.h>
#include <stdlib.h>

struct CCRAFTE_TextureArray *
CCRAFTE_create_texture_array(GLsizei width, GLsizei height,
                             GLsizei texture_count) {
    struct CCRAFTE_TextureArray *texture_array =
        malloc(sizeof(struct CCRAFTE_TextureArray));
    if (texture_array == NULL) {
        CCRAFTE_DEBUG_LOG("could not allocate memory for texture array");
        return NULL;
    }

    texture_array->width = width;
    texture_array->height = height;
    texture_array->texture = 0;
    texture_array->texture_count = 0;
    texture_array->max_textures = texture_count;

    glGenTextures(1, &texture_array->texture);
    if (texture_array->texture == 0) {
        CCRAFTE_DEBUG_LOG(
            "could not allocate memory for texture array's internal texture");
        free(texture_array);
        return NULL;
    }

    int a_width, a_height, nr_channels;
    unsigned char *data =
        stbi_load("assets/amogus.png", &a_width, &a_height, &nr_channels, 4);

    if (data == NULL) {
        CCRAFTE_DEBUG_LOG_ARGS("could not load texture from file: \"%s\"",
                               "assets/amogus.png");
        return NULL;
    }

    if (width != texture_array->width || height != texture_array->height) {
        stbi_image_free(data);
        CCRAFTE_DEBUG_LOG_ARGS("image size mismatch: \"%s\"",
                               "assets/amogus.png");
        return NULL;
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array->texture);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, texture_count,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    return texture_array;
}

int CCRAFTE_texture_array_add_texture(
    struct CCRAFTE_TextureArray *texture_array, const char *file) {
    stbi_set_flip_vertically_on_load(true);

    assert(texture_array != NULL);
    assert(texture_array->texture_count < texture_array->max_textures &&
           "max number of textures reached");

    int width, height, nr_channels;
    unsigned char *data = stbi_load(file, &width, &height, &nr_channels, 4);

    if (data == NULL) {
        CCRAFTE_DEBUG_LOG_ARGS("could not load texture from file: \"%s\"",
                               file);
        return -1;
    }

    if (width != texture_array->width || height != texture_array->height) {
        stbi_image_free(data);
        CCRAFTE_DEBUG_LOG_ARGS("image size mismatch: \"%s\"", file);
        return -1;
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array->texture);

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, texture_array->texture_count,
                    width, height, 1, GL_RGBA,
                    GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    stbi_image_free(data);

    return texture_array->texture_count++;
}

void CCRAFTE_free_texture_array(struct CCRAFTE_TextureArray *texture_array) {
    assert(texture_array != NULL);

    if (texture_array->texture > 0) {
        glDeleteTextures(1, &texture_array->texture);
        texture_array->texture = 0;
    }

    free(texture_array);
}
