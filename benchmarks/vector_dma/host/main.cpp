#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../../common/dma.h"
#include "../../common/m5ops.h"
#include "../defines.h"

#define FIRST_ARG_SIZE sizeof(TYPE) * INPUT_COUNT
#define SECOND_ARG_SIZE sizeof(TYPE) * CACHE_SIZE * CACHE_CONFIG_COUNT

#define START_ACC(spm1, m1, spm2, m2, spm3, m3) \
  dmacpy(spm1, m1, FIRST_ARG_SIZE); \
  while (!pollDma()); \
  resetDma(); \
  dmacpy(spm2, m2, SECOND_ARG_SIZE); \
  while (!pollDma()); \
  resetDma();\
  acc = 0x01; \
  while (acc != 0x0 ) {  \
    printf("RUNNING\n"); \
  } \
  acc = 0x00; \
  dmacpy(m2, spm2, SECOND_ARG_SIZE); \
  while (!pollDma()); \
  resetDma();\
  dmacpy(m3, spm3, sizeof(TYPE) * 2 ); \
  while (!pollDma()); \
  resetDma(); \

#define TRUNCATE(X, truncationSize) ((X >> truncationSize) << truncationSize)
#define HASH(input, truncationSize) ((TRUNCATE(input, truncationSize)) % CACHE_SIZE)

#define INSERT_CACHE(spm1, m1, spm2, m2, spm3, m3, input, output) \
  m1[0] = 1; \
  m1[1] = 0; \
  m1[2] = input; \
  m1[3] = output; \
  START_ACC(spm1, m1, spm2, m2, spm3, m3); \

  
#define PROBE_CACHE(spm1, m1, spm2, m2, spm3, m3, input) \
  m1[0] = 0; \
  m1[1] = 1; \
  m1[2] = input; \
  m1[3] = 0; \
  START_ACC(spm1, m1, spm2, m2, spm3, m3); 


int main(void) {
  m5_reset_stats();
  int input = 5;
  int output = 10;
  int hits = 0, misses = 0;
  uint64_t base = 0x80c00000;
  uint64_t spm_base = 0x2f100000;
  TYPE *m1 = (TYPE *) base;
  TYPE *m2 = (TYPE *)(base + FIRST_ARG_SIZE);
  TYPE *m3 = (TYPE *)(base + SECOND_ARG_SIZE * FIRST_ARG_SIZE);
  // TYPE *check = (TYPE *)(base+ sizeof(TYPE)* (N)  + sizeof(entry_t) + sizeof(cache_result_t));
#ifdef SPM
  TYPE *spm1 = (TYPE *)spm_base;
  TYPE *spm2 = (TYPE *)(spm_base + FIRST_ARG_SIZE);
  TYPE *spm3 = (TYPE *)(spm_base + SECOND_ARG_SIZE + FIRST_ARG_SIZE);
#endif
  // *********************************** Zero Out m2 ***********************************
  for (int i = 0; i < CACHE_SIZE * CACHE_CONFIG_COUNT; i++)
    m2[i] = 0;

  for (int i = 0; i < INPUT_COUNT; i++)
    m1[i] = 0;
  m1[4] = 0;
  // printf("addr, %x\n", &((entry_t *)m1)[0].output);
  // Passing the scratchpad pointers to accelerator
  // Asking it to work of those addresses.
  val_a = (uint64_t)spm_base;
  val_b = (uint64_t)(spm_base + FIRST_ARG_SIZE);
  val_c = (uint64_t)(spm_base + SECOND_ARG_SIZE + FIRST_ARG_SIZE);
  // // init
  START_ACC(spm1, m1, spm2, m2, spm3, m3);
  int in_arr[15] = {0, 2, 12, 2, 3, 4, 5, 1, 0, 10, 12, 2, 1, 11, 10};
  int out_arr[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int truncationSize = m1[4];
  for (int i = 0; i < 15; i++) {
    input = in_arr[i];
    output = out_arr[i];
    int index = HASH(input, truncationSize);
    PROBE_CACHE(spm1, m1, spm2, m2, spm3, m3, input);
    printf("m3: %d, %d\n", m3[0], m3[1]);
    if (m2[index * CACHE_CONFIG_COUNT + 1]) {
      printf("hit: input: %d, index: %d, m2[%d]\n", input, index, index * CACHE_CONFIG_COUNT + 1);
      hits++;
    } else {
      printf("miss: input: %d, index: %d, valid: %d\n", input, index, m2[index * CACHE_CONFIG_COUNT + 1]);
      misses++;
      INSERT_CACHE(spm1, m1, spm2, m2, spm3, m3, input, output);
      printf("inserted: input: %d, index: %d, valid: %d\n", input, index, m2[index * CACHE_CONFIG_COUNT + 1]);
    }
  }
  printf("Hits : %d\n", hits);
  printf("Misses : %d\n", misses);

  m5_dump_stats();
  m5_exit();
}
