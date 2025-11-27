#include "56910.h"
#include "common.h"
#include "task_scheduler.h"

extern void *func_80035F80_36B80(s32);
extern void func_80035074_35C74(void);
extern void func_80034D58_35958(void);

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
} Func358FCStruct;

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ void *unk8;
} Func34574Arg;

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800333E0_33FE0);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80033458_34058);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_8003365C_3425C);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80033688_34288);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80033800_34400);

extern void *freeNodeMemory(void *);

void func_80033974_34574(Func34574Arg *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800339AC_345AC);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80033AE4_346E4);

void func_80033E08_34A08(Func34574Arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80033E40_34A40);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80033EDC_34ADC);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80033F50_34B50);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80033F7C_34B7C);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800340F4_34CF4);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800343D0_34FD0);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800343FC_34FFC);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800344A8_350A8);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80034640_35240);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_8003466C_3526C);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80034750_35350);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_8003498C_3558C);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800349B8_355B8);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80034A30_35630);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80034A94_35694);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80034AC0_356C0);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80034BD8_357D8);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80034CD0_358D0);

void func_80034CFC_358FC(Func358FCStruct *arg0) {
    Func358FCStruct *temp_s0;

    temp_s0 = arg0;
    getCurrentAllocation();
    temp_s0->unk0 = loadAsset_34F7E0();
    temp_s0->unk4 = func_80035F80_36B80(1);
    setCleanupCallback(func_80035074_35C74);
    temp_s0->unk8 = 0;
    temp_s0->unkA = 0;
    setCallback(func_80034D58_35958);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80034D58_35958);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035074_35C74);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800350AC_35CAC);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_8003513C_35D3C);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035234_35E34);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035260_35E60);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035408_36008);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035548_36148);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800356AC_362AC);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035878_36478);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035DE0_369E0);
