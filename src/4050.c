#include "1DFAA0.h"
#include "1E60D0.h"
#include "6E840.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_OVERLAY(cutscene);

typedef union {
    struct {
        s16 lower;
        s16 upper;
    } half;
    s32 full;
} CompositeInt;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    CompositeInt unk8;
    CompositeInt unkC;
} D_8008BF90_8CB90_type;

D_8008BF90_8CB90_type D_8008BF90_8CB90 = { .unk0 = 0,
                                           .unk2 = 0,
                                           .unk4 = 0,
                                           .unk6 = 0,
                                           .unk8 = { .half = { .lower = 0, .upper = 0 } },
                                           .unkC = { .half = { .lower = 0, .upper = 2 } } };

extern void func_80014480_15080(void);
extern s16 D_800AB070_A23E0;
extern s16 D_800AFEF0_A7260;
typedef struct {
    s16 unk0;
    s8 unk2;
    u8 unk3;
    s32 unk4;
    Node_70B00 unk8;
    Node_70B00 unk1E0;
    Node_70B00 unk3B8;
    u8 padding1[0x20];
    u8 unk5B0;
    u8 padding[0x122F];
    func_800B9020_arg unk17E0;
} allocateTaskMemory_return;

void func_80003C34_4834(void);
void func_80003898_4498(void);
void loadCutsceneOverlay(void);
void func_8000378C_438C(void);
void func_80003C88_4888(void);
void func_80003D30_4930(void);
void func_80003CC4_48C4(void);
void func_80003508_4108(void);

extern void *func_800B00C0_9FF70(void);

void func_80003450_4050(s16 arg0, s16 arg1) {
    D_800AB070_A23E0 = arg0;
    D_800AFEF0_A7260 = arg1;
}

s32 __udiv_w_sdiv(void) {
    return 0;
}

void func_8000346C_406C(s16 arg0, s16 arg1, s16 arg2) {
    D_8008BF90_8CB90.unk0 = arg0;
    D_8008BF90_8CB90.unk2 = arg1;
    D_8008BF90_8CB90.unk4 = arg2;
}

void loadCutsceneOverlay(void) {
    LOAD_OVERLAY(cutscene)
    setGameStateHandler(&func_80003508_4108);
}

void func_80003508_4108(void) {
    Node_70B00 *new_var3;
    Node_70B00 *s0;
    Node_70B00 *s1;
    f32 f20 = 1.0f;
    u8 temp[0x20];

    struct {
        allocateTaskMemory_return *s1;
    } s;
    s.s1 = (allocateTaskMemory_return *)allocateTaskMemory(0x17E8);
    s.s1->unk2 = 0;
    s.s1->unk0 = 0;
    s.s1->unk3 = 0;
    setupTaskSchedulerNodes(0x40, 0x30, 4, 4, 4, 0, 0, 0);

    s0 = &s.s1->unk8;
    func_8006FAA4_706A4(s0, 0, 0, 10, f20);
    func_8006F9BC_705BC(s0, f20, f20);
    func_8006FEF8_70AF8(s0, 1);
    setModelCameraTransform(s0, 0, 0, -160, -120, 159, 119);
    func_8006FA0C_7060C(s0, 40.0f, 1.3333334f, 10.0f, 10000.0f);
    func_8006BEDC_6CADC(temp, 0, 0, 0x01000000, 0, 0, 0);

    new_var3 = &s.s1->unk1E0;
    func_8006FD3C_7093C(s.s1->unk8.id, temp);
    func_8006FDA0_709A0(NULL, 0, 0);

    s0 = new_var3;
    func_8006FAA4_706A4(s0, 0, 1, 11, 0);
    func_8006F9BC_705BC(s0, f20, f20);

    func_8006FEF8_70AF8(s0, 1);
    setModelCameraTransform(s0, 0, 0, -160, -120, 159, 119);
    func_8006FE28_70A28(s0, 0, 0, 0);
    func_8006FDA0_709A0(s0, 0xFF, 0);
    func_8006FDA0_709A0(s0, 0, 8);

    s1 = &s.s1->unk3B8;
    func_8006FAA4_706A4(s1, 0, 3, 8, 0);
    func_8006F9BC_705BC(s1, f20, f20);
    func_8006FEF8_70AF8(s1, 1);
    setModelCameraTransform(s1, 0, 0, -160, -120, 159, 119);
    func_8006FE28_70A28(s1, 0, 0, 0);
    func_8006FDA0_709A0(s1, 0xFF, 0);
    func_8006FDA0_709A0(s1, 0, 8);
    setGameStateHandler(func_8000378C_438C);
}

void func_8000378C_438C(void) {
    allocateTaskMemory_return *temp_s0 = (allocateTaskMemory_return *)getCurrentAllocation();

    if (temp_s0->unk0 >= func_800B3360(D_800AB070_A23E0, D_800AFEF0_A7260) || temp_s0->unk3 != 0) {
        if (D_800AB070_A23E0 == 0xB && D_800AFEF0_A7260 == 1) {
            func_8006FDA0_709A0(&temp_s0->unk1E0, 0, 0);
            func_800B9020(&temp_s0->unk17E0);
            setGameStateHandler(&func_80003C34_4834);

            return;
        }

        func_80057564_58164(2);
        unlinkNode(&temp_s0->unk3B8);
        unlinkNode(&temp_s0->unk1E0);
        unlinkNode(&temp_s0->unk8);
        terminateSchedulerWithCallback(&func_80003C88_4888);

        return;
    }

    func_8000346C_406C(D_800AB070_A23E0, D_800AFEF0_A7260, temp_s0->unk0);
    temp_s0->unk0++;
    initializeCutsceneSystem(&temp_s0->unk5B0);
    setGameStateHandler(&func_80003898_4498);
}

INCLUDE_ASM("asm/nonmatchings/4050", func_80003898_4498);

void func_80003C34_4834(void) {
    allocateTaskMemory_return *temp_v0;

    temp_v0 = (allocateTaskMemory_return *)getCurrentAllocation();
    if (temp_v0->unk17E0.unk0 != 0) {
        unlinkNode(&temp_v0->unk3B8);
        unlinkNode(&temp_v0->unk1E0);
        unlinkNode(&temp_v0->unk8);
        terminateSchedulerWithCallback(&func_80003C88_4888);
    }
}

void func_80003C88_4888(void) {
    func_800697F4_6A3F4(1);
}

void func_80003CA4_48A4(void) {
    setGameStateHandler(&func_80003CC4_48C4);
}

void func_80003CC4_48C4(void) {
    if ((D_8008BF90_8CB90.unk8.full == 5) && (D_8008BF90_8CB90.unkC.full == 0)) {
        D_8008BF90_8CB90.unkC.full = 1;
    }
    func_80003450_4050(D_8008BF90_8CB90.unk8.half.upper, D_8008BF90_8CB90.unkC.half.upper);
    createTaskQueue(&loadCutsceneOverlay, 0x64);
    setGameStateHandler(&func_80003D30_4930);
}

INCLUDE_ASM("asm/nonmatchings/4050", func_80003D30_4930);

void func_80003E10_4A10(void) {
    func_800693C4_69FC4(&func_80014480_15080, 0xC8);
}
