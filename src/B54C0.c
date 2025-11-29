#include "common.h"
#include "task_scheduler.h"

INCLUDE_ASM("asm/nonmatchings/B54C0", func_800BB2B0_B54C0);

INCLUDE_ASM("asm/nonmatchings/B54C0", func_800BB350_B5560);

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_800BB420_B5630_arg;

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
