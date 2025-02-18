#include <msp430.h>
#include <stdint.h>
#include "clock-lcd.h"

void SetVcoreUp(uint8_t level)
{
    // Abre os registradores PMM para escrita
    PMMCTL0_H = PMMPW_H;
    
	// Configura o novo nível do SVS/SVM no lado de alta tensão
    SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
    
	// Configura o SVM no lado de baixa tensão para o novo nível
    SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
    
	// Aguarda até que o SVM esteja estabilizado
    while ((PMMIFG & SVSMLDLYIFG) == 0);
    
	// Limpa flags já setados
    PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
    
	// Define o VCore para o novo nível
    PMMCTL0_L = PMMCOREV0 * level;
    
	// Aguarda até que o novo nível seja alcançado
    if ((PMMIFG & SVMLIFG))
        while ((PMMIFG & SVMLVLRIFG) == 0);
    
	// Configura o SVS/SVM no lado de baixa tensão para o novo nível
    SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;

    // Bloqueia os registradores PMM para evitar escrita
    PMMCTL0_H = 0x00;
}
