#include "ccraft/engine/texture.h"
#include "ccraft/engine/gl_includes.h"
#include "ccraft/engine/utils.h"

#include <assert.h>
#include <stb_image.h>
#include <stdbool.h>

struct CCRAFTE_Texture *CCRAFTE_load_texture(const char *path_to_image) {
    stbi_set_flip_vertically_on_load(true);

    int width, height, nr_channels;
    unsigned char *data =
        stbi_load(path_to_image, &width, &height, &nr_channels, 0);

    if (!data) {
        return NULL;
    }

    GLenum format;

    switch (nr_channels) {
    case 1:
        format = GL_RED;
        break;

    case 2:
        format = GL_RGB;
        break;

    case 3:
        format = GL_RGB;
        break;

    case 4:
        format = GL_RGBA;
        break;

    default:
        return NULL;
    }

    unsigned int texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(data);

    struct CCRAFTE_Texture *sprite =
        CCRAFTE_malloc(sizeof(struct CCRAFTE_Texture));
    sprite->texture = texture;
    sprite->width = width;
    sprite->height = height;

    return sprite;
}

void CCRAFTE_free_texture(struct CCRAFTE_Texture *sprite) {
    assert(sprite != NULL);

    // TODO: This could be optimized if all the textues were stored in an array
    // (when deleting all the textures)
    if (sprite->texture) {
        glDeleteTextures(1, &sprite->texture);
    }

    free(sprite);
}
