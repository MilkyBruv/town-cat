#ifndef WORLD_H
#define WORLD_H

#include <allegro5/allegro.h>

#include "./../entity/entity.h"

typedef struct home_world
{
    rect_t bounds[90]; // 90 is about total number of solid tiles (should do this better)
    ALLEGRO_BITMAP* bitmap;
} home_world_t;

void init_home_world(home_world_t* home);

#endif