#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_DEBUG_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_INCLUDE_CCRAFT_ENGINE_DEBUG_H

#include <stdio.h>

#define CCRAFTE_DEBUG_LOG(str) \
    fprintf(stderr, "[%s:%d] %s\n", __FILE__, __LINE__, (str));

#define CCRAFTE_DEBUG_LOG_ARGS(str, ...) \
    fprintf(stderr, "[%s:%d]" str "\n", __FILE__, __LINE__, __VA_ARGS__);
#endif
