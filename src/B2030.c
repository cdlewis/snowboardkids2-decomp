#include "common.h"
#include "src/gamestate.h"
#include "src/task_scheduler.h"

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

void func_800BB2B0_B2030(void);

INCLUDE_ASM("asm/nonmatchings/B2030", func_800BB2B0_B2030);

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
