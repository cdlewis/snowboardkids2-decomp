#include "common.h"
#include "displaylist.h"
#include "overlay.h"
#include "rand.h"
#include "task_scheduler.h"

USE_ASSET(_647F90);
USE_ASSET(_49B420);

typedef struct {
    void *unk0;
    void *unk4;
    s32 unk8;
    u8 padding[0x16];
    s8 unk22;
    s32 unk24;
    s32 unk28;
    u8 padding2[0x16];
    s8 unk42;
    u8 padding3[0x10];
    s16 unk54;
    s16 unk56;
} func_8000CAF0_D6F0_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} unk0_node;

typedef struct {
    unk0_node *unk0;
    void *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s8 unk20;
    s8 unk21;
    s8 unk22;
    s32 unk24;
    s32 unk28;
    u8 padding2[0xC];
    s32 unk38;
    s32 unk3C;
    s8 unk40;
    s8 unk41;
    s8 unk42;
    u8 padding3[0x5];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s16 unk54;
    s16 unk56;
} func_8000CB94_D794_arg;

typedef struct {
    void *unk0;
    void *unk4;
} func_8000CC84_D884_arg;

typedef struct {
    u8 padding[0xC];
    s32 unkC;
    u8 padding2[0x1C];
    s32 unk2C;
    u8 padding3[0x18];
    s32 unk48;
    s32 unk4C;

    s32 unk50;
    s16 unk54;
    s16 unk56;
} func_8000CAF0_D6F0_task_memory;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} func_8000CCBC_D8BC_arg2;

void func_8000CB94_D794(func_8000CB94_D794_arg *arg0);
void func_8000CC84_D884(func_8000CC84_D884_arg *);
void func_80067EDC_68ADC(u16, void *);

void func_8000CAF0_D6F0(func_8000CAF0_D6F0_arg *arg0) {
    void *temp_v0;

    arg0->unk0 = dmaRequestAndUpdateStateWithSize(&_647F90_ROM_START, &_647F90_ROM_END, 0xF18);
    temp_v0 = dmaRequestAndUpdateStateWithSize(&_49B420_ROM_START, &_49B420_ROM_END, 0x240);
    arg0->unk4 = temp_v0;
    arg0->unk8 = (s32)(temp_v0 + (arg0->unk56 << 6));
    arg0->unk22 = (u8)((randA() & 0x1F) + 0x70);
    arg0->unk54 = 0;
    arg0->unk28 = (s32)arg0->unk8;
    arg0->unk42 = (u8)arg0->unk22;
    setCleanupCallback(&func_8000CC84_D884);
    setCallbackWithContinue(&func_8000CB94_D794);
}

void func_8000CB94_D794(func_8000CB94_D794_arg *arg0) {
    loadAssetMetadata(&arg0->unk8, arg0->unk0, arg0->unk54);

    arg0->unk38 = arg0->unk18;
    arg0->unk3C = arg0->unk1C;
    arg0->unk40 = arg0->unk20;
    arg0->unk41 = arg0->unk21;

    func_80067EDC_68ADC(0, &arg0->unk8);
    func_80067EDC_68ADC(0, &arg0->unk28);

    if (arg0->unk54 != 0) {
        s32 i;
        unk0_node *node = (unk0_node *)&arg0->unk0;
        for (i = 0; i < 2; i++) {
            node[i].unkC += arg0->unk48;
            node[i].unk10 += arg0->unk4C;
            node[i].unk14 += arg0->unk50;
        }
    }

    arg0->unk54++;
    if (arg0->unk54 == 5) {
        func_80069CF8_6A8F8();
    }
}

void func_8000CC84_D884(func_8000CC84_D884_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void func_8000CCBC_D8BC(void *arg0, void *arg1, func_8000CCBC_D8BC_arg2 *arg2, s32 arg3) {
    func_8000CAF0_D6F0_task_memory *temp_v0 =
        (func_8000CAF0_D6F0_task_memory *)scheduleTask(&func_8000CAF0_D6F0, 0, 0, 0);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unkC, arg0, 0xC);
        memcpy(&temp_v0->unk2C, arg1, 0xC);
        temp_v0->unk56 = arg3;
        temp_v0->unk48 = (s32)((s32)arg2->unk0 / 2);
        temp_v0->unk4C = (s32)((s32)arg2->unk4 / 2);
        temp_v0->unk50 = (s32)((s32)arg2->unk8 / 2);
    }
}

void func_8000CD88_D988(void) {
    dmaRequestAndUpdateStateWithSize(&_647F90_ROM_START, &_647F90_ROM_END, 0xF18);
}

void func_8000CDB4_D9B4(void) {
    dmaRequestAndUpdateStateWithSize(&_49B420_ROM_START, &_49B420_ROM_END, 0x240);
}

void func_8000CDE0_D9E0(void *node) {
    freeNodeMemory(node);
}

void func_8000CDFC_D9FC(void *node) {
    freeNodeMemory(node);
}
