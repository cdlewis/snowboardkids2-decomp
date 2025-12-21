#include "5520.h"
#include "common.h"
#include "geometry.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_2151D0);
USE_ASSET(_663BE0);
USE_ASSET(_6637B0);

extern func_80004FF8_5BF8_arg1_unk20 D_80089520;

typedef struct {
    u8 _pad0[0x24];
    void *unk24;
    void *unk28;
    u8 _pad2C[0x10];
    void *unk3C;
} func_800B5794_1E2844_arg;

void func_800B5590_1E2640(func_800B5500_Task *arg0);
void func_800B5660_1E2710(func_800B5500_Task *arg0);
void func_800B5794_1E2844(func_800B5794_1E2844_arg *arg0);

void func_800B5500_1E25B0(u16 arg0, void *arg1, s16 arg2, s32 arg3) {
    func_800B5500_Task *sp10;

    sp10 = (func_800B5500_Task *)scheduleTask(&func_800B5590_1E2640, 1, 0, 0);

    if (sp10 != NULL) {
        sp10->unk74 = arg0;
        memcpy(sp10->unk54, arg1, 0x20);
        sp10->unk76 = arg2;
        sp10->unk78_union.unk78 = 0;
        sp10->unk7C = (s32)arg3;
        sp10->unk84 = 0;
    }
}

void func_800B5590_1E2640(func_800B5500_Task *arg0) {
    setCleanupCallback(func_800B5794_1E2844);
    arg0->unk24 = dmaRequestAndUpdateState(&_2151D0_ROM_START, &_2151D0_ROM_END);
    arg0->unk28 = dmaRequestAndUpdateStateWithSize(&_663BE0_ROM_START, &_663BE0_ROM_END, 0x200);
    arg0->unk20 = &D_80089520;
    arg0->unk50 = 1;
    arg0->unk80 = 0x800000;
    arg0->unk2C = 0;
    arg0->unk3B = 0x80;
    arg0->unk3C = dmaRequestAndUpdateStateWithSize(&_6637B0_ROM_START, &_6637B0_ROM_END, 0x858);
    arg0->unk44_union.unk44 = 0;
    arg0->unk46_union.unk46 = 0;
    arg0->unk48 = 0;
    arg0->unk4A = 0x10;
    arg0->unk4C = 0;
    arg0->unk4E = 0;
    setCallback(func_800B5660_1E2710);
}

void func_800B5660_1E2710(func_800B5500_Task *arg0) {

    switch (arg0->unk84) {
        case 0:
            arg0->unk78_union.unk78 += 0x222;
            if (arg0->unk7C < arg0->unk78_union.unk78) {
                arg0->unk78_union.unk78 = arg0->unk7C;
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

    arg0->unk44_union.unk44 = arg0->unk44_union.unk44 + arg0->unk48;
    arg0->unk46_union.unk46 = arg0->unk46_union.unk46 + arg0->unk4A;
    arg0->unk44_union.unk44 = arg0->unk44_union.unk44_bytes[1];
    arg0->unk46_union.unk46 = arg0->unk46_union.unk46_bytes[1];

    memcpy(arg0, arg0->unk54, 0x20);

    scaleMatrix((Mat3x3Padded *)arg0, 0x2000, arg0->unk78_union.unk78_shorts[1], 0x2000);

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
