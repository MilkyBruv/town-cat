#include "./gfx.h"

void draw_entity(render_entity_t entity)
{
    al_draw_bitmap(entity.bitmap, entity.rect.x, entity.rect.y, 0);
}