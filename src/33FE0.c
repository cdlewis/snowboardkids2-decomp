#include "33FE0.h"
#include "10AD0.h"
#include "36B80.h"
#include "3E160.h"
#include "56910.h"
#include "6E840.h"
#include "B040.h"
#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_459310);
USE_ASSET(_41A1D0);
USE_ASSET(_3F6670);
USE_ASSET(_4547D0);
USE_ASSET(_459310);

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
    /* 0x04 */ void *unk4;
    /* 0x08 */ void *unk8;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 pad11[0x3];
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ void *unk18;
    /* 0x1C */ void *unk1C;
    /* 0x20 */ s16 unk20;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ u8 unk24;
} Func343FCArg;

typedef struct {
    /* 0x000 */ u8 pad0[0xABC];
    /* 0xABC */ u16 unkABC;
    /* 0xABE */ u16 unkABE;
    /* 0xAC0 */ u16 unkAC0;
    /* 0xAC2 */ u16 unkAC2;
    /* 0xAC4 */ u16 unkAC4;
    /* 0xAC6 */ u16 unkAC6;
    /* 0xAC8 */ u8 unkAC8;
    /* 0xAC9 */ u8 unkAC9;
    /* 0xACA */ u8 unkACA;
    /* 0xACB */ u8 unkACB;
    /* 0xACC */ u8 padACC[0xA];
    /* 0xAD6 */ u8 unkAD6;
} AllocationStruct;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 padB;
} Func356C0Entry; // size 0x0C

typedef struct {
    /* 0x00 */ Func356C0Entry entries[4]; // 0x00 - 0x2F
    /* 0x30 */ u8 pad30[0x18];
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 unk49;
} Func356C0Arg;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u8 pad4[0xC];
} Func33800Entry;

typedef struct {
    /* 0x00 */ Func33800Entry *unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8 pad8[4];
    /* 0x0C */ u16 unkC;
} Func33800Arg;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
} Func3466CElement14;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ void *unk8;
} Func3466CElement0C;

typedef struct {
    /* 0x00 */ u8 data[0xA];
} Func3466CElement0A;

typedef struct {
    /* 0x00 */ Func3466CElement14 arr00[4]; // 4 * 0x14 = 0x50
    /* 0x50 */ Func3466CElement0C arr50[4]; // 4 * 0x0C = 0x30
    /* 0x80 */ Func3466CElement0A arr80[4]; // 4 * 0x0A = 0x28
    /* 0xA8 */ u8 arrA8[4];
} Func3466CArg;

typedef struct {
    /* 0x000 */ u8 pad0[0xABC];
    /* 0xABC */ s16 unkABC;
} Func34BD8AllocationStruct;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 padE[2];
} Func345ACEntry; // size 0x10

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ Func345ACEntry *unk4;
    /* 0x08 */ u8 pad8[0x14];
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
} Func345ACArg;

typedef struct {
    /* 0x000 */ u8 data[0xAD2]; // Everything including unkACE[4]
} Func345ACAllocationFlat;

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ u8 pad4[0x4];
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ void *unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 pad19[0x3];
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 unk1E;
} Func34A40Arg;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ void *unk4;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 padB;
} Func34BD8Entry;

typedef struct {
    /* 0x00 */ Func34BD8Entry entries[4];
    /* 0x30 */ u8 pad30[0x18];
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 unk49;
} Func34BD8Arg;

extern u16 D_8008F2B8_8FEB8;
extern u16 D_8008F2AC_8FEAC[];
extern s16 D_8008F2C4_8FEC4[];
extern void *D_8008F7CC_903CC[];
extern void *D_8008F7DC_903DC;
extern void *D_8008F7E0_903E0;
extern s32 D_8008F7D8_903D8;
extern u16 D_8009ADE0_9B9E0;

