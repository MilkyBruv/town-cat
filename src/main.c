#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "./macros.h"
#include "./entity/entity.h"
#include "./timer/timer.h"
#include "./world/world.h"
#include "./text/text.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char const *argv[])
{
    al_init();
    al_init_image_addon();
    // al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_RESIZABLE);

    ALLEGRO_DISPLAY* display = al_create_display(800, 800);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    double fps = 1.0 / 60.0;
    ALLEGRO_TIMER* timer = al_create_timer(fps);
    al_set_window_title(display, "Cool thing!!!");

    ALLEGRO_BITMAP* text_bitmap = al_load_bitmap("./res/text.png");
    init_text(text_bitmap);

    al_install_keyboard();
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));

    // Framebuffer
    const int fb_width = 160;
    const int fb_height = 160;
    ALLEGRO_BITMAP* fb = al_create_bitmap(fb_width, fb_height);
    float fb_scale = 1.0f;
    int fb_scaled_width = 160;
    int fb_scaled_height = 160;

    // Player stuff
    player_t* player = malloc(sizeof(player_t));
    player->rect.x = 16;
    player->rect.y = 16;
    player->rect.width = 8;
    player->rect.height = 8;
    ALLEGRO_BITMAP* player_bitmap = al_load_bitmap("./res/cat.png");
    ALLEGRO_BITMAP* player_bitmaps[] = {al_create_sub_bitmap(player_bitmap, 0, 0, 8, 8),
        al_create_sub_bitmap(player_bitmap, 0, 8, 8, 8)};
    player->anim = create_animation(player_bitmaps, 2);
    bool up = true, down = true, left = true, right = true;
    ttimer_t* player_anim_timer = create_timer(0.3, REPEAT);
    start_timer(player_anim_timer);

    // World stuff
    world_t main_world = create_world(MAIN, "./res/world.png", "./res/world_collisions.png");
    world_t market_world = create_world(MARKET, "./res/market.png", "./res/market_collisions.png");
    world_t worlds[2] = {main_world, market_world};
    uint8_t current_world = 0;
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

    // Text
    text_t* t = create_text("faggot!", 128, 128);

    al_start_timer(timer);
    bool redraw = false;
    bool running = true;
    
    while (running)
    {
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
            for (uint8_t i = 0; i < worlds[current_world].total_bounds; i++)
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
            if (worlds[current_world].id == MAIN)
            {
                al_draw_bitmap(get_current_animation_frame(smoke.anim), 48, 20, 0);
                al_draw_bitmap(chimeny_bitmap, 48, 24, 0);
                al_draw_bitmap(get_current_animation_frame(water.anim), 72, 72, 0);
            }
            else
            {
                al_draw_bitmap(t->bitmap, t->rect.x, t->rect.y, 0);
                puts("DRAWING");
            }
            
            al_set_target_bitmap(al_get_backbuffer(display));
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_use_shader(shader);
            al_set_shader_float("time", (float) al_get_timer_count(timer) / 60.0f);
            fb_scale = (float) min(
                (float) ((float) al_get_display_width(display) / (float) fb_width),
                (float) ((float) al_get_display_height(display) / (float) fb_height)
            );
            fb_scaled_width = fb_width * fb_scale;
            fb_scaled_height = fb_height * fb_scale;
            al_draw_scaled_bitmap(fb, 0, 0, fb_width, fb_height, 
                (al_get_display_width(display) / 2) - (fb_scaled_width / 2),
                (al_get_display_height(display) / 2) - (fb_scaled_height / 2),
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
    al_shutdown_image_addon();
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(eventQueue);
    
    free(player);
    player = NULL;

    return 0;
}
