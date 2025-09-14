#include "5520.h"
#include "common.h"
#include "displaylist.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_215120);
USE_ASSET(_2151D0);
USE_ASSET(_646DF0);
USE_ASSET(_6636F0);
USE_ASSET(_6637B0);
USE_ASSET(_663BE0);

typedef struct {
    s32 unk0;
    void *unk4;
} func_800B5210_1E22C0_arg;

typedef struct {
    u8 padding[0x20];
    s32 unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 padding2[0xC];
    void *unk3C;
    s32 unk40;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 unk56;
    s16 unk58;
} func_800B5318_1E23C8_arg;

typedef struct {
    Node n;
    u8 padding[0x28];
    s16 unk54;
    s16 unk56;
    s16 unk58;
} func_800B5290_1E2340_task;

typedef struct Vec3i {
    s32 x, y, z;
} Vec3i;

typedef struct {
    struct {
        u8 padding[0x2C];
        s32 unk2C;
        s32 unk30;
        s32 unk34;
    } *unk0;
    DataTable_19E80 *unk4;
    loadAssetMetadataByIndex_arg assetMetadata;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 *unk38;
    TableEntry_19E80 *unk3C;
    u8 unk40;
    u8 unk41;
    u8 unk42;
    u8 padding43;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s16 unk60;
    s16 unk62;
} func_800B50E4_1E2194_arg;

typedef struct {
    struct {
        s32 unk0;
        u8 pad[0x28];
        s32 unk2C;
        s32 unk30;
        s32 unk34;
    } *unk0;
    void *unk4;
    s32 *unk8;
    char pad[0x16];
    u8 unk22;
    char pad2[5];
    s32 *unk28;
    char pad3[0x16];
    u8 unk42;
    char pad4[5];
    Vec3i unk48[2];
    u16 unk60;
    u16 unk62;
} func_800B4FC0_1E2070_arg;

void func_800B5438_1E24E8(func_800B5318_1E23C8_arg *);
void func_800B54B4_1E2564(func_800B5318_1E23C8_arg *);
void func_800B5318_1E23C8(func_800B5318_1E23C8_arg *);
void func_800B50E4_1E2194(func_800B50E4_1E2194_arg *);
void func_800B5210_1E22C0(func_800B5210_1E22C0_arg *);
void func_800B4FC0_1E2070(func_800B4FC0_1E2070_arg *);

extern void transformVector(s16 *, s32, void *);
extern s32 D_80089510_8A110;
extern s32 D_80089520;
extern void func_80067EDC_68ADC(s32, void *);

u32 D_800BAD10_1E7DC0[] = { 0xFFE80018, 0x00000000, 0xFFF0FFF0, 0xFFFFFFFF, 0x00180018, 0x00000000,
                            0x07F0FFF0, 0xFFFFFFFF, 0x0018FFE8, 0x00000000, 0x07F007F0, 0xFFFFFFFF,
                            0xFFE8FFE8, 0x00000000, 0xFFF007F0, 0xFFFFFFFF, 0x00040000, 0x00000000,
                            0x000D0000, 0xFFFC0000, 0x00000000, 0x000D0000 };

s32 D_800BAD68_1E7E18[2][3] = {
    { 0xFFFC0000, 0x00000000, 0xFFF30000 },
    { 0x00040000, 0x00000000, 0xFFF30000 }
};

void func_800B4FC0_1E2070(func_800B4FC0_1E2070_arg *arg0) {
    s32 i;

    arg0->unk4 = dmaRequestAndUpdateStateWithSize(&_646DF0_ROM_START, &_646DF0_ROM_END, 0x1188);
    arg0->unk8 = &D_800BAD10_1E7DC0;
    arg0->unk22 = 0xFF;

    arg0->unk28 = arg0->unk8;
    arg0->unk42 = arg0->unk22;

    for (i = 0; i < 2; i++) {
        transformVector(D_800BAD68_1E7E18[i], arg0->unk0->unk0 + 0x3C0, (u8 *)arg0 + (i * 12 + 72));
        arg0->unk48[i].x -= arg0->unk0->unk2C;
        arg0->unk48[i].y -= arg0->unk0->unk30;
        arg0->unk48[i].z -= arg0->unk0->unk34;
    }

    arg0->unk60 = 0;
    arg0->unk62 = arg0->unk62;

    setCleanupCallback(&func_800B5210_1E22C0);
    setCallbackWithContinue(&func_800B50E4_1E2194);
}

