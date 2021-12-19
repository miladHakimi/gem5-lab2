#include "vector.h"

#ifdef DMA_MODE
#include "gem5/dma_interface.h"
#endif

#define TRUNCATE(X, truncationSize) ((X >> truncationSize) << truncationSize)
#define HASH(m1, truncationSize) ((TRUNCATE(m1[0], truncationSize)) % CACHE_SIZE)

void vadd(INPUT_TYPE *m1, TYPE * config, INPUT_TYPE * result, INPUT_TYPE* cache) {
  int trucation_size = config[2], patch_en = config[3];
  INPUT_TYPE key = 0, patch_key = 0, second_region_out = 0;
  if (patch_en == 1) {
    patch_key = (((~m1[0] + 1)>> trucation_size) << trucation_size) ;
  }
  if (patch_en == 2) {
    for (int i = 0; i < 6; i++) {
      patch_key = ((m1[i] >> trucation_size) << trucation_size);
    }  
    second_region_out = m1[0] * m1[1] + m1[2] * m1[3] + m1[4] * m1[5];
  }
  // Hashing
  for (int i = 0; i < INPUT_COUNT; i++) {
    key = ((m1[i] >> trucation_size) << trucation_size) ;
  }

  key = key % CACHE_SIZE;
  patch_key = patch_key % CACHE_SIZE;
  INPUT_TYPE *valid = &cache[key * 3 + 2], *patch_valid = &cache[patch_key * 3 + 2], *data = &cache[key * 3 + 1];
  if (config[0] == 1) { // insert
    *valid = 1;
    *data = m1[INPUT_COUNT];
  }
  if (config[1] == 1) { // probe
    result[0] = *valid == 1 || (*patch_valid == 1 && patch_en >= 1);
    result[1] = (*valid == 1? *data : (*patch_valid==1 && patch_en >= 1)? cache[patch_key * 3 + 1] + second_region_out : 0);
  } 
  else {
    result[0] = 0;
    result[1] = key;
  }
}
