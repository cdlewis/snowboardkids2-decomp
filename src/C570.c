#include "C570.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "overlay.h"

USE_ASSET(_1FB8B0);
USE_ASSET(_4CA440);

extern void *D_80088670_89270;
extern void *D_80088680_89280;
extern void *D_80088690_89290;
extern Mat3x3Padded D_8009A8B0_9B4B0;
extern s32 D_8009A8A4_9B4A4;
extern s32 D_8009A8A8_9B4A8;
extern s32 D_8009A8AC_9B4AC;
extern s32 gFrameCounter;
extern void *freeNodeMemory(void *);
extern void func_80069CF8_6A8F8(void);
extern void setCleanupCallback(void *);
extern void setCallbackWithContinue(void *);
extern void setCallback(void *);
extern void *dmaRequestAndUpdateState(void *, void *);
extern void *dmaRequestAndUpdateStateWithSize(void *, void *, s32);
extern void *scheduleTask(void *, u8, u8, u8);

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
    u8 _pad0[0x78];
    SceneModel *unk78;
    u8 _pad7C[0x4];
    s16 unk80;
    u8 _pad82[0x2];
    s16 unk84;
} func_8000BC10_C810_task;

void func_8000B970_C570(func_8000BBA8_C7A8_arg *arg0) {
    arg0->unk20 = &D_80088670_89270;
    arg0->unk24 = dmaRequestAndUpdateState(&_1FB8B0_ROM_START, &_1FB8B0_ROM_END);
    arg0->unk28 = dmaRequestAndUpdateStateWithSize(&_4CA440_ROM_START, &_4CA440_ROM_END, 0xE90);
    arg0->unk2C = 0;
    arg0->unk68 = 0;
    arg0->unk82 = 0x200;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    setCleanupCallback(&func_8000BBA8_C7A8);
    setCallbackWithContinue(&func_8000BA08_C608);
}

void func_8000BA08_C608(func_8000BA08_C608_arg *arg0) {
    s32 pad[8];
    void *temp_v0;
    s32 *ptr;

    createYRotationMatrix(&D_8009A8B0_9B4B0, arg0->unk80);
    temp_v0 = (void *)((u8 *)(*arg0->unk78) + 0x3C0);
    func_8006B084_6BC84(&D_8009A8B0_9B4B0, temp_v0, arg0);
    scaleMatrix((Mat3x3Padded *)&arg0->unk0, arg0->unk82, arg0->unk82, arg0->unk82);

    ptr = &D_8009A8A4_9B4A4;
    *ptr = 0;
    D_8009A8A8_9B4A8 = 0x9CCCC;
    D_8009A8AC_9B4AC = 0xFFE44CCD;

    func_8006B084_6BC84(ptr - 5, arg0, &arg0->unk3C);

    if (gFrameCounter & 1) {
        arg0->unk3C.unk20 = (DisplayLists *)&D_80088680_89280;
    } else {
        arg0->unk3C.unk20 = (DisplayLists *)&D_80088690_89290;
    }

    enqueueDisplayListObject(0, &arg0->unk0);

    if (arg0->unk82 == 0x2000) {
        enqueueDisplayListObject(0, &arg0->unk3C);
    }

    if (arg0->unk84 != 0) {
        if (arg0->unk82 == 0x2000) {
            arg0->unk84--;
        } else {
            arg0->unk82 = arg0->unk82 + 0x200;
        }
    } else {
        arg0->unk7C = 0x40000;
        setCallback(&func_8000BB48_C748);
    }
}

void func_8000BB48_C748(func_8000BB48_C748_arg *arg0) {
    s32 pad[8];
    arg0->unk7C -= 0x8000;
    if (arg0->unk7C <= (s32)0xFFF80000) {
        func_80069CF8_6A8F8();
    }
    arg0->unk0.unk10.position.unk4 += arg0->unk7C;
    enqueueDisplayListObject(0, &arg0->unk0);
}

void func_8000BBA8_C7A8(func_8000BBA8_C7A8_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_8000BBE0_C7E0(SceneModel *arg0, s16 arg1) {
    func_8000BC10_C810(arg0, arg1, 1, 0, 100);
}

void func_8000BC10_C810(SceneModel *arg0, s16 arg1, u8 arg2, u8 arg3, u8 arg4) {
    func_8000BC10_C810_task *task;

    task = (func_8000BC10_C810_task *)scheduleTask(&func_8000B970_C570, arg2, arg3, arg4);
    if (task != NULL) {
        task->unk78 = arg0;
        task->unk80 = 0;
        task->unk84 = arg1;
    }
}
