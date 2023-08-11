#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_CAMERA_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_CAMERA_H

#include "ccraft/engine/vec3.h"

struct CCRAFTE_Camera{
    union CCRAFTE_Vec3 position;

    union CCRAFTE_Vec3 target;
    union CCRAFTE_Vec3 up;

    union CCRAFTE_Vec3 world_up;
};

struct CCRAFTE_Camera CCRAFTE_create_camera();

#endif
