#include "ccraft/engine/error.h"
#include "ccraft/engine/renderer.h"
#include "ccraft/engine/input.h"
#include "ccraft/engine/texture.h"
#include "ccraft/engine/mesh.h"
#include "gsl/gsl_blas.h"
#include "gsl/gsl_vector_double.h"

#include <stdbool.h>
#include <stdio.h>

int main() {
    enum CCRAFTE_Error error = CCRAFTE_init(800, 600, 0);
    if (error != CCRAFTE_SUCCESS) {
        printf("%s\n", CCRAFTE_get_error_message(error));
        return error;
    }

    struct CCRAFTE_Vertex vertices[] = {
        {{{-0.5, -0.5, 0}}},
        {{{ 0.5, -0.5, 0}}},
        {{{ 0.0,  0.5, 0}}},
    };

    struct CCRAFTE_Texture* mogus = CCRAFTE_load_texture("assets/amogus.png");
    struct CCRAFTE_Mesh* mesh = CCRAFTE_create_mesh_from_vertices(vertices, 6);
    if (!mesh) {
        fprintf(stderr, "ERO\n");
        return 1;
    }

    bool is_running = true;

    while (is_running) {
        // Events
        if (CCRAFTE_is_key_pressed(CCRAFTE_KEY_ESCAPE)) {
            is_running = false;
        }

        // Render
        CCRAFTE_clear();

        CCRAFTE_draw_mesh(mesh);

        CCRAFTE_present();

        int error = glGetError();
        while (error) {
            printf("Error: %d\n", error);
            error = glGetError();
        }
    }

    CCRAFTE_free_texture(mogus);
    mogus = NULL;

    CCRAFTE_free_mesh(mesh);
    mesh = NULL;

    CCRAFTE_terminate();

    return 0;
}
