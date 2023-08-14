#include "ccraft/world.h"
#include "ccraft/chunk.h"
#include "ccraft/engine/utils.h"

#include <klib/linked_list.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct CCRAFT_World *CCRAFT_create_world(union CCRAFTE_Vec3i position, union CCRAFTE_Vec3i size) {
    struct CCRAFT_World *world = malloc(sizeof(struct CCRAFT_World));
    if (world == NULL) {
        fprintf(stderr, "could not allocate memory for world");
        return NULL;
    }

    world->position = position;
    world->size = size;

    world->chunks = KLIB_create_linked_list();

    if (!world->chunks) {
        free(world);
        world = NULL;

        return NULL;
    }
    fprintf(stderr, "aqui\n");

    for (int i = 0; i < world->size.x * world->size.y * world->size.z; i++) {
        KLIB_LINKED_LIST_PUSH_BACK(world->chunks, struct CCRAFT_Chunk, {0});

        for (int i = 0;
             i < CCRAFT_CHUNK_SIZE * CCRAFT_CHUNK_SIZE * CCRAFT_CHUNK_SIZE;
             i++) {
            ((struct CCRAFT_Chunk *)world->chunks->tail->data)->voxels[i].type =
                CCRAFT_VOXEL_TYPE_STONE;
        }
    }


    return world;
}

void CCRAFT_free_world(struct CCRAFT_World *world) {
    assert(world != NULL);

    KLIB_free_linked_list(world->chunks);
    world->chunks = NULL;

    free(world);
}

// Para mover o mundo, iterar pelo index, calculando as coordenadas, removendo os chunks muito distantes do centro e adicionando novos onde não tiver
void CCRAFTE_world_move_to(struct CCRAFT_World* world, union CCRAFTE_Vec3i new_world_pos) {
    world->position = new_world_pos;
}
