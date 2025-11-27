#include "common.h"

extern u32 D_1FB8B0;
extern u32 D_1FBE00;
extern u32 D_4CA440;
extern u32 D_4CAD30;
extern void *D_80088670_89270;
extern void *freeNodeMemory(void *);
extern void func_80069CF8_6A8F8(void);
extern void enqueueDisplayListObject(s32 arg0, void *arg1);
extern void setCleanupCallback(void *);
extern void setCallbackWithContinue(void *);
extern void *dmaRequestAndUpdateState(void *, void *);
extern void *dmaRequestAndUpdateStateWithSize(void *, void *, s32);
extern void func_8000BA08_C608(void *);
extern void *scheduleTask(void *, u8, u8, u8);
void func_8000BC10_C810(void *arg0, s16 arg1, u8 arg2, u8 arg3, u8 arg4);

typedef struct func_8000BBA8_C7A8_arg func_8000BBA8_C7A8_arg;
void func_8000BBA8_C7A8(func_8000BBA8_C7A8_arg *arg0);

struct func_8000BBA8_C7A8_arg {
    u8 _pad0[0x20];
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 _pad30[0x30];
    void *unk60;
    void *unk64;
    s32 unk68;
    u8 _pad6C[0x16];
    s16 unk82;
};

typedef struct {
    u8 _pad0[0x18];
    s32 unk18;
    u8 _pad1[0x60];
    s32 unk7C;
} func_8000BB48_C748_arg;

typedef struct {
    u8 _pad0[0x78];
    void *unk78;
    u8 _pad7C[0x4];
    s16 unk80;
    u8 _pad82[0x2];
    s16 unk84;
} func_8000BC10_C810_task;

void func_8000B970_C570(func_8000BBA8_C7A8_arg *arg0) {
    arg0->unk20 = &D_80088670_89270;
    arg0->unk24 = dmaRequestAndUpdateState(&D_1FB8B0, &D_1FBE00);
    arg0->unk28 = dmaRequestAndUpdateStateWithSize(&D_4CA440, &D_4CAD30, 0xE90);
    arg0->unk2C = 0;
    arg0->unk68 = 0;
    arg0->unk82 = 0x200;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    setCleanupCallback(&func_8000BBA8_C7A8);
    setCallbackWithContinue(&func_8000BA08_C608);
}

INCLUDE_ASM("asm/nonmatchings/C570", func_8000BA08_C608);

void func_8000BB48_C748(func_8000BB48_C748_arg *arg0) {
    s32 pad[8];
    arg0->unk7C -= 0x8000;
    if (arg0->unk7C <= (s32)0xFFF80000) {
        func_80069CF8_6A8F8();
    }
    arg0->unk18 += arg0->unk7C;
    enqueueDisplayListObject(0, arg0);
}

void func_8000BBA8_C7A8(func_8000BBA8_C7A8_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_8000BBE0_C7E0(void *arg0, s16 arg1) {
    func_8000BC10_C810(arg0, arg1, 1, 0, 100);
}

void func_8000BC10_C810(void *arg0, s16 arg1, u8 arg2, u8 arg3, u8 arg4) {
    func_8000BC10_C810_task *task;

    task = (func_8000BC10_C810_task *)scheduleTask(&func_8000B970_C570, arg2, arg3, arg4);
    if (task != NULL) {
        task->unk78 = arg0;
        task->unk80 = 0;
        task->unk84 = arg1;
    }
}
