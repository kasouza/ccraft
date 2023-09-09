#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_SHADERS_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_SHADERS_H

#include "ccraft/engine/gl_includes.h"

extern const char *CCRAFTE_sprite_vertex_shader;
extern const char *CCRAFTE_sprite_fragment_shader;

extern const char *CCRAFTE_3d_vertex_shader;
extern const char *CCRAFTE_3d_fragment_shader;

extern const char *CCRAFTE_debug_vertex_shader;
extern const char *CCRAFTE_debug_fragment_shader;

GLuint CCRAFTE_load_shader_from_file(const char *path, GLenum shader_type);
GLuint CCRAFTE_load_shader(const char *shader_src, GLenum shader_type);
GLuint CCRAFTE_create_program_from_files(const char *vertex_shader_path,
                                 const char *fragment_shader_path);
GLuint CCRAFTE_create_program(GLuint vertex_shader, GLuint fragment_shader);

#endif
