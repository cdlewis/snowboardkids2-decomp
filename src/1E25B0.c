#include "common.h"
#include "task_scheduler.h"

typedef struct {
    u8 _pad0[0x24];
    void *unk24;
    void *unk28;
    u8 _pad2C[0x10];
    void *unk3C;
} func_800B5794_1E2844_arg;

INCLUDE_ASM("asm/nonmatchings/1E25B0", func_800B5500_1E25B0);

INCLUDE_ASM("asm/nonmatchings/1E25B0", func_800B5590_1E2640);

INCLUDE_ASM("asm/nonmatchings/1E25B0", func_800B5660_1E2710);

void func_800B5794_1E2844(func_800B5794_1E2844_arg *arg0) {
    arg0->unk3C = freeNodeMemory(arg0->unk3C);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk24 = freeNodeMemory(arg0->unk24);
}
