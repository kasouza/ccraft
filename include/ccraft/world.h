#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_INCLUDE_CCRAFT_WORLD_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_INCLUDE_CCRAFT_WORLD_H

#include "ccraft/engine/vec3i.h"

#include <klib/linked_list.h>

struct CCRAFT_World {
    union CCRAFTE_Vec3i position;
    union CCRAFTE_Vec3i size;

    struct KLIB_LinkedList *chunks;
};

struct CCRAFT_World *CCRAFT_create_world(union CCRAFTE_Vec3i position, union CCRAFTE_Vec3i size);
void CCRAFT_free_world(struct CCRAFT_World *world);

void CCRAFTE_world_move_to(struct CCRAFT_World* world, union CCRAFTE_Vec3i new_world_pos);

#endif
