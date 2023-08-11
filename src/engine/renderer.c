#include "ccraft/engine/renderer.h"
#include "ccraft/engine/error.h"
#include "ccraft/engine/input.h"
#include "ccraft/engine/math.h"
#include "ccraft/engine/mesh.h"
#include "ccraft/engine/shaders.h"
#include "ccraft/engine/texture.h"
#include "ccraft/engine/utils.h"

#include <math.h>
#include <stb_image.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Call the setup function for `thing` and return a error if it ocurred.
#define SETUP(thing)                                                           \
    enum CCRAFTE_Error thing##_error = setup_##thing();                        \
    if (thing##_error != CCRAFTE_SUCCESS)                                      \
        return thing##_error;

static double s_delta_time = 1.0 / 60.0;

static bool init = false;
static enum CCRAFTE_InitFlags s_flags = 0;

static GLFWwindow *s_window = NULL;
static GLuint s_sprite_program = 0;
static GLuint s_3d_program = 0;

static GLuint s_rect_vao = 0;
static GLuint s_rect_vbo = 0;

static int s_window_width = 1024;
static int s_window_height = 600;

static float s_fov = CCRAFTE_RADIANS(90.0f);

static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height) {
    CCRAFTE_UNUSED(window);
    glViewport(0, 0, width, height);

    s_window_width = width;
    s_window_height = height;
}

static int setup_glfw() {
    // GLFW Initialization
    if (!glfwInit()) {
        return CCRAFTE_ERROR_GLFW_INIT;
    }

    // Window Creation
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    s_window = glfwCreateWindow(s_window_width, s_window_height, "CCRAFTE",
                                NULL, NULL);

    if (!s_window) {
        return CCRAFTE_ERROR_WINDOW_CREATION;
    }

    glfwMakeContextCurrent(s_window);

    glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(s_window, framebuffer_size_callback);

    return CCRAFTE_SUCCESS;
}

static int setup_gl() {
    // OpenGL setup
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return CCRAFTE_ERROR_GL_LOAD;
    }

    // glCullFace(GL_BACK);
    // glEnable(GL_CULL_FACE);

    glViewport(0, 0, s_window_width, s_window_height);

    return CCRAFTE_SUCCESS;
}

static enum CCRAFTE_Error setup_buffers() {
    // Rectangle
    float rect_vertices[] = {
        0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
        1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0,

        0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0,
        1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0,
    };

    glGenVertexArrays(1, &s_rect_vao);
    glGenBuffers(1, &s_rect_vbo);

