#include "world.h"
#include <stdio.h>

world_t create_world(world_id_t id, const char* map, const char* collision_map)
{
    world_t world;
    world.id = id;
    world.bitmap = al_load_bitmap(map);

    // Loop through each pixel in world_collisions.png at intervals of 8
    // and add to home_world_t.bounds for each RED pixel
    ALLEGRO_BITMAP* col_map = al_load_bitmap(collision_map);

    u8 solid_counter = 1;
    u8 water_counter = 1;
    for (size_t y = 0; y < al_get_bitmap_height(col_map); y += 8)
    {
        for (size_t x = 0; x < al_get_bitmap_width(col_map); x += 8)
        {
            // Red = solid
            if (al_get_pixel(col_map, x, y).r == 1.0)
            {
                puts("SOLID");
                solid_counter++;
            }
            // Blue = water
            else if (al_get_pixel(col_map, x, y).b == 1.0)
            {
                puts("WATER");
                water_counter++;
                solid_counter++;
            }
        }
    }
    world.solid_bounds = malloc(solid_counter * sizeof(rect_t));
    world.total_solid_bounds = solid_counter;
    world.water_bounds = malloc(water_counter * sizeof(rect_t));
    world.total_water_bounds = water_counter;

    u8 bounds_index = 0;
    u8 water_index = 0;
    for (size_t y = 0; y < al_get_bitmap_height(col_map); y += 8)
    {
        for (size_t x = 0; x < al_get_bitmap_width(col_map); x += 8)
        {
            if (al_get_pixel(col_map, x, y).r == 1.0)
            {
                world.solid_bounds[bounds_index] = (rect_t) {x, y, 8, 8};
                bounds_index++;
            }

            if (al_get_pixel(col_map, x, y).b == 1.0)
            {
                world.water_bounds[water_index] = (rect_t) {x, y, 8, 8};
                world.solid_bounds[bounds_index] = (rect_t) {x, y, 8, 8};
                water_index++;
                bounds_index++;
            }
        }
    }

    al_destroy_bitmap(col_map);

    return world;
}

void destroy_world(world_t* world)
{
    al_destroy_bitmap(world->bitmap);
    free(world->solid_bounds);
}
