#include "text.h"

#include <string.h>
#include <stdio.h>

void init_text(ALLEGRO_BITMAP* text_bitmap)
{
    // Loop over all sub bitmaps and create image at each char int value index
    for (size_t i = 0; i < 40; i++)
    {
        char_bitmaps[(uint8_t) char_ref[i]] = al_create_sub_bitmap(text_bitmap, i * 8, 0, 8, 8);
    }
}

text_t* create_text(char *text, uint8_t cx, uint16_t y)
{
    text_t* new_text;
    new_text->text = text;
    new_text->bitmap = al_create_bitmap(8 * strlen(text), 8);
    new_text->cx = cx;
    new_text->rect = (rect_t) {cx - (al_get_bitmap_width(new_text->bitmap) / 2), y, 8 * strlen(text), 8};

    al_set_target_bitmap(new_text->bitmap);

    for (size_t i = 0; i < strlen(text); i++)
    {
        al_draw_bitmap(char_bitmaps[(uint8_t) text[i]], i * 8, 0, 0);
    }
    
    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));

    return new_text;
}

void destroy_text()
{
    for (size_t i = 0; i < 40; i++)
    {
        if (char_bitmaps[char_ref[i]])
        {
            al_destroy_bitmap(char_bitmaps[(uint8_t) char_ref[i]]);
        }
    }
}