    glBindVertexArray(s_rect_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_rect_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5,
                          (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5,
                          (void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    return CCRAFTE_SUCCESS;
}

static enum CCRAFTE_Error setup_programs() {
    // Sprite shaders
    GLuint sprite_vertex =
        CCRAFTE_load_shader(CCRAFTE_sprite_vertex_shader, GL_VERTEX_SHADER);
    GLuint sprite_fragment =
        CCRAFTE_load_shader(CCRAFTE_sprite_fragment_shader, GL_FRAGMENT_SHADER);
    if (!sprite_vertex || !sprite_fragment) {
        return CCRAFTE_ERROR_SHADER_COMPILATION;
    }

    s_sprite_program = CCRAFTE_create_program(sprite_vertex, sprite_fragment);
    if (!s_sprite_program) {
        return CCRAFTE_ERROR_PROGRAM_CREATION;
    }

    glDeleteShader(sprite_vertex);
    glDeleteShader(sprite_fragment);

    // 3d shaders
    GLuint vertex_3d =
        CCRAFTE_load_shader(CCRAFTE_3d_vertex_shader, GL_VERTEX_SHADER);
    GLuint fragment_3d =
        CCRAFTE_load_shader(CCRAFTE_3d_fragment_shader, GL_FRAGMENT_SHADER);
    if (!vertex_3d || !fragment_3d) {
        return CCRAFTE_ERROR_SHADER_COMPILATION;
    }

    s_3d_program = CCRAFTE_create_program(vertex_3d, fragment_3d);
    if (!s_3d_program) {
        return CCRAFTE_ERROR_PROGRAM_CREATION;
    }

    glDeleteShader(vertex_3d);
    glDeleteShader(fragment_3d);

    return CCRAFTE_SUCCESS;
}

enum CCRAFTE_Error CCRAFTE_init(int window_width, int window_height,
                                enum CCRAFTE_InitFlags flags) {
    s_window_width = window_width;
    s_window_height = window_height;

    SETUP(glfw);
    SETUP(gl);
    SETUP(programs);
    SETUP(buffers);

    init = true;
    s_flags = flags;

    CCRAFTE_input_init();

    return CCRAFTE_SUCCESS;
}

void CCRAFTE_clear() {
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void CCRAFTE_clear_colored(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void CCRAFTE_present() {
    // Update subsystems
    CCRAFTE_input_update();

    // Render
    static double start = -1;
    if (start == -1) {
        start = glfwGetTime();

    } else {
        double now = glfwGetTime();
        s_delta_time = now - start;
        start = now;
    }

    if (s_flags & CCRAFTE_UNLOCK_FPS) {
        glfwSwapInterval(0);
    }

    glfwSwapBuffers(s_window);
    glfwPollEvents();
}

void CCRAFTE_terminate() {
    if (!init) {
        return;
    }

    // Programs
    glDeleteProgram(s_sprite_program);
    s_sprite_program = 0;

    // VAOs and VBOs
    glDeleteVertexArrays(1, &s_rect_vao);
    glDeleteBuffers(1, &s_rect_vbo);

    glfwDestroyWindow(s_window);
    glfwTerminate();
}

void CCRAFTE_draw_texture(struct CCRAFTE_Texture *sprite, double x, double y,
                          double width, double height, double rotation,
                          double pivot_x, double pivot_y) {
    assert(sprite != NULL);
    assert(sprite->texture != 0);
    assert(sprite->width > 0 && sprite->height > 0);
    CCRAFTE_draw_sub_texture(sprite, x, y, width, height, rotation, pivot_x,
                             pivot_y, 0.0f, 0.0f, sprite->width,
                             sprite->height);
}

void CCRAFTE_draw_sub_texture(struct CCRAFTE_Texture *sprite, double x,
                              double y, double width, double height,
                              double rotation, double pivot_x, double pivot_y,
                              double sub_x1, double sub_y1, double sub_x2,
                              double sub_y2) {
    assert(sprite != NULL);
    assert(sprite->texture != 0);
    assert(sprite->width > 0 && sprite->height > 0);
    assert(sub_x1 >= 0 && sub_x1 <= sprite->width && sub_y1 >= 0 &&
           sub_y1 <= sprite->height &&
           "Sub sprite is out of the texture's bounds.");
    assert(sub_x2 >= 0 && sub_x2 <= sprite->width && sub_y2 >= 0 &&
           sub_y2 <= sprite->height &&
           "Sub sprite is out of the texture's bounds.");

    glUseProgram(s_sprite_program);
    glBindVertexArray(s_rect_vao);
    glBindTexture(GL_TEXTURE_2D, sprite->texture);

    glUniform1f(glGetUniformLocation(s_sprite_program, "window_width"),
                s_window_width);
    glUniform1f(glGetUniformLocation(s_sprite_program, "window_height"),
                s_window_height);

    // All of this can be simplified by using Matrices
    glUniform1f(glGetUniformLocation(s_sprite_program, "translation_x"), x);
    glUniform1f(glGetUniformLocation(s_sprite_program, "translation_y"), y);

    glUniform2f(glGetUniformLocation(s_sprite_program, "scale"), width, height);

    glUniform1f(glGetUniformLocation(s_sprite_program, "rotation"), rotation);

    glUniform2f(glGetUniformLocation(s_sprite_program, "pivot"), pivot_x,
                pivot_y);

    glUniform2f(glGetUniformLocation(s_sprite_program, "sprite_size"),
                sprite->width, sprite->height);

    glUniform2f(glGetUniformLocation(s_sprite_program, "uv_1"), sub_x1, sub_y1);
    glUniform2f(glGetUniformLocation(s_sprite_program, "uv_2"), sub_x2, sub_y2);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void CCRAFTE_draw_mesh(struct CCRAFTE_Camera *camera,
                       struct CCRAFTE_Mesh *mesh) {
    double x, y;
    glfwGetCursorPos(s_window, &x, &y);

    glUseProgram(s_3d_program);

    glBindVertexArray(mesh->VAO);
    glBindVertexArray(mesh->VBO);

    union CCRAFTE_Mat4 projection_matrix = CCRAFTE_mat4_perspective(
        s_fov, (float)s_window_width / s_window_height, 0.1f, 100.0f);

    union CCRAFTE_Mat4 model_matrix =
        CCRAFTE_mat4_model_from_transform(&mesh->transform);

    union CCRAFTE_Mat4 view_matrix = CCRAFTE_mat4_view_from_camera(camera);

    glUniformMatrix4fv(glGetUniformLocation(s_3d_program, "projection_matrix"),
                       1, GL_FALSE, projection_matrix.data);
    glUniformMatrix4fv(glGetUniformLocation(s_3d_program, "view_matrix"), 1,
                       GL_FALSE, view_matrix.data);
    glUniformMatrix4fv(glGetUniformLocation(s_3d_program, "model_matrix"), 1,
                       GL_FALSE, model_matrix.data);

    glDrawArrays(GL_TRIANGLES, 0, mesh->vertices_length);

    glBindVertexArray(0);
}

double CCRAFTE_get_deltatime() { return s_delta_time; }

GLFWwindow *CCRAFTE_get_window() { return s_window; }
