#include "common.h"
#include "src/task_scheduler.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 pad4[0x20];
    void *unk24;
    void *unk28;
} B2030TaskPayload;

INCLUDE_ASM("asm/nonmatchings/B2030", func_800BB2B0_B2030);

INCLUDE_ASM("asm/nonmatchings/B2030", func_800BB3A0_B2120);

void func_800BB650_B23D0(B2030TaskPayload *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800BB6B4_B2434(void);

void func_800BB688_B2408(B2030TaskPayload *arg0) {
    arg0->unk0 = 0x78;
    arg0->unk2 = 0;
    setCallback(func_800BB6B4_B2434);
}

INCLUDE_ASM("asm/nonmatchings/B2030", func_800BB6B4_B2434);
