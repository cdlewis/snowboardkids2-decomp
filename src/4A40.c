#include "1DC260.h"
#include "1DC480.h"
#include "1DC900.h"
#include "20F0.h"
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

void *func_8000B6B8_C2B8(s32);
void *func_80035F80_36B80(s32);

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
    { &_63C9F0_ROM_START, &_63C9F0_ROM_END, 0x9578 },
};

s32 padding[2] = { 0, 0 };

s32 D_8008BFF0_8CBF0 = 0xB00000;

s32 D_8008BFF4_8CBF4 = 0xB000;

s32 D_8008BFF8_8CBF8 = 0x0;

s32 D_8008BFFC_8CBFC = 0x600333;

struct {
    s32 unk0;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} D_8008C000_8CC00 = {
    0x1400000,
    0,
    0,
    0,
};

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 unk4;
    s8 unk5;
    u16 unk6;
    u16 unk8;
    s16 unkA;
    s32 unkC;
} D_8008C00C_entry; // size 0x10

D_8008C00C_entry D_8008C00C_8CC0C[] = {
    { 0x64, 0,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x65, 1,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x71, 0xD,  0, 1, 0xF800, 0, 0x0CCC, 0          },
    { 0x66, 2,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x67, 3,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x74, 0x10, 0, 1, 0,      0, 0x2000, 0          },
    { 0x68, 4,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x69, 5,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x70, 0xC,  0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x6A, 6,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x6B, 7,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x6C, 8,    0, 0, 0xF800, 0, 0x2000, 0          },
    { 0x73, 0xF,  0, 4, 0xF800, 0, 0x0CCC, 0x00100000 },
    { 0x6E, 0xA,  0, 0, 0xF800, 0, 0x0E66, 0          },
    { 0x6F, 0xB,  0, 0, 0xF800, 0, 0x0B33, 0          },
    { 0x6D, 9,    0, 0, 0xF800, 0, 0x0999, 0          },
    { 0x72, 0xE,  0, 0, 0xF800, 0, 0x2000, 0x00199999 }
};

struct {
    u16 unk0;
    u16 unk2;
} D_8008C11C_8CD1C = { 0x11, 0 };

s32 D_8008C120_8CD20[] = { 0x001428F5, 0x00099999, 0x00266666, 0xFFEBD70B, 0x00099999, 0x00266666,
                           0x001428F5, 0x00099999, 0xFFE2E148, 0xFFEBD70B, 0x00099999, 0xFFE2E148 };

void func_80004368_4F68(void);
void func_80004454_5054(void);
void func_8000454C_514C(void);

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
    void *unk960;
    void *unk964;
    void *unk968;
    s16 unk96C;
    s16 unk96E;
    u8 padding9A[0x4];
    void *unk974;
    s16 unk978;
    s16 unk97A;
    u8 unk97C;
    u8 padding9B[0x3];
    s16 unk980;
    s16 unk982;
    void *unk984;
    void *unk988;
    s16 unk98C;
    s16 unk98E;
    u8 unk990;
    u8 padding9C[0x3];
    void *unk994;
    void *unk998;
    void *unk99C;
    s16 unk9A0;
    u8 padding9D[0x2];
    s16 unk9A4;
    s16 unk9A6;
    u8 padding9E[0x4];
    s16 unk9AC;
    s16 unk9AE;
    s16 unk9B0;
    s16 unk9B2;
    u8 padding9F[0x4];
    void *unk9B8[6];
    u8 padding11[0x41C];
    void *unkDEC;
    s32 unkDF0;
    s32 unkDF4;
    s32 unkDF8;
    s32 unkDFC;
    u8 unkE00[0xA];
    s16 unkE0A;
    u8 paddingE0C[0x4];
    u8 unkE10[0xA];
    s16 unkE1A;
    u8 paddingE1C[0x4];
    u8 unkE20[0xA];
    s16 unkE2A;
    u8 paddingE2C[0x4];
    u8 unkE30[0xA];
    s16 unkE3A;
    u8 paddingE3C[0x4];
    s16 unkE40;
    s16 unkE42;
    ColorData unkE44[3];
    u8 paddingE5C[4];
    s16 unkE60;
} func_80003EE0_4AE0_task_memory;

