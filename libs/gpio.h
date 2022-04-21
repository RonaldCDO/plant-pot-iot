#ifndef GPIO_H
#define GPIO_H

#include <msp430.h>
#include <stdint.h>


#define BIT(n) (1) << n

#define REDLED      1,0
#define GREENLED    4,7
#define SW2RK       1,1
#define SW1LK       2,1

typedef enum {input, output, inPullUp, inPullDown} pinMode_t;

void pinMode(uint8_t port, uint8_t bit, pinMode_t mode);

uint8_t pinRead(uint8_t port, uint8_t bit);

void pinWrite(uint8_t port, uint8_t bit, uint8_t value);


#endif
