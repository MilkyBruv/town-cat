#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "./macros.h"
#include "./entity/entity.h"
#include "./gfx/gfx.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char const *argv[])
{
    al_init();
    al_init_image_addon();
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_RESIZABLE);

    ALLEGRO_DISPLAY* display = al_create_display(800, 800);
    ALLEGRO_EVENT_QUEUE* eventQueue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    al_set_window_title(display, "Cool thing!!!");

    al_install_keyboard();
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));

    // Framebuffer
    const int fb_width = 240;
    const int fb_height = 160;
    ALLEGRO_BITMAP* fb = al_create_bitmap(fb_width, fb_height);
    float fb_scale = 1.0f;
    int fb_scaled_width = 240;
    int fb_scaled_height = 160;

    render_entity_t* player = malloc(sizeof(render_entity_t));
    player->rect.x = 0;
    player->rect.y = 0;
    player->bitmap = al_load_bitmap("./res/player.png");
    bool left = false, right = false, up = false, down = false;

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
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) { left = true; }
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) { right = true; }
            if (event.keyboard.keycode == ALLEGRO_KEY_UP) { up = true; }
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) { down = true; }
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) { running = false; }
        }

        if (event.type == ALLEGRO_EVENT_KEY_UP)
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) { left = false; }
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) { right = false; }
            if (event.keyboard.keycode == ALLEGRO_KEY_UP) { up = false; }
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) { down = false; }
        }

        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            // update
            if (left) { player->rect.x -= 1; }
            if (right) { player->rect.x += 1; }
            if (up) { player->rect.y -= 1; }
            if (down) { player->rect.y += 1; }

            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(eventQueue))
        {
            // render
            al_set_target_bitmap(fb);
            al_clear_to_color(al_map_rgb(0, 0, 255));

            draw_entity(*player);

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
    
    al_destroy_bitmap(player->bitmap);
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
