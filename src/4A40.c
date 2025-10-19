#include "6E840.h"
#include "common.h"
#include "geometry.h"
#include "graphics.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_OVERLAY(_1DC260)

USE_ASSET(_49BA20);
USE_ASSET(_619E10);
USE_ASSET(_621390);
USE_ASSET(_6280F0);
USE_ASSET(_62F2F0);
USE_ASSET(_635AB0);
USE_ASSET(_63C9F0);
USE_ASSET(_67E860)

s32 func_8000B6B8_C2B8(s32);
s32 func_80035F80_36B80(s32);

typedef struct {
    void *start;
    void *end;
    s32 size;
} asset;

static asset D_8008BFA0_8CBA0[6] = {
    { &_619E10_ROM_START, &_619E10_ROM_END, 0x9578 },
    { &_621390_ROM_START, &_621390_ROM_END, 0x9578 },
    { &_6280F0_ROM_START, &_6280F0_ROM_END, 0x9578 },
    { &_62F2F0_ROM_START, &_62F2F0_ROM_END, 0x9578 },
    { &_635AB0_ROM_START, &_635AB0_ROM_END, 0x9578 },
    { &_63C9F0_ROM_START, &_63C9F0_ROM_END, 0x9578 }
};

void func_80004368_4F68(void);

typedef struct {
    s8 unk0;
    u8 padding[1];
    s16 unk2;
    s32 unk4;
    Node_70B00 unk8;
    Node_70B00 unk1E0;
    Node_70B00 unk3B8;
    Node_70B00 unk590;
    Node_70B00 unk768;
    s16 unk940;
    s16 unk942;
    s16 unk944;
    void *unk948;
    s8 unk94C;
    u8 padding8[0xF];
    void *unk95C;
    s32 unk960;
    s32 unk964;
    s32 unk968;
    u8 padding9[0x4C];
    void *unk9B8[6];
    u8 padding11[0x41C];
    void *unkDEC;
    u8 padding12[0x52];
    u8 unkE42;
    u8 unkE43;
    ColorData unkE44[3];
} func_80003EE0_4AE0_task_memory;

void func_80003E40_4A40(func_80003EE0_4AE0_task_memory *arg0) {
    arg0->unkE44[0].r2 = 0;
    arg0->unkE44[0].g2 = 0x7F;
    arg0->unkE44[0].r = 0xE0;
    arg0->unkE44[0].b = 0xA0;
    arg0->unkE44[0].g = 0xE0;
    arg0->unkE44[1].r = 0x32;
    arg0->unkE44[1].g = 0x32;
    arg0->unkE44[1].b = 0x32;
    arg0->unkE44[2].r2 = -0x7F;
    arg0->unkE44[2].r = 0x65;
    arg0->unkE44[2].g = 0x65;
    arg0->unkE44[2].b = 0x65;
    arg0->unkE44[3].r = 0x80;
    arg0->unkE44[3].b = 0xA0;
    arg0->unkE44[0].b2 = 0x7F;
    arg0->unkE44[1].r2 = 0x7F;
    arg0->unkE44[1].g2 = 0x7F;
    arg0->unkE44[1].b2 = 0;
    arg0->unkE44[2].g2 = 0x7F;
    arg0->unkE44[2].b2 = 0;
    arg0->unkE44[3].g = 0x90;

    func_8006FC70_70870(arg0->unk768.id, 3, &arg0->unkE44[0], &arg0->unkE44[3]);
}

