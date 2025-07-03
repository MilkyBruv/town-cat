#ifndef TIMER_H
#define TIMER_H

#include <stdlib.h>
#include <stdio.h>
#include <usrtypes.h>

typedef enum timer_type
{
    REPEAT, NO_REPEAT
} timer_type_t;

typedef struct timer
{
    d64 elapsed, wait;
    timer_type_t type;
    b32 done;
} ttimer_t;

ttimer_t* create_timer(d64 wait, timer_type_t type);
void start_timer(ttimer_t* timer);
b32 tick_timer(ttimer_t* timer, d64 delta);
void stop_timer(ttimer_t* timer);
void kill_timer(ttimer_t* timer);

#endif