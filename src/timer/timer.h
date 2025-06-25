#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

enum timer_type
{
    REPEAT, NO_REPEAT
};

typedef struct timer
{
    double elapsed, wait;
    uint8_t type;
    bool done;
} timer_t;

timer_t* create_timer(double wait, uint8_t type);
void start_timer(timer_t* timer);
bool tick_timer(timer_t* timer, double delta);
void stop_timer(timer_t* timer);
void kill_timer(timer_t* timer);

#endif