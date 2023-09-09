#include "ccraft/engine/shaders.h"
#include "ccraft/engine/utils.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// clang-format off
const char *CCRAFTE_sprite_vertex_shader =
    "#version 330 core\n"

    "layout (location = 0) in vec3 a_pos;"
    "layout (location = 1) in vec2 a_uv;"

    "uniform float window_width;"
    "uniform float window_height;"

    "uniform float translation_x;"
    "uniform float translation_y;"

    "uniform vec2 scale;"

    "uniform float rotation;"
    
    "uniform vec2 pivot;"

    "uniform vec2 uv_1;"
    "uniform vec2 uv_2;"

    "uniform vec2 sprite_size;"

    "out vec2 tex_coords;"

    "void main() {"
        "float cos_theta = cos(rotation);"
        "float sin_theta = sin(rotation);"

        "float x_scaled = a_pos.x * scale.x - pivot.x;"
        "float y_scaled = a_pos.y * scale.y - pivot.y;"

        "float x_rotated = x_scaled * cos_theta - y_scaled * sin_theta;"
        "float y_rotated = x_scaled * sin_theta + y_scaled * cos_theta;"

        "gl_Position = vec4((x_rotated + translation_x) / window_width * 2.0 - "
        "1.0, (y_rotated + translation_y) / window_height * 2.0 - 1.0, 0.0, 1.0);"

        "tex_coords.x = (uv_1.x + ((uv_2.x - uv_1.x) * a_uv.x)) / sprite_size.x;"
        "tex_coords.y = (uv_1.y + ((uv_2.y - uv_1.y) * a_uv.y)) / sprite_size.y;"
    "}";

const char *CCRAFTE_sprite_fragment_shader =
    "#version 330 core\n"

    "in vec2 tex_coords;"
    
    "uniform sampler2D tex;"
    "out vec4 frag_color;"
    
    "void main() {"
        "frag_color = texture(tex, tex_coords);"
        "if (frag_color.a == 0.0) {"
            "discard;"
        "}"
    "}";


const char *CCRAFTE_3d_vertex_shader =
    "#version 330 core\n"

    "layout (location = 0) in vec3 a_pos;"
    "layout (location = 1) in vec2 a_tex_coords;"
    "layout (location = 2) in float a_texture_array_index;"

    "uniform mat4 model_matrix;"
    "uniform mat4 view_matrix;"
    "uniform mat4 projection_matrix;"

    "out vec2 tex_coords;"
    "out float texture_array_index;"

    "void main() {"
        "gl_Position = projection_matrix * view_matrix * model_matrix * vec4(a_pos, 1.0);"
        "tex_coords = a_tex_coords;"
        "texture_array_index = a_texture_array_index;"
    "}";

const char *CCRAFTE_3d_fragment_shader =
    "#version 330 core\n"

    "uniform sampler2DArray tex;"

    "in vec2 tex_coords;"
    "in float texture_array_index;"
    "out vec4 frag_color;"
    
    "void main() {"
        "frag_color = texture(tex, vec3(tex_coords, texture_array_index));"
        "if (frag_color.a == 0) {"
            "discard;"
        "}"
    "}";

const char *CCRAFTE_debug_vertex_shader =
    "#version 330 core\n"

    "layout (location = 0) in vec3 a_pos;"

    "uniform float scale;"
    "uniform vec3 translation;"
    "uniform mat4 view_matrix;"
    "uniform mat4 projection_matrix;"

    "void main() {"
        "gl_Position = projection_matrix * view_matrix * vec4(a_pos * scale + translation, 1.0);"
    "}";

const char *CCRAFTE_debug_fragment_shader =
    "#version 330 core\n"

    "uniform vec4 u_color;"

    "out vec4 frag_color;"
    
    "void main() {"
        "frag_color = u_color;"
    "}";

// clang-format on

GLuint CCRAFTE_load_shader_from_file(const char *path, GLenum shader_type) {
    char *shader_src = CCRAFTE_read_file(path);
    GLuint shader = CCRAFTE_load_shader(shader_src, shader_type);
    free(shader_src);

    return shader;
}

GLuint CCRAFTE_load_shader(const char *shader_src, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    if (!shader) {
        /* DEBUG_LOG("Could not load shader\"%s\"", path); */
        return 0;
    }

    glShaderSource(shader, 1, (const char *const *)&shader_src, NULL);

    glCompileShader(shader);

    int res = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
    if (!res) {
        const GLsizei INFO_LOG_MAX_LENGTH = 256;
        GLchar info_log[INFO_LOG_MAX_LENGTH];
        GLsizei info_log_length = 0;

        glGetShaderInfoLog(shader, INFO_LOG_MAX_LENGTH, &info_log_length,
                           info_log);

        fprintf(stderr, "shader  error: %s\n", info_log);

        glDeleteShader(shader);

        return 0;
    }

    return shader;
}

GLuint CCRAFTE_create_program_from_files(const char *vertex_shader_path,
                                         const char *fragment_shader_path) {
    GLuint vertex =
        CCRAFTE_load_shader_from_file(vertex_shader_path, GL_VERTEX_SHADER);
    GLuint fragment =
        CCRAFTE_load_shader_from_file(fragment_shader_path, GL_FRAGMENT_SHADER);

    if (!vertex || !fragment)
        return 0;

    return CCRAFTE_create_program(vertex, fragment);
}

GLuint CCRAFTE_create_program(GLuint vertex_shader, GLuint fragment_shader) {
    assert(vertex_shader != 0);
    assert(fragment_shader != 0);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    int res = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &res);
    if (!res) {
        const GLsizei INFO_LOG_MAX_LENGTH = 256;
        GLchar info_log[INFO_LOG_MAX_LENGTH];
        GLsizei info_log_length = 0;

        glGetProgramInfoLog(program, INFO_LOG_MAX_LENGTH, &info_log_length,
                            info_log);

        fprintf(stderr, "program error: %s\n", info_log);

        glDeleteProgram(program);
        return 0;
    }

    return program;
}
