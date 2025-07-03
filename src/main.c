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
#include "./text/text.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char const *argv[])
{
    al_init();
    al_init_image_addon();
    al_init_primitives_addon();
    // al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_RESIZABLE);

    ALLEGRO_DISPLAY* display = al_create_display(800, 800);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    d64 fps = 1.0 / 60.0;
    ALLEGRO_TIMER* timer = al_create_timer(fps);
    al_set_window_title(display, "Cool thing!!!");

    ALLEGRO_BITMAP* text_bitmap = al_load_bitmap("./res/text.png");
    init_text(text_bitmap);

    al_install_keyboard();
    al_install_mouse();
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));

    // Framebuffer
    const u8 fb_width = 160;
    const u8 fb_height = 160;
    ALLEGRO_BITMAP* fb = al_create_bitmap(fb_width, fb_height);
    f32 fb_scale = 1.0f;
    u16 fb_scaled_width = 160;
    u16 fb_scaled_height = 160;
    u16 fb_x = 0;
    u16 fb_y = 0;
    u8 fb_mouse_x = 0;
    u8 fb_mouse_y = 0;

    // Player stuff
    player_t* player = malloc(sizeof(player_t));
    player->rect.x = 96;
    player->rect.y = 72;
    player->rect.width = 8;
    player->rect.height = 8;
    ALLEGRO_BITMAP* player_bitmap = al_load_bitmap("./res/cat.png");
    ALLEGRO_BITMAP* player_bitmaps[] = {al_create_sub_bitmap(player_bitmap, 0, 0, 8, 8),
        al_create_sub_bitmap(player_bitmap, 0, 8, 8, 8)};
    player->anim = create_animation(player_bitmaps, 2);
    b32 up = true, down = true, left = true, right = true;
    ttimer_t* player_anim_timer = create_timer(0.3, REPEAT);
    start_timer(player_anim_timer);
    ALLEGRO_BITMAP* rod_bitmap = al_load_bitmap("./res/rod.png");
    u8 rod_x = 0;
    u8 rod_y = 0;
    s16 rod_len_x = 0;
    s16 rod_len_y = 0;
    s16 rod_len = 0;
    f32 rod_angle_start = 0.0f;
    f32 rod_angle_end = 0.0f;

    // World stuff
    world_t main_world = create_world(MAIN, "./res/world.png", "./res/world_collisions.png");
    world_t market_world = create_world(MARKET, "./res/market.png", "./res/market_collisions.png");
    world_t worlds[2] = {main_world, market_world};
    u8 current_world = 0;
    printf("%d\n", current_world);

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

    // Shader stuff
    ALLEGRO_SHADER* shader = al_create_shader(ALLEGRO_SHADER_GLSL);
    al_attach_shader_source_file(shader, ALLEGRO_VERTEX_SHADER, "./res/shader/vertex.glsl");
    al_attach_shader_source_file(shader, ALLEGRO_PIXEL_SHADER, "./res/shader/fragment.glsl");
    al_build_shader(shader);

    // Text stuff
    ttimer_t* text_timer = create_timer(0.5, REPEAT);
    char* t = "hello";
    start_timer(text_timer);

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
            if (left && event.keyboard.keycode == ALLEGRO_KEY_LEFT) { player->rect.x -= 8; }
            if (right && event.keyboard.keycode == ALLEGRO_KEY_RIGHT) { player->rect.x += 8; }
            if (up && event.keyboard.keycode == ALLEGRO_KEY_UP) { player->rect.y -= 8; }
            if (down && event.keyboard.keycode == ALLEGRO_KEY_DOWN) { player->rect.y += 8; }
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) { running = false; }
        }

        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            // get mouse pos
            al_get_mouse_state(&mouse_state);
            fb_mouse_x = (mouse_state.x - fb_x) / fb_scale;
            fb_mouse_y = (mouse_state.y - fb_y) / fb_scale;

            // update
            if (tick_timer(player_anim_timer, fps))
            {
                animate(&player->anim);
                player_anim_timer->done = false;
            }

            if (tick_timer(smoke_timer, fps))
            {
                animate(&smoke.anim);
                smoke_timer->done = false;
            }

            if (tick_timer(water_timer, fps))
            {
                animate(&water.anim);
                water_timer->done = false;
            }

            if (player->rect.x == 152 && current_world == 0)
            {
                current_world = 1;
                player->rect.x = 8;
            }
            if (player->rect.x == 0 && current_world == 1)
            {
                current_world = 0;
                player->rect.x = 144;
            }

            up = true; down = true; left = true; right = true;
            for (u8 i = 0; i < worlds[current_world].total_bounds; i++)
            {
                if (!is_rect_in_range(player->rect, worlds[current_world].bounds[i])) { continue; }
                if (hits_rect((rect_t) {player->rect.x, player->rect.y - 8, player->rect.width, player->rect.height}, worlds[current_world].bounds[i]))
                {
                    up = false;
                }

                if (hits_rect((rect_t) {player->rect.x, player->rect.y + 8, player->rect.width, player->rect.height}, worlds[current_world].bounds[i]))
                {
                    down = false;
                }

                if (hits_rect((rect_t) {player->rect.x - 8, player->rect.y, player->rect.width, player->rect.height}, worlds[current_world].bounds[i]))
                {
                    left = false;
                }

                if (hits_rect((rect_t) {player->rect.x + 8, player->rect.y, player->rect.width, player->rect.height}, worlds[current_world].bounds[i]))
                {
                    right = false;
                }
            }

            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(eventQueue))
        {
            // render
            al_set_target_bitmap(fb);
            al_clear_to_color(al_map_rgb(0, 0, 255));
            
            al_draw_bitmap(worlds[current_world].bitmap, 0, 0, 0);
            al_draw_bitmap(get_current_animation_frame(player->anim), player->rect.x, player->rect.y, 0);
            if (current_world == MAIN)
            {
                al_draw_bitmap(get_current_animation_frame(smoke.anim), 48, 20, 0);
                al_draw_bitmap(chimeny_bitmap, 48, 24, 0);
                al_draw_bitmap(get_current_animation_frame(water.anim), 72, 72, 0);

                if (player->rect.x == 96 && player->rect.y == 72)
                {
                    rod_x = player->rect.x + 3;
                    rod_y = player->rect.y + 3;
                    rod_len_x = rod_x - fb_mouse_x;
                    rod_len_y = rod_y - fb_mouse_y;
                    rod_len = sqrt(pow(rod_len_x, 2) + pow(rod_len_y, 2));
                    rod_angle_start = asin((sin(90) / rod_len) * rod_len_y);
                    
                    al_draw_bitmap(rod_bitmap, player->rect.x, player->rect.y, 0);
                    al_draw_line(
                        rod_x, rod_y,
                        fb_mouse_x,
                        fb_mouse_y,
                        al_map_rgb(255, 255, 255), 0
                    );
                    al_draw_elliptical_arc(
                        rod_x - (rod_len_x / 2), // centre x
                        rod_y - (rod_len_y / 2), // centre y
                        abs(rod_len) / 2, // radius x
                        abs(rod_len) / 5, // radius y
                        rod_angle_start, // start rads
                        M_PI, // delta rads
                        al_map_rgb(255, 0, 0), // col
                        2 // thicc
                    );
                    al_draw_line(
                        rod_x,
                        fb_mouse_y,
                        fb_mouse_x,
                        fb_mouse_y,
                        al_map_rgb(0, 255, 255),
                        1
                    );
                    al_draw_line(
                        rod_x,
                        rod_y,
                        rod_x,
                        fb_mouse_y,
                        al_map_rgb(0, 255, 255),
                        1
                    );
                    al_draw_line(
                        (rod_x - (rod_len_x / 2)) - (rod_len / 2),
                        rod_y - (rod_len_y / 2),
                        
                        (rod_x - (rod_len_x / 2)) + (rod_len / 2),
                        rod_y - (rod_len_y / 2),

                        al_map_rgb(0, 0, 255),
                        1
                    );
                    al_draw_line(
                        rod_x - (rod_len_x / 2),
                        (rod_y - (rod_len_y / 2)) - (rod_len / 2),
                        
                        rod_x - (rod_len_x / 2),
                        (rod_y - (rod_len_y / 2)) + (rod_len / 2),

                        al_map_rgb(0, 255, 0),
                        1
                    );
                }
            }
            else
            {
                if (player->rect.x == 40 && player->rect.y == 56)
                {
                    draw_text("hello", 43, 27);
                }
            }
            
            al_set_target_bitmap(al_get_backbuffer(display));
            al_clear_to_color(al_map_rgb(0, 0, 0));
            // al_use_shader(shader);
            al_set_shader_float("time", (f32) al_get_timer_count(timer) / 60.0f);
            fb_scale = (f32) min(
                (f32) ((f32) al_get_display_width(display) / (f32) fb_width),
                (f32) ((f32) al_get_display_height(display) / (f32) fb_height)
            );
            fb_scaled_width = fb_width * fb_scale;
            fb_scaled_height = fb_height * fb_scale;
            fb_x = (al_get_display_width(display) / 2) - (fb_scaled_width / 2);
            fb_y = (al_get_display_height(display) / 2) - (fb_scaled_height / 2);
            al_draw_scaled_bitmap(fb, 0, 0, fb_width, fb_height, fb_x, fb_y,
                fb_scaled_width, fb_scaled_height, 0);
                
            al_use_shader(NULL);
            al_flip_display();
            redraw = false;
        }
    }
    
    al_use_shader(NULL);
    al_destroy_shader(shader);

    stop_timer(player_anim_timer);
    kill_timer(player_anim_timer);
    stop_timer(smoke_timer);
    kill_timer(smoke_timer);
    stop_timer(water_timer);
    kill_timer(water_timer);

    destroy_text();
    al_destroy_bitmap(text_bitmap);
    al_destroy_bitmap(rod_bitmap);

    destroy_animation(&player->anim);
    destroy_animation(&smoke.anim);
    destroy_animation(&water.anim);
    al_destroy_bitmap(player_bitmap);
    al_destroy_bitmap(smoke_bitmap);
    al_destroy_bitmap(chimeny_bitmap);
    al_destroy_bitmap(water_bitmap);

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
