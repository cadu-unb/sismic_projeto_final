#include <msp430.h> 
#include "tft-lcd.h"
#include "clock-lcd.h"
#include "time-ctrl.h"

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    SetVcoreUp(1);
    SetVcoreUp(2);
    SetVcoreUp(3);

    UCSCTL3 |= SELREF_2;                    // Define REFO (32.768 Hz) como referência para o FLL
    __bis_SR_register(SCG0);                // Desativa o loop de controle do FLL (Frequency-Locked Loop)
    UCSCTL0 = 0x0000;                       // Configura os valores mínimos para DCOx e MODx
    UCSCTL1 = DCORSEL_5;                    // Seleciona a faixa de operação do DCO para 25 MHz
    UCSCTL2 = FLLD_0 + (FPROC / 32768 - 1); // Configura o multiplicador do DCO para gerar 25 MHz
                                            // Fórmula: (N + 1) * FLLRef = Fdco
                                            // (761 + 1) * 32768 = 25 MHz
    __bic_SR_register(SCG0);                // Reativa o loop de controle do FLL
    UCSCTL4 |= SELA_3;                      // Seleciona ACLK como MCLK/1024
    UCSCTL5 |= DIVPA_5 | DIVA_5;            // Divide ACLK por 32
    P1DIR |= BIT0;                          // Configura P1.0 como saída
    P1SEL |= BIT0;                          // Seleciona a função alternativa de P1.0 (ACLK)

	tftConfig();

	uint8_t j = 10;
    while (j--) {
        LCD_Clear(0xff, 0xff, 0xff, 1);

        //drawRect(50, 10, 65, 31);
        drawString_4x(50, 10, "0 9 @ A B C", 0x0000, 0x0000, 0x0000);
//        drawString_4x(50, 10, "0", 0x0000, 0x0000, 0x0000);
//        drawString_4x(50, 30, "9", 0x0000, 0x0000, 0x0000);
//        drawString_4x(50, 50, "@", 0x0000, 0x0000, 0x0000);
//        drawString_4x(50, 38, "A", 0x0000, 0x0000, 0x0000);
//        drawString_4x(50, 66, "B", 0x0000, 0x0000, 0x0000);
//        drawString_4x(50, 66, "C", 0x0000, 0x0000, 0x0000);
//        drawString_4x(100, 10, , 0x0000, 0x0000, 0x0000);
        //drawString(100, 20, "!", 0x0000, 0x0000, 0x0000);



        delay(10);
    }

    __bis_SR_register(LPM4_bits);

	return 0;
}
