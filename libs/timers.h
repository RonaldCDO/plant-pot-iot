#ifndef TIMER_H
#define TIMER_H

#define MAX_TIMERS 10

#include <stdint.h>

typedef enum {us, ms, sec, min, hour} time_unit_t;

uint16_t timerNew(uint32_t time, time_unit_t unit);

uint16_t timerIsRunning(uint16_t timer);

void wait(uint16_t time, time_unit_t unit);

#endif