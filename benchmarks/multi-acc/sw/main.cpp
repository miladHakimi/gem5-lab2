#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "bench.h"
#include "../../common/m5ops.h"

volatile uint8_t  * top   = (uint8_t  *)0x2f000000;
volatile uint32_t * val_a = (uint32_t *)0x2f000001;
volatile uint32_t * val_b = (uint32_t *)0x2f000009;
volatile uint32_t * val_c = (uint32_t *)0x2f000011;

int main(void) {
	m5_reset_stats();
    bool fail = false;
    uint32_t base = 0x80c00000;
    TYPE *m1 = (TYPE *)base;
    TYPE *m2 = (TYPE *)(base + sizeof(TYPE) * N);
    TYPE *m3 = (TYPE *)(base + 2 * sizeof(TYPE) * N);
 
    *val_a = (uint32_t)(void *)m1;
    *val_b = (uint32_t)(void *)m2;
    *val_c = (uint32_t)(void *)m3;
    // printf("%d\n", *top);
    *top = 0x01;
    int count;
    while (stage < 1)
        count++;
    printf("Job complete\n");
#ifdef CHECK
    printf("Checking result\n");
    for(int i=0; i<N; i++) {
        if(m3[i] != m1[i]+m2[i]) {
            printf("Expected:%f Actual:%f\n", m1[i]+m2[i], m3[i]);
            fail = true;
            break;
        }
    }
    if(fail)
        printf("Check Failed\n");
    else
        printf("Check Passed\n");
#endif
	m5_dump_stats();
	m5_exit();
}
