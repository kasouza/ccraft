#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_TRANSFORM_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_TRANSFORM_H

#include "ccraft/engine/quaternion.h"
#include "ccraft/engine/vec3.h"

struct CCRAFTE_Transform {
    union CCRAFTE_Vec3 translation;
    union CCRAFTE_Vec3 scale;
    union CCRAFTE_Quaternion rotation;
};

#endif

