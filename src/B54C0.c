#include "56910.h"
#include "common.h"
#include "task_scheduler.h"

extern void func_800BB350_B5560(void *);

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_800BB420_B5630_arg;

void func_800BB420_B5630(func_800BB420_B5630_arg *arg0);

typedef struct {
    u8 _pad[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 _pad2[0xC];
    s16 unk3C;
    u8 _pad3[0x2];
    s32 unk40;
    s32 unk44;
    s32 unk48;
} func_800BB2B0_arg;

void func_800BB2B0_B54C0(func_800BB2B0_arg *arg0) {
    s32 pad[3];
    GameState *gameState = getCurrentAllocation();

    arg0->unk20 = (void *)((u32)func_80055E68_56A68(gameState->memoryPoolId) + 0xA0);
    arg0->unk24 = func_80055DC4_569C4(gameState->memoryPoolId);
    arg0->unk28 = func_80055DF8_569F8(gameState->memoryPoolId);
    arg0->unk14 = 0x03E90000;
    arg0->unk18 = 0x1D500000;
    arg0->unk1C = 0xF8460000;
    arg0->unk2C = 0;
    arg0->unk3C = 0;
    arg0->unk48 = 0;
    arg0->unk40 = 0;
    arg0->unk44 = 0xFE6A0000;
    setCleanupCallback(func_800BB420_B5630);
    setCallback(func_800BB350_B5560);
}

INCLUDE_ASM("asm/nonmatchings/B54C0", func_800BB350_B5560);

void func_800BB420_B5630(func_800BB420_B5630_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/B54C0", func_800BB458_B5668);

INCLUDE_ASM("asm/nonmatchings/B54C0", func_800BB5B0_B57C0);

void func_800BB7B8_B59C8(func_800BB420_B5630_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800BB828_B5A38(void *);

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} func_800BB7F0_B5A00_arg;

void func_800BB7F0_B5A00(func_800BB7F0_B5A00_arg *arg0) {
    arg0->unk2 = 0x50;
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk6 = 0x28;
    setCallback(func_800BB828_B5A38);
}

INCLUDE_ASM("asm/nonmatchings/B54C0", func_800BB828_B5A38);