void func_80003EE0_4AE0(void) {
    s32 buffer[8];
    s32 i;
    func_80003EE0_4AE0_task_memory *taskMemory = (func_80003EE0_4AE0_task_memory *)allocateTaskMemory(0xE68);

    LOAD_OVERLAY(_1DC260);

    setupTaskSchedulerNodes(0x40, 0, 0, 0, 0, 0, 0, 0);

    taskMemory->unk940 = -0x90;
    taskMemory->unk942 = 0x68;
    *(s16 *)((u8 *)taskMemory + 0xe40) = 0;
    *(s16 *)((u8 *)taskMemory + 0xe42) = 0;
    taskMemory->unk0 = 0;
    taskMemory->unk2 = 0;
    taskMemory->unk944 = 0;
    taskMemory->unk948 = &taskMemory->unk94C;
    taskMemory->unk94C = 0;
    taskMemory->unk960 = func_80035F80_36B80(1);
    taskMemory->unk964 = func_8000B6B8_C2B8(1);

    taskMemory->unk968 = func_8000B6B8_C2B8(3);
    taskMemory->unkDEC = dmaRequestAndUpdateStateWithSize(&_67E860_ROM_START, &_67E860_ROM_END, 0x2448);
    taskMemory->unk95C = dmaRequestAndUpdateStateWithSize(&_49BA20_ROM_START, &_49BA20_ROM_END, 0x2B0);

    for (i = 0; i < 6; i++) {
        taskMemory->unk9B8[i] = dmaRequestAndUpdateStateWithSize(
            D_8008BFA0_8CBA0[i].start,
            D_8008BFA0_8CBA0[i].end,
            D_8008BFA0_8CBA0[i].size
        );
    }

    func_8006FAA4_706A4(&taskMemory->unk8, 0, 0, 0xB, 0);
    func_8006F9BC_705BC(&taskMemory->unk8, 1.0f, 1.0f);
    setModelCameraTransform(&taskMemory->unk8, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FEF8_70AF8(&taskMemory->unk8, 1);
    func_8006FAA4_706A4(&taskMemory->unk1E0, 0, 1, 0xC, 0);
    func_8006F9BC_705BC(&taskMemory->unk1E0, 1.0f, 1.0f);
    func_8006FEF8_70AF8(&taskMemory->unk1E0, 1);
    setModelCameraTransform(&taskMemory->unk1E0, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FAA4_706A4(&taskMemory->unk3B8, 0, 2, 0xB, 0);
    func_8006F9BC_705BC(&taskMemory->unk3B8, 1.0f, 1.0f);
    func_8006FEF8_70AF8(&taskMemory->unk3B8, 1);
    setModelCameraTransform(&taskMemory->unk3B8, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FAA4_706A4(&taskMemory->unk590, 0, 3, 0xB, 0);
    func_8006F9BC_705BC(&taskMemory->unk590, 1.0f, 1.0f);
    setModelCameraTransform(&taskMemory->unk590, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FEF8_70AF8(&taskMemory->unk590, 1);
    func_8006FAA4_706A4(&taskMemory->unk768, 0, 4, 0xD, 1);
    func_8006F9BC_705BC(&taskMemory->unk768, 1.0f, 1.0f);
    setModelCameraTransform(&taskMemory->unk768, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FEF8_70AF8(&taskMemory->unk768, 1);
    func_8006FA0C_7060C(&taskMemory->unk768, 40.0f, 1.3333334f, 10.0f, 10000.0f);
    func_8006BEDC_6CADC(buffer, 0, 0, 0x01400000, 0, 0, 0);
    func_8006FD3C_7093C(taskMemory->unk768.id, buffer);
    func_8006FE28_70A28(&taskMemory->unk8, 0, 0, 0);
    func_8006FDA0_709A0(&taskMemory->unk8, 0, 0);
    func_8006FDA0_709A0(&taskMemory->unk1E0, 0, 0);
    func_8006FDA0_709A0(&taskMemory->unk3B8, 0, 0);
    func_8006FDA0_709A0(&taskMemory->unk590, 0, 0);
    func_8006FDA0_709A0(&taskMemory->unk768, 0, 0);
    func_80057514_58114(0xB, 0x80, 0);
    func_80003E40_4A40(taskMemory);
    setGameStateHandler(&func_80004368_4F68);
}

INCLUDE_ASM("asm/nonmatchings/4A40", func_80004368_4F68);

INCLUDE_ASM("asm/nonmatchings/4A40", func_80004454_5054);

INCLUDE_ASM("asm/nonmatchings/4A40", func_8000454C_514C);

INCLUDE_ASM("asm/nonmatchings/4A40", func_80004570_5170);

INCLUDE_ASM("asm/nonmatchings/4A40", func_8000464C_524C);

INCLUDE_ASM("asm/nonmatchings/4A40", func_800047A0_53A0);

INCLUDE_ASM("asm/nonmatchings/4A40", func_800048D0_54D0);
