#include "ccraft/engine/error.h"
#include "ccraft/engine/input.h"
#include "ccraft/engine/mat4.h"
#include "ccraft/engine/mesh.h"
#include "ccraft/engine/renderer.h"
#include "ccraft/engine/texture.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

void handle_camera_controls(struct CCRAFTE_Camera *camera) {
    float speed = 10 * CCRAFTE_get_deltatime();

    if (CCRAFTE_is_key_pressed(CCRAFTE_KEY_W)) {
        CCRAFTE_camera_move(camera, CCRAFTE_DIRECTION_FORWARD, speed);
    }

    if (CCRAFTE_is_key_pressed(CCRAFTE_KEY_S)) {
        CCRAFTE_camera_move(camera, CCRAFTE_DIRECTION_BACKWARD, speed);
    }

    if (CCRAFTE_is_key_pressed(CCRAFTE_KEY_A)) {
        CCRAFTE_camera_move(camera, CCRAFTE_DIRECTION_LEFT, speed);
    }

    if (CCRAFTE_is_key_pressed(CCRAFTE_KEY_D)) {
        CCRAFTE_camera_move(camera, CCRAFTE_DIRECTION_RIGHT, speed);
    }

    if (CCRAFTE_is_key_pressed(CCRAFTE_KEY_SPACE)) {
        CCRAFTE_camera_move(camera, CCRAFTE_DIRECTION_UPWARD, speed);
    }

    if (CCRAFTE_is_key_pressed(CCRAFTE_KEY_Z)) {
        CCRAFTE_camera_move(camera, CCRAFTE_DIRECTION_DOWNWARD, speed);
    }

    CCRAFTE_camera_update_rotation(camera);
}

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

    struct CCRAFTE_Camera camera = CCRAFTE_create_camera();
    camera.position.z = 10;

    struct CCRAFTE_Texture *mogus = CCRAFTE_load_texture("assets/amogus.png");
    struct CCRAFTE_Mesh *mesh = CCRAFTE_create_mesh_from_vertices(vertices, 36);
    if (!mesh) {
        fprintf(stderr, "ERO\n");
        return 1;
    }

    bool is_running = true;

    mesh->transform.translation.z = -20;
    union CCRAFTE_Vec3 dir = {{1, 1, 0}};
    dir = CCRAFTE_vec3_normalize(dir);

    while (is_running) {
        // Events
        if (CCRAFTE_is_key_pressed(CCRAFTE_KEY_ESCAPE)) {
            is_running = false;
        }

        handle_camera_controls(&camera);

        // Render
        CCRAFTE_clear();
        CCRAFTE_draw_mesh(&camera, mesh);

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
