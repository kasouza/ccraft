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
    static int i = 0;
    struct CCRAFT_Chunk *chunk = CCRAFT_create_chunk();
    if (chunk == NULL) {
        exit(1);
    }

    CCRAFTE_vec3i_print(chunk_pos);
    if (chunk_pos.y <= 1) {
        /*printf("hrer %d\n", i++);*/
        for (int x = 0; x < CCRAFT_CHUNK_SIZE; x++) {
            for (int y = 0; y < CCRAFT_CHUNK_SIZE; y++) {
                for (int z = 0; z < CCRAFT_CHUNK_SIZE; z++) {
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

    world->chunks = malloc(world->size.x * world->size.y * world->size.z *
                           sizeof(struct CCRAFT_Chunk *));

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

    struct CCRAFT_Chunk **new_chunks =
        malloc(sizeof(struct CCRAFT_Chunk *) * world->size.x * world->size.y *
               world->size.z);
    struct CCRAFT_Chunk **chunks_to_free =
        malloc(sizeof(struct CCRAFT_Chunk *) * world->size.x * world->size.y *
               world->size.z);
    size_t chunks_to_free_len = 0;

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

                int new_x = x + delta_pos.x;
                int new_y = y + delta_pos.y;
                int new_z = z + delta_pos.z;

                if (new_x >= 0 && new_x < world->size.x && new_y >= 0 &&
                    new_y < world->size.y && new_z >= 0 &&
                    new_z < world->size.z) {
                    int new_idx = CCRAFTE_3d_to_index(
                        new_x, new_y, new_z, world->size.x, world->size.y);
                    new_chunks[idx] = world->chunks[new_idx];
                } else {
                    new_chunks[idx] = generate_chunk(CCRAFTE_vec3i_add(
                        new_world_pos, (union CCRAFTE_Vec3i){{x, y, z}}));
                }

                int world_x = new_world_pos.x + x;
                int world_y = new_world_pos.y + y;
                int world_z = new_world_pos.z + z;

                if ((world_x < new_world_pos.x ||
                     world_x > (new_world_pos.x + world->size.x)) ||
                    (world_y < new_world_pos.y ||
                     world_y > (new_world_pos.y + world->size.y)) ||
                    (world_z < new_world_pos.z ||
                     world_z > (new_world_pos.z + world->size.z))) {
                    chunks_to_free[chunks_to_free_len++] = world->chunks[idx];
                }
            }
        }
    }

    for (size_t i = 0; i < chunks_to_free_len; i++) {
        CCRAFT_free_chunk(chunks_to_free[i]);
        chunks_to_free[i] = NULL;
    }

    free(chunks_to_free);
    chunks_to_free = NULL;
    chunks_to_free_len = 0;

    free(world->chunks);
    world->chunks = new_chunks;

    /*for (int x = 0; x < world->size.x; x++) {*/
    /*for (int y = 0; y < world->size.y; y++) {*/
    /*for (int z = 0; z < world->size.z; z++) {*/
    /*int idx =*/
    /*CCRAFTE_3d_to_index(x, y, z, world->size.x, world->size.y);*/
    /*CCRAFT_free_chunk(world->chunks[idx]);*/
    /*world->chunks[idx] = generate_chunk(CCRAFTE_vec3i_add(world->size, (union
     * CCRAFTE_Vec3i){{x, y, z}}));*/
    /*}*/
    /*}*/
    /*}*/

    world->position = new_world_pos;
}

void CCRAFT_world_update_dirty_chunks(struct CCRAFT_World *world) {
    assert(world != NULL);

    for (int x = 0; x < world->size.x; x++) {
        for (int y = 0; y < world->size.y; y++) {
            for (int z = 0; z < world->size.z; z++) {
                int idx =
                    CCRAFTE_3d_to_index(x, y, z, world->size.x, world->size.y);
                if (world->chunks[idx]->is_dirty) {
                    CCRAFT_remesh(world->chunks[idx]);

                    world->chunks[idx]->is_dirty = false;
                }
            }
        }
    }
}
