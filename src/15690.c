#include "4050.h"
#include "D_800AFE8C_A71FC_type.h"
#include "common.h"
#include "task_scheduler.h"

extern void func_80015248_15E48(s32);
extern void func_80014D78_15978(void);
extern void func_80015064_15C64(void);
extern void func_80015218_15E18(void);
extern void func_80014C94_15894(void);
extern void func_800150DC_15CDC(void);

INCLUDE_ASM("asm/nonmatchings/15690", func_80014A90_15690);

INCLUDE_ASM("asm/nonmatchings/15690", func_80014AEC_156EC);

void func_80014B1C_1571C(void) {
    s16 result = func_80069810_6A410();

    if (result != 0) {
        if (result == 0xFE) {
            terminateSchedulerWithCallback(func_80015218_15E18);
        } else {
            setGameStateHandler(func_80014C94_15894);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/15690", func_80014B70_15770);

extern void func_80014C0C_1580C(void);

void func_80014BA0_157A0(void) {
    s16 result;

    getCurrentAllocation();
    result = func_80069810_6A410();

    if (result != 0) {
        if (result == 0x44 || result == 0xFF) {
            setGameStateHandler(func_80014C0C_1580C);
        } else {
            terminateSchedulerWithCallback(func_80015218_15E18);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/15690", func_80014C0C_1580C);

INCLUDE_ASM("asm/nonmatchings/15690", func_80014C3C_1583C);

INCLUDE_ASM("asm/nonmatchings/15690", func_80014C94_15894);

INCLUDE_ASM("asm/nonmatchings/15690", func_80014CC4_158C4);

INCLUDE_ASM("asm/nonmatchings/15690", func_80014CF8_158F8);

void func_80014D3C_1593C(void) {
    if ((func_80069810_6A410() << 16) != 0) {
        func_80015248_15E48(3);
        setGameStateHandler(func_80014D78_15978);
    }
}

INCLUDE_ASM("asm/nonmatchings/15690", func_80014D78_15978);

INCLUDE_ASM("asm/nonmatchings/15690", func_80014DA8_159A8);

INCLUDE_ASM("asm/nonmatchings/15690", func_80014F60_15B60);

INCLUDE_ASM("asm/nonmatchings/15690", func_80014FA4_15BA4);

void func_80015028_15C28(void) {
    func_80003450_4050(0xA, 2);
    createTaskQueue(&loadCutsceneOverlay, 0x64);
    setGameStateHandler(func_80015064_15C64);
}

INCLUDE_ASM("asm/nonmatchings/15690", func_80015064_15C64);

void func_80015098_15C98(void) {
    func_80003450_4050(D_800AFE8C_A71FC->errorFlag, 2);
    createTaskQueue(&loadCutsceneOverlay, 0x64);
    setGameStateHandler(func_800150DC_15CDC);
}

INCLUDE_ASM("asm/nonmatchings/15690", func_800150DC_15CDC);

INCLUDE_ASM("asm/nonmatchings/15690", func_8001511C_15D1C);

INCLUDE_ASM("asm/nonmatchings/15690", func_8001514C_15D4C);

INCLUDE_ASM("asm/nonmatchings/15690", func_800151A4_15DA4);

INCLUDE_ASM("asm/nonmatchings/15690", func_800151E4_15DE4);

INCLUDE_ASM("asm/nonmatchings/15690", func_80015218_15E18);

INCLUDE_ASM("asm/nonmatchings/15690", func_8001523C_15E3C);

INCLUDE_ASM("asm/nonmatchings/15690", func_80015248_15E48);

INCLUDE_ASM("asm/nonmatchings/15690", func_80015254_15E54);

INCLUDE_ASM("asm/nonmatchings/15690", func_800154A8_160A8);
