#include "../hw_defines.h"
void vadd(TYPE m1[N], TYPE m2[N], TYPE sum[N]) {
#pragma clang loop unroll_count(8)
  for (int i = 0; i < N; i++) {
    sum[i] = (m1[i] + m2[i]);
  }
}
