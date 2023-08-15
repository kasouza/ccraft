#include "ccraft/world.h"
#include "ccraft/chunk.h"
#include "ccraft/engine/utils.h"
#include "ccraft/meshing.h"

#include <klib/linked_list.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct CCRAFT_Chunk *generate_chunk(union CCRAFTE_Vec3i chunk_pos) {
    struct CCRAFT_Chunk *chunk = CCRAFT_create_chunk();
    if (chunk == NULL) {
        exit(1);
    }

    for (int x = 0; x < CCRAFT_CHUNK_SIZE; x++) {
        for (int y = 0; y < CCRAFT_CHUNK_SIZE; y++) {
            for (int z = 0; z < CCRAFT_CHUNK_SIZE; z++) {
                if (chunk_pos.y < 0) {
                    int idx = CCRAFT_CHUNK_INDEX(x, y, z);
                    chunk->voxels[idx].type = CCRAFT_VOXEL_TYPE_STONE;
                }
            }
        }
    }

    chunk->is_dirty = true;

    return chunk;
}

struct CCRAFT_World *CCRAFT_create_world(union CCRAFTE_Vec3i position,
                                         union CCRAFTE_Vec3i size) {
    struct CCRAFT_World *world = malloc(sizeof(struct CCRAFT_World));
    if (world == NULL) {
        fprintf(stderr, "could not allocate memory for world");
        return NULL;
    }

    world->position = position;
    world->size = size;

    world->chunks = calloc(world->size.x * world->size.y * world->size.z,
                           sizeof(struct CCRAFT_Chunk));

    if (!world->chunks) {
        free(world);
        world = NULL;

        return NULL;
    }

    for (int x = 0; x < world->size.x; x++) {
        for (int y = 0; y < world->size.y; y++) {
            for (int z = 0; z < world->size.z; z++) {
                int idx =
                    CCRAFTE_3d_to_index(x, y, z, world->size.x, world->size.y);
                world->chunks[idx] = generate_chunk(CCRAFTE_vec3i_add(
                    world->position, (union CCRAFTE_Vec3i){{x, y, z}}));
            }
        }
    }

    return world;
}

void CCRAFT_free_world(struct CCRAFT_World *world) {
    assert(world != NULL);

    for (int i = 0; i < world->size.x * world->size.y * world->size.z; i++) {
        if (world->chunks[i] != NULL) {
            CCRAFT_free_chunk(world->chunks[i]);
            world->chunks[i] = NULL;
        }
    }

    free(world->chunks);
    world->chunks = NULL;

    free(world);
}

void CCRAFTE_world_move_to(struct CCRAFT_World *world,
                           union CCRAFTE_Vec3i new_world_pos) {

    union CCRAFTE_Vec3i delta_pos =
        CCRAFTE_vec3i_subtract(new_world_pos, world->position);

    // All this ugly stuff simply changes the direction of the loop for each
    // axis individually, based on the direction the player is going. If going
    // positive direction on a given axis, loops regularly (0 to world_size). If
    // goind negative dirction, loops in reverse (world_size to 0) This can
    // probably be refactored, but im not in the mood right now
    int start_x = delta_pos.x < 0 ? world->size.x - 1 : 0;
    int increment_x = delta_pos.x < 0 ? -1 : 1;

    int start_y = delta_pos.y < 0 ? world->size.y - 1 : 0;
    int increment_y = delta_pos.y < 0 ? -1 : 1;

    int start_z = delta_pos.z < 0 ? world->size.z - 1 : 0;
    int increment_z = delta_pos.z < 0 ? -1 : 1;

    for (int x = start_x;
         (delta_pos.x < 0 && x >= 0) || (delta_pos.x >= 0 && x < world->size.x);
         x += increment_x) {
        for (int y = start_y; (delta_pos.y < 0 && y >= 0) ||
                              (delta_pos.y >= 0 && y < world->size.y);
             y += increment_y) {
            for (int z = start_z; (delta_pos.z < 0 && z >= 0) ||
                                  (delta_pos.z >= 0 && z < world->size.z);
                 z += increment_z) {
                int idx =
                    CCRAFTE_3d_to_index(x, y, z, world->size.x, world->size.y);
                int new_idx = CCRAFTE_3d_to_index(
                    x + delta_pos.x, y + delta_pos.y, z + delta_pos.z,
                    world->size.x, world->size.y);

                if (world->chunks[idx] != NULL) {
                    CCRAFT_free_chunk(world->chunks[idx]);
                    world->chunks[idx] = NULL;
                }

                if (new_idx < 0 ||
                    new_idx >= world->size.x * world->size.y * world->size.z) {
                    world->chunks[idx] = generate_chunk(CCRAFTE_vec3i_add(
                        world->position, (union CCRAFTE_Vec3i){{x, y, z}}));

                } else {
                    world->chunks[idx] = world->chunks[new_idx];
                    world->chunks[new_idx] = generate_chunk(CCRAFTE_vec3i_add(
                        world->position, (union CCRAFTE_Vec3i){{x, y, z}}));
                }
            }
        }
    }

    world->position = new_world_pos;
}

void CCRAFT_world_update_dirty_chunks(struct CCRAFT_World *world) {
    assert(world != NULL);

    for (int i = 0; i < world->size.x * world->size.y * world->size.z; i++) {
        if (world->chunks[i]->is_dirty) {
            CCRAFT_remesh(world->chunks[i]);

            world->chunks[i]->is_dirty = false;
        }
    }
}
