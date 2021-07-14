#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../defines.h"
#include "A1.h"
#include "A2.h"
#include "../../common/m5ops.h"

// Memory Base Offsets
#define BASE            	0x80c00000
#define SPM_BASE        	0x2f100000
int main(void) {
	
  m5_reset_stats();
  uint64_t base = 0x80c00000;
  uint64_t spm_base = 0x2f100000;
  TYPE *m1 = (TYPE *)base;
  TYPE *m2 = (TYPE *)(base + sizeof(TYPE) * N);
  TYPE *m3 = (TYPE *)(base + 2 * sizeof(TYPE) * N);
  TYPE *check = (TYPE *)(base+3*sizeof(TYPE)*N);
  acc_A2 = 0x0;
  acc_A1 = 0x0;
  // register int sp asm("sp");
  // uint64_t base_input = 0x90c00000;
  // uint32_t* input = (uint32_t*)0x90c00000;
  for (int i = 0; i < N; i++) {
            printf("%p %ld\n",m1+i,m1[i]);
  }

  for (int i = 0; i < N; i++) {
            printf("%p %ld\n",m2+i,m2[i]);
  }

  // Set up parameters for accelerators. 
  // Base address of m1, Base address of m2, Base address of result.
  // All addresses are in CPU space
  val_a_A2 = (uint64_t)base;
  val_b_A2 = (uint64_t)(base + sizeof(TYPE) * N);
  val_c_A2 = (uint64_t)(base + 2 * sizeof(TYPE) * N);

  printf("Acc Activated: %d\n", acc_A2);
  acc_A2 = 0x01;
  while (acc_A2 != 0x0) {
    printf("Acc Status : %d\n", acc_A2);
  }
  printf("Acc Done : %d\n", acc_A2);

  #ifdef CHECK
   bool fail = false;
   for (int i = 0; i < N; i++) {
     if (4*(m1[i] + m2[i]) == m3[i])
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