extern u8 identityMatrix[];

typedef struct {
    SceneModel *unk0;
    s16 unk4;
    s16 unk6;
    u8 unk8;
    s8 unk9;
    s16 unkA;
} StructUnk800048D0;

typedef struct {
    u8 padding[0x2C];
    s32 unk2C;
    s32 unk30;
} SceneModel30;

void func_800047A0_53A0(StructUnk800048D0 *arg0);
void func_800048D0_54D0(StructUnk800048D0 *arg0);
void func_80004570_5170(func_80003EE0_4AE0_task_memory *);
void func_8000464C_524C(StructUnk800048D0 *arg0);

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

void func_80004368_4F68(void) {
    func_80003EE0_4AE0_task_memory *state;

    state = (func_80003EE0_4AE0_task_memory *)getCurrentAllocation();

    if (state->unk2 == 0x1C98) {
        state->unk2 = 0x1C98;
        func_80057564_58164(4);
        func_8006FDA0_709A0(&state->unk8, 0xFF, 0x1E);
        func_8006FDA0_709A0(&state->unk1E0, 0xFF, 0x1E);
        func_8006FDA0_709A0(&state->unk3B8, 0xFF, 0x1E);
        func_8006FDA0_709A0(&state->unk590, 0xFF, 0x1E);
        func_8006FDA0_709A0(&state->unk768, 0xFF, 0x1E);
        setGameStateHandler(func_80004454_5054);
    }

    if (state->unk0 == 0) {
        func_800B02E0(state);
        func_800B0760(state);
        func_800B00C0_1DC260(state);
        state->unk0 = 1;
    }

    func_800B0930(state);
    func_800B016C(state);
    func_80004570_5170(state);

    state->unk2 = (u16)state->unk2 + 1;
}

void func_80004454_5054(void) {
    func_80003EE0_4AE0_task_memory *state;
    s32 i;

    state = (func_80003EE0_4AE0_task_memory *)getCurrentAllocation();

    if (state->unk2 == 0x1CB8) {
        unlinkNode(&state->unk768);
        unlinkNode(&state->unk590);
        unlinkNode(&state->unk3B8);
        unlinkNode(&state->unk1E0);
        unlinkNode(&state->unk8);
        state->unk95C = freeNodeMemory(state->unk95C);
        state->unk968 = freeNodeMemory(state->unk968);
        state->unk964 = freeNodeMemory(state->unk964);
        state->unk960 = freeNodeMemory(state->unk960);
        state->unkDEC = freeNodeMemory(state->unkDEC);
        for (i = 0; i < 6; i++) {
            state->unk9B8[i] = freeNodeMemory(state->unk9B8[i]);
        }
        terminateSchedulerWithCallback(func_8000454C_514C);
    } else {
        func_800B0930(state);
        func_800B016C(state);
        func_80004570_5170(state);
        state->unk2 = (u16)state->unk2 + 1;
    }
}

void func_8000454C_514C(void) {
    getCurrentAllocation();
    func_800697F4_6A3F4(1);
}

