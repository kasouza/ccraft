#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_MESH_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_MESH_H

#include "ccraft/engine/gl_includes.h"
#include "ccraft/engine/vertex.h"

#define CCRAFTE_INITIAL_VERTEX_CAPACITY 4

struct CCRAFTE_Mesh {
    struct CCRAFTE_Vertex *vertices;
    GLuint *indices;
    size_t vertices_length;
    size_t vertices_capacity;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};

struct CCRAFTE_Mesh *CCRAFTE_create_mesh();
struct CCRAFTE_Mesh *
CCRAFTE_create_mesh_from_vertices(struct CCRAFTE_Vertex *vertices,
                                  size_t vertex_count);

void CCRAFTE_free_mesh(struct CCRAFTE_Mesh *mesh);

#endif
