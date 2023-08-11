#include "ccraft/engine/camera.h"
#include "ccraft/engine/vec3.h"

struct CCRAFTE_Camera CCRAFTE_create_camera() {
    struct CCRAFTE_Camera camera = {0};

    camera.target = (union CCRAFTE_Vec3){{ 0.0, 0.0, -1.0 }};
    camera.up = (union CCRAFTE_Vec3){{ 0.0, 1.0, 0.0 }};

    return camera;
}
