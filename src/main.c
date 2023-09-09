#include "ccraft/chunk.h"
#include "ccraft/engine/debug.h"
#include "ccraft/engine/error.h"
#include "ccraft/engine/input.h"
#include "ccraft/engine/mat4.h"
#include "ccraft/engine/mesh.h"
#include "ccraft/engine/renderer.h"
#include "ccraft/engine/texture.h"
#include "ccraft/engine/texture_array.h"
#include "ccraft/engine/utils.h"
#include "ccraft/meshing.h"
#include "ccraft/world.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RENDER_DISTANCE 1
#define WORLD_SIZE 2 * RENDER_DISTANCE + 1

void handle_camera_controls(struct CCRAFTE_Camera *camera,
                            struct CCRAFT_World *world) {
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
        .x = floor(camera->position.x / CCRAFT_CHUNK_SIZE),
        .y = floor(camera->position.y / CCRAFT_CHUNK_SIZE),
        .z = floor(camera->position.z / CCRAFT_CHUNK_SIZE),
    };

    union CCRAFTE_Vec3i world_center_offset = {
        .x = world->size.x / 2,
        .y = world->size.y / 2,
        .z = world->size.z / 2,
    };

    union CCRAFTE_Vec3i world_center =
        CCRAFTE_vec3i_add(world->position, world_center_offset);

    if (!CCRAFTE_vec3i_equals(camera_world_pos, world_center)) {
        union CCRAFTE_Vec3i new_world_pos =
            CCRAFTE_vec3i_subtract(camera_world_pos, world_center_offset);

        CCRAFTE_world_move_to(world, new_world_pos);
    }

    CCRAFTE_camera_update_rotation(camera);
}

int main() {
    srand(time(NULL));

    enum CCRAFTE_Error error = CCRAFTE_init(800, 600, 0);
    if (error != CCRAFTE_SUCCESS) {
        printf("%s\n", CCRAFTE_get_error_message(error));
        return error;
    }

    struct CCRAFTE_Camera camera = CCRAFTE_create_camera();

    struct CCRAFTE_TextureArray *tex_array =
        CCRAFTE_create_texture_array(16, 16, CCRAFT_VOXEL_TYPE_LAST * 6);
    if (tex_array == NULL) {
        return 1;
    }

    CCRAFT_init_voxel_types(tex_array);

    union CCRAFTE_Vec3i camera_world_pos = {
        .x = camera.position.x / CCRAFT_CHUNK_SIZE,
        .y = camera.position.y / CCRAFT_CHUNK_SIZE,
        .z = camera.position.z / CCRAFT_CHUNK_SIZE,
    };

    union CCRAFTE_Vec3i world_center_offset = {
        .x = WORLD_SIZE / 2,
        .y = WORLD_SIZE / 2,
        .z = WORLD_SIZE / 2,
    };

    union CCRAFTE_Vec3i world_pos =
        CCRAFTE_vec3i_subtract(camera_world_pos, world_center_offset);
    /*world_pos.x = -1;*/
    /*world_pos.y = 0;*/
    /*world_pos.z = 0;*/

    struct CCRAFT_World *world = CCRAFT_create_world(
        world_pos, (union CCRAFTE_Vec3i){{WORLD_SIZE, WORLD_SIZE, WORLD_SIZE}});
    if (world == NULL) {
        return 1;
    }

    bool is_running = true;

    while (is_running) {
        // Events
        if (CCRAFTE_is_key_pressed(CCRAFTE_KEY_ESCAPE)) {
            is_running = false;
        }

        handle_camera_controls(&camera, world);

        // Update
        CCRAFT_world_update_dirty_chunks(world);

        // Render
        CCRAFTE_clear();

        for (int x = 0; x < world->size.x; x++) {
            for (int y = 0; y < world->size.y; y++) {
                for (int z = 0; z < world->size.z; z++) {
                    int idx = CCRAFTE_3d_to_index(x, y, z, world->size.x,
                                                  world->size.y);

                    if (world->chunks[idx] == NULL ||
                        world->chunks[idx]->mesh == NULL) {
                        CCRAFTE_DEBUG_LOG("null chunk or mesh when drawing");
                        continue;
                    }

                    struct CCRAFTE_Mesh *mesh = world->chunks[idx]->mesh;
                    mesh->transform.translation.x =
                        (world->position.x + x) * CCRAFT_CHUNK_SIZE;
                    mesh->transform.translation.y =
                        (world->position.y + y) * CCRAFT_CHUNK_SIZE;
                    mesh->transform.translation.z =
                        (world->position.z + z) * CCRAFT_CHUNK_SIZE;
                    mesh->transform.scale.x = .75;
                    mesh->transform.scale.y = .75;
                    mesh->transform.scale.z = .75;

                    CCRAFTE_draw_mesh(&camera, mesh, tex_array);
                    /*printf("%d\n", mesh->VAO);*/
                }
            }
        }

        /*printf("\n");*/
        CCRAFTE_draw_debug(&camera);

        CCRAFTE_present();

        CCRAFTE_handle_gl_errors();
    }

    CCRAFT_free_world(world);
    world = NULL;

    CCRAFTE_terminate();

    return 0;
}
