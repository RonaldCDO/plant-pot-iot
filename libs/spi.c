#include "spi.h"

void spiConfig (uint8_t pol, uint8_t pha, uint8_t MSBfirst, uint8_t isMaster){

    UCB1CTL1 = UCSWRST;                // Enable SW reset
    UCB0CTL0 = UCMODE_0 | UCSYNC;      // SPI mode 0, synchronous
    if (isMaster){
        UCB0CTL1 |= UCSSEL__SMCLK;      // Use SMCLK
        UCB0BRW = 0x01;                // fSCL = fSMCLK/2
        UCB0CTL0 |= UCMST;             // Master mode
    }
    if (pol)
        UCB0CTL0 |= UCCKPL;            // Polarity high
    if (pha)
        UCB0CTL0 |= UCCKPH;            // Phase high
    if (MSBfirst)
        UCB0CTL0 |= UCMSB;             // MSB first
    
    P3SEL |= BIT0 | BIT1 | BIT2;       // Set P3.0,1,2 to SPI
    
    UCB0CTL1 &= ~UCSWRST;              // Clear SW reset, resume operation

    // UCB0IE = UCTXIE0 | UCRXIE0;        // Enable TX and RX interrupt

}
uint8_t spiTransfer (uint8_t byte){
    while (!(UCB0IFG & UCTXIFG));      // Wait for TX buffer to be ready
    UCB0TXBUF = byte;                  // Write byte to TX buffer
    while (!(UCB0IFG & UCRXIFG));      // Wait for RX buffer to be ready
    return UCB0RXBUF;                  // Read byte from RX buffer
}

void spiSlave(){

    UCB1CTL1 = UCSWRST;
    UCB1CTL0 = UCMODE_0 | UCMSB | UCSYNC;

    P4SEL |= BIT1 | BIT2 | BIT3;

    UCB1CTL1 &= ~UCSWRST;
    UCB1IE    = UCTXIE | UCRXIE;

}

#pragma vector = USCI_B1_VECTOR
__interrupt void SPI_ISR(){
    switch (__even_in_range(UCB1IV, 4)){
    
        case 0x02: 
            if (UCB1RXBUF)
                P1OUT ^= BIT0;
        break;
        
        case 0x04:
            UCB1TXBUF = UCB1RXBUF;
            break;
        default: break;
    }
}
