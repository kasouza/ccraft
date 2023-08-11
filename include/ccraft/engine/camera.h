#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_CAMERA_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_CAMERA_H

#include "ccraft/engine/direction.h"
#include "ccraft/engine/vec3.h"

struct CCRAFTE_Camera{
    union CCRAFTE_Vec3 position;

    float yaw;
    float pitch;

    union CCRAFTE_Vec3 world_up;
};

struct CCRAFTE_Camera CCRAFTE_create_camera();

void CCRAFTE_camera_update_rotation(struct CCRAFTE_Camera *camera);
void CCRAFTE_camera_move(struct CCRAFTE_Camera* camera, enum CCRAFTE_Direction direction, float speed);

#endif
