#include "common.h"
#include "task_scheduler.h"

extern u8 identityMatrix[];
extern void setCallback(void *);
void func_80007E30_8A30(void);
extern void func_80007B58_8758(void);

typedef struct {
    u8 _pad[0x4];
    u8 unk4[0x20];
    s32 unk24;
    s16 unk28;
    s16 unk2A;
} Func80007B00Arg;

void func_80007B00_8700(Func80007B00Arg *arg0) {
    memcpy(&arg0->unk4, identityMatrix, 0x20);
    arg0->unk24 = 0;
    arg0->unk28 = 0;
    arg0->unk2A = 0;
    setCleanupCallback(func_80007E30_8A30);
    setCallback(func_80007B58_8758);
}

INCLUDE_ASM("asm/nonmatchings/8700", func_80007B58_8758);

void func_80007E30_8A30(void) {
}
