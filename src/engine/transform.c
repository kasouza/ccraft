#include "ccraft/engine/transform.h"

struct CCRAFTE_Transform CCRAFTE_create_transform() {
    struct CCRAFTE_Transform transform = { 0 };

    transform.scale.x = 1;
    transform.scale.y = 1;
    transform.scale.z = 1;
    transform.rotation.w = 1;

    return transform;
}
