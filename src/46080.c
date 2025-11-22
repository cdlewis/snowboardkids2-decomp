#include "19E80.h"
#include "56910.h"
#include "5E590.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_3F6670);
USE_ASSET(_3F3EF0);

typedef struct {
    u8 padding0[0x8];
    s32 unk8;
    u8 padding2[0x18];
    s16 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 padding[2];
    s32 unk3C;
} func_8004A9A8_4B5A8_node;

extern void func_8004BCFC_4C8FC(void *);
extern void func_8004AF2C_4BB2C(void *);

extern void func_80048E34_49A34(void);
extern void func_800462D8_46ED8(void);
extern void func_8004B834_4C434(void);
extern u8 func_800698DC_6A4DC(void);
extern void func_800455BC_461BC(void);
extern void func_8004562C_4622C(void);

typedef struct {
    void *unk0;
} func_8004C0D0_4CCD0_arg;

void func_8004C0D0_4CCD0(func_8004C0D0_4CCD0_arg *arg0);

extern s32 D_3F3EF0;
extern s32 D_3F58E0;

void func_8004AE58_4BA58(s32 **);
void func_8004A634_4B234(void *);
void func_8004A96C_4B56C(s32 **);
extern void func_8004AA90_4B690(void *);
extern void func_800458AC_464AC(void);
extern void func_800457E0_463E0(void);

typedef struct {
    DataTable_19E80 *unk0;
    OutputStruct_19E80 unk4;
    u8 _pad[0x38 - 0x4 - sizeof(OutputStruct_19E80)];
    s32 unk38;
    s32 unk3C;
} func_80049300_49F00_arg;

void func_80049300_49F00(func_80049300_49F00_arg *arg0);

typedef struct {
    void *unk0;
} Struct_func_80049404_4A004;

void func_80049404_4A004(Struct_func_80049404_4A004 *arg0);

typedef struct {
    void *unk0;
} func_8004B264_4BE64_arg;

void func_8004B264_4BE64(func_8004B264_4BE64_arg *arg0);

INCLUDE_ASM("asm/nonmatchings/46080", func_80045480_46080);

typedef struct {
    u8 _pad[0xB4];
    s16 unkB4;
} ScheduledTask;

void func_80045564_46164(ScheduledTask *arg0) {
    if (arg0->unkB4 == 1) {
        if ((func_800698DC_6A4DC() & 0xFF) == 0x37) {
            setCallbackWithContinue(func_8004562C_4622C);
            return;
        }
    }
    setCallbackWithContinue(func_800455BC_461BC);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_800455BC_461BC);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004562C_4622C);

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_80045768_46368_arg;

void func_80045768_46368(func_80045768_46368_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

extern void func_80045480_46080(void);

void func_800457A0_463A0(s16 arg0) {
    ScheduledTask *task = scheduleTask(func_80045480_46080, 0, 0, 0xD2);
    if (task != NULL) {
        task->unkB4 = arg0;
    }
}

INCLUDE_ASM("asm/nonmatchings/46080", func_800457E0_463E0);

void func_80045878_46478(void) {
    GameState *gs = (GameState *)getCurrentAllocation();
    gs->unk60 -= 1;
    setCallbackWithContinue(&func_800458AC_464AC);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_800458AC_464AC);

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_8004592C_4652C_arg;

void func_8004592C_4652C(func_8004592C_4652C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

typedef struct {
    u8 _pad[0x3C];
    s32 unk3C;
} func_80045964_46564_node;

void func_80045964_46564(s32 arg0) {
    func_80045964_46564_node *task = (func_80045964_46564_node *)scheduleTask(&func_800457E0_463E0, 0, 0, 0xD3);
    if (task != NULL) {
        task->unk3C = arg0;
    }
}

INCLUDE_ASM("asm/nonmatchings/46080", func_800459A4_465A4);

INCLUDE_ASM("asm/nonmatchings/46080", func_80045A28_46628);

INCLUDE_ASM("asm/nonmatchings/46080", func_80045B3C_4673C);

typedef struct {
    void *unk0;
    u8 _pad[0x8];
    void *unkC;
    void *unk10;
} func_80045C84_46884_arg;

void func_80045C84_46884(func_80045C84_46884_arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80045CC8_468C8);

extern void func_800459A4_465A4(void);

typedef struct {
    u8 _pad[0x14];
    s16 unk14;
    s16 unk16;
} func_80046244_46E44_Task;

void func_80046244_46E44(s16 arg0, s16 arg1) {
    func_80046244_46E44_Task *task = (func_80046244_46E44_Task *)scheduleTask(func_800459A4_465A4, 0, 0, 0xD3);
    if (task != NULL) {
        task->unk14 = arg0;
        task->unk16 = arg1;
    }
}

typedef struct {
    void *unk0;
} Struct_func_8004657C_4717C;

void func_8004657C_4717C(Struct_func_8004657C_4717C *arg0);

void func_80046298_46E98(Struct_func_8004657C_4717C *arg0) {
    arg0->unk0 = loadAsset_34CB50();
    setCleanupCallback(func_8004657C_4717C);
    setCallback(func_800462D8_46ED8);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_800462D8_46ED8);

INCLUDE_ASM("asm/nonmatchings/46080", func_800463F4_46FF4);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046464_47064);

INCLUDE_ASM("asm/nonmatchings/46080", func_800464F4_470F4);

void func_8004657C_4717C(Struct_func_8004657C_4717C *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_800465A8_471A8);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046628_47228);

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
    u8 _pad2[0x10];
    void *unk3C;
} func_80046708_47308_arg;

