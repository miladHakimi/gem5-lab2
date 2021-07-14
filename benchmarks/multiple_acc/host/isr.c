#include <stdio.h>
#include "A1.h"
#include "A2.h"

void A1_isr(void)
{
    uint8_t * hwacc = (uint8_t *)0x2f000000;
    hwacc[0] = 0x0;
    printf("Interrupt A1:\n\r");

}


void A2_isr(void)
{
    uint8_t * hwacc = (uint8_t *)0x2f000100;
    hwacc[0] = 0x0;
    printf("Interrupt A2:\n\r");

}
