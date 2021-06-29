#include <stdio.h>
#include "gemm.h"

void isr(void)
{
	printf("Interrupt\n\r");
	acc = 0;
}
