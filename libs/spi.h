#ifndef SPI_H
#define SPI_H
#include "gpio.h"

#define MOSI 3,0
#define MISO 3,1
#define SCLK 3,2
#define STEN 3,3

#define TCLK2 10,ms

void spiConfig (uint8_t pol, uint8_t pha, uint8_t MSBfirst, uint8_t isMaster);
uint8_t spiTransfer (uint8_t byte);

#endif
