#include "ccraft/engine/error.h"
#include <assert.h>

const char *s_error_messages[CCRAFTE_ERROR_LAST] = {
    [CCRAFTE_SUCCESS] = "",
    [CCRAFTE_ERROR_GLFW_INIT] = "Could not initialize the GLFW library.",
    [CCRAFTE_ERROR_WINDOW_CREATION] = "Could not create a window.",
    [CCRAFTE_ERROR_GL_LOAD] = "Could not load OpenGL.",
    [CCRAFTE_ERROR_PROGRAM_CREATION] = "Could not create shader program.",
    [CCRAFTE_ERROR_FRAMEBUFFER_CREATION] = "Could not create framebuffer.",
};

const char *kgl_get_error_message(enum CCRAFTE_Error error)
{
    assert(error >= 0 && error < CCRAFTE_ERROR_LAST && "Error out of bounds.");
    return s_error_messages[error];
}
