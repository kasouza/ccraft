#ifndef KASOUZA_KGL_INCLUDE_KGL_UTILS_H
#define KASOUZA_KGL_INCLUDE_KGL_UTILS_H

#include "ccraft/engine/vec3i.h"

#define BOX_LOG(...) fprintf(stderr, __VA_ARGS__)

#define BOX_MIN(a, b) ((a) < (b) ? (a) : (b))
#define BOX_MAX(a, b) ((a) > (b) ? (a) : (b))
#define BOX_CLAMP(n, min, max) (BOX_MIN(BOX_MAX(n, min), max))

#define CCRAFTE_UNUSED(x) (void)(x)

int CCRAFTE_3d_to_index(int x, int y, int z, int width, int height);
union CCRAFTE_Vec3i CCRAFTE_index_to_3d(int idx, int x_max, int y_max);

char *CCRAFTE_read_file(const char *path);
void *CCRAFTE_malloc(unsigned long bytes);
void *CCRAFTE_calloc(unsigned long n, unsigned long size);

#endif
