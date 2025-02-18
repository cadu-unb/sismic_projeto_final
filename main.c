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

    UCSCTL3 |= SELREF_2;                    // Set DCO FLL reference = REFO (32 768)
    __bis_SR_register(SCG0);                // Disable the FLL control loop
    UCSCTL0 = 0x0000;                       // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_5;                    // Select DCO range 25MHz operation
    UCSCTL2 = FLLD_0 + (FPROC/32768 - 1);   // Set DCO Multiplier for 25MHz (487 pour 16MHz) (761 pour 25MHz)
                                            // (N + 1) * FLLRef = Fdco
                                            // (761 + 1) * 32768 = 25MHz
                                            // Set FLL Div = fDCOCLK
    __bic_SR_register(SCG0);                // Enable the FLL control loopthen select clock source by write SPI control register (UCSSELx)

    UCSCTL4 |= SELA_3;                      // presence of MCLK/1024 on P1.0
    UCSCTL5 |= DIVPA_5 | DIVA_5;
    P1DIR |= BIT0;
    P1SEL |= BIT0;

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
