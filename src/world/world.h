#ifndef WORLD_H
#define WORLD_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "./../entity/entity.h"

typedef enum world_id
{
    MAIN, HOUSE, MARKET
} world_id_t;

typedef struct world
{
    world_id_t id;
    rect_t* solid_bounds;
    rect_t* water_bounds;
    ALLEGRO_BITMAP* bitmap;
    u8 total_solid_bounds;
    u8 total_water_bounds;
} world_t;

world_t create_world(world_id_t id, const char* map, const char* collision_map);
void destroy_world(world_t* world);

#endif