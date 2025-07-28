#include "1A770.h"

#include "20F0.h"
#include "6E840.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "task_scheduler.h"

typedef struct {
    u8 padding[0x1D8];
    s16 unk1D8;
} func_8001AE80_1BA80_task_memory;

s32 func_80069810_6A410();
extern void func_8001A0F4_1ACF4;
void func_80027CA0_288A0(void *, s32, s32, s32);
extern void func_80019CD0_1A8D0;
extern void func_8001AF18_1BB18;
extern void func_80019BBC_1A7BC;
extern void func_80019CB4_1A8B4;

void func_80019B70_1A770(func_80019B70_1A770_arg *arg0) {
    arg0->unk20 = func_8000198C_258C(0x13, getCurrentAllocation());
    setCleanupCallback(&func_80019CB4_1A8B4);
    setCallback(&func_80019BBC_1A7BC);
}

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019BBC_1A7BC);

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019BF0_1A7F0);

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019C7C_1A87C);

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019CB4_1A8B4);

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019CD0_1A8D0);

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019D24_1A924);

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019D50_1A950);

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019D84_1A984);

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019DC4_1A9C4);

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019E18_1AA18);

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019E60_1AA60);

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019EA8_1AAA8);

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019EFC_1AAFC);

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019F30_1AB30);

INCLUDE_ASM("asm/nonmatchings/1A770", func_80019F60_1AB60);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001A070_1AC70);

void func_8001A0B4_1ACB4(void) {
    if ((func_80069810_6A410() << 0x10) != 0) {
        D_800AFE8C_A71FC->unk23 = 0;
        terminateSchedulerWithCallback(&func_8001A0F4_1ACF4);
    }
}

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001A0F4_1ACF4);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001A110_1AD10);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001A3D8_1AFD8);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001A434_1B034);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001A478_1B078);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001AC30_1B830);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001AC70_1B870);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001ACC8_1B8C8);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001AD80_1B980);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001AD9C_1B99C);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001ADFC_1B9FC);

void func_8001AE80_1BA80(void) {
    func_8001AE80_1BA80_task_memory *temp_s0 = (func_8001AE80_1BA80_task_memory *)allocateTaskMemory(0x1E0);
    setupTaskSchedulerNodes(0x14, 0, 0, 0, 0, 0, 0, 0);
    temp_s0->unk1D8 = 0;
    func_80027CA0_288A0(temp_s0, 0, 0xA, 0);
    func_8006FDA0_709A0(0, 0, 8);
    scheduleTask(&func_80019CD0_1A8D0, 0U, 0U, 0x5AU);
    setGameStateHandler(&func_8001AF18_1BB18);
}

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001AF18_1BB18);

INCLUDE_ASM("asm/nonmatchings/1A770", func_8001AF80_1BB80);
