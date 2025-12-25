#include "1DFAA0.h"
#include "20F0.h"
#include "3E160.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "graphics.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_1FB4E0);
USE_ASSET(_4C9E70);

extern StateEntry D_80088650;
extern StateEntry D_80088660;
extern s16 D_800BAD0E_1E7DBE;
extern s16 D_800BAA60_1E7B10[];

s16 func_800B4AF0_1E1BA0(void) {
    return D_800BAD0E_1E7DBE;
}

s16 func_800B4AFC_1E1BAC(s16 arg0) {
    s16 temp_v1;
    s16 temp_v0;
    s16 index;

    temp_v1 = D_800BAD0E_1E7DBE;
    temp_v0 = arg0;

    if (temp_v0 < temp_v1) {
        index = arg0;
    } else {
        index = 0;
    }

    return D_800BAA60_1E7B10[index];
}

void func_800B4B30_1E1BE0(s16 arg0, s16 arg1, s16 arg2, s16 arg3) {
    s16 temp;

    if (arg3 <= 0) {
        temp = func_800B4AFC_1E1BAC(arg0);
        func_80058360_58F60(temp, arg1, arg2 + 0x80, 0);
    } else {
        temp = func_800B4AFC_1E1BAC(arg0);
        func_80057DF0_589F0(temp, arg1, arg2 + 0x80, 0, arg3);
    }
}

void func_800B4BDC_1E1C8C(s16 arg0, s16 arg1, s16 arg2, CutsceneSlotData *arg3) {
    s32 buffer[3];
    s16 duration;
    s16 soundId;

    if (arg3 != NULL) {
        memcpy(buffer, &arg3->unk2C, 0xC);
    } else {
        buffer[0] = 0;
        buffer[1] = 0;
        buffer[2] = 0;
    }

    duration = arg2;
    if (duration <= 0) {
        soundId = func_800B4AFC_1E1BAC(arg0);
        func_80056B7C_5777C(buffer, soundId);
        return;
    }

    soundId = func_800B4AFC_1E1BAC(arg0);
    func_80056D64_57964(buffer, soundId, 0, duration);
}

void func_800B4C80_1E1D30(s16 arg0) {
    s16 temp;

    temp = func_800B4AFC_1E1BAC(arg0);
    func_80058220_58E20(temp, 0);
}

void func_800B4CB0_1E1D60(void) {
    func_80057ABC_586BC(0, 0);
}

typedef struct {
    u8 _pad0[0x20];
    StateEntry *unk20;
    StateEntry *unk24;
    StateEntry *unk28;
    u32 unk2C;
    u8 _pad30[0x2C];
    StateEntry *unk5C;
    StateEntry *unk60;
    StateEntry *unk64;
    u32 unk68;
    u8 _pad6C[0x18];
    s16 unk84;
} func_800B4CD0_1E1D80_arg;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#pragma GCC diagnostic ignored "-Wdeprecated-non-prototype"
void func_800B4D74_1E1E24();
void func_800B4F28_1E1FD8();
#pragma GCC diagnostic pop

void func_800B4CD0_1E1D80(func_800B4CD0_1E1D80_arg *arg0) {
    StateEntry **temp_a1;

    arg0->unk20 = &D_80088650;
    arg0->unk24 = loadUncompressedData(&_1FB4E0_ROM_START, &_1FB4E0_ROM_END);

    arg0->unk28 = loadCompressedData(&_4C9E70_ROM_START, &_4C9E70_ROM_END, 0xA10);
    temp_a1 = &arg0->unk24;

    arg0->unk5C = &D_80088660;
    arg0->unk2C = 0;
    arg0->unk68 = 0;
    arg0->unk84 = 0x200;
    arg0->unk60 = *temp_a1;
    arg0->unk64 = arg0->unk28;

    setCleanupCallback(&func_800B4F28_1E1FD8);
    setCallbackWithContinue(&func_800B4D74_1E1E24);
}

