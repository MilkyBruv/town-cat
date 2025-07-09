#include "entity.h"

#include <stdio.h>

b32 hits_rect(rect_t a, rect_t b)
{
    if (a.x == b.x && a.y == b.y)
    {
        return true;
    }
    return false;
}

b32 is_rect_in_range(rect_t a, rect_t b)
{
    if ((a.x == b.x - 8 || a.x == b.x || a.x == b.x + 8)
    && ((a.y == b.y - 8 || a.y == b.y || a.y == b.y + 8)))
    {
        return true;
    }
    return false;
}

void update_animation(animation_t* anim)
{
    anim->current_frame = anim->current_frame == anim->total_frames - 1 ? 0 : anim->current_frame + 1;
}

animation_t create_animation(ALLEGRO_BITMAP* bitmaps[], u8 total_frames)
{
    animation_t anim;
    anim.bitmaps = malloc(total_frames * sizeof(ALLEGRO_BITMAP*));
    for (u8 i = 0; i < total_frames; i++) { anim.bitmaps[i] = bitmaps[i]; }
    anim.total_frames = total_frames;
    anim.current_frame = 0;

    return anim;
}

ALLEGRO_BITMAP *get_current_animation_frame(animation_t anim)
{
    return anim.bitmaps[anim.current_frame];
}

void destroy_animation(animation_t* anim)
{
    for (u8 i = 0; i < anim->total_frames; i++)
    {
        al_destroy_bitmap(anim->bitmaps[i]);
    }
    free(anim->bitmaps);
}
