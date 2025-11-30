#include "56910.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "task_scheduler.h"

typedef struct {
    s16 unk0;
    s16 unk2;
    u8 pad4[0x20];
    void *unk24;
    void *unk28;
} B2030TaskPayload;

typedef struct {
    u8 padding[0x5C];
    void (*callback)(void *);
} ScheduledTask;

extern void D_800BB7B0_B2530(void *);
extern void D_800BB860_B25E0(void *);
extern void D_800BB910_B2690(void *);

typedef struct {
    u8 unk0;
    u8 pad1[3];
    s32 unk4;
    s32 unk8;
    s32 unkC;
} WaypointEntry;

typedef struct {
    Mat3x3Padded unk0;
    DisplayLists *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 pad30[0xC];
    s16 matrix3C[9];
    u8 pad4E[2];
    s32 position50[3];
    WaypointEntry *unk5C;
    u8 pad60[4];
    s16 angleX;
    s16 angleY;
    s16 unk68;
    s16 index;
    s16 unk6C;
} B2030Payload;

typedef struct {
    u8 pad0[0x5C];
    u8 memoryPoolId;
} Allocation;

extern void func_800BB3A0_B2120(void);
void func_800BB650_B23D0(B2030TaskPayload *arg0);

void func_800BB2B0_B2030(B2030Payload *arg0) {
    Allocation *allocation;
    WaypointEntry *waypoints;
    unsigned int angle;
    allocation = getCurrentAllocation();
    arg0->unk20 = &func_80055E68_56A68(allocation->memoryPoolId)->unk90;
    arg0->unk24 = func_80055DC4_569C4(allocation->memoryPoolId);
    arg0->unk28 = func_80055DF8_569F8(allocation->memoryPoolId);
    arg0->index = 0;
    arg0->unk2C = 0;
    arg0->unk6C = 0;
    memcpy(&arg0->position50, &(arg0->unk5C - (-arg0->index))->unk4, 12);
    arg0->index++;
    waypoints = arg0->unk5C;
    arg0->angleX = 0;
    angle = func_8006D21C_6DE1C(
        waypoints[arg0->index].unk4,
        waypoints[arg0->index].unkC,
        arg0->position50[0],
        arg0->position50[2]
    );
    arg0->angleY = angle & 0x1FFF;
    createYRotationMatrix(&arg0->unk0, arg0->angleY);
    createCombinedRotationMatrix(&arg0->matrix3C, arg0->angleX, arg0->angleY);
    setCleanupCallback(func_800BB650_B23D0);
    setCallback(func_800BB3A0_B2120);
}

INCLUDE_ASM("asm/nonmatchings/B2030", func_800BB3A0_B2120);

void func_800BB650_B23D0(B2030TaskPayload *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800BB6B4_B2434(B2030TaskPayload *arg0);

void func_800BB688_B2408(B2030TaskPayload *arg0) {
    arg0->unk0 = 0x78;
    arg0->unk2 = 0;
    setCallback(func_800BB6B4_B2434);
}

void func_800BB6B4_B2434(B2030TaskPayload *arg0) {
    GameState *allocation;
    ScheduledTask *task;

    allocation = (GameState *)getCurrentAllocation();
    if (allocation->gamePaused == 0) {
        if (arg0->unk0 == 0) {
            task = (ScheduledTask *)scheduleTask(func_800BB2B0_B2030, 0, 0, 0xD3);
            if (task != NULL) {
                switch (arg0->unk2) {
                    case 0:
                        task->callback = D_800BB7B0_B2530;
                        break;
                    case 1:
                        task->callback = D_800BB860_B25E0;
                        break;
                    case 2:
                        task->callback = D_800BB910_B2690;
                        break;
                }
                arg0->unk2++;
                if (arg0->unk2 >= 3) {
                    arg0->unk2 = 0;
                }
            }
            arg0->unk0 = 0x78;
        } else {
            arg0->unk0--;
        }
    }
}
