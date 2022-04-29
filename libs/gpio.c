#include "gpio.h"
 
volatile uint8_t * PDIR[] = { &P1DIR, &P1DIR, &P2DIR, &P3DIR, &P4DIR};
volatile uint8_t * PREN[] = { &P1REN, &P1REN, &P2REN, &P3REN, &P4REN};
volatile uint8_t * POUT[] = { &P1OUT, &P1OUT, &P2OUT, &P3OUT, &P4OUT};
volatile uint8_t * PIN[] = { &P1IN, &P1IN, &P2IN, &P3IN, &P4IN};

void pinMode(uint8_t port, uint8_t bit, pinMode_t mode){
    
    uint8_t mask = 0x01 << bit;

    if (mode == input) {
        *(PSEL[port]) &= ~mask;
        *(PDIR[port]) &= ~mask;
    }

    if (mode == output){
        *(PSEL[port]) &= ~mask;
        *(PDIR[port]) |= mask;
    }

    if (mode == inPullUp){
        *(PSEL[port]) &= ~mask;
        *(PDIR[port]) &= ~mask;
        *(PREN[port]) |= mask;
        *(POUT[port]) |= mask;
    }

    if (mode == module){
        *(PSEL[port]) |= mask;
    }
}

uint8_t pinRead(uint8_t port, uint8_t bit){

    return *PIN[port] & BIT(bit);
}

void pinWrite(uint8_t port, uint8_t bit, uint8_t value){
    
    if(value)
        * (POUT[port]) |= BIT(bit);
    else 
        * (POUT[port]) &= ~BIT(bit);

}

void pinInit(){
    
    PADIR = 0; PAOUT = 0;
    PBDIR = 0; PBOUT = 0;
    PCDIR = 0; PCOUT = 0;
    PDDIR = 0; PDOUT = 0;

    pinMode(GREENLED, output);
    pinMode(REDLED, output);
    pinMode(RIGHTBUTTON, inPullUp);
    pinMode(LEFTBUTTON, inPullUp);

}


void pinToggle(uint8_t port, uint8_t bit){
    *(POUT[port]) ^= BIT(bit);
}

