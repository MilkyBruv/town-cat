#include "world.h"

void init_home_world(home_world_t* home)
{
    home->bitmap = al_load_bitmap("./res/world.png");

    // Loop through each pixel in world_collisions.png at intervals of 8
    // and add to home_world_t.bounds for each RED pixel
    ALLEGRO_BITMAP* col_map = al_load_bitmap("./res/world_collisions.png");

    size_t bounds_index = 0;
    for (size_t y = 0; y < al_get_bitmap_height(col_map); y += 8)
    {
        for (size_t x = 0; x < al_get_bitmap_width(col_map); x += 8)
        {
            if (al_get_pixel(col_map, x, y).r == 255)
            {
                home->bounds[bounds_index] = (rect_t) {x, y, 8, 8};
            }
        }
    }

    al_destroy_bitmap(col_map);
}