void func_80004570_5170(func_80003EE0_4AE0_task_memory *arg0) {
    u16 temp_a0;
    s32 temp_a0_2;
    s32 temp_v1;
    s32 var_v0;
    StructUnk800048D0 *temp_v0;

    temp_a0 = arg0->unk2;
    if ((u32)(temp_a0 - 0x12C) >= 0x1717U) {
        arg0->unkE60 = 0x12C;
        return;
    }
    if ((s16)temp_a0 == arg0->unkE60) {
        if (arg0->unkE40 >= (s16)D_8008C11C_8CD1C.unk0) {
            temp_a0_2 = arg0->unkE42;
            arg0->unkE40 = 0;
            temp_v1 = temp_a0_2 + 1;
            var_v0 = temp_v1;
            if (temp_v1 < 0) {
                var_v0 = temp_a0_2 + 4;
            }
            arg0->unkE42 = temp_v1 - ((var_v0 >> 2) * 4);
        }
        temp_v0 = scheduleTask(func_8000464C_524C, 0, 0, 0);
        if (temp_v0 != NULL) {
            temp_v0->unk6 = arg0->unkE40;
            *(s16 *)&temp_v0->unk8 = arg0->unkE42;
        }
        arg0->unkE40 = arg0->unkE40 + 1;
        arg0->unkE60 = arg0->unk2 + 0x3D;
    }
}

void func_8000464C_524C(StructUnk800048D0 *arg0) {
    s32 buffer[8];
    func_80003EE0_4AE0_task_memory *taskMemory;
    D_8008C00C_entry *entry;
    s16 scale;

    taskMemory = getCurrentAllocation();
    entry = &D_8008C00C_8CC0C[arg0->unk6];
    arg0->unkA = 0;
    arg0->unk4 = 0;

    arg0->unk0 = allocateNodeMemory(0x160);

    initializeGameEntity(arg0->unk0, entry->unk0, &taskMemory->unk768, arg0->unk9, -1, -1, -1);

    memcpy((void *)((u8 *)arg0->unk0 + 0x18), identityMatrix, 0x20);

    createYRotationMatrix((Mat3x3Padded *)((u8 *)arg0->unk0 + 0x18), entry->unk6);

    scale = entry->unkA;
    scaleMatrix((Mat3x3Padded *)((u8 *)arg0->unk0 + 0x18), scale, scale, scale);

    ((SceneModel30 *)arg0->unk0)->unk2C = D_8008BFF0_8CBF0;
    ((SceneModel30 *)arg0->unk0)->unk30 += entry->unkC;

    func_800015CC_21CC(arg0->unk0, entry->unk5);

    {
        s32 temp_a1 = D_8008BFF8_8CBF8;
        s32 temp_a2 = D_8008BFFC_8CBFC;
        s32 temp_a3 = D_8008C000_8CC00.unk0;
        func_8006BEDC_6CADC(buffer, temp_a1, temp_a2, temp_a3, 0, D_8008C000_8CC00.unk6, D_8008C000_8CC00.unkA);
    }

    func_8006FD3C_7093C(taskMemory->unk768.id, buffer);

    setCleanupCallback(func_800048D0_54D0);
    setCallback(func_800047A0_53A0);
}

void func_800047A0_53A0(StructUnk800048D0 *arg0) {
    D_8008C00C_entry *entry;

    getCurrentAllocation();

    entry = &D_8008C00C_8CC0C[arg0->unk6];

    switch (arg0->unk4) {
        case 0:
            func_800021B8_2DB8(arg0->unk0, entry->unk2);
            arg0->unk4 = 1;
            break;
        case 1:
            if (entry->unk0 == 0x73) {
                func_800015CC_21CC(arg0->unk0, 3);
                arg0->unk4 = 2;
            }
            clearModelRotation(arg0->unk0);
            break;
        case 2:
            if (entry->unk0 == 0x73) {
                func_800015CC_21CC(arg0->unk0, 8);
            }
            clearModelRotation(arg0->unk0);
            break;
        default:
            break;
    }

    arg0->unk0->unk2C -= D_8008BFF4_8CBF4;

    if (arg0->unk0->unk2C < -D_8008BFF0_8CBF0) {
        func_80002040_2C40(arg0->unk0);
        arg0->unkA = 1;
        func_80069CF8_6A8F8();
    } else {
        updateModelGeometry(arg0->unk0);
    }
}

void func_800048D0_54D0(StructUnk800048D0 *arg0) {
    getCurrentAllocation();
    if (arg0->unkA == 0) {
        func_80002040_2C40(arg0->unk0);
    }
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}
