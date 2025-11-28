#include "56910.h"
#include "common.h"
#include "task_scheduler.h"

extern void *func_80035F80_36B80(s32);
extern void func_80034D58_35958(void);
extern void debugEnqueueCallback(u16, u8, void *, void *);

extern u8 D_459310[];
extern u8 D_459E00[];
extern u8 D_41A1D0[];
extern u8 D_41AD80[];

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

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s16 unk8;
} Func349B8Arg;

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
} Func33FE0Entry; // size 0x10

typedef struct {
    /* 0x00 */ Func33FE0Entry entries[13];
} Func33FE0Arg;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ void *unk8;
    /* 0x0C */ u8 padC[0x8];
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8 pad16[0x6];
    /* 0x1C */ u8 unk1C;
} Func34ADCArg;

void func_80035074_35C74(Func358FCStruct *arg0);
void func_80033458_34058(void);
void func_8003365C_3425C(Func34574Arg *arg0);
void func_80034A30_35630(void);
void func_80034A94_35694(Func34574Arg *arg0);
void func_80035408_36008(void);

void func_800333E0_33FE0(Func33FE0Arg *arg0) {
    void *allocation;
    s32 i;

    getCurrentAllocation();
    allocation = dmaRequestAndUpdateStateWithSize(D_459310, D_459E00, 0x2278);
    setCleanupCallback(func_8003365C_3425C);

    for (i = 12; i >= 0; i--) {
        arg0->entries[i].unk4 = allocation;
    }

    setCallback(func_80033458_34058);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80033458_34058);

void func_8003365C_3425C(Func34574Arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80033688_34288);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80033800_34400);

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

void func_80033EDC_34ADC(Func34ADCArg *arg0) {
    u8 *allocation;
    u16 temp;

    allocation = getCurrentAllocation();

    if (arg0->unk1C == allocation[0xAC8]) {
        goto check_ac6;
    }
    if (arg0->unk1C != 3) {
        arg0->unk14 = 0x60;
        goto end;
    }
check_ac6:
    temp = *(u16 *)(allocation + 0xAC6);
    if (temp < 0x32) {
        arg0->unk14 = 0xFF;
        goto end;
    }
    arg0->unk14 = 0x60;

end:
    debugEnqueueCallback(arg0->unk1C + 9, 0, func_80035408_36008, &arg0->unk8);
}

void func_80033F50_34B50(Func34574Arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80033F7C_34B7C);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800340F4_34CF4);

void func_800343D0_34FD0(Func34574Arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800343FC_34FFC);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800344A8_350A8);

void func_80034640_35240(Func34574Arg *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_8003466C_3526C);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80034750_35350);

void func_8003498C_3558C(Func34574Arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800349B8_355B8(Func349B8Arg *arg0) {
    void *temp_s1 = dmaRequestAndUpdateStateWithSize(D_41A1D0, D_41AD80, 0x1B48);

    setCleanupCallback(&func_80034A94_35694);

    arg0->unk0 = -0x2C;
    arg0->unk2 = -0x14;
    arg0->unk8 = 0xD;
    arg0->unk4 = temp_s1;

    setCallback(&func_80034A30_35630);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80034A30_35630);

void func_80034A94_35694(Func34574Arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80034AC0_356C0);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80034BD8_357D8);

void func_80034CD0_358D0(Func34574Arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

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

void func_80035074_35C74(Func358FCStruct *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800350AC_35CAC);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_8003513C_35D3C);

void func_80035234_35E34(Func34574Arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035260_35E60);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035408_36008);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035548_36148);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800356AC_362AC);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035878_36478);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035DE0_369E0);