void func_80046708_47308(func_80046708_47308_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk3C = freeNodeMemory(arg0->unk3C);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004674C_4734C);

void func_8004674C_4734C(DisplayListObject *);
extern void func_8006300C_63C0C(DisplayListObject *);

void func_80046CB4_478B4(DisplayListObject *arg0) {
    func_8004674C_4734C(arg0);
    func_8006300C_63C0C(arg0);
}

extern void func_80063058_63C58(DisplayListObject *);

void func_80046CE0_478E0(DisplayListObject *arg0) {
    func_8004674C_4734C(arg0);
    func_80063058_63C58(arg0);
}

extern void func_800630A4_63CA4(DisplayListObject *);

void func_80046D0C_4790C(DisplayListObject *arg0) {
    func_8004674C_4734C(arg0);
    func_800630A4_63CA4(arg0);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80046D38_47938);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046DCC_479CC);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046F44_47B44);

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_80046FEC_47BEC_arg;

void func_80046FEC_47BEC(func_80046FEC_47BEC_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80047024_47C24);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004711C_47D1C);

INCLUDE_ASM("asm/nonmatchings/46080", func_800471D0_47DD0);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004728C_47E8C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047330_47F30);

INCLUDE_ASM("asm/nonmatchings/46080", func_800473F4_47FF4);

INCLUDE_ASM("asm/nonmatchings/46080", func_800474B4_480B4);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047590_48190);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047660_48260);

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_80047718_48318_arg;

void func_80047718_48318(func_80047718_48318_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80047750_48350);

INCLUDE_ASM("asm/nonmatchings/46080", func_800477E4_483E4);

INCLUDE_ASM("asm/nonmatchings/46080", func_800478FC_484FC);

typedef struct {
    void *unk0;
    u8 _pad[0x8];
    void *unkC;
    void *unk10;
} func_80047A64_48664_arg;

void func_80047A64_48664(func_80047A64_48664_arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80047AA8_486A8);

extern void func_80047750_48350(void);

typedef struct {
    u8 _pad[0x14];
    s16 unk14;
} func_80047E7C_48A7C_ScheduledTask;

void func_80047E7C_48A7C(s16 arg0) {
    func_80047E7C_48A7C_ScheduledTask *result = scheduleTask(func_80047750_48350, 0, 0, 0xD3);
    if (result != NULL) {
        result->unk14 = arg0;
    }
}

typedef struct {
    void *unk0;
} Struct_func_800482A4_48EA4;

extern void func_80047EFC_48AFC(void);
extern void func_800482A4_48EA4(Struct_func_800482A4_48EA4 *);

typedef struct {
    void *unk0;
} Struct_func_80047EBC_48ABC;

