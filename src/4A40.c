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
extern void func_80002040_2C40(void *);
extern void *freeNodeMemory(void *);

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
void func_80004454_5054(void);
void func_8000454C_514C(void);
extern void func_800B02E0(void *);
extern void func_800B0760(void *);
extern void func_800B00C0_9FF70(void *);
extern void func_800B0930(void *);
extern void func_800B016C(void *);
void func_80004570_5170(void *);

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
    u8 padding9[0x4C];
    void *unk9B8[6];
    u8 padding11[0x41C];
    void *unkDEC;
    u8 padding12[0x50];
    s16 unkE40;
    s16 unkE42;
    ColorData unkE44[3];
} func_80003EE0_4AE0_task_memory;

extern s32 D_8008BFF4_8CBF4;
extern s32 D_8008BFF0_8CBF0;

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 _pad[0xC];
} D_8008C00C_entry; // size 0x10

extern D_8008C00C_entry D_8008C00C_8CC0C[];

typedef struct {
    SceneModel *unk0;
    s16 unk4;
    s16 unk6;
    u8 _pad[0x2];
    s16 unkA;
} StructUnk800048D0;

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
        func_800B00C0_9FF70(state);
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

INCLUDE_ASM("asm/nonmatchings/4A40", func_80004570_5170);

INCLUDE_ASM("asm/nonmatchings/4A40", func_8000464C_524C);

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
