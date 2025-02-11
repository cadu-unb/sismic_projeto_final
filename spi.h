#ifndef __SPI_H
#define __SPI_H

#include <msp430.h> 
#include <stdint.h>

void spiConfig();
uint8_t spiTransfer(uint8_t byte);


#endif // __SPI_H
