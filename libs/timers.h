#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

typedef enum {us, ms, sec, min} time_unit_t;

void wait(uint16_t time, time_unit_t unit);


#endif