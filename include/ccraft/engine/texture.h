#ifndef KASOUZA_CCRAFT_ENGINE_INCLUDE_KGL_TEXTURE_H
#define KASOUZA_CCRAFT_ENGINE_INCLUDE_KGL_TEXTURE_H

struct CCRAFTE_Texture
{
    unsigned int texture;
    double width;
    double height;
};

struct CCRAFTE_Texture* CCRAFTE_load_texture(const char *path_to_image);
void CCRAFTE_free_texture(struct CCRAFTE_Texture *texture);

#endif
