#ifndef ENTITY_H
#define ENTITY_H

#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "./../macros.h"

typedef struct rect
{
    int16_t x, y, width, height;
} rect_t;

typedef struct player
{
    rect_t rect;
    ALLEGRO_BITMAP* bitmaps[2];
    uint8_t current_bitmap;
} player_t;

bool hits_rect(rect_t a, rect_t b);
void animate_player(player_t* player);

#endif