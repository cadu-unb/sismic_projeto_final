#include <msp430.h>
#include <stdint.h>
#include "time-ctrl.h"

void delay(unsigned int i)
{
    while(i--)
        __delay_cycles(FPROC/1000);  // 1000 for 1MHz
}
