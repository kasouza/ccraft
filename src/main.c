#include "ccraft/engine/error.h"
#include "ccraft/engine/input.h"
#include "ccraft/engine/mat4.h"
#include "ccraft/engine/mesh.h"
#include "ccraft/engine/renderer.h"
#include "ccraft/engine/texture.h"

#include <stdbool.h>
#include <stdio.h>

int main() {
    enum CCRAFTE_Error error = CCRAFTE_init(800, 600, 0);
    if (error != CCRAFTE_SUCCESS) {
        printf("%s\n", CCRAFTE_get_error_message(error));
        return error;
    }

    struct CCRAFTE_Vertex vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
        0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
        0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
        0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f,
    };

    struct CCRAFTE_Texture *mogus = CCRAFTE_load_texture("assets/amogus.png");
    struct CCRAFTE_Mesh *mesh = CCRAFTE_create_mesh_from_vertices(vertices, 36);
    if (!mesh) {
        fprintf(stderr, "ERO\n");
        return 1;
    }

    mesh->matrix.a34 = 10;

    bool is_running = true;

    float theta = 0.1;

    while (is_running) {
        // Events
        if (CCRAFTE_is_key_pressed(CCRAFTE_KEY_ESCAPE)) {
            is_running = false;
        }

        theta += 0.01;

        // Update
        union CCRAFTE_Mat4 rotation_matrix = CCRAFTE_mat4_rotation(theta, theta, theta);
        rotation_matrix.a34 = 4;

        mesh->matrix = rotation_matrix;

        // Render
        CCRAFTE_clear();
        CCRAFTE_draw_mesh(mesh);
        CCRAFTE_present();

        CCRAFTE_handle_gl_errors();
    }

    CCRAFTE_free_texture(mogus);
    mogus = NULL;

    CCRAFTE_free_mesh(mesh);
    mesh = NULL;

    CCRAFTE_terminate();

    return 0;
}
