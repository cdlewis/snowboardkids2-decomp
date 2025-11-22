#include "common.h"

extern u8 D_800BAF08_1E7FB8;
extern void setCleanupCallback(void *);
extern void setCallback(void *);
extern void func_800B58D0_1E2980(void);
extern void *scheduleTask(void *, u8, u8, u8);

void func_800B5984_1E2A34(void);
void func_800B5A34_1E2AE4(void);
void func_800B5A6C_1E2B1C(void);
void func_800B5B20_1E2BD0(void);
void func_800B5898_1E2948(void);

void func_800B57E0_1E2890(void) {
    D_800BAF08_1E7FB8 = 1;
}

void func_800B57F0_1E28A0(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5) {
    s16 *result;

    if (arg5 <= 0) {
        arg5 = 1;
    }

    result = scheduleTask(func_800B5898_1E2948, 0, 0, 0);

    if (result != NULL) {
        result[0] = arg0;
        result[1] = arg1;
        result[2] = arg2;
        result[3] = arg3;
        result[4] = arg4;
        result[5] = arg5;
        result[6] = 0;
    }
}

void func_800B5898_1E2948(void) {
    setCleanupCallback(func_800B5984_1E2A34);
    D_800BAF08_1E7FB8 = 0;
    setCallback(func_800B58D0_1E2980);
}

INCLUDE_ASM("asm/nonmatchings/1E2890", func_800B58D0_1E2980);

void func_800B5984_1E2A34(void) {
}

void func_800B598C_1E2A3C(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5) {
    s16 *result;
    s16 temp_arg4;

    temp_arg4 = arg4;
    if (arg4 <= 0) {
        temp_arg4 = 1;
    }

    result = scheduleTask(func_800B5A34_1E2AE4, 0, 0, 0);

    if (result != NULL) {
        result[0] = arg0;
        result[1] = arg1;
        result[2] = arg2;
        result[3] = arg3;
        result[4] = temp_arg4;
        result[5] = 0;
        *(s32 *)&result[6] = arg5;
    }
}

void func_800B5A34_1E2AE4(void) {
    setCleanupCallback(func_800B5B20_1E2BD0);
    D_800BAF08_1E7FB8 = 0;
    setCallback(func_800B5A6C_1E2B1C);
}

INCLUDE_ASM("asm/nonmatchings/1E2890", func_800B5A6C_1E2B1C);

void func_800B5B20_1E2BD0(void) {
}