void func_80047EBC_48ABC(Struct_func_80047EBC_48ABC *arg0) {
    arg0->unk0 = loadAsset_34CB50();
    setCleanupCallback(&func_800482A4_48EA4);
    setCallbackWithContinue(&func_80047EFC_48AFC);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80047EFC_48AFC);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047F90_48B90);

INCLUDE_ASM("asm/nonmatchings/46080", func_800480A8_48CA8);

INCLUDE_ASM("asm/nonmatchings/46080", func_800481A0_48DA0);

void func_800482A4_48EA4(Struct_func_800482A4_48EA4 *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

typedef struct {
    u8 padding[0x64];
    void *unk64;
} Struct_func_800482D0_48ED0_task;

void func_800482D0_48ED0(void *arg0) {
    Struct_func_800482D0_48ED0_task *task;

    task = (Struct_func_800482D0_48ED0_task *)scheduleTask(func_80047EBC_48ABC, 0, 0, 0xD4);
    if (task != NULL) {
        task->unk64 = arg0;
    }
}

typedef struct {
    void *unk0;
} Struct_func_80048834_49434;

extern void func_80048350_48F50(void);
extern void func_80048834_49434(Struct_func_80048834_49434 *arg0);

void func_80048310_48F10(Struct_func_80048834_49434 *arg0) {
    arg0->unk0 = loadAsset_34CB50();
    setCleanupCallback(&func_80048834_49434);
    setCallbackWithContinue(&func_80048350_48F50);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80048350_48F50);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004841C_4901C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80048540_49140);

INCLUDE_ASM("asm/nonmatchings/46080", func_80048720_49320);

void func_80048834_49434(Struct_func_80048834_49434 *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

typedef struct {
    u8 padding[0xA4];
    void *unkA4;
} Struct_func_80048860_49460_task;

void func_80048860_49460(Player *arg0) {
    Struct_func_80048860_49460_task *task;

    task = (Struct_func_80048860_49460_task *)scheduleTask(&func_80048310_48F10, 0, 0, 0xD4);
    if (task != NULL) {
        task->unkA4 = arg0;
    }
}

INCLUDE_ASM("asm/nonmatchings/46080", func_800488A0_494A0);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004899C_4959C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80048AE8_496E8);

INCLUDE_ASM("asm/nonmatchings/46080", func_80048E34_49A34);

INCLUDE_ASM("asm/nonmatchings/46080", func_80048F0C_49B0C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80049104_49D04);

typedef struct {
    u8 _pad[0x84];
} func_800491CC_49DCC_Element;

typedef struct {
    u8 _pad[0x8];
    func_800491CC_49DCC_Element *unk8;
    u8 _pad2[0xA];
    s16 unk16;
} func_800491CC_49DCC_arg;

extern void func_80048AE8_496E8(func_800491CC_49DCC_Element *arg0, func_800491CC_49DCC_arg *arg1);

void func_800491CC_49DCC(func_800491CC_49DCC_arg *arg0) {
    s32 i;

    for (i = 0; i < arg0->unk16; i++) {
        func_80048AE8_496E8(&arg0->unk8[i], arg0);
    }
}

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    void *unkC;
} func_80049230_49E30_arg;

void func_80049230_49E30(func_80049230_49E30_arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unk4 = freeNodeMemory(arg0->unk4);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}

typedef struct {
    u8 _pad[0x14];
    s16 unk14;
} func_80049280_49E80_Task;

void func_80049280_49E80(s32 arg0) {
    func_80049280_49E80_Task *task = (func_80049280_49E80_Task *)scheduleTask(&func_80048E34_49A34, 0, 0, 0xEA);
    if (task != NULL) {
        task->unk14 = arg0;
    }
}

void func_800492C0_49EC0(void **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(&func_80049404_4A004);
    setCallbackWithContinue(&func_80049300_49F00);
}

extern void func_8004934C_49F4C(void);

void func_80049300_49F00(func_80049300_49F00_arg *arg0) {
    u16 index;

    if (arg0->unk3C == 0) {
        index = 0x33;
    } else {
        index = 0x34;
    }

    getTableEntryByU16Index(arg0->unk0, index, &arg0->unk4);
    arg0->unk38 = 0;
    setCallbackWithContinue(&func_8004934C_49F4C);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004934C_49F4C);

