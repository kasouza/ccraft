#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_INCLUDE_CCRAFT_VOXEL_TYPE_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_INCLUDE_CCRAFT_VOXEL_TYPE_H

#include "ccraft/side.h"
#include "ccraft/engine/texture_array.h"

#include <stdbool.h>

enum CCRAFT_VoxelType {
    CCRAFT_VOXEL_TYPE_AIR,
    CCRAFT_VOXEL_TYPE_STONE,

    CCRAFT_VOXEL_TYPE_LAST,
};

struct CCRAFT_VoxelTypeInfo {
    const char *textures[CCRAFT_SIDE_LAST];
    int texture_indices[CCRAFT_SIDE_LAST];
    bool is_visible;
};


extern struct CCRAFT_VoxelTypeInfo CCRAFT_voxel_types[CCRAFT_VOXEL_TYPE_LAST];

void CCRAFT_init_voxel_types(struct CCRAFTE_TextureArray *tex_array);

#endif
