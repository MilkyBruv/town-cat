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

typedef struct smoke
{
    rect_t rect;
    animation_t anim;
} smoke_t;

typedef struct player
{
    rect_t rect;
    animation_t anim;
    u32 coins;
} player_t;

typedef struct water
{
    rect_t rect;
    animation_t anim;
} water_t;

typedef enum rod_level_t
{
    STICK_AND_STRING, TOY, BEGINNER, GOOD, SUPER
} rod_level_t;

typedef struct rod
{
    rect_t rect;
    rod_level_t level;
} rod_t;

typedef enum fish_id_t
{
    TRASH, TADPOLE, NEMO
} fish_id_t;

typedef struct fish
{
    char* name;
    u16 value;
    fish_id_t id;
    ALLEGRO_BITMAP* bitmap;
} fish_t;

b32 hits_rect(rect_t a, rect_t b);
b32 is_rect_in_range(rect_t a, rect_t b);
void animate(animation_t* anim);
animation_t create_animation(ALLEGRO_BITMAP* bitmaps[], u8 total_frames);
ALLEGRO_BITMAP* get_current_animation_frame(animation_t anim);
void destroy_animation(animation_t* anim);

#endif