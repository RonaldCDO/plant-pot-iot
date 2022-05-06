#include <msp430.h>
#include "timers.h"

uint16_t count;

uint32_t vTimer[MAX_TIMERS];
uint8_t timerCount = 0;

uint16_t timerNew(uint32_t time, time_unit_t unit){
    TA1CTL = TASSEL__SMCLK | MC__UP | TACLR;
    TA1CCR0 = 1024-1;
    TA1CCTL0 = CCIE;

    if (unit == sec) time *= 1000;
    if (unit == min) time *= (1000 * 60);
    if (unit == hour) time *= (1000 * 60 * 60);
    
    vTimer[timerCount++] = time;

    return timerCount - 1;

}

uint16_t timerIsRunning(uint16_t t){
 
    return vTimer[t];
 
}


void wait(uint16_t time, time_unit_t unit){
    if (unit == us){
        TA2CTL = TASSEL__SMCLK | MC__UP | TACLR;
        TA2CCR0 = time;
        while(!(TA2CCTL0 & CCIFG));
        TA2CCTL0 &= ~CCIFG;
    } else {
        TA2CTL = TASSEL__ACLK | MC__UP | TACLR;
        if (unit == ms){

            TA2CCR0 = (time << 5) -1;
            while(!(TA2CCTL0 & CCIFG));
            TA2CCTL0 &= ~CCIFG;
        }

        if (unit == sec){
            count = time;
            TA2CCR0 = 0x8000 -1;
            TA2CCTL0 = CCIE;
            while(count > 0);
        }
        if (unit == min){
            count = time * 60;
            TA2CCR0 = 0x8000 -1;
            TA2CCTL0 = CCIE;
            while(count > 0);
        }
    }
    TA2CTL = MC__STOP;
}

void ta0Config(clock_t clk){

    if (clk == smclk){
        TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;
        TA0CCR0 = TASSEL__SMCLK/100;
    } else {
        TA0CTL = TASSEL__ACLK | MC__UP | TACLR;
        TA0CCR0 = TASSEL__ACLK/100;
    }
    TA0CCTL1 = OUTMOD_6;
    TA0CCR1 = TA0CCR0/2;
//    TA0CCTL0 = CCIE;
}


#pragma vector = TIMER2_A0_VECTOR
__interrupt void Timer2_CCR0_ISR(){
    count--;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_CCR0_ISR(){
    uint8_t i = MAX_TIMERS;

    while(i--){
        if (vTimer[i] != 0)
            vTimer[i]--;
    }
}
