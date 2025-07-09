#ifndef GFX_H
#define GFX_H

#include <allegro5/allegro.h>
#include <usrtypes.h>
#include <stdio.h>

#define FB_WIDTH 160
#define FB_HEIGHT 160
#define min(a, b) ((a) < (b) ? (a) : (b))

typedef struct framebuffer
{
    ALLEGRO_BITMAP* bitmap;
    f32 scale;
    u16 scaled_width, scaled_height;
    u16 x, y, mouse_x, mouse_y;
} framebuffer_t;

static framebuffer_t* fb = NULL;
static ALLEGRO_SHADER* shader;
static ALLEGRO_BITMAP* text_bitmap;
static const char* char_ref = "abcdefghijklmnopqrstuvwxyz1234567890!?$ "; // 40 chars
static ALLEGRO_BITMAP* char_bitmaps[128] = {NULL};

void init_gfx();
void init_framebuffer();
void init_shaders();
void update_framebuffer_mouse_pos(ALLEGRO_MOUSE_STATE mouse_state);
void scale_and_draw_framebuffer(ALLEGRO_DISPLAY* display);
void enable_framebuffer_drawing();
void disable_framebuffer_drawing(ALLEGRO_DISPLAY* display);
void enable_shader();
void disable_shader();
void destroy_framebuffer();
void destroy_shader();
void destroy_gfx();

void init_text();
void draw_text(char* text, u8 cx, u8 y);
void destroy_text();

#endif