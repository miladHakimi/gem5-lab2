#include "vector.h"

#ifdef DMA_MODE
#include "gem5/dma_interface.h"
#endif

void vadd(INPUT_TYPE *x_x, INPUT_TYPE *y_y, INPUT_TYPE *result_r) {
  *result_r = *x_x + *y_y;
}
