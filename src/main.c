#include "ccraft/chunk.h"
#include "ccraft/engine/error.h"
#include "ccraft/engine/input.h"
#include "ccraft/engine/mat4.h"
#include "ccraft/engine/mesh.h"
#include "ccraft/engine/renderer.h"
#include "ccraft/engine/texture.h"
#include "ccraft/engine/utils.h"
#include "ccraft/meshing.h"
#include "ccraft/world.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define WORLD_SIZE 4

void handle_camera_controls(struct CCRAFTE_Camera *camera, struct CCRAFT_World* world) {
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


    union CCRAFTE_Vec3i camera_world_pos = {
        .x = camera->position.x / CCRAFT_CHUNK_SIZE,
        .y = camera->position.y / CCRAFT_CHUNK_SIZE,
        .z = camera->position.z / CCRAFT_CHUNK_SIZE,
    };

    union CCRAFTE_Vec3i world_center_offset = {
        .x = world->size.x / 2,
        .y = world->size.y / 2,
        .z = world->size.z / 2,
    };

    union CCRAFTE_Vec3i world_center = CCRAFTE_vec3i_add(world->position, world_center_offset);


    if (!CCRAFTE_vec3i_equals(camera_world_pos, world_center)) {
        union CCRAFTE_Vec3i new_world_pos = CCRAFTE_vec3i_subtract(camera_world_pos, world_center_offset);
        CCRAFTE_world_move_to(world, new_world_pos);
        CCRAFTE_vec3i_print(camera_world_pos);
        CCRAFTE_vec3i_print(world_center);
        printf("\n");
    }

    CCRAFTE_camera_update_rotation(camera);
}

int main() {
    enum CCRAFTE_Error error = CCRAFTE_init(800, 600, 0);
    if (error != CCRAFTE_SUCCESS) {
        printf("%s\n", CCRAFTE_get_error_message(error));
        return error;
    }

    struct CCRAFTE_Camera camera = CCRAFTE_create_camera();
    camera.position.z = 10;

    struct CCRAFTE_Texture *mogus = CCRAFTE_load_texture("assets/amogus.png");
    struct CCRAFT_World *world =
        CCRAFT_create_world((union CCRAFTE_Vec3i){{0, 0, 0}}, (union CCRAFTE_Vec3i){{WORLD_SIZE, WORLD_SIZE, WORLD_SIZE}});
    if (!world) {
        fprintf(stderr, "ERO\n");
        return 1;
    }

    bool is_running = true;

    union CCRAFTE_Vec3 dir = {{1, 1, 0}};
    dir = CCRAFTE_vec3_normalize(dir);

    struct CCRAFTE_Mesh *meshes[WORLD_SIZE * WORLD_SIZE * WORLD_SIZE];
    for (int i = 0; i < WORLD_SIZE * WORLD_SIZE * WORLD_SIZE; i++) {
        meshes[i] =
            CCRAFT_create_simple_mesh(KLIB_linked_list_at(world->chunks, i));
    }

    while (is_running) {
        // Events
        if (CCRAFTE_is_key_pressed(CCRAFTE_KEY_ESCAPE)) {
            is_running = false;
        }

        handle_camera_controls(&camera, world);

        // Render
        CCRAFTE_clear();

        for (int x = 0; x < world->size.x; x++) {
            for (int y = 0; y < world->size.y; y++) {
                for (int z = 0; z < world->size.z; z++) {
                    int idx = CCRAFTE_3d_to_index(x, y, z, world->size.x,
                                              world->size.y);

                    struct CCRAFTE_Mesh* mesh = meshes[idx];
                    mesh->transform.translation.x = (world->position.x + x) * CCRAFT_CHUNK_SIZE;
                    mesh->transform.translation.y = (world->position.y + y) * CCRAFT_CHUNK_SIZE;
                    mesh->transform.translation.z = (world->position.z + z) * CCRAFT_CHUNK_SIZE;

                    CCRAFTE_draw_mesh(&camera, mesh);
                }
            }
        }

        CCRAFTE_present();

        CCRAFTE_handle_gl_errors();
    }

    CCRAFTE_free_texture(mogus);
    mogus = NULL;

    for (int i = 0; i < WORLD_SIZE * WORLD_SIZE * WORLD_SIZE; i++) {
        CCRAFTE_free_mesh(meshes[i]);
    }

    CCRAFTE_terminate();

    return 0;
}
