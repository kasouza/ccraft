#include "ccraft/engine/mesh.h"
#include "ccraft/engine/utils.h"
#include "ccraft/engine/vertex.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct CCRAFTE_Mesh *CCRAFTE_create_mesh_uninitialized_vertices() {
    struct CCRAFTE_Mesh *mesh = calloc(sizeof(struct CCRAFTE_Mesh), 1);
    if (!mesh) {
        return NULL;
    }

    mesh->matrix = CCRAFTE_mat4_identity();

    mesh->vertices = NULL;
    mesh->indices = NULL;

    glGenVertexArrays(1, &mesh->VAO);
    if (!mesh->VAO) {
        CCRAFTE_free_mesh(mesh);
        mesh = NULL;

        return NULL;
    }

    glGenBuffers(1, &mesh->VBO);
    if (!mesh->VBO) {
        CCRAFTE_free_mesh(mesh);
        mesh = NULL;

        return NULL;
    }

    glGenBuffers(1, &mesh->EBO);
    if (!mesh->EBO) {
        CCRAFTE_free_mesh(mesh);
        mesh = NULL;

        return NULL;
    }

    glBindVertexArray(mesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(struct CCRAFTE_Vertex),
                          (void *)offsetof(struct CCRAFTE_Vertex, position));

    glBindVertexArray(0);

    return mesh;
}

struct CCRAFTE_Mesh *CCRAFTE_create_mesh() {
    struct CCRAFTE_Mesh *mesh = CCRAFTE_create_mesh_uninitialized_vertices();
    if (!mesh) {
        return NULL;
    }

    mesh->vertices =
        malloc(CCRAFTE_INITIAL_VERTEX_CAPACITY * sizeof(struct CCRAFTE_Vertex));
    if (!mesh->vertices) {
        CCRAFTE_free_mesh(mesh);
        mesh = NULL;

        return NULL;
    }

    mesh->indices = malloc(CCRAFTE_INITIAL_VERTEX_CAPACITY * sizeof(GLuint));
    if (!mesh->indices) {
        CCRAFTE_free_mesh(mesh);
        mesh = NULL;

        return NULL;
    }

    mesh->vertices_capacity = CCRAFTE_INITIAL_VERTEX_CAPACITY;

    return mesh;
}

void CCRAFTE_free_mesh(struct CCRAFTE_Mesh *mesh) {
    if (mesh->vertices) {
        free(mesh->vertices);
        mesh->vertices = NULL;
    }

    if (mesh->indices) {
        free(mesh->indices);
        mesh->indices = NULL;
    }

    if (mesh->VAO) {
        glDeleteVertexArrays(1, &mesh->VAO);
        mesh->VAO = 0;
    }

    if (mesh->VBO) {
        glDeleteBuffers(1, &mesh->VBO);
        mesh->VBO = 0;
    }

    if (mesh->EBO) {
        glDeleteBuffers(1, &mesh->EBO);
        mesh->EBO = 0;
    }

    free(mesh);
}

void CCRAFTE_mesh_update_buffers(struct CCRAFTE_Mesh *mesh) {
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 mesh->vertices_length * sizeof(struct CCRAFTE_Vertex),
                 mesh->vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

struct CCRAFTE_Mesh *
CCRAFTE_create_mesh_from_vertices(struct CCRAFTE_Vertex *vertices,
                                  size_t vertex_count) {
    struct CCRAFTE_Mesh *mesh = CCRAFTE_create_mesh_uninitialized_vertices();
    mesh->vertices = malloc(sizeof(struct CCRAFTE_Vertex) * vertex_count);
    if (!mesh->vertices) {
        CCRAFTE_free_mesh(mesh);
        mesh = NULL;

        return NULL;
    }

    mesh->vertices_length = vertex_count;
    mesh->vertices_capacity = vertex_count;
    memcpy(mesh->vertices, vertices,
           vertex_count * sizeof(struct CCRAFTE_Vertex));

    CCRAFTE_mesh_update_buffers(mesh);

    return mesh;
}
