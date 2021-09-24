#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../../common/dma.h"
#include "../../common/m5ops.h"
#include "../defines.h"

int main(void) {
  m5_reset_stats();
  uint64_t base = 0x80c00000;
  uint64_t spm_base = 0x2f100000;
  TYPE *m1 = (TYPE *)base;
  TYPE *m2 = (TYPE *)(base + sizeof(TYPE) * N);
  TYPE *m3 = (TYPE *)(base + 2 * sizeof(TYPE) * N);
  TYPE *check = (TYPE *)(base+3*sizeof(TYPE)*N);

  // Check that the gem5 script loaded the data in binary files into the appropriate location in memory.
  for (int i = 0; i < N; i++) {
            printf("m1: %p %ld\n",m1+i,m1[i]);
  }

  for (int i = 0; i < N; i++) {
            printf("m2: %p %ld\n",m2+i,m2[i]);
  }

#ifdef SPM
  TYPE *spm1 = (TYPE *)spm_base;
  TYPE *spm2 = (TYPE *)(spm_base + sizeof(TYPE) * N);
  TYPE *spm3 = (TYPE *)(spm_base + 2 * sizeof(TYPE) * N);
#endif

  // Passing the scratchpad pointers to accelerator
  // Asking it to work of those addresses.
  val_a = (uint64_t)spm_base;
  val_b = (uint64_t)(spm_base + sizeof(TYPE) * N);
  val_c = (uint64_t)(spm_base + 2 * sizeof(TYPE) * N);

  printf("%d\n", acc);
  // acc = 0x01;

  dmacpy(spm1, m1, sizeof(TYPE) * N);
  while (!pollDma());
  resetDma();
  dmacpy(spm2, m2, sizeof(TYPE) * N);
  while (!pollDma());
  resetDma();

  printf("Acc Activated: %d\n", acc);
  acc = 0x01;
  while (acc != 0x0) {
    printf("Acc Status : %d\n", acc);
  }
  printf("Acc Done : %d\n", acc);
  acc = 0x00;

  // Copy  results back from accelerator
  dmacpy(m3, spm3, sizeof(TYPE) * N);
  while (!pollDma());
  resetDma();

    // Passing the scratchpad pointers to accelerator
  // Asking it to work of those addresses.
  val_a = (uint64_t)spm_base;
  val_b = (uint64_t)(spm_base + sizeof(TYPE) * N);
  val_c = (uint64_t)(spm_base + 2 * sizeof(TYPE) * N);

  printf("%d\n", acc);
  // acc = 0x01;

  dmacpy(spm1, m1, sizeof(TYPE) * N);
  while (!pollDma());
  resetDma();
  dmacpy(spm2, m2, sizeof(TYPE) * N);
  while (!pollDma());
  resetDma();

  printf("Acc Activated: %d\n", acc);
  acc = 0x01;
  while (acc != 0x0) {
    printf("Acc Status : %d\n", acc);
  }
  printf("Acc Done : %d\n", acc);
  acc = 0x00;

  // Copy  results back from accelerator
  dmacpy(m3, spm3, sizeof(TYPE) * N);
  while (!pollDma());
  resetDma();

#ifdef CHECK
   bool fail = false;
   for (int i = 0; i < N; i++) {
     if (4*(m1[i] + m2[i]) != m3[i])
       {
       	printf("Expected : %ld  Actual : %ld\n",m1[i]+m2[i],m3[i]);
 	      fail = true;
       }
     }

   if (fail)
     printf("Check Failed\n");
   else
    printf("Check Passed\n");
#endif
  //*(char *)0x2FFFFFFF = 1;
  m5_dump_stats();
  m5_exit();
}
