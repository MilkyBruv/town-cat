#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum timer_type
{
    REPEAT, NO_REPEAT
} timer_type_t;

typedef struct timer
{
    double elapsed, wait;
    timer_type_t type;
    bool done;
} ttimer_t;

ttimer_t* create_timer(double wait, timer_type_t type);
void start_timer(ttimer_t* timer);
bool tick_timer(ttimer_t* timer, double delta);
void stop_timer(ttimer_t* timer);
void kill_timer(ttimer_t* timer);

#endif