#include "ccraft/meshing.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define FACES_PER_CUBE 6
#define VERTICES_PER_FACE 6
#define VERTICES_PER_CUBE (FACES_PER_CUBE) * (VERTICES_PER_FACE)

// A chunk will have, at most [block_per_chunk * faces_per_block *
// vertices_per_face] vertices
static struct CCRAFTE_Vertex
    meshing_buffer[CCRAFT_CHUNK_SIZE * CCRAFT_CHUNK_SIZE * CCRAFT_CHUNK_SIZE *
                   VERTICES_PER_CUBE] = {0};

struct CCRAFTE_Vertex vertices[] = {
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

    1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,

    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,

    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,

    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
};

#define BOTTOM_FACE_INDEX 0
#define TOP_FACE_INDEX 1
#define FRONT_FACE_INDEX 2
#define BACK_FACE_INDEX 3
#define LEFT_FACE_INDEX 4
#define RIGHT_FACE_INDEX 5

struct CCRAFTE_Vertex cube_vertices[] = {
    // Bottom face
    {{{0.0f, 0.0f, 0.0f}}},
    {{{1.0f, 0.0f, 0.0f}}},
    {{{1.0f, 0.0f, 1.0f}}},

    {{{0.0f, 0.0f, 0.0f}}},
    {{{1.0f, 0.0f, 1.0f}}},
    {{{0.0f, 0.0f, 1.0f}}},

    // Top face
    {{{0.0f, 1.0f, 1.0f}}},
    {{{1.0f, 1.0f, 1.0f}}},
    {{{1.0f, 1.0f, 0.0f}}},

    {{{0.0f, 1.0f, 1.0f}}},
    {{{1.0f, 1.0f, 0.0f}}},
    {{{0.0f, 1.0f, 0.0f}}},

    // Front face
    {{{0.0f, 0.0f, 1.0f}}},
    {{{1.0f, 0.0f, 1.0f}}},
    {{{1.0f, 1.0f, 1.0f}}},

    {{{0.0f, 0.0f, 1.0f}}},
    {{{1.0f, 1.0f, 1.0f}}},
    {{{0.0f, 1.0f, 1.0f}}},

    // Back face
    {{{1.0f, 0.0f, 0.0f}}},
    {{{0.0f, 0.0f, 0.0f}}},
    {{{0.0f, 1.0f, 0.0f}}},

    {{{1.0f, 0.0f, 0.0f}}},
    {{{0.0f, 1.0f, 0.0f}}},
    {{{1.0f, 1.0f, 0.0f}}},

    // Left face
    {{{0.0f, 0.0f, 0.0f}}},
    {{{0.0f, 0.0f, 1.0f}}},
    {{{0.0f, 1.0f, 1.0f}}},

    {{{0.0f, 0.0f, 0.0f}}},
    {{{0.0f, 1.0f, 1.0f}}},
    {{{0.0f, 1.0f, 0.0f}}},

    // Right face
    {{{1.0f, 0.0f, 1.0f}}},
    {{{1.0f, 0.0f, 0.0f}}},
    {{{1.0f, 1.0f, 0.0f}}},

    {{{1.0f, 0.0f, 1.0f}}},
    {{{1.0f, 1.0f, 0.0f}}},
    {{{1.0f, 1.0f, 1.0f}}},
};

bool is_block_visible(struct CCRAFT_Chunk *chunk, int x, int y, int z) {
    if ((x < 0 || x >= CCRAFT_CHUNK_SIZE) ||
        (y < 0 || y >= CCRAFT_CHUNK_SIZE) ||
        (z < 0 || z >= CCRAFT_CHUNK_SIZE)) {
        return false;
    }

    size_t idx = CCRAFT_CHUNK_INDEX(x, y, z);
    return chunk->voxels[idx].type != CCRAFT_VOXEL_TYPE_AIR;
}

struct CCRAFTE_Mesh *CCRAFT_create_simple_mesh(struct CCRAFT_Chunk *chunk) {
    size_t vertex_count = 0;

    for (int x = 0; x < CCRAFT_CHUNK_SIZE; x++) {
        for (int y = 0; y < CCRAFT_CHUNK_SIZE; y++) {
            for (int z = 0; z < CCRAFT_CHUNK_SIZE; z++) {
                if (!is_block_visible(chunk, x, y, z)) {
                    continue;
                }

                int added_vertices = 0;

                // Bottom
                if (!is_block_visible(chunk, x, y - 1, z)) {
                    memcpy(meshing_buffer + vertex_count + added_vertices,
                           cube_vertices + BOTTOM_FACE_INDEX * VERTICES_PER_FACE,
                           sizeof(struct CCRAFTE_Vertex) * VERTICES_PER_FACE);
                    added_vertices += VERTICES_PER_FACE;
                }

                // Top
                if (!is_block_visible(chunk, x, y + 1, z)) {
                    memcpy(meshing_buffer + vertex_count + added_vertices,
                           cube_vertices + TOP_FACE_INDEX * VERTICES_PER_FACE,
                           sizeof(struct CCRAFTE_Vertex) * VERTICES_PER_FACE);
                    added_vertices += VERTICES_PER_FACE;
                }

                // Left
                if (!is_block_visible(chunk, x - 1, y, z)) {
                    memcpy(meshing_buffer + vertex_count + added_vertices,
                           cube_vertices + LEFT_FACE_INDEX * VERTICES_PER_FACE,
                           sizeof(struct CCRAFTE_Vertex) * VERTICES_PER_FACE);
                    added_vertices += VERTICES_PER_FACE;
                }

                // Right
                if (!is_block_visible(chunk, x + 1, y, z)) {
                    memcpy(meshing_buffer + vertex_count + added_vertices,
                           cube_vertices + RIGHT_FACE_INDEX * VERTICES_PER_FACE,
                           sizeof(struct CCRAFTE_Vertex) * VERTICES_PER_FACE);
                    added_vertices += VERTICES_PER_FACE;
                }

                // Back
                if (!is_block_visible(chunk, x, y, z - 1)) {
                    memcpy(meshing_buffer + vertex_count + added_vertices,
                           cube_vertices + BACK_FACE_INDEX * VERTICES_PER_FACE,
                           sizeof(struct CCRAFTE_Vertex) * VERTICES_PER_FACE);
                    added_vertices += VERTICES_PER_FACE;
                }

                // Front
                if (!is_block_visible(chunk, x, y, z + 1)) {
                    memcpy(meshing_buffer + vertex_count + added_vertices,
                           cube_vertices + FRONT_FACE_INDEX * VERTICES_PER_FACE,
                           sizeof(struct CCRAFTE_Vertex) * VERTICES_PER_FACE);
                    added_vertices += VERTICES_PER_FACE;
                }

                /*memcpy(meshing_buffer + vertex_count, cube_vertices,*/
                       /*sizeof(cube_vertices));*/
                for (int i = 0; i < added_vertices; i++) {
                    meshing_buffer[vertex_count + i].position.x += x;
                    meshing_buffer[vertex_count + i].position.y += y;
                    meshing_buffer[vertex_count + i].position.z += z;
                }

                vertex_count += added_vertices;
            }
        }
    }

    return CCRAFTE_create_mesh_from_vertices(meshing_buffer, vertex_count);
}