void func_80034D58_35958(void);
void func_80035878_36478(s16, s16, u16, u16, u16, u8, void *);
void func_80033EDC_34ADC(Func34ADCArg *arg0);
void func_80033F50_34B50(Func34574Arg *arg0);
void func_8003498C_3558C(Func34574Arg *arg0);
void func_80034750_35350(void);
void func_80034BD8_357D8(Func34BD8Arg *arg0);
void func_80034CD0_358D0(Func34574Arg *arg0);
void func_80033AE4_346E4(void);
void func_80035074_35C74(Func358FCStruct *arg0);
void func_80033458_34058(void);
void func_8003365C_3425C(Func34574Arg *arg0);
void func_80034A30_35630(void *arg0);
void func_80034A94_35694(Func34574Arg *arg0);
void func_8003513C_35D3C(Func350ACArg *arg0);
void func_80035234_35E34(Func34574Arg *arg0);
void func_80034640_35240(Func34574Arg *arg0);
void func_800344A8_350A8(Func343FCArg *arg0);
void func_80033E08_34A08(Func34574Arg *arg0);

void func_800333E0_33FE0(Func33FE0Arg *arg0) {
    void *allocation;
    s32 i;

    getCurrentAllocation();
    allocation = loadCompressedData(&_459310_ROM_START, &_459310_ROM_END, 0x2278);
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

void func_80033800_34400(Func33800Arg *arg0) {
    AllocationStruct *allocation;
    s32 row;
    s32 outer_cnt;
    s32 idx;
    s32 col;
    s32 inner_cnt;
    s32 saved_idx;
    s32 entry_idx;
    u16 temp;
    u16 temp2;

    allocation = getCurrentAllocation();

    row = 0;
    outer_cnt = 0;
    idx = 0;

    do {
        for (col = 0, inner_cnt = 0, saved_idx = idx; inner_cnt < 0xB; inner_cnt++, col += 0x10) {
            entry_idx = saved_idx + inner_cnt;
            arg0->unk0[entry_idx].unk0 = allocation->unkABE + col;
            arg0->unk0[entry_idx].unk2 = allocation->unkAC0 + row;
            debugEnqueueCallback(8U, 0U, func_80012004_12C04, &arg0->unk0[entry_idx]);
        }

        row += 0x10;
        outer_cnt += 1;
        idx += 0xB;
    } while (outer_cnt < 5);

    temp = allocation->unkAC6;
    if ((temp == 8) | (temp == 0xB)) {
        arg0->unk6 = allocation->unkAC2 + 0x38;
    } else {
        arg0->unk6 = allocation->unkAC0 + 0x38;
    }

    if (!(D_8009ADE0_9B9E0 & 7)) {
        temp2 = arg0->unkC + 1;
        arg0->unkC = temp2;
        if ((u32)(temp2 & 0xFFFF) >= 0x15U) {
            arg0->unkC = 0x13U;
        }
    }

    debugEnqueueCallback(8U, 1U, func_8000FED0_10AD0, &arg0->unk4);
}

void func_80033974_34574(Func34574Arg *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void func_800339AC_345AC(Func345ACArg *arg0) {
    u8 *allocation;
    s32 i;
    s16 unk2_val;
    u8 *new_var;

    allocation = (u8 *)getCurrentAllocation();
    arg0->unk4 = (Func345ACEntry *)allocateNodeMemory(0xF0);
    arg0->unk0 = loadCompressedData(&_459310_ROM_START, &_459310_ROM_END, 0x2278);
    arg0->unk1E = *(allocation + arg0->unk1C + 0xACE);
    setCleanupCallback(func_80033E08_34A08);

    for (i = 0; i < 15; i++) {
        unk2_val = ((-((*((new_var = (allocation + (arg0->unk1C * 0x5C)) + i) + 0x948)) != 0)) & (-8)) | 0xFF70;
        arg0->unk4[i].unk0 = -0x78 + i * 0x10;
        arg0->unk4[i].unk2 = unk2_val;
        arg0->unk4[i].unk4 = arg0->unk0;
        arg0->unk4[i].unk8 = 5;
        arg0->unk4[i].unkA = 0xFF;
        arg0->unk4[i].unkD = 0;
        arg0->unk4[i].unkC = 0;
    }

    arg0->unk1D = 0;
    setCallback(func_80033AE4_346E4);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80033AE4_346E4);

void func_80033E08_34A08(Func34574Arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80033E40_34A40(Func34A40Arg *arg0) {
    u8 *allocation;
    s32 temp_c;
    void *temp_10;

    allocation = (u8 *)getCurrentAllocation();
    arg0->unk0 = func_80035F80_36B80(1);
    arg0->unk1E = *(allocation + arg0->unk1C + 0xACE);
    setCleanupCallback(func_80033F50_34B50);

    *(volatile s16 *)&arg0->unk8 = -0x26;
    *(volatile s16 *)&arg0->unkA = -4;

    temp_c = D_8008F7D8_903D8;
    temp_10 = arg0->unk0;

    arg0->unk14 = 0xFF;
    arg0->unk18 = 0;
    arg0->unk16 = 0xFF;
    arg0->unk1D = 0;

    arg0->unkC = temp_c;
    arg0->unk10 = temp_10;

    setCallback(func_80033EDC_34ADC);
}

void func_80033EDC_34ADC(Func34ADCArg *arg0) {
    AllocationStruct *allocation;
    u16 temp;

    allocation = getCurrentAllocation();

    if (arg0->unk1C == allocation->unkAC8) {
        goto check_ac6;
    }
    if (arg0->unk1C != 3) {
        arg0->unk14 = 0x60;
        goto end;
    }
check_ac6:
    temp = allocation->unkAC6;
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

void func_800344A8_350A8(Func343FCArg *arg0) {
    AllocationStruct *alloc;
    void *temp_a0;
    s16 temp_a2;
    u16 temp_v1;
    u16 temp_v0;
    s16 var_v0;

    alloc = getCurrentAllocation();

    if ((alloc->unkAC6 < 0x3D) || (alloc->unkAC9 != 0)) {
        temp_a0 = D_8008F7CC_903CC[alloc->unkACB];
        arg0->unk4 = temp_a0;
        temp_a2 = ((0x120 - getMaxLinePixelWidth(temp_a0)) / 2) - 0x90;
        arg0->unk0 = temp_a2;
        func_80035260_35E60(arg0->unk8, arg0->unk4, temp_a2, arg0->unk2, arg0->unkC, arg0->unkE, arg0->unk10, 8, 0);

        temp_v1 = alloc->unkAC6;
        if (temp_v1 >= 6) {
            if (temp_v1 >= 0xA) {
                arg0->unk18 = D_8008F7E0_903E0;
            } else {
                arg0->unk18 = D_8008F7DC_903DC;
            }

            temp_v0 = alloc->unkAC6;
            if (temp_v0 == 8 || temp_v0 == 0xB) {
                var_v0 = alloc->unkAC2 + 0xA;
            } else {
                var_v0 = alloc->unkAC0 + 0xA;
            }
            arg0->unk16 = var_v0;

            if ((u32)(alloc->unkAC6 - 9) < 2U) {
                if (alloc->unkAC4 & 1) {
                    arg0->unk24 = 0xFF;
                } else {
                    arg0->unk24 = 4;
                }
            } else {
                arg0->unk24 = 4;
            }

            func_80035260_35E60(
                arg0->unk1C,
                arg0->unk18,
                arg0->unk14,
                arg0->unk16,
                arg0->unk20,
                arg0->unk22,
                arg0->unk24,
                8,
                0
            );
        }
    }
}

void func_80034640_35240(Func34574Arg *arg0) {
    arg0->unk8 = freeNodeMemory(arg0->unk8);
}

void func_8003466C_3526C(Func3466CArg *arg0) {
    void *temp_s1;
    s32 i;

    temp_s1 = loadCompressedData(&_3F6670_ROM_START, &_3F6670_ROM_END, 0x388);
    setCleanupCallback(func_8003498C_3558C);

    for (i = 0; i < 4; i++) {
        arg0->arr50[i].unk0 = 0x18;
        arg0->arr50[i].unk2 = 0xC;
        arg0->arr50[i].unk4 = 0;
        arg0->arr50[i].unk6 = 0xFF;
        arg0->arr50[i].unk8 = &arg0->arr80[i];

        arg0->arr00[i].unk0 = 0x5C;
        arg0->arr00[i].unk2 = 0x10;
        arg0->arr00[i].unk4 = temp_s1;
        arg0->arr00[i].unk8 = 0;
        arg0->arr00[i].unkA = 0x555;
        arg0->arr00[i].unkC = 0x555;
        arg0->arr00[i].unkE = 0;
        arg0->arr00[i].unk10 = 0xFF;
        arg0->arr00[i].unk13 = 0;
        arg0->arr00[i].unk12 = 0;

        arg0->arrA8[i] = 0;
    }

    setCallback(func_80034750_35350);
}

INCLUDE_ASM("asm/nonmatchings/33FE0", func_80034750_35350);

void func_8003498C_3558C(Func34574Arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_800349B8_355B8(Func349B8Arg *arg0) {
    void *temp_s1 = loadCompressedData(&_41A1D0_ROM_START, &_41A1D0_ROM_END, 0x1B48);

    setCleanupCallback(&func_80034A94_35694);

    arg0->unk0 = -0x2C;
    arg0->unk2 = -0x14;
    arg0->unk8 = 0xD;
    arg0->unk4 = temp_s1;

    setCallback(&func_80034A30_35630);
}

void func_80034A30_35630(void *arg0) {
    AllocationStruct *allocation;
    u16 val;

    allocation = getCurrentAllocation();
    val = allocation->unkAC6;

    if (val == 3 || val == 0x3C || val == 0x17) {
        debugEnqueueCallback(8, 7, func_8000FED0_10AD0, arg0);
    }
}

void func_80034A94_35694(Func34574Arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80034AC0_356C0(Func356C0Arg *arg0) {
    AllocationStruct *allocation;
    void *asset;
    s32 i;
    s16 temp_a0;
    s16 xOffset;
    s16 yOffset;

    allocation = getCurrentAllocation();
    asset = loadCompressedData(&_4547D0_ROM_START, &_4547D0_ROM_END, 0x9488);
    setCleanupCallback(func_80034CD0_358D0);

    arg0->unk49 = 0;

    temp_a0 = allocation->unkABC - 6;
    xOffset = temp_a0;
    yOffset = (allocation->unkAC8 * 0x38) - 0x3C;

    if (arg0->unk48 == 0) {
        xOffset = -(s16)((temp_a0 << 16) >> 16);
    }

    for (i = 0; i < 4; i++) {
        arg0->entries[i].unk0 = xOffset + D_8008F2AC_8FEAC[i];
        arg0->entries[i].unk2 = yOffset + (&D_8008F2B8_8FEB8)[i];
        arg0->entries[i].unk8 = (i % 8) + 0x10;
        arg0->entries[i].unkA = 0x12;
        arg0->entries[i].unk4 = asset;
    }

    setCallback(func_80034BD8_357D8);
}

void func_80034BD8_357D8(Func34BD8Arg *arg0) {
    Func34BD8AllocationStruct *allocation;
    unsigned int new_var;
    s16 baseVal;
    s32 i;

    allocation = (Func34BD8AllocationStruct *)getCurrentAllocation();

    if (allocation->unkABC != 0) {
        baseVal = allocation->unkABC;
        if (arg0->unk48 == 0) {
            new_var = baseVal;
            baseVal = -new_var;
        }

        arg0->unk49 = (u8)((arg0->unk49 + 1) & 1);

        for (i = 0; i < 4; i++) {
            arg0->entries[i].unk0 = baseVal + D_8008F2AC_8FEAC[i];

            if (arg0->unk49 == 0) {
                arg0->entries[i].unk8++;
                if (arg0->entries[i].unk8 > 0x17) {
                    arg0->entries[i].unk8 = 0x10;
                }
            }

            debugEnqueueCallback(8, 0, func_80010240_10E40, (void *)&arg0->entries[i]);
        }
    } else {
        func_80069CF8_6A8F8();
    }
}

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

    temp_s1 = loadCompressedData(&_459310_ROM_START, &_459310_ROM_END, 0x2278);
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
