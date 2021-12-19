#ifndef __DEFINES_H__
#define __DEFINES_H__


#define common_val *(unsigned *)0x8f000000
#define acc        *(char *)0x2f000000
#define val_a      *(int64_t *)0x2f000001
#define val_b      *(int64_t *)0x2f000009
#define val_c      *(int64_t *)0x2f000011
#define val_d      *(unsigned *)0x2f000019

#define rcIndex     (r*ROW + c)

#define SPM
#define CHECK

#define TYPE uint32_t
#define N 16
#define CACHE_SIZE 1024
#define INPUT_COUNT 18
#define CACHE_CONFIG_COUNT 4 // key, valid, data
#define INPUT_TYPE uint32_t
#define CACHE_INPUT_SIZE sizeof(INPUT_TYPE) * (INPUT_COUNT+1)

typedef struct InputArgs {
    INPUT_TYPE in[INPUT_COUNT];
    INPUT_TYPE out;
};

typedef struct CacheOutputArgs {
    uint32_t valid;
    uint32_t result;
};

typedef struct Cache {
    uint32_t key;
    uint32_t value;
    uint32_t valid;
};

#define PROGRAM_INPUT_COUNT 100
#define PROGRAM_INPUT_SIZE sizeof(InputArgs) * PROGRAM_INPUT_COUNT


#endif
