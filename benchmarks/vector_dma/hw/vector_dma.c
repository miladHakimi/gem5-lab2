#include "vector.h"

#ifdef DMA_MODE
#include "gem5/dma_interface.h"
#endif

#define TRUNCATE(X, truncationSize) ((X >> truncationSize) << truncationSize)
#define HASH(m1, truncationSize) ((TRUNCATE(m1[0], truncationSize)) % CACHE_SIZE)

void vadd(TYPE *restrict m1, TYPE * restrict m2, TYPE * restrict prod) {
  TYPE insert = m1[0], probe = m1[1], input = m1[2], output = m1[3], truncationSize = m1[4];
  int index = ((input >> truncationSize) << truncationSize) % CACHE_SIZE;
  TYPE *key = &m2[index * CACHE_CONFIG_COUNT], *valid = &m2[index * CACHE_CONFIG_COUNT + 1], *data = &m2[index * CACHE_CONFIG_COUNT + 2];
  if (probe == 1) {
    prod[0]  = *valid;
    prod[1]  = *data;
  }
  else {
    prod[0]  = 0;
    prod[1]  = -1;
  }

  if (insert == 1) {
    *key  = (input >> truncationSize) << truncationSize;
    *data  = output;
    *valid  = 1;
  }
}
