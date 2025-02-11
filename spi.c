#include <msp430.h> 
#include <stdint.h>

void spiConfig(uint8_t phase, uint8_t polarity)
{
    UCB0CTL1 |=  UCSWRST;
    UCB0CTL0 =
//          UCCKPH     |
//          UCCKPL     |
            UCMSB      |
//          UC7BIT     |
            UCMST      |
            UCMODE_0   |
            UCSYNC     |
            0;

    if(phase)
        UCB0CTL0 |= UCCKPH;
    if(polarity)
        UCB0CTL0 |= UCCKPL;

    P3SEL    |= BIT0 | BIT1 | BIT2; // SPI
    UCB0CTL1 |= UCSSEL__SMCLK;
    UCB0BRW   = 1;

    UCB0CTL1 &= ~UCSWRST;

}

uint8_t spiTransfer(uint8_t byte)
{
    while(!(UCB0IFG & UCTXIFG));
    UCB0TXBUF = byte;
    while(!(UCB0IFG & UCRXIFG));
    return UCB0RXBUF;
}


