#include "./entity.h"
#include "entity.h"

#include <stdio.h>

bool hits_rect(rect_t a, rect_t b)
{
    if (a.x == b.x && a.y == b.y)
    {
        return true;
    }
    return false;
}

void animate_player(player_t * player)
{
    player->current_bitmap = player->current_bitmap == 1 ? 0 : 1;
}