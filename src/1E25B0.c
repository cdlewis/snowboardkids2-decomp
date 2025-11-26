#include "common.h"
#include "geometry.h"
#include "task_scheduler.h"

extern void func_8000576C_636C(s32 arg0, void *arg1);

typedef struct {
    u8 _pad0[0x24];
    void *unk24;
    void *unk28;
    u8 _pad2C[0x10];
    void *unk3C;
} func_800B5794_1E2844_arg;

typedef struct {
    /* 0x00 */ u8 _pad0[0x20];
    /* 0x20 */ struct {
        s32 unk0;
        s32 unk4;
        s32 unk8;
        s32 unkC;
    } *unk20;
    /* 0x24 */ u8 _pad24[0x10];
    /* 0x34 */ s32 unk30;
    /* 0x38 */ u8 _pad38[3];
    /* 0x3B */ s8 unk3B;
    /* 0x3C */ u8 _pad3C[8];
    /* 0x44 */ u16 unk44;
    /* 0x46 */ u16 unk46;
    /* 0x48 */ u16 unk48;
    /* 0x4A */ u16 unk4A;
    /* 0x4C */ u8 _pad4C[8];
    /* 0x54 */ u8 unk54[0x20];
    /* 0x74 */ u16 unk74;
    /* 0x76 */ s16 unk76;
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ s8 unk84;
} func_800B5500_Task;

void func_800B5590_1E2640(void);

void *func_800B5500_1E25B0(s16 arg0, void *arg1, s16 arg2, void *arg3) {
    func_800B5500_Task *sp10;

    sp10 = (func_800B5500_Task *)scheduleTask(&func_800B5590_1E2640, 1, 0, 0);

    if (sp10 != NULL) {
        sp10->unk74 = arg0;
        memcpy(sp10->unk54, arg1, 0x20);
        sp10->unk76 = arg2;
        sp10->unk78 = 0;
        sp10->unk7C = (s32)arg3;
        sp10->unk84 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/1E25B0", func_800B5590_1E2640);

void func_800B5660_1E2710(func_800B5500_Task *arg0) {

    switch (arg0->unk84) {
        case 0:
            arg0->unk78 += 0x222;
            if (arg0->unk7C < arg0->unk78) {
                arg0->unk78 = arg0->unk7C;
                arg0->unk84 = 1;
            }
            break;
        case 1:
            if (arg0->unk76 == 0) {
                arg0->unk84 = 2;
            }
            break;
        case 2:
            arg0->unk80 += 0xFFFA0000;
            arg0->unk3B = arg0->unk80 >> 16;
            if (arg0->unk80 < 0) {
                arg0->unk80 = 0;
                func_80069CF8_6A8F8();
                return;
            }
            break;
    }

    arg0->unk44 = arg0->unk44 + arg0->unk48;
    arg0->unk46 = arg0->unk46 + arg0->unk4A;
    arg0->unk44 = *((u8 *)&arg0->unk44 + 1);
    arg0->unk46 = *((u8 *)&arg0->unk46 + 1);

    memcpy(arg0, arg0->unk54, 0x20);

    scaleMatrix((Mat3x3Padded *)arg0, 0x2000, *(s16 *)((u8 *)&arg0->unk78 + 2), 0x2000);

    func_8000576C_636C(arg0->unk74, arg0);

    if (arg0->unk76 > 0) {
        arg0->unk76 = arg0->unk76 - 1;
    }
}

void func_800B5794_1E2844(func_800B5794_1E2844_arg *arg0) {
    arg0->unk3C = freeNodeMemory(arg0->unk3C);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk24 = freeNodeMemory(arg0->unk24);
}
