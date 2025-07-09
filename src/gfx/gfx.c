#include "gfx.h"

void init_framebuffer()
{   
    fb = malloc(sizeof(framebuffer_t));
    fb->bitmap = al_create_bitmap(FB_WIDTH, FB_HEIGHT);
    fb->scale = 1.0f;
    fb->x = 0;
    fb->y = 0;
    fb->scaled_width = FB_WIDTH;
    fb->scaled_height = FB_HEIGHT;
    fb->mouse_x = 0;
    fb->mouse_y = 0;
}

void init_shaders()
{
    shader = al_create_shader(ALLEGRO_SHADER_GLSL);
    al_attach_shader_source_file(shader, ALLEGRO_VERTEX_SHADER, "./res/shader/vertex.glsl");
    al_attach_shader_source_file(shader, ALLEGRO_PIXEL_SHADER, "./res/shader/fragment.glsl");
    al_build_shader(shader);
}

void init_text()
{
    text_bitmap = al_load_bitmap("./res/text.png");
    // Loop over all sub bitmaps and create image at each char int value index
    for (size_t i = 0; i < 40; i++)
    {
        char_bitmaps[(u8) char_ref[i]] = al_create_sub_bitmap(text_bitmap, i * 8, 0, 8, 8);
    }
}

void init_gfx()
{
    init_framebuffer();
    init_shaders();
    init_text();
}

void update_framebuffer_mouse_pos(ALLEGRO_MOUSE_STATE mouse_state)
{
    fb->mouse_x = (mouse_state.x - fb->x) / fb->scale;
    fb->mouse_y = (mouse_state.y - fb->y) / fb->scale;
    printf("%d,%d", fb->mouse_x, fb->mouse_y);
}

void scale_and_draw_framebuffer(ALLEGRO_DISPLAY* display)
{
    fb->scale = (f32) min(
        (f32) ((f32) al_get_display_width(display) / (f32) FB_WIDTH),
        (f32) ((f32) al_get_display_height(display) / (f32) FB_HEIGHT)
    );
    fb->scaled_width = FB_WIDTH * fb->scale;
    fb->scaled_height = FB_HEIGHT * fb->scale;
    fb->x = (al_get_display_width(display) / 2) - (fb->scaled_width / 2);
    fb->y = (al_get_display_height(display) / 2) - (fb->scaled_height / 2);
    al_draw_scaled_bitmap(fb->bitmap, 0, 0, FB_WIDTH, FB_HEIGHT, fb->x, fb->y,
        fb->scaled_width, fb->scaled_height, 0);
}

void enable_framebuffer_drawing()
{
    al_set_target_bitmap(fb->bitmap);
}

void disable_framebuffer_drawing(ALLEGRO_DISPLAY * display)
{
    al_set_target_bitmap(al_get_backbuffer(display));
}

void enable_shader()
{
    al_use_shader(shader);
}

void disable_shader()
{
    al_use_shader(NULL);
}

void draw_text(char* text, u8 cx, u8 y)
{
    u8 width = strlen(text);
    for (size_t i = 0; i < width; i++)
    {
        al_draw_bitmap(char_bitmaps[(u8) text[i]], (cx - ((width * 8) / 2)) + (i * 8), y, 0);
    }
}

void destroy_framebuffer()
{
    al_destroy_bitmap(fb->bitmap);
    free(fb);
    fb = NULL;
}

void destroy_shader()
{
    al_destroy_shader(shader);
}

void destroy_text()
{
    for (size_t i = 0; i < 40; i++)
    {
        if (char_bitmaps[char_ref[i]])
        {
            al_destroy_bitmap(char_bitmaps[(u8) char_ref[i]]);
        }
    }

    al_destroy_bitmap(text_bitmap);
}

void destroy_gfx()
{
    destroy_text();
    destroy_framebuffer();
    destroy_shader();
}
