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

        drawString(5, 5, "ISM", 1, 136, 214);
        drawString(120, 40, "COMPLEMENTO", 0x0, 0x0000, 0x0000);
        drawString(120, 100, "Ovos mexidos", 0x0000, 0x0000, 0x0000);
        drawString(80, 120, "Contem: Leite e derivados", 255, 0x0000, 0x0000);
        drawString(80, 140, "        ovos", 255, 0x0000, 0x0000);
        drawString(10, 220, "Porcao: 100g", 0x0, 0x0000, 0x0000);
        drawString(210, 220, "Valor Cal: 30kCal", 0x0, 0x0000, 0x0000);
        delay(100);

        LCD_Clear(0xff, 0xff, 0xff, 1);

        drawString(5, 5, "ISM", 1, 136, 214);
        drawString(120, 40, "PRATO PRINCIPAL", 0x0, 0x0000, 0x0000);
        drawString(90, 100, "Isca de Frango acebolada", 0x0000, 0x0000, 0x0000);
        //drawString(80, 120, "Contem: Leite e derivados", 255, 0x0000, 0x0000);
        //drawString(80, 140, "        ovos", 255, 0x0000, 0x0000);
        drawString(10, 220, "Porcao: 200g", 0x0, 0x0000, 0x0000);
        drawString(210, 220, "Valor Cal: 60kCal", 0x0, 0x0000, 0x0000);
        delay(100);

        LCD_Clear(0xff, 0xff, 0xff, 1);

        drawString(5, 5, "ISM", 1, 136, 214);
        drawString(140, 40, "Sopa", 0x0, 0x0000, 0x0000);
        drawString(90, 100, "Sopa de cenoura com curry", 0x0000, 0x0000, 0x0000);
        drawString(80, 120, "      e leite de  coco", 0, 0x0000, 0x0000);
        drawString(10, 220, "Porcao: 150ml", 0x0, 0x0000, 0x0000);
        drawString(210, 220, "Valor Cal: 15kCal", 0x0, 0x0000, 0x0000);

        delay(100);
    }

    __bis_SR_register(LPM4_bits);

	return 0;
}
