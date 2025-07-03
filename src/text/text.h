#ifndef TEXT_H
#define TEXT_H

#include "../entity/entity.h"

typedef struct
{
    rect_t rect;
    u8 cx; // center c
    char* text;
    ALLEGRO_BITMAP* bitmap;
} text_t;

static const char* char_ref = "abcdefghijklmnopqrstuvwxyz1234567890!?$ "; // 40 chars
static ALLEGRO_BITMAP* char_bitmaps[128] = {NULL};

void init_text(ALLEGRO_BITMAP* text_bitmap);
void draw_text(char* text, u8 cx, u8 y);
void destroy_text();

#endif