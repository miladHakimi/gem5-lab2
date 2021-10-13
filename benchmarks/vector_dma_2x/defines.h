#ifndef __DEFINES_H__
#define __DEFINES_H__


#define common_val *(unsigned *)0x8f000000
#define acc        *(char *)0x2f000000
#define val_a      *(int *)0x2f000001
#define val_b      *(int *)0x2f000009
#define val_c      *(int *)0x2f000011
#define rcIndex     (r*ROW + c)

#define SPM
#define CHECK

#define TYPE uint32_t
#define N 8

#endif
