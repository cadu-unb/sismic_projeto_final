/// time-ctrl.h - Controle de tempo para MSP430
#ifndef __TIME_CTRL_H
#define __TIME_CTRL_H

#include <msp430.h>
#include <stdint.h>

#define FPROC 25000000 // Frequência do processador em Hz

// Função para gerar um atraso (delay) em milissegundos
void delay(unsigned int i);

#endif // __TIME_CTRL_H
