#ifndef TEXT_H
#define TEXT_H

#include "../entity/entity.h"

typedef struct
{
    rect_t rect;
    uint8_t cx; // center c
    char* text;
    ALLEGRO_BITMAP* bitmap;
} text_t;

static const char* char_ref = "abcdefghijklmnopqrstuvwxyz1234567890!?$ "; // 40 chars
static ALLEGRO_BITMAP* char_bitmaps[128] = {NULL};

void init_text(ALLEGRO_BITMAP* text_bitmap);
text_t* create_text(char *text, uint8_t cx, uint16_t y);
void destroy_text();

#endif