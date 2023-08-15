#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_INCLUDE_CCRAFT_CHUNK_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_INCLUDE_CCRAFT_CHUNK_H

#include "ccraft/engine/mesh.h"
#include "ccraft/voxel.h"

#define CCRAFT_CHUNK_SIZE 16

#define CCRAFT_CHUNK_INDEX(x, y, z)                                            \
    ((x) + CCRAFT_CHUNK_SIZE * ((y) + CCRAFT_CHUNK_SIZE * (z)))

struct CCRAFT_Chunk {
    struct CCRAFT_Voxel
        voxels[CCRAFT_CHUNK_SIZE * CCRAFT_CHUNK_SIZE * CCRAFT_CHUNK_SIZE];
    struct CCRAFTE_Mesh *mesh;

    bool is_dirty;
};

struct CCRAFT_Chunk *CCRAFT_create_chunk();
void CCRAFT_free_chunk(struct CCRAFT_Chunk* chunk);

#endif
