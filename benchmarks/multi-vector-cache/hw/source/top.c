#include "../hw_defines.h"

void top(uint64_t m1_addr, uint64_t m2_addr, uint64_t m3_addr) {

  // Define Device MMRs
  volatile uint8_t *V1Flag = (uint8_t *)V1;
  volatile uint64_t *V1Arg1 = (uint64_t *)(V1 + 1);
  volatile uint64_t *V1Arg2 = (uint64_t *)(V1 + 9);
  volatile uint64_t *V1Arg3 = (uint64_t *)(V1 + 17);

  volatile uint8_t *V2Flag = (uint8_t *)V2;
  volatile uint64_t *V2Arg1 = (uint64_t *)(V2 + 1);
  volatile uint64_t *V2Arg2 = (uint64_t *)(V2 + 9);

  *V1Flag = 0x0;
  *V2Flag = 0x0;

  // Accelerator 1 invocation

  // Set up arguments for accelerator.
  *V1Arg1 = m1_addr;
  *V1Arg2 = m2_addr;
  *V1Arg3 = m3_addr;

  // Start the accelerated function
  *V1Flag = DEV_INIT;
  // Poll function for finish
  while ((*V1Flag & DEV_INTR) != DEV_INTR)
    ;
  *V1Flag = 0x0;

  // Accelerator 2 invocation
  *V2Arg1 = m3_addr;
  *V2Arg2 = m3_addr;
  // Start the accelerated function
  *V2Flag = DEV_INIT;
  // Poll function for finish
  while ((*V2Flag & DEV_INTR) != DEV_INTR)
    ;

  *V2Flag = 0x0;

  return;
}