extern Mat3x3Padded D_8009A8B0_9B4B0;

typedef struct {
    u8 _pad0[0x18];
    s32 unk18;
    u8 _pad1C[0x20];
    u8 unk3C[0x40];
    s32 unk7C;
    u8 _pad80[0x2];
    u16 unk82;
} func_800B4E7C_Struct;

void func_800B4E7C_1E1F2C(func_800B4E7C_Struct *arg0);

typedef struct {
    u8 _pad0[0x3C];
    u8 unk3C[0x3C];
    void **unk78;
    s32 unk7C;
    u16 unk80;
    u16 unk82;
    s16 unk84;
    s16 unk86;
} func_800B4D74_1E1E24_arg;

void func_800B4D74_1E1E24(func_800B4D74_1E1E24_arg *arg0) {
    Mat3x3Padded sp10;
    void *temp_v0;
    s16 temp_v0_2;
    s16 temp_a0;

    createYRotationMatrix(&D_8009A8B0_9B4B0, arg0->unk80);

    temp_v0 = (void *)((u8 *)(*arg0->unk78) + 0x3C0);

    func_8006B084_6BC84(&D_8009A8B0_9B4B0, temp_v0, arg0);

    scaleMatrix((Mat3x3Padded *)arg0, arg0->unk84, arg0->unk84, arg0->unk84);

    temp_v0_2 = arg0->unk82 + 0x300;
    arg0->unk82 = temp_v0_2;

    createZRotationMatrix(&sp10, temp_v0_2);

    sp10.unk18 = 0xBB333;
    sp10.unk14 = 0;
    sp10.unk1C = 0xFFEA0000;

    func_8006B084_6BC84(&sp10, arg0, &arg0->unk3C);

    enqueueDisplayListObject(0, (DisplayListObject *)arg0);
    enqueueDisplayListObject(0, (DisplayListObject *)&arg0->unk3C);

    if (arg0->unk86 != 0) {
        arg0->unk86--;
        temp_a0 = arg0->unk84;
        if (temp_a0 != 0x2000) {
            arg0->unk84 = temp_a0 + 0x200;
        }
    } else {
        arg0->unk7C = 0x40000;
        setCallback(&func_800B4E7C_1E1F2C);
    }
}

void func_800B4E7C_1E1F2C(func_800B4E7C_Struct *arg0) {
    Mat3x3Padded matrix;
    s32 temp_v0;

    temp_v0 = arg0->unk7C - 0x8000;
    arg0->unk7C = temp_v0;

    if ((s32)0xFFF80000 >= temp_v0) {
        func_80069CF8_6A8F8();
    }

    arg0->unk18 += arg0->unk7C;
    createZRotationMatrix(&matrix, arg0->unk82);

    matrix.unk18 = 0xBB333;
    matrix.unk14 = 0;
    matrix.unk1C = 0xFFEA0000;

    func_8006B084_6BC84(&matrix, (DisplayListObject *)arg0, (DisplayListObject *)&arg0->unk3C);
    enqueueDisplayListObject(0, (DisplayListObject *)arg0);
    enqueueDisplayListObject(0, (DisplayListObject *)&arg0->unk3C);
}

void func_800B4F28_1E1FD8(SceneModel_unk98 *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

typedef struct {
    u8 padding[0x78];
    s32 unk78;
    u8 padding2[4];
    s16 unk80;
    u8 padding3[4];
    s16 unk86;
} func_800B4F60_1E2010_task;

void func_800B4F60_1E2010(s32 arg0, s16 arg1) {
    func_800B4F60_1E2010_task *task = (func_800B4F60_1E2010_task *)scheduleTask(&func_800B4CD0_1E1D80, 1, 0, 0x64);

    if (task != NULL) {
        task->unk78 = arg0;
        task->unk80 = 0;
        task->unk86 = arg1;
    }
}
