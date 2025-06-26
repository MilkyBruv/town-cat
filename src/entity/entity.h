#ifndef ENTITY_H
#define ENTITY_H

#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "./../macros.h"

typedef struct
{
    int16_t x, y, width, height;
} rect_t;

typedef struct
{
    ALLEGRO_BITMAP** bitmaps;
    uint8_t current_frame, total_frames;
} animation_t;

typedef struct
{
    rect_t rect;
    animation_t anim;
} smoke_t;

typedef struct
{
    rect_t rect;
    animation_t anim;
} player_t;

bool hits_rect(rect_t a, rect_t b);
bool is_rect_in_range(rect_t a, rect_t b);
void animate(animation_t* anim);
animation_t create_animation(ALLEGRO_BITMAP* bitmaps[], uint8_t total_frames);
ALLEGRO_BITMAP* get_current_animation_frame(animation_t anim);
void destroy_animation(animation_t* anim);

#endif