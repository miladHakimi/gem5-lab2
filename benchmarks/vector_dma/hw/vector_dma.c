#include "vector.h"

#ifdef DMA_MODE
#include "gem5/dma_interface.h"
#endif
#define CACHE_SIZE 5

typedef struct Entry {
  uint8_t  probe;
  uint64_t input;
  uint64_t output;
  uint64_t insert;
} entry_t;

typedef struct CacheResult {
  uint32_t hit;
  uint32_t hit2;
} cache_result_t;

#define HASH(m1) ((m1) % N)

void vadd(entry_t * restrict m1, TYPE * restrict m2, TYPE * restrict prod){
    int index = HASH(m1->input);
    if (m1->probe == 1) {
      prod[0]  = m2[index] != 0 ? 1: -1;
      prod[1]  = m2[index];
    }
    else {
      prod[0]  = -1;
      prod[1]  = -1;
        /* code */
    }

    if (m1->insert == 1) {
      m2[index]  = m1->output;
    }
}