void func_800B50E4_1E2194(func_800B50E4_1E2194_arg *arg0) {
    s16 temp_v0;

    loadAssetMetadataByIndex(&arg0->assetMetadata, arg0->unk4, arg0->unk60, arg0->unk62);
    arg0->unk38 = arg0->assetMetadata.data_ptr;
    arg0->unk3C = arg0->assetMetadata.index_ptr;
    arg0->unk40 = arg0->assetMetadata.unk18;
    arg0->unk41 = arg0->assetMetadata.unk19;
    arg0->assetMetadata.unk4 = arg0->unk48 + arg0->unk0->unk2C;
    arg0->assetMetadata.unk8 = arg0->unk4C + arg0->unk0->unk30;
    arg0->assetMetadata.unkC = arg0->unk50 + arg0->unk0->unk34;
    arg0->unk2C = arg0->unk54 + arg0->unk0->unk2C;
    arg0->unk30 = arg0->unk58 + arg0->unk0->unk30;
    arg0->unk34 = arg0->unk5C + arg0->unk0->unk34;

    func_80067EDC_68ADC(0, &arg0->assetMetadata);
    func_80067EDC_68ADC(0, &arg0->unk28);

    arg0->assetMetadata.unk1A -= 0x14;
    arg0->unk42 -= 0x14;
    temp_v0 = ++arg0->unk60;

    if (temp_v0 == 8) {
        func_80069CF8_6A8F8();
    }
}

void func_800B5210_1E22C0(func_800B5210_1E22C0_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

typedef struct {
    Node n;
    u8 padding[0x36];
    s16 unk62;
} func_800B523C_1E22EC_task;
void func_800B523C_1E22EC(void *arg0, s16 arg1) {
    func_800B523C_1E22EC_task *task = (func_800B523C_1E22EC_task *)scheduleTask(&func_800B4FC0_1E2070, 1, 0, 0x64);
    if (task != NULL) {
        task->n.prev = arg0;
        task->unk62 = arg1;
    }
}

void func_800B5290_1E2340(s16 arg0, void *arg1, s16 arg2, s16 arg3) {
    func_800B5290_1E2340_task *task = (func_800B5290_1E2340_task *)scheduleTask(&func_800B5318_1E23C8, 1, 0, 0);
    if (task != NULL) {
        task->unk54 = arg0;
        memcpy(task, arg1, 0x20);
        task->unk56 = arg2;
        task->unk58 = arg3;
    }
}

void func_800B5318_1E23C8(func_800B5318_1E23C8_arg *arg0) {
    setCleanupCallback(&func_800B54B4_1E2564);

    if (arg0->unk58 == 0) {
        arg0->unk24 = dmaRequestAndUpdateState(&_215120_ROM_START, &_215120_ROM_END);
        arg0->unk28 = dmaRequestAndUpdateStateWithSize(&_6636F0_ROM_START, &_6636F0_ROM_END, 0x200);
        arg0->unk2C = 0;
        arg0->unk20 = (s32)&D_80089510_8A110;
        arg0->unk50 = 0;
    } else if (arg0->unk58 != 1) {
        arg0->unk24 = dmaRequestAndUpdateState(&_215120_ROM_START, &_215120_ROM_END);
        arg0->unk28 = dmaRequestAndUpdateStateWithSize(&_6636F0_ROM_START, &_6636F0_ROM_END, 0x200);
        arg0->unk2C = 0;
        arg0->unk20 = (s32)&D_80089510_8A110;
        arg0->unk50 = 0;
    } else {
        arg0->unk24 = dmaRequestAndUpdateState(&_2151D0_ROM_START, &_2151D0_ROM_END);
        arg0->unk28 = dmaRequestAndUpdateStateWithSize(&_663BE0_ROM_START, &_663BE0_ROM_END, 0x200);
        arg0->unk20 = (s32)&D_80089520;
        arg0->unk2C = 0;
        arg0->unk50 = 1;
    }

    arg0->unk3C = dmaRequestAndUpdateStateWithSize(&_6637B0_ROM_START, &_6637B0_ROM_END, 0x858);
    arg0->unk44 = 0;
    arg0->unk46 = 0;
    arg0->unk48 = 0;
    arg0->unk4A = 0x10;
    arg0->unk4C = 0;
    arg0->unk4E = 0;

    setCallback(&func_800B5438_1E24E8);
}

void func_800B5438_1E24E8(func_800B5318_1E23C8_arg *arg0) {
    if (arg0->unk56 == 0) {
        func_80069CF8_6A8F8();
    }

    arg0->unk44 += arg0->unk48;
    arg0->unk46 += arg0->unk4A;

    // mask off the first 8 bits to stop overflow?
    arg0->unk44 = 0xFF & arg0->unk44;
    arg0->unk46 = 0xFF & arg0->unk46;

    func_80004FF8_5BF8(arg0->unk54, arg0);

    arg0->unk56--;
}

void func_800B54B4_1E2564(func_800B5318_1E23C8_arg *arg0) {
    arg0->unk3C = freeNodeMemory(arg0->unk3C);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk24 = freeNodeMemory(arg0->unk24);
}