#include "ccraft/engine/camera.h"
#include "ccraft/engine/input.h"
#include "ccraft/engine/math.h"
#include "ccraft/engine/renderer.h"
#include "ccraft/engine/vec3.h"

struct CCRAFTE_Camera CCRAFTE_create_camera() {
    struct CCRAFTE_Camera camera = {0};

    camera.yaw = CCRAFTE_RADIANS(-90);
    camera.world_up.x = 0.0f;
    camera.world_up.y = 1.0f;
    camera.world_up.z = 0.0f;

    return camera;
}

void CCRAFTE_camera_update_rotation(struct CCRAFTE_Camera *camera) {
    union CCRAFTE_Vec2 mouse_pos = CCRAFTE_get_mouse_position();

    static bool first = true;
    static double previous_x = 0;
    static double previous_y = 0;

    if (first) {
        previous_x = mouse_pos.x;
        previous_y = mouse_pos.y;
        first = false;
    }


    double dx = (mouse_pos.x - previous_x) * (M_PI / 180) * CCRAFTE_get_deltatime();
    double dy = (mouse_pos.y - previous_y) * (M_PI / 180) * CCRAFTE_get_deltatime();

    camera->yaw += dx;
    camera->pitch += dy;

    if (camera->pitch >= CCRAFTE_RADIANS(89.0f)) {
        camera->pitch = CCRAFTE_RADIANS(89.0f);
    } else if (camera->pitch <= CCRAFTE_RADIANS(-89.0f)) {
        camera->pitch = CCRAFTE_RADIANS(-89.0f);
    }

    previous_x = mouse_pos.x;
    previous_y = mouse_pos.y;
}

void CCRAFTE_camera_move(struct CCRAFTE_Camera* camera, enum CCRAFTE_Direction direction, float speed) {

    union CCRAFTE_Vec3 direction_vec = {0};

    union CCRAFTE_Vec3 target =
        CCRAFTE_vec3_direction(camera->yaw, camera->pitch);
    union CCRAFTE_Vec3 right =
        CCRAFTE_vec3_normalize(CCRAFTE_vec3_cross(target, camera->world_up));
    union CCRAFTE_Vec3 up =
        CCRAFTE_vec3_normalize(CCRAFTE_vec3_cross(target, right));

    switch (direction) {
    case CCRAFTE_DIRECTION_FORWARD:
        direction_vec = target;
        break;
    case CCRAFTE_DIRECTION_BACKWARD:
        direction_vec = CCRAFTE_vec3_scale(target, -1);
        break;

    case CCRAFTE_DIRECTION_LEFT:
        direction_vec = CCRAFTE_vec3_scale(right, -1);
        break;

    case CCRAFTE_DIRECTION_RIGHT:
        direction_vec = right;
        break;

    case CCRAFTE_DIRECTION_UPWARD:
        direction_vec = up;
        break;

    case CCRAFTE_DIRECTION_DOWNWARD:
        direction_vec = CCRAFTE_vec3_scale(up, -1);
        break;
    }

    camera->position = CCRAFTE_vec3_add(
        camera->position, CCRAFTE_vec3_scale(direction_vec, speed));
}
