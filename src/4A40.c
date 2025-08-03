#include "common.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_OVERLAY(_1DC260)

INCLUDE_ASM("asm/nonmatchings/4A40", func_80003E40_4A40);

void func_80003E40_4A40(void *);
s32 func_8000B6B8_C2B8(s32);
s32 func_80035F80_36B80(s32);
void func_80057514_58114(u32 arg0, s16 arg1, s16 arg2);
void func_8006BEDC_6CADC(void *, s32, s32, s32, s32, s32, s32);
void func_8006F994_70594(void *, s32, s32, s32, s32, s32, s32);
void func_8006F9BC_705BC(void *arg0, f32 arg1, f32 arg2);
void func_8006FA0C_7060C(void *, f32, f32, f32, f32);
void func_8006FAA4_706A4(void *, s32, s32, s32, s32);
void func_8006FD3C_7093C(u16, void *);
void func_8006FDA0_709A0(void *arg0, u8 arg1, u8 arg2);
void func_8006FE28_70A28(void *arg0, s8 arg1, s8 arg2, s8 arg3);
void func_8006FEF8_70AF8(void *, s32);
extern s32 D_1DC260;
extern s32 D_1DD170;
extern s32 D_49BA20;
extern s32 D_49BC40;
extern s32 D_67E860;
extern s32 D_67FB80;

typedef struct {
    void *start;
    void *end;
    s32 size;
} D_8008BFA0_8CBA0_placeholder;
extern D_8008BFA0_8CBA0_placeholder D_8008BFA0_8CBA0[];

typedef struct {
    u8 padding[0x1D8];
} unknownStruct;

void func_80004368_4F68(void);

typedef struct {
    s8 unk0;
    u8 padding[1];
    s16 unk2;
    s32 unk4;
    unknownStruct unk8;
    unknownStruct unk1E0;
    unknownStruct unk3B8;
    unknownStruct unk590;
    void *unk768;
    u8 padding6[0xD6];
    s16 unk842;
    u8 padding7[0xFC];
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
    u8 padding12[0x50];
    s16 unkE40;
    s16 unkE42;
} func_80003EE0_4AE0_task_memory;

void func_80003EE0_4AE0(void) {
    s32 buffer[8];
    s32 i;
    func_80003EE0_4AE0_task_memory *taskMemory = (func_80003EE0_4AE0_task_memory *)allocateTaskMemory(0xE68);

    LOAD_OVERLAY(_1DC260);

    setupTaskSchedulerNodes(0x40, 0, 0, 0, 0, 0, 0, 0);

    taskMemory->unk940 = -0x90;
    taskMemory->unk942 = 0x68;
    taskMemory->unkE40 = 0;
    taskMemory->unkE42 = 0;
    taskMemory->unk0 = 0;
    taskMemory->unk2 = 0;
    taskMemory->unk944 = 0;
    taskMemory->unk948 = &taskMemory->unk94C;
    taskMemory->unk94C = 0;
    taskMemory->unk960 = func_80035F80_36B80(1);
    taskMemory->unk964 = func_8000B6B8_C2B8(1);

    taskMemory->unk968 = func_8000B6B8_C2B8(3);
    taskMemory->unkDEC = dmaRequestAndUpdateStateWithSize(&D_67E860, &D_67FB80, 0x2448);
    taskMemory->unk95C = dmaRequestAndUpdateStateWithSize(&D_49BA20, &D_49BC40, 0x2B0);

    for (i = 0; i < 6; i++) {
        taskMemory->unk9B8[i] = dmaRequestAndUpdateStateWithSize(
            D_8008BFA0_8CBA0[i].start,
            D_8008BFA0_8CBA0[i].end,
            D_8008BFA0_8CBA0[i].size
        );
    }

    func_8006FAA4_706A4(&taskMemory->unk8, 0, 0, 0xB, 0);
    func_8006F9BC_705BC(&taskMemory->unk8, 1.0f, 1.0f);
    func_8006F994_70594(&taskMemory->unk8, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FEF8_70AF8(&taskMemory->unk8, 1);
    func_8006FAA4_706A4(&taskMemory->unk1E0, 0, 1, 0xC, 0);
    func_8006F9BC_705BC(&taskMemory->unk1E0, 1.0f, 1.0f);
    func_8006FEF8_70AF8(&taskMemory->unk1E0, 1);
    func_8006F994_70594(&taskMemory->unk1E0, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FAA4_706A4(&taskMemory->unk3B8, 0, 2, 0xB, 0);
    func_8006F9BC_705BC(&taskMemory->unk3B8, 1.0f, 1.0f);
    func_8006FEF8_70AF8(&taskMemory->unk3B8, 1);
    func_8006F994_70594(&taskMemory->unk3B8, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FAA4_706A4(&taskMemory->unk590, 0, 3, 0xB, 0);
    func_8006F9BC_705BC(&taskMemory->unk590, 1.0f, 1.0f);
    func_8006F994_70594(&taskMemory->unk590, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FEF8_70AF8(&taskMemory->unk590, 1);
    func_8006FAA4_706A4(&taskMemory->unk768, 0, 4, 0xD, 1);
    func_8006F9BC_705BC(&taskMemory->unk768, 1.0f, 1.0f);
    func_8006F994_70594(&taskMemory->unk768, 0, 0, -0xA0, -0x78, 0x9F, 0x77);
    func_8006FEF8_70AF8(&taskMemory->unk768, 1);
    func_8006FA0C_7060C(&taskMemory->unk768, 40.0f, 1.3333334f, 10.0f, 10000.0f);
    func_8006BEDC_6CADC(buffer, 0, 0, 0x01400000, 0, 0, 0);
    func_8006FD3C_7093C(taskMemory->unk842, buffer);
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
