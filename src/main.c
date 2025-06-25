#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "./macros.h"
#include "./entity/entity.h"
#include "./gfx/gfx.h"
#include "./timer/timer.h"
#include "./world/world.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char const *argv[])
{
    al_init();
    al_init_image_addon();
    // al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    al_set_new_display_flags(ALLEGRO_RESIZABLE);

    ALLEGRO_DISPLAY* display = al_create_display(800, 800);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    double fps = 1.0 / 60.0;
    ALLEGRO_TIMER* timer = al_create_timer(fps);
    al_set_window_title(display, "Cool thing!!!");

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
    ALLEGRO_BITMAP* player_bitmap = al_load_bitmap("./res/cat.png");
    player_t* player = malloc(sizeof(player_t));
    player->rect.x = 16;
    player->rect.y = 16;
    player->rect.width = 8;
    player->rect.height = 8;
    player->bitmaps[0] = al_create_sub_bitmap(player_bitmap, 0, 0, 8, 8);
    player->bitmaps[1] = al_create_sub_bitmap(player_bitmap, 0, 8, 8, 8);
    player->current_bitmap = 0;
    bool up = true, down = true, left = true, right = true;
    timer_t* player_anim_timer = create_timer(0.3, REPEAT);
    start_timer(player_anim_timer);

    // World stuff
    home_world_t home;
    init_home_world(&home);

    al_start_timer(timer);
    bool running = true;
    bool redraw = false;

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
                animate_player(player);
                player_anim_timer->done = false;
            }

            up = true; down = true; left = true; right = true;
            for (uint8_t i = 0; i < 90; i++)
            {
                if (hits_rect((rect_t) {player->rect.x, player->rect.y - 8, player->rect.width, player->rect.height}, home.bounds[i]))
                {
                    up = false;
                }

                if (hits_rect((rect_t) {player->rect.x, player->rect.y + 8, player->rect.width, player->rect.height}, home.bounds[i]))
                {
                    down = false;
                }

                if (hits_rect((rect_t) {player->rect.x - 8, player->rect.y, player->rect.width, player->rect.height}, home.bounds[i]))
                {
                    left = false;
                }

                if (hits_rect((rect_t) {player->rect.x + 8, player->rect.y, player->rect.width, player->rect.height}, home.bounds[i]))
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

            al_draw_bitmap(home.bitmap, 0, 0, 0);
            al_draw_bitmap(player->bitmaps[player->current_bitmap], player->rect.x, player->rect.y, 0);

            al_set_target_bitmap(al_get_backbuffer(display));
            al_clear_to_color(al_map_rgb(0, 0, 0));
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

            al_flip_display();
            redraw = false;
        }
    }
    
    stop_timer(player_anim_timer);
    kill_timer(player_anim_timer);
    al_destroy_bitmap(player->bitmaps[0]);
    al_destroy_bitmap(player->bitmaps[1]);
    al_destroy_bitmap(player_bitmap);
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
