#include "ccraft/voxel_type.h"
#include "ccraft/engine/texture_array.h"

struct CCRAFT_VoxelTypeInfo CCRAFT_voxel_types[CCRAFT_VOXEL_TYPE_LAST] = {
    [CCRAFT_VOXEL_TYPE_AIR] =
        (struct CCRAFT_VoxelTypeInfo){
            .is_visible = false,
            .textures = {0},
        },
    [CCRAFT_VOXEL_TYPE_STONE] = (struct CCRAFT_VoxelTypeInfo){
        .is_visible = true,
        .textures = {
            [CCRAFT_SIDE_BOTTOM] = "assets/amogus.png",
            [CCRAFT_SIDE_TOP] = "assets/amogus.png",
            [CCRAFT_SIDE_BACK] = "assets/gorda.png",
            [CCRAFT_SIDE_FRONT] = "assets/amogus.png",
            [CCRAFT_SIDE_LEFT] = "assets/amogus.png",
            [CCRAFT_SIDE_RIGHT] = "assets/amogus.png",
        }}};

void CCRAFT_init_voxel_types(struct CCRAFTE_TextureArray *tex_array) {
    for (int i = 0; i < CCRAFT_VOXEL_TYPE_LAST; i++) {
        for (int side = 0; side < CCRAFT_SIDE_LAST; side++) {
            if (CCRAFT_voxel_types[i].is_visible) {
                CCRAFT_voxel_types[i].texture_indices[side] =
                    CCRAFTE_texture_array_add_texture(
                        tex_array, CCRAFT_voxel_types[i].textures[side]);
            }
        }
    }
}
