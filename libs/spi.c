#include "spi.h"

void spiConfigUCB0 (uint8_t pol, uint8_t pha, uint8_t MSBfirst, uint8_t isMaster){

    UCB0CTL1 = UCSWRST;                     // Enable SW reset
    UCB0CTL0 = UCMODE_0 | UCSYNC;           // SPI mode 0, synchronous
    if (isMaster){
        UCB0CTL1 |= UCSSEL__SMCLK;          // Use SMCLK
        UCB0BRW = 0x01;                     // f_UCB = f_SMCLK/2
        UCB0CTL0 |= UCMST;                  // Master mode
    }
    if (pol)
        UCB0CTL0 |= UCCKPL;                 // Polarity high
    if (pha)
        UCB0CTL0 |= UCCKPH;                 // Phase high
    if (MSBfirst)
        UCB0CTL0 |= UCMSB;                  // MSB first
    
    pinMode(MOSI, module);                  // Set P3.0 to SPI
    pinMode(MISO, module);                  // Set P3.1 to SPI
    pinMode(SCLK, module);                  // Set p3.2 to SPI

    UCB0CTL1 &= ~UCSWRST;                   // Clear SW reset, resume operation

    // UCB0IE = UCTXIE0 | UCRXIE0;          // Enable TX and RX interrupt

}
uint8_t spiTransfer (uint8_t byte){
    while (!(UCB0IFG & UCTXIFG));           // Wait for TX buffer to be ready
    UCB0TXBUF = byte;                       // Write byte to TX buffer
    while (!(UCB0IFG & UCRXIFG));           // Wait for RX buffer to be ready
    return UCB0RXBUF;                       // Read byte from RX buffer
}

void spiConfigUCB1EchoTest(){

    UCB1CTL1 = UCSWRST;                     // Enable SW reset        
    UCB1CTL0 = UCMODE_0 | UCMSB | UCSYNC;   // SPI mode 0, synchronous

    pinMode(4, 1, module);                  // Set P4.4 to SPI
    pinMode(4, 2, module);                  // Set P4.5 to SPI
    pinMode(4, 3, module);                  // Set P4.6 to SPI

    UCB1CTL1 &= ~UCSWRST;                   // Clear SW reset, resume operation
    UCB1IE    = UCTXIE | UCRXIE;            // Enable TX and RX interrupt

}

#pragma vector = USCI_B1_VECTOR
__interrupt void SPI_USCIB1_ISR(){
    switch (__even_in_range(UCB1IV, 4)){
    
        case 0x02: 
            if (UCB1RXBUF)                  // If RX buffer is not empty
                pinToggle(1, 0);            // Toggle P1.0
        break;
        
        case 0x04:
            UCB1TXBUF = UCB1RXBUF;          // Echo received data
            break;
        default: break;
    }
}
