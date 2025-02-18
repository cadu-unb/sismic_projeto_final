#include <msp430.h> 
#include <stdint.h>

void spiConfig(uint8_t phase, uint8_t polarity)
{
    UCB0CTL1 |= UCSWRST;  // Coloca o módulo SPI em modo de reset para configuração
    UCB0CTL0 =
//  UCCKPH     |
//  UCCKPL     |
    UCMSB      |  // MSB first (bit mais significativo primeiro)
//  UC7BIT     |
    UCMST      |  // Modo mestre (master mode)
    UCMODE_0   |  // Modo SPI de 3 fios (4 fios se CS for usado externamente)
    UCSYNC     |  // Modo síncrono (SPI é síncrono por natureza)
    0;

    if(phase)
        UCB0CTL0 |= UCCKPH;  // Define a fase do clock (CPHA)
    if(polarity)
        UCB0CTL0 |= UCCKPL;  // Define a polaridade do clock (CPOL)

	P3SEL |= BIT0 | BIT1 | BIT2;  // Configura os pinos P3.0, P3.1 e P3.2 como SPI
	UCB0CTL1 |= UCSSEL__SMCLK;    // Usa SMCLK (Sub-Main Clock) como fonte de clock
	UCB0BRW = 1;                  // Define o divisor de clock (sem divisão)
	UCB0CTL1 &= ~UCSWRST;         // Sai do modo de reset e habilita o módulo SPI
}

uint8_t spiTransfer(uint8_t byte)
{
    while(!(UCB0IFG & UCTXIFG));  // Aguarda até que o buffer de transmissão esteja vazio
    UCB0TXBUF = byte;             // Envia o byte para o buffer de transmissão
    while(!(UCB0IFG & UCRXIFG));  // Aguarda até que o buffer de recepção tenha um dado disponível
    return UCB0RXBUF;             // Retorna o byte recebido no buffer de recepção
}
