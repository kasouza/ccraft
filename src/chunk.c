#include "ccraft/chunk.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct CCRAFT_Chunk* CCRAFT_create_chunk() {
    struct CCRAFT_Chunk *chunk = calloc(1, sizeof(struct CCRAFT_Chunk));

    if (chunk == NULL) {
        fprintf(stderr, "could not allocate memory for chunk");
        return NULL;
    }

    chunk->mesh = CCRAFTE_create_mesh();
    if (chunk->mesh == NULL) {
        free(chunk);
        chunk = NULL;

        fprintf(stderr, "could not allocate memory for chunk mesh");
        return NULL;
    }

    return chunk;
}

void CCRAFT_free_chunk(struct CCRAFT_Chunk* chunk) {
    assert(chunk != NULL);

    if (chunk->mesh != NULL) {
        CCRAFTE_free_mesh(chunk->mesh);
        chunk->mesh = NULL;
    }

    free(chunk);
}
