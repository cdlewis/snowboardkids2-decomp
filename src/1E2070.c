#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_215120);
USE_ASSET(_2151D0);
USE_ASSET(_6636F0);
USE_ASSET(_6637B0);
USE_ASSET(_663BE0);

extern s32 D_80089510_8A110;
extern s32 D_80089520;
extern void func_800B5438_1E24E8(void);
extern void func_800B54B4_1E2564(void);

typedef struct {
    u8 padding[0x20];
    s32 unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 padding2[0xC];
    void *unk3C;
    s32 unk40;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    s32 unk54;
    s16 unk58;
} func_800B5318_1E23C8_arg;

INCLUDE_ASM("asm/nonmatchings/1E2070", func_800B4FC0_1E2070);

INCLUDE_ASM("asm/nonmatchings/1E2070", func_800B50E4_1E2194);

INCLUDE_ASM("asm/nonmatchings/1E2070", func_800B5210_1E22C0);

INCLUDE_ASM("asm/nonmatchings/1E2070", func_800B523C_1E22EC);

INCLUDE_ASM("asm/nonmatchings/1E2070", func_800B5290_1E2340);

void func_800B5318_1E23C8(func_800B5318_1E23C8_arg *arg0) {
    setCleanupCallback((void (*)(void *))&func_800B54B4_1E2564);

    if (arg0->unk58 == 0) {
        arg0->unk24 = dmaRequestAndUpdateState(&_215120_ROM_START, &_215120_ROM_END);
        arg0->unk28 = dmaRequestAndUpdateStateWithSize(&_6636F0_ROM_START, &_6636F0_ROM_END, 0x200);
        arg0->unk2C = 0;
        arg0->unk20 = (s32)&D_80089510_8A110;
        arg0->unk50 = 0;
    } else if (arg0->unk58 != 1) {
        arg0->unk24 = dmaRequestAndUpdateState(&_215120_ROM_START, &_215120_ROM_END);
        arg0->unk28 = dmaRequestAndUpdateStateWithSize(&_6636F0_ROM_START, &_6636F0_ROM_END, 0x200);
        arg0->unk2C = 0;
        arg0->unk20 = (s32)&D_80089510_8A110;
        arg0->unk50 = 0;
    } else {
        arg0->unk24 = dmaRequestAndUpdateState(&_2151D0_ROM_START, &_2151D0_ROM_END);
        arg0->unk28 = dmaRequestAndUpdateStateWithSize(&_663BE0_ROM_START, &_663BE0_ROM_END, 0x200);
        arg0->unk20 = (s32)&D_80089520;
        arg0->unk2C = 0;
        arg0->unk50 = 1;
    }

    arg0->unk3C = dmaRequestAndUpdateStateWithSize(&_6637B0_ROM_START, &_6637B0_ROM_END, 0x858);
    arg0->unk44 = 0;
    arg0->unk46 = 0;
    arg0->unk48 = 0;
    arg0->unk4A = 0x10;
    arg0->unk4C = 0;
    arg0->unk4E = 0;

    setCallback((void (*)(void *))&func_800B5438_1E24E8);
}

INCLUDE_ASM("asm/nonmatchings/1E2070", func_800B5438_1E24E8);

INCLUDE_ASM("asm/nonmatchings/1E2070", func_800B54B4_1E2564);
