#include "ccraft/engine/utils.h"

#include <stdio.h>
#include <stdlib.h>

int CCRAFTE_3d_to_index(int x, int y, int z, int width, int height) {
    return (z * height + y) * width + x;
}

union CCRAFTE_Vec3i CCRAFTE_index_to_3d(int idx, int x_max, int y_max) {
    int z = idx / (x_max * y_max);
    idx -= (z * x_max * y_max);
    int y = idx / x_max;
    int x = idx % x_max;

    union CCRAFTE_Vec3i vec = {{ x, y, z }};
    return vec;
}

char *CCRAFTE_read_file(const char *path)
{
    FILE *file_pointer = fopen(path, "r");
    if (!file_pointer)
    {
        BOX_LOG("Could not open file: \"%s\"\n", path);
        exit(-1);
    }

    fseek(file_pointer, 0L, SEEK_END);
    long size = ftell(file_pointer);
    fseek(file_pointer, 0L, SEEK_SET);

    char *file_content = malloc(size + 1);
    fread(file_content, sizeof(char), size, file_pointer);

    file_content[size] = '\0';

    return file_content;
}

void *CCRAFTE_malloc(unsigned long size)
{
    void *memory = malloc(size);
    if (!memory)
    {
        fprintf(stderr, "Could not allocate memory, exiting...\n");
        exit(-1);
    }

    return memory;
}

void *CCRAFTE_calloc(unsigned long n, unsigned long size)
{
    void *memory = calloc(n, size);
    if (!memory)
    {
        fprintf(stderr, "Could not allocate memory, exiting...\n");
        exit(-1);
    }

    return memory;
}