void func_80049404_4A004(Struct_func_80049404_4A004 *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80049430_4A030);

typedef struct {
    Node n;
    s32 unk2C;
    s32 unk30;
    s32 padding[2];
    s32 unk3C;
} NodeWithPayload;

void func_80049794_4A394(void *payload, s32 arg1) {
    NodeWithPayload *temp_v0 = (NodeWithPayload *)scheduleTask(&func_800492C0_49EC0, 0, 0, 0xEB);
    if (temp_v0 != NULL) {
        memcpy((void *)((s32)temp_v0 + 0x10), payload, 0x20);
        temp_v0->unk3C = arg1;
    }
}

INCLUDE_ASM("asm/nonmatchings/46080", func_800497FC_4A3FC);

void func_80049BFC_4A7FC(void) {
    GameState *allocation = (GameState *)getCurrentAllocation();
    if (allocation->unk7A == 0) {
        scheduleTask(&func_80046298_46E98, 0, 0, 0xD3);
    }
}

void func_80049C38_4A838(s32 arg0) {
    void (*func_ptr)(s32) = (void (*)(s32))func_80047E7C_48A7C;
    GameState *allocation;
    u8 check;

    allocation = (GameState *)getCurrentAllocation();
    check = allocation->unk83;
    if (check == 0) {
        func_ptr(arg0);
    }
}

void func_80049C70_4A870(s32 arg0) {
    GameState *allocation;
    u8 check;

    allocation = (GameState *)getCurrentAllocation();
    check = allocation->unk84;
    if (check != 0) {
        return;
    }
    func_80049280_49E80(arg0);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80049CA8_4A8A8);

void func_8004A5E0_4B1E0(void **arg0) {
    *arg0 = dmaRequestAndUpdateStateWithSize(&_3F6670_ROM_START, &_3F6670_ROM_END, 0x388);
    setCleanupCallback(&func_8004A96C_4B56C);
    setCallbackWithContinue(&func_8004A634_4B234);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004A634_4B234);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004A6D4_4B2D4);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004A850_4B450);

void func_8004A96C_4B56C(s32 **arg0) {
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    temp_v0->PAD_5[0xB] += 1;
    *arg0 = freeNodeMemory(*arg0);
}

void func_8004A9A8_4B5A8(void *arg0, s32 arg1, void *arg2) {
    GameState *gs;
    func_8004A9A8_4B5A8_node *node;

    gs = (GameState *)getCurrentAllocation();
    if (gs->PAD_5[0xB] != 0) {
        node = (func_8004A9A8_4B5A8_node *)scheduleTask(&func_8004A5E0_4B1E0, 3U, 0U, 0xEFU);
        if (node != NULL) {
            gs->PAD_5[0xB] -= 1;
            memcpy(&node->unk8, arg0, 0xC);
            memcpy(&node->unk2C, arg2, 0xC);
            node->unk24 = arg1;
        }
    }
}

void func_8004AA50_4B650(s32 *arg0) {
    *arg0 = (s32)load_3ECE40();
    setCleanupCallback(func_8004AE58_4BA58);
    setCallbackWithContinue(&func_8004AA90_4B690);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004AA90_4B690);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004AB50_4B750);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004AD18_4B918);

void func_8004AE58_4BA58(s32 **arg0) {
    GameState *temp_v0;

    temp_v0 = (GameState *)getCurrentAllocation();
    temp_v0->PAD_5[0xB] += 1;
    *arg0 = freeNodeMemory(*arg0);
}

void func_8004AE94_4BA94(s32 arg0) {
    NodeWithPayload *temp_v0;

    getCurrentAllocation();
    temp_v0 = (NodeWithPayload *)scheduleTask(&func_8004AA50_4B650, 3, 0, 0xEFU);
    if (temp_v0 != NULL) {
        temp_v0->unk30 = arg0;
    }
}

