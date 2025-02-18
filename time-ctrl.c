/// time-ctrl.c - Implementação das funções de controle de tempo
#include <msp430.h>
#include <stdint.h>
#include "time-ctrl.h"

void delay(unsigned int i)
{
    while(i--)  // Decrementa até zero
        __delay_cycles(FPROC/1000);  // Gera um atraso baseado na frequência do processador
}
