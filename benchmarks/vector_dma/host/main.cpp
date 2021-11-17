#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../../common/dma.h"
#include "../../common/m5ops.h"
#include "../defines.h"

#define CACHE_SIZE 5
typedef struct Entry {
  uint8_t  probe;
  uint64_t input;
  uint64_t output;
  uint64_t insert;
} entry_t;

#define START_ACC(spm1, m1, spm2, m2, spm3, m3) \
  dmacpy(spm1, m1, sizeof(entry_t)); \
  while (!pollDma()); \
  resetDma(); \
  dmacpy(spm2, m2, sizeof(TYPE) * N);\
  while (!pollDma());\
  resetDma();\
  acc = 0x01; \
  while (acc != 0x0 ) {  \
    printf("Acc Status : %d\n", acc); \
  } \
  acc = 0x00; \
  dmacpy(m3, spm3, sizeof(TYPE) * 2); \
  while (!pollDma()); \
  resetDma();\
  dmacpy(m2, spm2, sizeof(TYPE) * N); \
  while (!pollDma()); \
  resetDma(); \
  printf("%d\n", m2[0]); \


#define INSERT_CACHE(spm1, m1, spm2, m2, spm3, m3, input, output) \
  m1[0].probe = 0; \
  m1[0].insert = 1; \
  m1[0].input = input; \
  m1[0].output = output; \
  START_ACC(spm1, m1, spm2, m2, spm3, m3); 

#define PROBE_CACHE(spm1, m1, spm2, m2, spm3, m3, input) \
  m1[0].probe = 1; \
  m1[0].insert = 0; \
  m1[0].input = input; \
  START_ACC(spm1, m1, spm2, m2, spm3, m3); 


typedef struct CacheResult {
  uint32_t hit;
  uint32_t hit2;
} cache_result_t;

int main(void) {
  m5_reset_stats();
  int input = 5;
  int output = 11;

  uint64_t base = 0x80c00000;
  uint64_t spm_base = 0x2f100000;
  entry_t *m1 = (entry_t *)base;
  TYPE *m2 = (TYPE *)(base + sizeof(entry_t));
  TYPE *m3 = (TYPE *)(base + sizeof(TYPE) * N + sizeof(entry_t));
  // TYPE *check = (TYPE *)(base+ sizeof(TYPE)* (N)  + sizeof(entry_t) + sizeof(cache_result_t));

#ifdef SPM
  entry_t *spm1 = (entry_t *)spm_base;
  TYPE *spm2 = (TYPE *)(spm_base + sizeof(entry_t));
  TYPE *spm3 = (TYPE *)(spm_base + sizeof(TYPE) * N + sizeof(entry_t));
#endif
  // *********************************** Zero Out m2 ***********************************
  for (int i = 0; i < N; i++) {
    m2[i] = i*2;
  }

  // Passing the scratchpad pointers to accelerator
  // Asking it to work of those addresses.
  val_a = (uint64_t)spm_base;
  val_b = (uint64_t)(spm_base + sizeof(entry_t));
  val_c = (uint64_t)(spm_base + sizeof(TYPE) * N + sizeof(entry_t));
  
  // init
  START_ACC(spm1, m1, spm2, m2, spm3, m3);
  
  PROBE_CACHE(spm1, m1, spm2, m2, spm3, m3, input);
  INSERT_CACHE(spm1, m1, spm2, m2, spm3, m3, input, output);

  PROBE_CACHE(spm1, m1, spm2, m2, spm3, m3, input);
  m5_dump_stats();
  m5_exit();
}
