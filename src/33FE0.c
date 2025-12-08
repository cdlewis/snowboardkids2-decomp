#include "10AD0.h"
#include "36B80.h"
#include "56910.h"
#include "6E840.h"
#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

extern void func_80034D58_35958(void);
extern void func_8000FED0_10AD0(void);
extern void *getCurrentAllocation(void);
extern void func_80035878_36478(s16, s16, u16, u16, u16, u8, void *);
extern s16 D_8008F2C4_8FEC4[];

USE_ASSET(_459310);
USE_ASSET(_41A1D0);

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
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
} Func350ACArg;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u16 *unk4;
    /* 0x08 */ void *unk8;
    /* 0x0C */ u16 unkC;
    /* 0x0E */ u16 unkE;
    /* 0x10 */ u8 unk10;
} Func80035408Arg;

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

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u8 pad4[0x4];
    /* 0x08 */ void *unk8;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 pad11[0x3];
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 pad18[0x4];
    /* 0x1C */ void *unk1C;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ u8 unk24;
} Func343FCArg;

typedef struct {
    /* 0x000 */ u8 pad0[0xABE];
    /* 0xABE */ u16 unkABE;
    /* 0xAC0 */ u16 unkAC0;
    /* 0xAC2 */ u8 padAC2[0x2];
    /* 0xAC4 */ u16 unkAC4;
    /* 0xAC6 */ u16 unkAC6;
    /* 0xAC8 */ u8 padAC8[0xE];
    /* 0xAD6 */ u8 unkAD6;
} AllocationStruct;

void func_80035074_35C74(Func358FCStruct *arg0);
void func_80033458_34058(void);
void func_8003365C_3425C(Func34574Arg *arg0);
void func_80034A30_35630(void *arg0);
void func_80034A94_35694(Func34574Arg *arg0);
void func_80035408_36008(Func80035408Arg *arg0);
void func_8003513C_35D3C(Func350ACArg *arg0);
void func_80035234_35E34(Func34574Arg *arg0);
void func_80034640_35240(Func34574Arg *arg0);
void func_800344A8_350A8(void *);

void func_800333E0_33FE0(Func33FE0Arg *arg0) {
    void *allocation;
    s32 i;

    getCurrentAllocation();
    allocation = dmaRequestAndUpdateStateWithSize(&_459310_ROM_START, &_459310_ROM_END, 0x2278);
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

void func_800343FC_34FFC(Func343FCArg *arg0) {
    AllocationStruct *allocation;
    void *temp_s1;
    s16 temp_abe;
    s16 temp_ac0;

    allocation = getCurrentAllocation();
    temp_s1 = func_80035F80_36B80(1);
    setCleanupCallback(func_80034640_35240);

    arg0->unk0 = -0x78;
    arg0->unk2 = -0x60;
    arg0->unk8 = temp_s1;
    arg0->unkC = 0xFF;
    arg0->unk10 = 0;
    arg0->unkE = 0xFF;
    temp_abe = allocation->unkABE + 0xA;
    arg0->unk14 = temp_abe;
    temp_ac0 = allocation->unkAC0 + 0xA;
    arg0->unk1C = temp_s1;
    arg0->unk20 = 0xFF;
    arg0->unk24 = 4;
    arg0->unk22 = 0xFF;
    arg0->unk16 = temp_ac0;

    setCallback(func_800344A8_350A8);
}

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
    void *temp_s1 = dmaRequestAndUpdateStateWithSize(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);

    setCleanupCallback(&func_80034A94_35694);

    arg0->unk0 = -0x2C;
    arg0->unk2 = -0x14;
    arg0->unk8 = 0xD;
    arg0->unk4 = temp_s1;

    setCallback(&func_80034A30_35630);
}

void func_80034A30_35630(void *arg0) {
    u8 *allocation;
    u16 val;

    allocation = getCurrentAllocation();
    val = *(u16 *)(allocation + 0xAC6);

    if (val == 3 || val == 0x3C || val == 0x17) {
        debugEnqueueCallback(8, 7, func_8000FED0_10AD0, arg0);
    }
}

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

void func_800350AC_35CAC(Func350ACArg *arg0) {
    void *temp_s1;

    temp_s1 = dmaRequestAndUpdateStateWithSize(&_459310_ROM_START, &_459310_ROM_END, 0x2278);
    setCleanupCallback(func_80035234_35E34);

    arg0->unk0 = -0x52;
    arg0->unk2 = -0x14;
    arg0->unk8 = 0xA;
    arg0->unk10 = 0;
    arg0->unk11 = 0;
    arg0->unk4 = temp_s1;
    arg0->unkA = 0xFF;
    arg0->unkD = 0;
    arg0->unkC = 0;

    setCallback(func_8003513C_35D3C);
}

void func_8003513C_35D3C(Func350ACArg *arg0) {
    AllocationStruct *allocation;
    u32 temp;

    allocation = getCurrentAllocation();

    temp = allocation->unkAC6;
    if ((u32)(temp - 0x33) < 2U) {
        arg0->unk2 = (allocation->unkAD6 * 3 * 8) - 0x14;

        if (allocation->unkAC6 == 0x34) {
            if (allocation->unkAC4 & 1) {
                arg0->unkD = 0xFF;
            } else {
                arg0->unkD = 0;
            }
        } else {
            arg0->unkD = 0;
        }

        debugEnqueueCallback(8, 1, func_80012004_12C04, arg0);

        if (allocation->unkAC6 == 0x33) {
            arg0->unk11++;
            arg0->unk11 &= 3;
            if (arg0->unk11 == 0) {
                arg0->unk10++;
                arg0->unk10 &= 3;
                arg0->unk8 = D_8008F2C4_8FEC4[arg0->unk10];
            }
        } else {
            arg0->unk11 = 0;
            arg0->unk10 = 0;
            arg0->unk8 = 0xA;
        }
    } else {
        arg0->unk11 = 0;
        arg0->unk10 = 0;
        arg0->unk8 = 0xA;
    }
}

void func_80035234_35E34(Func34574Arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035260_35E60);

#ifdef NON_MATCHING
void func_80035408_36008(Func80035408Arg *arg0) {
    u16 *ptr;
    s16 x;
    s16 y;
    u8 a;
    u16 cmd;
    s16 sz;
    s16 unused;

    ptr = arg0->unk4;
    x = arg0->unk0;
    y = arg0->unk2;
    cmd = *ptr;
    a = arg0->unk10;
    unused = 0xFFFE;

    if ((cmd & 0xFFFF) != 0xFFFF) {
        do {
            if ((cmd & 0xFFFF) == 0xFFFD) {
                x = arg0->unk0;
                y += 0x10;
            } else if ((cmd & 0xFFFF) == 0xFFFE || (cmd & 0xFFFF) == 0xFFFB) {
                x += 4;
            } else if ((cmd & 0xFFFF) == 0xFFFC) {
                ptr++;
                if (arg0->unk10 == 0) {
                    a = *ptr;
                    ptr++;
                }
            } else if ((cmd & 0xFFFF) == 0xFFF0) {
                ptr += 3;
            } else if ((cmd & 0xFFFF) != 0xFFF1) {
                sz = cmd >> 12;
                if (sz == 0) {
                    sz = 12;
                }
                func_80035878_36478(x, y, cmd & 0xFFF, arg0->unkC, arg0->unkE, a, arg0->unk8);
                x += sz;
            }
            ptr++;
            cmd = *ptr;
        } while ((cmd & 0xFFFF) != 0xFFFF);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035408_36008);
#endif

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035548_36148);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_800356AC_362AC);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035878_36478);

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80035DE0_369E0);
