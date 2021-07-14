#include "A2.h"

void A2(TYPE m1[N], TYPE m2[N], TYPE prod[N] ){
    int i, j, k;
    int k_col, i_col;
    TYPE mult;
    #pragma clang loop unroll_count(8)
    for(i=0;i<N;i++) {
            prod[i]  = m1[i];
        
    }
}

