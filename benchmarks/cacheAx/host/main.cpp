#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../../common/dma.h"
#include "../../common/m5ops.h"
#include "../defines.h"

#define START_ACC() \
  dmacpy(spm1, &m1[index], CACHE_INPUT_SIZE); \
  while (!pollDma()); \
  resetDma(); \
  dmacpy(spm2, config, sizeof(TYPE)* CACHE_CONFIG_COUNT); \
  while (!pollDma()); \
  resetDma(); \
  dmacpy(spm4, cache, sizeof(Cache) * CACHE_SIZE); \
  while (!pollDma()); \
  resetDma(); \
  acc = 0x01; \
  while (acc != 0x0 ) {  \
    printf("RUNNING\n"); \
  } \
  acc = 0x00; \
  dmacpy(&m1[index], spm1, CACHE_INPUT_SIZE); \
  printf("m1[0] = %d\n", m1[index].in[0]);\
  while (!pollDma()); \
  resetDma(); \
  dmacpy(res, spm3, 2*sizeof(TYPE)); \
  while (!pollDma()); \
  resetDma(); \
  dmacpy(cache, spm4, sizeof(Cache) * CACHE_SIZE); \
  while (!pollDma()); \
  resetDma(); \
  printf("res[0] = %d, res[1] = %d\n", res[0], res[1]);\


#define TRUNCATE(X, truncationSize) ((X >> truncationSize) << truncationSize)
#define HASH(input, truncationSize) ((TRUNCATE(input, truncationSize)) % CACHE_SIZE)

int main(void) {
  m5_reset_stats();
  int input = 5;
  int output = 10;
  int hits = 0, misses = 0;
  uint64_t base = 0x80c00000;
  uint64_t spm_base = 0x2f100000;

  InputArgs *m1 = (InputArgs *) base;
  TYPE *config = (TYPE *)(base + PROGRAM_INPUT_SIZE);
  CacheOutputArgs *res = (CacheOutputArgs *)(base + PROGRAM_INPUT_SIZE  + sizeof(TYPE)* CACHE_CONFIG_COUNT);
  Cache *cache = (Cache *)(CacheOutputArgs *)(base + PROGRAM_INPUT_SIZE  + sizeof(TYPE)* CACHE_CONFIG_COUNT + sizeof(CacheOutputArgs));
#ifdef SPM
  InputArgs *spm1 = (InputArgs *)spm_base;
  TYPE *spm2 = (TYPE *)(spm_base + CACHE_INPUT_SIZE);
  CacheOutputArgs *spm3 = (CacheOutputArgs *)(spm_base + CACHE_INPUT_SIZE + sizeof(TYPE)* CACHE_CONFIG_COUNT);
  Cache *spm4 = (Cache *)(spm_base + CACHE_INPUT_SIZE + sizeof(TYPE)* CACHE_CONFIG_COUNT + sizeof(CacheOutputArgs));
#endif

  val_a = (INPUT_TYPE)spm_base; // input
  val_b = (TYPE)(spm_base + CACHE_INPUT_SIZE); // config
  val_c = (INPUT_TYPE)(spm_base + CACHE_INPUT_SIZE + sizeof(TYPE)* CACHE_CONFIG_COUNT); // output
  val_d = (INPUT_TYPE)(spm_base + CACHE_INPUT_SIZE + sizeof(TYPE)* CACHE_CONFIG_COUNT + sizeof(CacheOutputArgs)); // cache_addr
    // init
  config[0] = 1; // insert
  config[1] = 0; // probe
  config[2] = 10; // truncate bits
  config[3] = 2; // patch_en

  int index=0;
  
  for (index; index < PROGRAM_INPUT_COUNT; index++) {
    config[0] = 0;
    config[1] = 1;
    printf("ITER: %d\nres[0] = %d, res[1] = %d\n", index, res[0], res[1]);\

    START_ACC();
    if (res->valid != 1) {
      // inserting
      config[0] = 1;
      config[1] = 0;
      START_ACC();
      misses ++;
    }
    else
      hits ++;
  }

  printf("Hits : %d\n", hits);
  printf("Misses : %d\n", misses);

  m5_dump_stats();
  m5_exit();
}
