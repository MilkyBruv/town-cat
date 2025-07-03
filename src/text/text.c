#include "text.h"

#include <string.h>
#include <stdio.h>

void init_text(ALLEGRO_BITMAP* text_bitmap)
{
    // Loop over all sub bitmaps and create image at each char int value index
    for (size_t i = 0; i < 40; i++)
    {
        char_bitmaps[(u8) char_ref[i]] = al_create_sub_bitmap(text_bitmap, i * 8, 0, 8, 8);
    }
}

void draw_text(char* text, u8 cx, u8 y)
{
    u8 width = strlen(text);
    for (size_t i = 0; i < width; i++)
    {
        al_draw_bitmap(char_bitmaps[(u8) text[i]], (cx - ((width * 8) / 2)) + (i * 8), y, 0);
    }
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
}
