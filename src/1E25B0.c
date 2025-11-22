#include "common.h"
#include "task_scheduler.h"

typedef struct {
    u8 _pad0[0x24];
    void *unk24;
    void *unk28;
    u8 _pad2C[0x10];
    void *unk3C;
} func_800B5794_1E2844_arg;

typedef struct {
    u8 _pad0[0x54];
    u8 unk54[0x20];
    s16 unk74;
    s16 unk76;
    void *unk78;
    void *unk7C;
    u8 _pad80[4];
    u8 unk84;
} func_800B5500_Task;

void func_800B5590_1E2640(void);

void *func_800B5500_1E25B0(s16 arg0, void *arg1, s16 arg2, void *arg3) {
    func_800B5500_Task *sp10;

    sp10 = (func_800B5500_Task *)scheduleTask(&func_800B5590_1E2640, 1, 0, 0);

    if (sp10 != NULL) {
        sp10->unk74 = arg0;
        memcpy(sp10->unk54, arg1, 0x20);
        sp10->unk76 = arg2;
        sp10->unk78 = NULL;
        sp10->unk7C = arg3;
        sp10->unk84 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/1E25B0", func_800B5590_1E2640);

INCLUDE_ASM("asm/nonmatchings/1E25B0", func_800B5660_1E2710);

void func_800B5794_1E2844(func_800B5794_1E2844_arg *arg0) {
    arg0->unk3C = freeNodeMemory(arg0->unk3C);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk24 = freeNodeMemory(arg0->unk24);
}
