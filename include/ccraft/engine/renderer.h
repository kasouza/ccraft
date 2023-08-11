#ifndef KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_RENDERER_H
#define KASOUZA_CCRAFT_INCLUDE_CCRAFT_ENGINE_RENDERER_H

#include "ccraft/engine/camera.h"
#include "ccraft/engine/gl_includes.h"
#include "ccraft/engine/error.h"

struct CCRAFTE_Texture;
struct CCRAFTE_Mesh;

enum CCRAFTE_InitFlags
{
    CCRAFTE_UNLOCK_FPS = 0x00000001,
};

enum CCRAFTE_Error CCRAFTE_init(int canvas_width, int canvas_height, enum CCRAFTE_InitFlags flags);
void CCRAFTE_present();
void CCRAFTE_clear();
void CCRAFTE_clear_colored(float r, float g, float b, float a);
void CCRAFTE_terminate();

void CCRAFTE_draw_mesh(struct CCRAFTE_Camera* camera, struct CCRAFTE_Mesh *mesh);
void CCRAFTE_draw_texture(struct CCRAFTE_Texture *sprite, double x, double y, double width,
                     double height, double rotation, double pivot_x,
                     double pivot_y);

void CCRAFTE_draw_sub_texture(struct CCRAFTE_Texture *sprite, double x, double y, double width,
                         double height, double rotation, double pivot_x,
                         double pivot_y, double sub_x1, double sub_y1,
                         double sub_x2, double sub_y2);

double CCRAFTE_get_deltatime();

GLFWwindow *CCRAFTE_get_window();

#endif
