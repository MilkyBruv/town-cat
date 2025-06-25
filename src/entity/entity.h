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

typedef struct entity
{
    rect_t rect;
} entity_t;

typedef struct render_entity
{
    rect_t rect;
    bitmap_t bitmap;
} render_entity_t;

bool hits_rect(rect_t a, rect_t b);

#endif