void func_8004AED8_4BAD8(void **arg0) {
    *arg0 = dmaRequestAndUpdateStateWithSize(&_3F3EF0_ROM_START, &_3F3EF0_ROM_END, 0x2608);
    setCleanupCallback(&func_8004B264_4BE64);
    setCallbackWithContinue(&func_8004AF2C_4BB2C);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004AF2C_4BB2C);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004AFF8_4BBF8);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B130_4BD30);

void func_8004B264_4BE64(func_8004B264_4BE64_arg *arg0) {
    GameState *allocation;

    allocation = (GameState *)getCurrentAllocation();
    allocation->unk5B++;
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B2A0_4BEA0);

typedef struct {
    void *unk0;
} func_8004B36C_4BF6C_arg;

typedef struct {
    void *unk0;
} func_8004B758_4C358_arg;

extern void func_8004B3B0_4BFB0(void);
extern void func_8004B758_4C358(func_8004B758_4C358_arg *);

void func_8004B36C_4BF6C(func_8004B36C_4BF6C_arg *arg0) {
    arg0->unk0 = loadAssetByIndex_95470(9);
    setCleanupCallback(&func_8004B758_4C358);
    setCallbackWithContinue(&func_8004B3B0_4BFB0);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B3B0_4BFB0);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B4CC_4C0CC);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B648_4C248);

void func_8004B758_4C358(func_8004B758_4C358_arg *arg0) {
    GameState *allocation;

    allocation = (GameState *)getCurrentAllocation();
    allocation->unk5B++;
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

typedef struct {
    u8 pad[0x24];
    void *unk24;
} func_8004B794_4C394_Task;

void func_8004B794_4C394(void *arg0) {
    GameState *allocation;
    func_8004B794_4C394_Task *task;

    allocation = getCurrentAllocation();
    task = (func_8004B794_4C394_Task *)scheduleTask(&func_8004B36C_4BF6C, 3, 0, 0xEF);

    if (task != NULL) {
        task->unk24 = arg0;
        allocation->unk5B -= 1;
    }
}

typedef struct {
    void *unk0;
} func_8004BC20_4C820_arg;

void func_8004BC20_4C820(func_8004BC20_4C820_arg *arg0);

void func_8004B7F4_4C3F4(MemoryAllocatorNode **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(&func_8004BC20_4C820);
    setCallbackWithContinue(&func_8004B834_4C434);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B834_4C434);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B990_4C590);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004BB0C_4C70C);

void func_8004BC20_4C820(func_8004BC20_4C820_arg *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();
    allocation->unk5B++;
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

typedef struct {
    u8 pad[0x24];
    s32 unk24;
} func_8004BC5C_4C85C_Task;

void func_8004BC5C_4C85C(s32 arg0) {
    GameState *allocation;
    func_8004BC5C_4C85C_Task *task;

    allocation = (GameState *)getCurrentAllocation();
    task = (func_8004BC5C_4C85C_Task *)scheduleTask(func_8004B7F4_4C3F4, 3, 0, 0xEF);

    if (task != NULL) {
        task->unk24 = arg0;
        allocation->unk5B -= 1;
    }
}

void func_8004BCBC_4C8BC(MemoryAllocatorNode **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(&func_8004C0D0_4CCD0);
    setCallbackWithContinue(&func_8004BCFC_4C8FC);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004BCFC_4C8FC);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004BE40_4CA40);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004BFBC_4CBBC);

void func_8004C0D0_4CCD0(func_8004C0D0_4CCD0_arg *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();
    allocation->unk5B++;
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void func_8004C10C_4CD0C(void *arg0) {
    GameState *allocation;
    Node *task;

    allocation = (GameState *)getCurrentAllocation();
    task = (Node *)scheduleTask((void *)func_8004BCBC_4C8BC, 3, 0, 0xEF);

    if (task != NULL) {
        task->cleanupCallback = (void (*)(void *))arg0;
        allocation->unk5B--;
    }
}

INCLUDE_RODATA("asm/nonmatchings/46080", D_8009E868_9F468);

INCLUDE_RODATA("asm/nonmatchings/46080", D_8009E870_9F470);

INCLUDE_RODATA("asm/nonmatchings/46080", D_8009E878_9F478);
