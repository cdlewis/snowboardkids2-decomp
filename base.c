#include "common.h"

typedef struct Node {
    u8 padding[0x28];
} Node;

typedef struct {
    Node n;
    u8 padding[0x50];
    s32 unk78;
    u8 padding2[4];
    s16 unk80;
    u8 padding3[4];
    s16 unk86;
} func_800B4F60_1E2010_task;

extern void func_800B4CD0_1E1D80(void *);
void *scheduleTask(void *a0, u8, u8, u8);

void func_800B4F60_1E2010(s32 arg0, s16 arg1) {
    func_800B4F60_1E2010_task *task = (func_800B4F60_1E2010_task *)scheduleTask(&func_800B4CD0_1E1D80, 1, 0, 0x64);
    
    if (task != NULL) {
        task->unk78 = arg0;
        task->unk80 = 0;
        task->unk86 = arg1;
    }
}
