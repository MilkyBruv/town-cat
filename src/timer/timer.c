#include "timer.h"

#include <stdlib.h>

ttimer_t* create_timer(d64 wait, timer_type_t type)
{
    ttimer_t* timer = malloc(sizeof(ttimer_t));
    timer->wait = wait;
    timer->type = type;
    timer->done = false;

    return timer;
}

void start_timer(ttimer_t* timer)
{
    timer->done = false;
    timer->elapsed = 0.0;
}

b32 tick_timer(ttimer_t* timer, d64 delta)
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

void stop_timer(ttimer_t* timer)
{
    timer->done = true;
    timer->elapsed = 0.0;
}

void kill_timer(ttimer_t* timer)
{
    free(timer);
    timer = NULL;
}
