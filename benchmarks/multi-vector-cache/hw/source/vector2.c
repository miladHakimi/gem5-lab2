#include "../hw_defines.h"
void vmul(TYPE m1[N], TYPE prod[N]) {
#pragma clang loop unroll_count(8)
  for (int i = 0; i < N; i++) {
    prod[i] = (m1[i] * 8);
  }
}