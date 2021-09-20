#include "vector.h"

#ifdef DMA_MODE
#include "gem5/dma_interface.h"
#endif

void vadd(TYPE m1[N], TYPE m2[N], TYPE prod[N] ){
    int i, j, k;
    int k_col, i_col;
    TYPE mult;
    #pragma clang loop unroll_count(2)
    for(i=0;i<N;i++) {
            prod[i]  = 4*(m1[i] + m2[i]);
        
    }
}
