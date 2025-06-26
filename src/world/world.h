#ifndef WORLD_H
#define WORLD_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "./../entity/entity.h"

typedef enum
{
    MAIN, HOUSE, MARKET
} world_id_t;

typedef struct
{
    world_id_t id;
    rect_t* bounds;
    ALLEGRO_BITMAP* bitmap;
    uint8_t total_bounds;
} world_t;

world_t create_world(world_id_t id, const char* map, const char* collision_map);
void destroy_world(world_t* world);

#endif