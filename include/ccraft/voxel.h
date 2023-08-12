#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_INCLUDE_CCRAFT_VOXEL_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_INCLUDE_CCRAFT_VOXEL_H

enum CCRAFT_VoxelType {
    CCRAFT_VOXEL_TYPE_AIR,
    CCRAFT_VOXEL_TYPE_STONE,
};

struct CCRAFT_Voxel {
    enum CCRAFT_VoxelType type;
};

#endif
