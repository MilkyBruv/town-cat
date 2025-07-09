#ifndef ENTITY_H
#define ENTITY_H

#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <usrtypes.h>

typedef struct rect
{
    s16 x, y, width, height;
} rect_t;

typedef struct animation
{
    ALLEGRO_BITMAP** bitmaps;
    u8 current_frame, total_frames;
} animation_t;

typedef struct player
{
    rect_t rect;
    animation_t anim;
    u32 coins;
} player_t;

typedef struct static_animation
{
    rect_t rect;
    animation_t anim;
} static_animation_t;

b32 hits_rect(rect_t a, rect_t b);
b32 is_rect_in_range(rect_t a, rect_t b);
void update_animation(animation_t* anim);
animation_t create_animation(ALLEGRO_BITMAP* bitmaps[], u8 total_frames);
ALLEGRO_BITMAP* get_current_animation_frame(animation_t anim);
void destroy_animation(animation_t* anim);

static_animation_t create_static_animation(rect_t rect, animation_t anim);
void update_static_animation(static_animation_t* stanim);
void destroy_static_animation(static_animation_t* stanim);

#endif