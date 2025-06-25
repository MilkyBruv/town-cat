#include "timer.h"

#include <stdlib.h>

timer_t* create_timer(double wait, uint8_t type)
{
    timer_t* timer = malloc(sizeof(timer_t));
    timer->wait = wait;
    timer->type = type;
    timer->done = false;

    return timer;
}

void start_timer(timer_t* timer)
{
    timer->done = false;
    timer->elapsed = 0.0;
}

bool tick_timer(timer_t* timer, double delta)
{
    if (!timer) { return false; } // If timer is null
    if (timer->type == NO_REPEAT && timer->done) { return false; }

    timer->elapsed += delta;
    if (timer->elapsed >= timer->wait)
    {
        timer->elapsed = 0.0;
        timer->done = true;

        if (timer->type == NO_REPEAT)
        {
            stop_timer(timer);
        }

        return true;
    }

    return timer->done;
}

void stop_timer(timer_t* timer)
{
    timer->done = true;
    timer->elapsed = 0.0;
}

void kill_timer(timer_t* timer)
{
    free(timer);
    timer = NULL;
}
