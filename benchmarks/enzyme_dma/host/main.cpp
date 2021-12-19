#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../../common/dma.h"
#include "../../common/m5ops.h"
#include "../defines.h"

#define START_ACC() \
  dmacpy(spm1, x, sizeof(INPUT_TYPE)); \
  while (!pollDma()); \
  resetDma(); \
  dmacpy(spm2, y, sizeof(INPUT_TYPE)); \
  while (!pollDma()); \
  resetDma(); \
  acc = 0x01; \
  while (acc != 0x0 ) {  \
    printf("RUNNING\n"); \
  } \
  acc = 0x00; \
  dmacpy(res, spm3, sizeof(INPUT_TYPE)); \
  while (!pollDma()); \
  resetDma(); \


int main(void) {
  m5_reset_stats();
  uint64_t base = 0x80c00000;
  uint64_t spm_base = 0x2f100000;

  INPUT_TYPE *x = (INPUT_TYPE *) base;
  INPUT_TYPE *y = (INPUT_TYPE *)(base + sizeof(INPUT_TYPE));
  INPUT_TYPE *res = (INPUT_TYPE *)(base + sizeof(INPUT_TYPE) + sizeof(INPUT_TYPE));
#ifdef SPM
  INPUT_TYPE *spm1 = (INPUT_TYPE *)spm_base;
  INPUT_TYPE *spm2 = (INPUT_TYPE *)(spm_base + sizeof(INPUT_TYPE));
  INPUT_TYPE *spm3 = (INPUT_TYPE *)(spm_base + sizeof(INPUT_TYPE) + sizeof(INPUT_TYPE));
#endif

  val_a = (INPUT_TYPE)spm_base; // input
  val_b = (INPUT_TYPE)(spm_base + sizeof(INPUT_TYPE)); // config
  val_c = (INPUT_TYPE)(spm_base + sizeof(INPUT_TYPE) + sizeof(INPUT_TYPE)); // output
    // init
  *x = 1;
  *y = 2;

  START_ACC();
  printf("%d\n", *res);
  m5_dump_stats();
  m5_exit();
}
