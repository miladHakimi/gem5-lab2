#ifndef __DEFINES_H__
#define __DEFINES_H__


#define common_val *(unsigned *)0x8f000000
#define acc        *(char *)0x2f000000
#define val_a      *(int64_t *)0x2f000001
#define val_b      *(int64_t *)0x2f000009
#define val_c      *(int64_t *)0x2f000011
// #define val_d      *(unsigned *)0x2f000019

#define rcIndex     (r*ROW + c)

#define SPM
#define CHECK

#define TYPE uint32_t
#define N 16
#define CACHE_SIZE 16
#define INPUT_COUNT 5
#define CACHE_CONFIG_COUNT 3 // key, valid, data

#define INPUT_TYPE uint32_t

#endif
