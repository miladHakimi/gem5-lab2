#include "../hw_defines.h"

void top(uint64_t m1_addr, uint64_t m2_addr, uint64_t m3_addr) {

  // Define Device MMRs
  volatile uint8_t *V1Flags = (uint8_t *)V1;
  volatile uint8_t *V2Flags = (uint8_t *)V2;
  volatile uint8_t *DmaFlags = (uint8_t *)(DMA);
  volatile uint64_t *DmaRdAddr = (uint64_t *)(DMA + 1);
  volatile uint64_t *DmaWrAddr = (uint64_t *)(DMA + 9);
  volatile uint32_t *DmaCopyLen = (uint32_t *)(DMA + 17);
  *V1Flags = 0x0;
  *V2Flags = 0x0;
  *DmaFlags = 0x0;

  // Transfer Input Matrices
  // Transfer M1
  *DmaRdAddr = m1_addr;
  *DmaWrAddr = M1ADDR;
  *DmaCopyLen = vector_size;
  // Fence it
  while (*DmaFlags != 0x0)
    ;
  *DmaFlags = DEV_INIT;
  // Poll DMA for finish
  while ((*DmaFlags & DEV_INTR) != DEV_INTR)
    ;
  // Reset DMA
  *DmaFlags = 0x0;  
  
  
  // Transfer M2
  *DmaRdAddr = m2_addr;
  *DmaWrAddr = M2ADDR;
  *DmaCopyLen = vector_size;

  // Fence it
  while (*DmaFlags != 0x0)
    ;
  *DmaFlags = DEV_INIT;
  // Poll DMA for finish
  while ((*DmaFlags & DEV_INTR) != DEV_INTR)
    ;
  // Reset DMA
  *DmaFlags = 0x0;

  // Check if accelerator ready for kickstarting
  while (*V1Flags != 0x0)
    ;

  // Start the accelerated function
  *V1Flags = DEV_INIT;
  // Poll function for finish
  while ((*V1Flags & DEV_INTR) != DEV_INTR)
    ;
  *V1Flags = 0x0;

  // Invoke accelerator 2

  // Transfer Input Matrices
  // Transfer the output of V1 to V2.
  *DmaRdAddr = M3ADDR;
  *DmaWrAddr = M1ADDR_V2;
  *DmaCopyLen = vector_size;
  *DmaFlags = DEV_INIT;
  // //Poll DMA for finish
  while ((*DmaFlags & DEV_INTR) != DEV_INTR)
    ;

  // //	Transfer M2
  // *DmaRdAddr = M3ADDR;
  // *DmaWrAddr = M2ADDR_V2;
  // *DmaCopyLen = vector_size;
  // *DmaFlags = DEV_INIT;
  // //Poll DMA for finish
  // while ((*DmaFlags & DEV_INTR) != DEV_INTR)
  // 	;
  while (*V2Flags != 0x0)
    ;

  // Start the accelerated function
  *V2Flags = DEV_INIT;
  // Poll function for finish
  while ((*V2Flags & DEV_INTR) != DEV_INTR)
    ;
  *V2Flags = 0x0;

  // Transfer M3
  *DmaRdAddr = M3ADDR_V2;
  *DmaWrAddr = m3_addr;
  *DmaCopyLen = vector_size;
  *DmaFlags = DEV_INIT;
  // Poll DMA for finish
  while ((*DmaFlags & DEV_INTR) != DEV_INTR)
    ;

  return;
}