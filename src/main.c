#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <usrtypes.h>
#include "./entity/entity.h"
#include "./timer/timer.h"
#include "./world/world.h"
#include "./gfx/gfx.h"

const d64 fps = 1.0 / 60.0;

int main(int argc, char const *argv[])
{
    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    // al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_RESIZABLE);

    ALLEGRO_DISPLAY* display = al_create_display(800, 800);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(fps);
    al_set_window_title(display, "Town Cat");
    
    al_install_keyboard();
    al_install_mouse();
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));

    // GFX stuff
    init_gfx();

    // Player stuff
    player_t* player = malloc(sizeof(player_t));
    player->rect.x = 96;
    player->rect.y = 72;
    player->rect.width = 8;
    player->rect.height = 8;
    player->coins = 0;
    ALLEGRO_BITMAP* player_bitmap = al_load_bitmap("./res/cat.png");
    ALLEGRO_BITMAP* player_bitmaps[] = {al_create_sub_bitmap(player_bitmap, 0, 0, 8, 8),
        al_create_sub_bitmap(player_bitmap, 0, 8, 8, 8)};
    player->anim = create_animation(player_bitmaps, 2);
    b32 up = true, down = true, left = true, right = true;
    b32 locked = false;
    b32 fishing = false;
    ttimer_t* player_anim_timer = create_timer(0.3, REPEAT);
    start_timer(player_anim_timer);
    ALLEGRO_COLOR bob_col = al_map_rgb(200, 0, 0); // colour of bobber or whatever
    // inventory stuff
    // item_t* inv[16] = {NULL};


    // Rod
    ALLEGRO_BITMAP* rod_bitmap = al_load_bitmap("./res/rod.png");
    rod_t rod;
    rod.rect = (rect_t) {0, 0, 0, 0};
    rod.level = 1;
    ttimer_t* rod_timer = create_timer(1.2, REPEAT);
    start_timer(rod_timer);


    // World stuff
    world_t main_world = create_world(MAIN, "./res/world.png", "./res/world_collisions.png");
    world_t house_world = create_world(HOUSE, "./res/house.png", "./res/house_collisions.png");
    world_t market_world = create_world(MARKET, "./res/market.png", "./res/market_collisions.png");
    world_t worlds[3] = {main_world, main_world, market_world};
    u8 current_world = 0;


    // Smoke animation
    smoke_t smoke;
    ALLEGRO_BITMAP* chimeny_bitmap = al_load_bitmap("./res/chimney.png");
    ALLEGRO_BITMAP* smoke_bitmap = al_load_bitmap("./res/smoke.png");
    ALLEGRO_BITMAP* smoke_bitmaps[5];
    for (size_t i = 0; i < 5; i++)
    {
        smoke_bitmaps[i] = al_create_sub_bitmap(smoke_bitmap, 0, i * 8, 8, 8);
    }
    smoke.anim = create_animation(smoke_bitmaps, 5);
    ttimer_t* smoke_timer = create_timer(0.3, REPEAT);
    start_timer(smoke_timer);


    // Water animation
    water_t water;
    ALLEGRO_BITMAP* water_bitmap = al_load_bitmap("./res/water.png");
    ALLEGRO_BITMAP* water_bitmaps[2];
    for (size_t i = 0; i < 2; i++)
    {
        water_bitmaps[i] = al_create_sub_bitmap(water_bitmap, 0, i * 48, 56, 48);
    }
    water.anim = create_animation(water_bitmaps, 2);
    ttimer_t* water_timer = create_timer(1, REPEAT);
    start_timer(water_timer);


    al_start_timer(timer);
    b32 redraw = false;
    b32 running = true;
    
    while (running)
    {
        ALLEGRO_MOUSE_STATE mouse_state;
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
        {
            al_acknowledge_resize(display);
        }

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            running = false;
        }

        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (left && event.keyboard.keycode == ALLEGRO_KEY_A) { player->rect.x -= 8; }
            if (right && event.keyboard.keycode == ALLEGRO_KEY_D) { player->rect.x += 8; }
            if (up && event.keyboard.keycode == ALLEGRO_KEY_W) { player->rect.y -= 8; }
            if (down && event.keyboard.keycode == ALLEGRO_KEY_S) { player->rect.y += 8; }
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) { running = false; }
        }

        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            // get mouse pos
            al_get_mouse_state(&mouse_state);
            update_framebuffer_mouse_pos(mouse_state);

            // update
            if (tick_timer(player_anim_timer, fps))
            {
                animate(&player->anim);
            }

            if (tick_timer(water_timer, fps))
            {
                animate(&water.anim);
            }

            if (current_world == MAIN)
            {
                if (tick_timer(smoke_timer, fps))
                {
                    animate(&smoke.anim);
                }

                if (player->rect.x == 152)
                {
                    current_world = MARKET;
                    player->rect.x = 8;
                }
            }
            if (current_world == MARKET)
            {
                if (player->rect.x == 0)
                {
                    current_world = MAIN;
                    player->rect.x = 144;
                }
            }

            up = true; down = true; left = true; right = true;
            for (u8 i = 0; i < worlds[current_world].total_solid_bounds; i++)
            {
                if (!is_rect_in_range(player->rect, worlds[current_world].solid_bounds[i])) { continue; }
                if (hits_rect((rect_t) {player->rect.x, player->rect.y - 8, player->rect.width, player->rect.height}, worlds[current_world].solid_bounds[i]))
                {
                    up = false;
                }

                if (hits_rect((rect_t) {player->rect.x, player->rect.y + 8, player->rect.width, player->rect.height}, worlds[current_world].solid_bounds[i]))
                {
                    down = false;
                }

                if (hits_rect((rect_t) {player->rect.x - 8, player->rect.y, player->rect.width, player->rect.height}, worlds[current_world].solid_bounds[i]))
                {
                    left = false;
                }

                if (hits_rect((rect_t) {player->rect.x + 8, player->rect.y, player->rect.width, player->rect.height}, worlds[current_world].solid_bounds[i]))
                {
                    right = false;
                }
            }

            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(eventQueue))
        {
            // render
            enable_framebuffer_drawing();
            al_clear_to_color(al_map_rgb(0, 0, 255));
            
            if (current_world == MAIN)
            {
                al_draw_bitmap(worlds[current_world].bitmap, 0, 0, 0);
                al_draw_bitmap(get_current_animation_frame(player->anim), player->rect.x, player->rect.y, 0);
                al_draw_bitmap(get_current_animation_frame(smoke.anim), 48, 20, 0);
                al_draw_bitmap(chimeny_bitmap, 48, 24, 0);
                al_draw_bitmap(get_current_animation_frame(water.anim), 72, 72, 0);

                if (player->rect.x == 96 && player->rect.y == 72)
                {
                    rod.rect.x = player->rect.x + 3;
                    rod.rect.y = player->rect.y + 3;
                    
                    al_draw_bitmap(rod_bitmap, player->rect.x, player->rect.y, 0);
                    al_draw_line(
                        rod.rect.x, rod.rect.y,
                        fb->mouse_x,
                        fb->mouse_y,
                        al_map_rgb(255, 255, 255), 0
                    );
                }
            }
            else
            {
                if (player->rect.x == 40 && player->rect.y == 56)
                {
                    draw_text("hello", 43, 27);
                }

                al_draw_bitmap(get_current_animation_frame(water.anim), 48, 48, 0);
                al_draw_bitmap(worlds[current_world].bitmap, 0, 0, 0);
                al_draw_bitmap(get_current_animation_frame(player->anim), player->rect.x, player->rect.y, 0);
            }
            
            disable_framebuffer_drawing(display);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            enable_shader();
            al_set_shader_float("time", (f32) al_get_timer_count(timer) / 60.0f);
            scale_and_draw_framebuffer(display);
                
            disable_shader();
            al_flip_display();
            redraw = false;
        }
    }
    
    destroy_gfx();

    stop_timer(player_anim_timer);
    kill_timer(player_anim_timer);
    stop_timer(smoke_timer);
    kill_timer(smoke_timer);
    stop_timer(water_timer);
    kill_timer(water_timer);
    stop_timer(rod_timer);
    kill_timer(rod_timer);
    
    destroy_animation(&player->anim);
    destroy_animation(&smoke.anim);
    destroy_animation(&water.anim);
    al_destroy_bitmap(player_bitmap);
    al_destroy_bitmap(smoke_bitmap);
    al_destroy_bitmap(chimeny_bitmap);
    al_destroy_bitmap(water_bitmap);
    al_destroy_bitmap(rod_bitmap);

    destroy_world(&main_world);
    destroy_world(&market_world);

	al_unregister_event_source(eventQueue, al_get_timer_event_source(timer));
	al_unregister_event_source(eventQueue, al_get_display_event_source(display));
	al_unregister_event_source(eventQueue, al_get_keyboard_event_source());
    al_uninstall_keyboard();
    al_uninstall_mouse();
    al_shutdown_image_addon();
    al_shutdown_primitives_addon();
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQueue);
    
    free(player);
    player = NULL;

    return 0;
}
