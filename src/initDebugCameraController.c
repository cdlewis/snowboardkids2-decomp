#include "task_scheduler.h"

extern void updateDebugCameraYState();
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    void *unk8;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u8 padding[0x2];
    void *unk14;
    s32 unk18;
    s16 unk1C;
} func_8002ED90_2F990_arg;
void func_8002ED90_2F990(func_8002ED90_2F990_arg *arg0) {
    s32 v0;
    func_8002ED90_2F990_arg *new_var;
    arg0->unk2 = -0x58;
    arg0->unk8 = (void *)(((s32)arg0) + 0x3E);
    arg0->unkE = -0x50;
    v0 = -0x80;
    arg0->unk18 = 0;
    arg0->unk1C = 0;
    arg0->unk0 = (char)v0;
    v0 = ((s32)arg0) + 0x1E;
    new_var = arg0;
    new_var->unk0 = -0x80;
    new_var->unk4 = 0;
    new_var->unkC = -0x80;
    new_var->unk10 = 0;
    arg0->unk14 = (void *)v0;
    setCallback(&updateDebugCameraYState);
}
