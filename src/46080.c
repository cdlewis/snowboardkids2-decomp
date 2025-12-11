#include "19E80.h"
#include "3E160.h"
#include "51060.h"
#include "56910.h"
#include "594E0.h"
#include "5AA90.h"
#include "5E590.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "overlay.h"
#include "rand.h"
#include "task_scheduler.h"

USE_ASSET(_3F6670);
USE_ASSET(_3F6950);
USE_ASSET(_3F3EF0);

typedef struct {
    void *unk0;
    loadAssetMetadata_arg unk4;
    u8 _pad[0x4];
    u16 unk24;
    u8 _pad2[0x2];
    s32 unk28;
} Struct_func_8004657C_4717C;

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

extern void func_8004AFF8_4BBF8(void *);

typedef struct {
    void *unk0;
    void *unk4;
    u8 _pad[0x1C];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    u8 _pad2[0x2];
    s16 unk32;
    u8 _pad3[0x2];
    s16 unk36;
    s16 unk38;
} func_8004AF2C_4BB2C_arg;

typedef struct {
    u8 _pad0[0x4C];
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    u8 _pad58[0x3F4];
    s32 unk44C;
    s32 unk450;
    s32 unk454;
    u8 _pad458[0x63C];
    u16 unkA94;
    u8 _padA96[0xFE];
    u16 unkB94;
    u8 _padB96[0x22];
    u8 unkBB8;
} func_8004B834_Player;

typedef struct {
    void *unk0;
    void *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 _pad14[0x10];
    func_8004B834_Player *unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s16 unk34;
    s16 unk36;
    s16 unk38;
} func_8004B834_4C434_arg;

typedef struct {
    u8 _pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    u8 _pad20[0x1C];
    s32 unk3C;
} FunctionArg_80047330;

typedef struct {
    u8 pad1[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    u8 pad2[0x1C];
    s32 unk3C;
    s32 unk40;
} func_800473F4_47FF4_arg;

extern void func_8004AF2C_4BB2C(func_8004AF2C_4BB2C_arg *);

extern void *D_80090CEC_918EC[];
extern void *D_80090CF0_918F0[];
extern s32 D_80090CF4_918F4[];
extern u8 D_80090CA8_918A8[][5];
extern u8 D_80090CE0_918E0[];
extern u8 D_80090CBC_918BC[][9];
typedef struct {
    s32 unk0;
    func_80066444_67044_arg1 unk4;
    s16 unk38;
} func_8004B990_4C590_arg;

void func_8004B990_4C590(func_8004B990_4C590_arg *arg0);
extern void func_8004562C_4622C(void);

extern s32 D_80090E20_91A20;
extern s32 D_80090E40_91A40;
extern s32 D_80090E4C_91A4C;
extern s16 D_80090E50_91A50;
extern s32 D_80090BC8_917C8[3];
extern s32 D_80090BBC_917BC[3];
extern s32 D_80090B98_91798[];
extern s32 D_80090BB0_917B0;
extern s32 D_80090B80_91780;

extern s32 gFrameCounter;
extern u16 D_8009ADE0_9B9E0;

void func_80047330_47F30(FunctionArg_80047330 *arg0);
void func_800473F4_47FF4(func_800473F4_47FF4_arg *arg0);
void func_800474B4_480B4(func_800473F4_47FF4_arg *arg0);
void func_8004657C_4717C(Struct_func_8004657C_4717C *arg0);
void func_800462D8_46ED8(Struct_func_8004657C_4717C *);

typedef struct {
    u8 _pad0[0x4];
    func_80066444_67044_arg1 unk4;
    s16 unk38;
} func_8004BB0C_4C70C_arg;

typedef struct {
    u8 _pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    u8 _pad20[0x1C];
    s32 unk3C;
} func_80047590_48190_arg;

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x1C];
    s32 unk3C;
} func_80047660_48260_arg;

void func_80047590_48190(func_80047590_48190_arg *arg0);
void func_80047660_48260(func_80047660_48260_arg *arg0);

typedef struct {
    void *unk0;
} func_8004C0D0_4CCD0_arg;

void func_8004C0D0_4CCD0(func_8004C0D0_4CCD0_arg *arg0);

void func_8004AE58_4BA58(s32 **);
void func_8004A96C_4B56C(s32 **);
extern void func_8004A6D4_4B2D4(void);

typedef struct {
    DataTable_19E80 *unk0;
    OutputStruct_19E80 unk4;
    u8 _pad[0x30 - 0x4 - sizeof(OutputStruct_19E80)];
    s32 unk30;
    GameStateUnk44Unk2C0 *unk34;
    s32 unk38;
    s32 unk3C;
} func_80049300_49F00_arg;

void func_80049300_49F00(func_80049300_49F00_arg *arg0);
void func_8004934C_49F4C(func_80049300_49F00_arg *arg0);

typedef struct {
    void *unk0;
} Struct_func_80049404_4A004;

void func_80049404_4A004(Struct_func_80049404_4A004 *arg0);

typedef struct {
    void *unk0;
} func_8004B264_4BE64_arg;

void func_8004B264_4BE64(func_8004B264_4BE64_arg *arg0);

extern s32 identityMatrix[];
extern void *D_80094DD0_959D0;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_80045768_46368_arg;

void func_80045768_46368(func_80045768_46368_arg *);

typedef struct {
    u8 _pad[0xB4];
    s16 unkB4;
} ScheduledTask;

void func_80045564_46164(ScheduledTask *);

typedef struct {
    u8 _pad0[0x20];
    s32 unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 _pad30[0xC];
    u8 _pad3C[0x20];
    s32 unk5C;
    void *unk60;
    void *unk64;
    s32 unk68;
    u8 _pad6C[0xC];
    u8 _pad78[0x20];
    void *unk98;
    void *unk9C;
    void *unkA0;
    s32 unkA4;
    u8 _padA8[0xC];
    s16 unkB4;
} func_80045480_46080_arg;

void func_80045480_46080(func_80045480_46080_arg *arg0) {
    void *identity = identityMatrix;

    memcpy(arg0, identity, 0x20);

    arg0->unk20 = func_80055E40_56A40(arg0->unkB4);
    arg0->unk24 = func_80055DC4_569C4(arg0->unkB4);
    arg0->unk28 = func_80055DF8_569F8(arg0->unkB4);
    arg0->unk2C = 0;

    memcpy((u8 *)arg0 + 0x3C, identity, 0x20);

    arg0->unk5C = func_80055E54_56A54(arg0->unkB4);
    arg0->unk68 = 0;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;

    memcpy((u8 *)arg0 + 0x78, identity, 0x20);

    arg0->unk98 = &D_80094DD0_959D0;
    arg0->unkA4 = 0;
    arg0->unk9C = arg0->unk24;
    arg0->unkA0 = arg0->unk28;

    setCleanupCallback(&func_80045768_46368);
    setCallback(&func_80045564_46164);
}

typedef struct {
    u8 _pad[0x3C];
    u8 unk3C[0x20];
    s32 unk5C;
} func_800455BC_461BC_arg;

void func_800455BC_461BC(func_800455BC_461BC_arg *arg0);

void func_80045564_46164(ScheduledTask *arg0) {
    if (arg0->unkB4 == 1) {
        if ((func_800698DC_6A4DC() & 0xFF) == 0x37) {
            setCallbackWithContinue(func_8004562C_4622C);
            return;
        }
    }
    setCallbackWithContinue(func_800455BC_461BC);
}

void func_800455BC_461BC(func_800455BC_461BC_arg *arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        func_800630F0_63CF0(i, arg0);
    }

    if (arg0->unk5C != 0) {
        for (i = 0; i < 4; i++) {
            func_80065DA8_669A8(i + 4, (DisplayListObject *)&arg0->unk3C);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004562C_4622C);

void func_80045768_46368(func_80045768_46368_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800457A0_463A0(s32 arg0) {
    ScheduledTask *task = scheduleTask(func_80045480_46080, 0, 0, 0xD2);
    if (task != NULL) {
        task->unkB4 = arg0;
    }
}

typedef struct {
    u8 _pad[0x30];
    u8 unk30[0x2C];
    u8 unk5C;
    u8 _pad5D[0x19];
    u8 unk76;
} GameState_46080;

typedef struct {
    u8 _pad[0x14];
    s32 unk14[3];
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    u8 _pad3[0x10];
    s32 unk40;
} func_800457E0_463E0_arg;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_8004592C_4652C_arg;

void func_80045878_46478(void);
void func_8004592C_4652C(func_8004592C_4652C_arg *);

void func_800457E0_463E0(func_800457E0_463E0_arg *arg0) {
    u16 rotation;
    GameState_46080 *allocation;

    allocation = (GameState_46080 *)getCurrentAllocation();
    rotation = func_800625A4_631A4(&allocation->unk30, &arg0->unk14);
    createYRotationMatrix((Mat3x3Padded *)arg0, rotation);
    arg0->unk20 = (void *)((u32)func_80055E68_56A68(allocation->unk5C) + 0x30);
    arg0->unk24 = func_80055DC4_569C4(allocation->unk5C);
    arg0->unk28 = func_80055DF8_569F8(allocation->unk5C);
    arg0->unk2C = NULL;
    arg0->unk40 = 1;
    setCleanupCallback(func_8004592C_4652C);
    setCallback(func_80045878_46478);
}

typedef struct {
    u8 _pad[0x20];
    DisplayLists *unk20;
    u8 _pad2[0x3C - 0x24];
    s32 unk3C;
    s32 unk40;
} func_800458AC_464AC_arg;

void func_800458AC_464AC(func_800458AC_464AC_arg *arg0);

void func_80045878_46478(void) {
    GameState *gs = (GameState *)getCurrentAllocation();
    gs->unk60 -= 1;
    setCallbackWithContinue(&func_800458AC_464AC);
}

void func_800458AC_464AC(func_800458AC_464AC_arg *arg0) {
    GameState *allocation;
    s32 counter;
    s32 index;
    Player *data;
    u8 val1;
    u8 val2;
    func_80055E68_56A68_result *result;

    allocation = (GameState *)getCurrentAllocation();
    counter = arg0->unk40;

    if (counter != 0) {
        index = arg0->unk3C;
        data = allocation->players;

        val1 = data[index].unkBC5;
        val2 = allocation->unk74;

        if (val1 == val2) {
            arg0->unk40 = counter - 1;
            result = func_80055E68_56A68(allocation->memoryPoolId);
            arg0->unk20 = &result->unk40;
        }
    }

    enqueueDisplayListObject(arg0->unk3C, (DisplayListObject *)arg0);
}

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

typedef struct {
    s16 unk0;
    u8 _pad2[0x6];
    u8 unk8[0xC];
} Entry_46628;

typedef struct {
    void *unk0;
    void *unk4;
    Entry_46628 *volatile unk8;
    void *unkC;
    void *unk10;
    s16 unk14;
    s16 unk16;
    s16 unk18;
} func_80046244_46E44_Task;

typedef func_80046244_46E44_Task func_80045C84_46884_arg;

extern void func_80045C84_46884(func_80045C84_46884_arg *);
extern void func_80045A28_46628(func_80046244_46E44_Task *);
extern s32 D_8009A8A4_9B4A4;
extern void func_80045B3C_4673C(void);
extern s32 D_80090AF0_916F0[];
extern s32 D_80090AF4_916F4[];
extern s32 D_80090AF8_916F8[];

void func_800459A4_465A4(func_80046244_46E44_Task *arg0) {
    s16 temp_v1;
    s32 index;

    arg0->unkC = func_80055D7C_5697C(arg0->unk14);
    temp_v1 = arg0->unk16;
    index = temp_v1 * 3;
    arg0->unk10 = dmaRequestAndUpdateStateWithSize(
        (void *)D_80090AF0_916F0[index],
        (void *)D_80090AF4_916F4[index],
        D_80090AF8_916F8[index]
    );
    arg0->unk0 = 0;
    setCleanupCallback(func_80045C84_46884);
    setCallback(func_80045A28_46628);
}

void func_80045A28_46628(func_80046244_46E44_Task *arg0) {
    s32 i;
    s32 offset;
    s32 *ptr;
    Entry_46628 *entries;
    Entry_46628 *loop_ent;
    void *header;
    s32 sp10[2];

    header = arg0->unk10;
    arg0->unk4 = (void *)((s32)header + *(s32 *)header);
    arg0->unk8 = (Entry_46628 *)((s32)arg0->unk10 + *(s32 *)((s32)arg0->unk10 + 4));

    entries = arg0->unk8;
    arg0->unk18 = 0;

    if (entries->unk0 >= 0) {
        loop_ent = entries;
        do {
            arg0->unk18++;
        } while (loop_ent[arg0->unk18].unk0 >= 0);
    }

    i = 0;
    arg0->unk0 = allocateNodeMemory(arg0->unk18 << 6);

    if (arg0->unk18 > 0) {
        ptr = &D_8009A8A4_9B4A4;
        offset = 0;
        do {
            memcpy(ptr, (u8 *)(offset + (s32)arg0->unk8) + 8, 0xC);
            func_8006BFB8_6CBB8(ptr - 5, (u8 *)arg0->unk0 + (i << 6));
            i++;
            offset += 0x14;
        } while (i < arg0->unk18);
    }

    setCallback(&func_80045B3C_4673C);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80045B3C_4673C);

void func_80045C84_46884(func_80045C84_46884_arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80045CC8_468C8);

void func_80046244_46E44(s32 arg0, s16 arg1) {
    func_80046244_46E44_Task *task = (func_80046244_46E44_Task *)scheduleTask(func_800459A4_465A4, 0, 0, 0xD3);
    if (task != NULL) {
        task->unk14 = arg0;
        task->unk16 = arg1;
    }
}

void func_80046298_46E98(Struct_func_8004657C_4717C *arg0) {
    arg0->unk0 = loadAsset_34CB50();
    setCleanupCallback(func_8004657C_4717C);
    setCallback(func_800462D8_46ED8);
}

extern void func_80046464_47064(Struct_func_8004657C_4717C *);
void func_800463F4_46FF4(Struct_func_8004657C_4717C *);

void func_800462D8_46ED8(Struct_func_8004657C_4717C *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();

    loadAssetMetadata(&arg0->unk4, arg0->unk0, 0);
    arg0->unk4.unk0 = (loadAssetMetadata_arg *)((u8 *)state->unk44 + 0x40);

    switch (state->memoryPoolId) {
        case 0:
            memcpy(&arg0->unk4.unk4, state->unk48 + 0x24, 0xC);
            setCallback(func_80046464_47064);
            break;
        case 6:
            memcpy(&arg0->unk4.unk4, state->unk48 + 0x1C8, 0xC);
            setCallback(func_800463F4_46FF4);
            break;
        case 1:
            memcpy(&arg0->unk4.unk4, state->unk48 + 0x1EC, 0xC);
            setCallback(func_800463F4_46FF4);
            break;
        case 2:
            memcpy(&arg0->unk4.unk4, state->unk48 + 0x1F8, 0xC);
            setCallback(func_800463F4_46FF4);
            break;
        case 5:
            memcpy(&arg0->unk4.unk4, state->unk48 + 0x204, 0xC);
            setCallback(func_800463F4_46FF4);
            break;
        case 8:
            memcpy(&arg0->unk4.unk4, state->unk48 + 0x234, 0xC);
            setCallback(func_800463F4_46FF4);
            break;
        case 9:
            memcpy(&arg0->unk4.unk4, state->unk48 + 0x240, 0xC);
            setCallback(func_800463F4_46FF4);
            break;
    }
}

void func_800463F4_46FF4(Struct_func_8004657C_4717C *arg0) {
    s32 i;
    u16 counter;

    counter = arg0->unk24 + 1;
    arg0->unk24 = counter & 0xF;

    if ((counter & 0x7) != 0) {
        loadAssetMetadata(&arg0->unk4, arg0->unk0, (counter & 0xF) >> 3);
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
    }
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80046464_47064);

void func_800464F4_470F4(Struct_func_8004657C_4717C *arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_a2;

    temp_v0 = arg0->unk4.unk4 + 0x27D76;
    temp_v1 = arg0->unk4.unkC + 0x44DDE;
    temp_a0 = arg0->unk28 + 0xFFFE8000;
    temp_a1 = arg0->unk4.unk8 + temp_a0;
    temp_a2 = 0x2C150614 < temp_a1;

    arg0->unk4.unk4 = temp_v0;
    arg0->unk4.unkC = temp_v1;
    arg0->unk28 = temp_a0;
    arg0->unk4.unk8 = temp_a1;

    if (temp_a2 == 0) {
        func_80069CF8_6A8F8();
    }

    func_800463F4_46FF4(arg0);
}

void func_8004657C_4717C(Struct_func_8004657C_4717C *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

typedef union {
    s16 halfword;
    struct {
        u8 high;
        u8 low;
    } bytes;
} HalfwordBytes;

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    void *unkC;
} Struct_unk20;

typedef struct {
    u8 _pad[0x20];
    Struct_unk20 *unk20;
    u8 _pad2[0xC];
    s32 unk30;
    u8 _pad3[0x10];
    HalfwordBytes unk44;
    HalfwordBytes unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
} func_80046628_47228_arg;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
    void *unk2C;
    u8 _pad2[0xC];
    void *unk3C;
    u8 _pad3[0x4];
    HalfwordBytes unk44;
    HalfwordBytes unk46;
    s16 unk48;
} func_80046708_47308_arg;

extern s32 identityMatrix[];
void func_80046628_47228(func_80046628_47228_arg *);
void func_80046708_47308(func_80046708_47308_arg *arg0);

void func_800465A8_471A8(func_80046708_47308_arg *arg0) {
    arg0->unk24 = func_80055DC4_569C4(arg0->unk48);
    arg0->unk28 = func_80055DF8_569F8(arg0->unk48);
    arg0->unk2C = NULL;
    memcpy(arg0, identityMatrix, 0x20);
    arg0->unk3C = func_80055D7C_5697C(arg0->unk48);
    arg0->unk44.halfword = 0;
    arg0->unk46.halfword = 0;
    setCleanupCallback(&func_80046708_47308);
    setCallback(&func_80046628_47228);
}

extern void func_80046CB4_478B4(DisplayListObject *);
extern void func_80046CE0_478E0(DisplayListObject *);
extern void func_80046D0C_4790C(DisplayListObject *);

void func_80046628_47228(func_80046628_47228_arg *arg0) {
    u8 byte_45;
    u8 byte_47;
    s32 i;

    arg0->unk44.halfword = arg0->unk44.halfword + arg0->unk4A;
    byte_45 = arg0->unk44.bytes.low;

    arg0->unk46.halfword = arg0->unk46.halfword + arg0->unk4C;
    byte_47 = arg0->unk46.bytes.low;

    arg0->unk30 = 0;
    arg0->unk44.halfword = byte_45;
    arg0->unk46.halfword = byte_47;

    for (i = 0; i < 4; i++) {
        if (arg0->unk20->unk4 != NULL) {
            debugEnqueueCallback(i, 1, &func_80046CB4_478B4, arg0);
        }

        if (arg0->unk20->unk8 != NULL) {
            debugEnqueueCallback(i, 3, &func_80046CE0_478E0, arg0);
        }

        if (arg0->unk20->unkC != NULL) {
            debugEnqueueCallback(i, 5, &func_80046D0C_4790C, arg0);
        }
    }
}

void func_80046708_47308(func_80046708_47308_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
    arg0->unk3C = freeNodeMemory(arg0->unk3C);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004674C_4734C);

void func_8004674C_4734C(DisplayListObject *);

void func_80046CB4_478B4(DisplayListObject *arg0) {
    func_8004674C_4734C(arg0);
    func_8006300C_63C0C(arg0);
}

void func_80046CE0_478E0(DisplayListObject *arg0) {
    func_8004674C_4734C(arg0);
    func_80063058_63C58(arg0);
}

void func_80046D0C_4790C(DisplayListObject *arg0) {
    func_8004674C_4734C(arg0);
    func_800630A4_63CA4(arg0);
}

typedef struct {
    u8 _pad[0x20];
    void *unk20;
    u8 _pad2[0x24];
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
} func_80046D38_47938_Task;

void func_80046D38_47938(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_80046D38_47938_Task *task = (func_80046D38_47938_Task *)scheduleTask(&func_800465A8_471A8, 0, 0, 0xD2);
    if (task != NULL) {
        task->unk20 = arg0;
        task->unk48 = arg1;
        task->unk4A = arg2;
        task->unk4C = arg3;
        task->unk4E = arg4;
        task->unk50 = arg5;
    }
}

typedef struct {
    u8 _pad[0x3C];
    u8 unk3C[0x3C];
    u8 unk78[0x3C];
    s16 unkB4;
} func_80046F44_47B44_arg;

void func_80046F44_47B44(func_80046F44_47B44_arg *arg0);

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_80046FEC_47BEC_arg_fwd;

void func_80046FEC_47BEC(func_80046FEC_47BEC_arg_fwd *arg0);

typedef struct {
    u8 unk0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    u8 _pad2[0xC];
    u8 unk3C[0x14];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    void *unk5C;
    u8 _pad3[0x18];
    u8 unk78[0x18];
    s32 unk90;
    u8 _pad4[4];
    void *unk98;
} func_80046DCC_479CC_arg;

void func_80046DCC_479CC(func_80046DCC_479CC_arg *arg0) {
    s32 sp10[3];
    GameState_46080 *allocation;
    D_80090F90_91B90_item *s1;
    u16 rotation;
    func_80055E68_56A68_result *displayLists;

    allocation = (GameState_46080 *)getCurrentAllocation();
    rotation = func_800625A4_631A4(&allocation->unk30, sp10) + 0x800;
    s1 = func_80055D10_56910(allocation->unk5C);

    arg0->unk20 = func_80055E68_56A68(allocation->unk5C);
    arg0->unk24 = func_80055DC4_569C4(allocation->unk5C);
    arg0->unk28 = func_80055DF8_569F8(allocation->unk5C);
    arg0->unk2C = NULL;

    createYRotationMatrix((Mat3x3Padded *)arg0, (u16)(rotation + s1->unk8));

    arg0->unk18 = sp10[1];
    transformVector2(&D_80090B80_91780, arg0, sp10);

    arg0->unk14 = s1->unk0 + sp10[0];
    arg0->unk1C = s1->unk4 + sp10[2];

    memcpy(arg0->unk3C, arg0, 0x3C);

    displayLists = func_80055E68_56A68(allocation->unk5C);
    arg0->unk5C = (void *)((u32)displayLists + 0x20);
    createYRotationMatrix((Mat3x3Padded *)arg0->unk3C, 0x1000);

    arg0->unk50 = s1->unkC;
    arg0->unk54 = s1->unk10;
    arg0->unk58 = s1->unk14;

    if (allocation->unk5C == 4) {
        memcpy(arg0->unk78, arg0, 0x3C);
        displayLists = func_80055E68_56A68(allocation->unk5C);
        arg0->unk98 = (void *)((u32)displayLists + 0x90);
        arg0->unk90 = arg0->unk90 + 0xAB0000;
    }

    setCleanupCallback(func_80046FEC_47BEC);
    setCallback(func_80046F44_47B44);
}

void func_80046F44_47B44(func_80046F44_47B44_arg *arg0) {
    GameState_46080 *allocation;
    s32 i;
    s16 rotation;

    allocation = (GameState_46080 *)getCurrentAllocation();

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)&arg0->unk3C);
    }

    if (allocation->unk5C == 4) {
        rotation = arg0->unkB4 + 0x80;
        arg0->unkB4 = rotation;
        createYRotationMatrix((Mat3x3Padded *)&arg0->unk78, rotation);

        for (i = 0; i < 4; i++) {
            enqueueDisplayListObject(i, (DisplayListObject *)&arg0->unk78);
        }
    }
}

void func_80046FEC_47BEC(func_80046FEC_47BEC_arg_fwd *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

typedef struct {
    DisplayListObject displayListObject;
    s32 unk3C;
} func_8004711C_47D1C_arg;

extern s32 D_80090B8C_9178C[3];

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_80047718_48318_arg;

void func_80047718_48318(func_80047718_48318_arg *);
void func_8004711C_47D1C(func_8004711C_47D1C_arg *);

void func_80047024_47C24(func_8004711C_47D1C_arg *arg0) {
    s32 spStack[3];
    GameState_46080 *temp_s0;
    u16 temp_s2;
    D_80090F90_91B90_item *temp_s3;

    temp_s0 = (GameState_46080 *)getCurrentAllocation();
    temp_s2 = func_800625A4_631A4(&temp_s0->unk30, spStack) + 0x800;
    temp_s3 = func_80055D10_56910(temp_s0->unk5C);

    arg0->displayListObject.unk20 = (void *)((u32)func_80055E68_56A68(temp_s0->unk5C) + 0x10);
    arg0->displayListObject.unk24 = func_80055DC4_569C4(temp_s0->unk5C);
    arg0->displayListObject.unk28 = func_80055DF8_569F8(temp_s0->unk5C);
    arg0->displayListObject.unk2C = 0;

    createYRotationMatrix((Mat3x3Padded *)arg0, temp_s2 + temp_s3->unk8);
    arg0->displayListObject.unk10.position.Y = spStack[1];

    transformVector2(&D_80090B8C_9178C, arg0, spStack);

    arg0->displayListObject.unk10.position.X = temp_s3->unk0 + spStack[0];
    arg0->displayListObject.unk10.position.Z = temp_s3->unk4 + spStack[2];
    arg0->unk3C = 0x30;

    setCleanupCallback(&func_80047718_48318);
    setCallbackWithContinue(&func_8004711C_47D1C);
}

typedef struct {
    u8 _pad[0x76];
    u8 unk76;
} Allocation_47D1C;

void func_800471D0_47DD0(func_8004711C_47D1C_arg *arg0);

void func_8004711C_47D1C(func_8004711C_47D1C_arg *arg0) {
    s32 vec[3];
    s32 i;
    Allocation_47D1C *alloc;

    alloc = (Allocation_47D1C *)getCurrentAllocation();

    if (alloc->unk76 == 0) {
        transformVector2(D_80090B98_91798, arg0, vec);

        arg0->displayListObject.unk10.position.X += vec[0];
        arg0->displayListObject.unk10.position.Z += vec[2];

        if (arg0->unk3C != 0) {
            arg0->unk3C--;
        } else {
            arg0->unk3C = 0xB4;
            setCallback(func_800471D0_47DD0);
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

extern s32 D_80090BA4_917A4[3];

typedef struct {
    u8 _pad[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    u8 _pad2[0x1C];
    s32 unk3C;
} TaskStruct;

void func_8004728C_47E8C(TaskStruct *);

void func_800471D0_47DD0(func_8004711C_47D1C_arg *arg0) {
    Allocation_47D1C *allocation;
    s32 stackVec[3];
    s32 i;

    allocation = (Allocation_47D1C *)getCurrentAllocation();

    if (allocation->unk76 == 0) {
        transformVector2(D_80090BA4_917A4, arg0, stackVec);

        arg0->displayListObject.unk10.position.X += stackVec[0];
        arg0->displayListObject.unk10.position.Y += stackVec[1];
        arg0->displayListObject.unk10.position.Z += stackVec[2];

        if (arg0->unk3C != 0) {
            arg0->unk3C -= 1;
        } else {
            setCallback(&func_8004728C_47E8C);
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

typedef struct {
    u8 _pad[0xC];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 _pad2[0x44];
    u8 unk5C;
} AllocationStruct;

void func_8004728C_47E8C(TaskStruct *arg0) {
    s32 vec[3];
    AllocationStruct *alloc;
    void *matrix;

    alloc = (AllocationStruct *)getCurrentAllocation();
    createYRotationMatrix(arg0, 0x1000);
    matrix = func_80055D10_56910(alloc->unk5C);
    transformVector2(D_80090BC8_917C8, arg0, vec);

    arg0->unk14 = ((AllocationStruct *)matrix)->unkC + vec[0];
    arg0->unk18 = ((AllocationStruct *)matrix)->unk10 + vec[1];
    arg0->unk1C = ((AllocationStruct *)matrix)->unk14 + vec[2];
    arg0->unk3C = 0x32;

    setCallbackWithContinue(func_80047330_47F30);
}

typedef struct {
    u8 _pad0[0x76];
    u8 unk76;
} AllocationData;

void func_80047330_47F30(FunctionArg_80047330 *arg0) {
    AllocationData *allocation = getCurrentAllocation();
    s32 i;
    s32 tempVec[3];

    if (allocation->unk76 == 0) {
        transformVector2(D_80090BA4_917A4, arg0, tempVec);
        arg0->unk14 += tempVec[0];
        arg0->unk18 += tempVec[1];
        arg0->unk1C += tempVec[2];

        if (arg0->unk3C != 0) {
            arg0->unk3C--;
        } else {
            arg0->unk3C = 0x2C;
            setCallback(&func_800473F4_47FF4);
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

typedef struct {
    u8 pad[0x76];
    u8 unk76;
} Allocation_800473F4;

void func_800473F4_47FF4(func_800473F4_47FF4_arg *arg0) {
    s32 vec[3];
    s32 i;
    Allocation_800473F4 *temp_v0;

    temp_v0 = getCurrentAllocation();
    i = 0;

    if (temp_v0->unk76 == 0) {
        transformVector2(D_80090B98_91798, arg0, vec);

        arg0->unk14 += vec[0];
        arg0->unk18 += vec[1];
        arg0->unk1C += vec[2];

        if (arg0->unk3C != 0) {
            arg0->unk3C--;
        } else {
            arg0->unk40 = 0;
            setCallback(func_800474B4_480B4);
        }
        i = 0;
    }

    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        i++;
    } while (i < 4);
}

typedef struct {
    u8 pad0[0x76];
    u8 unk76;
} Allocation_800474B4;

void func_800474B4_480B4(func_800473F4_47FF4_arg *arg0) {
    Allocation_800474B4 *allocation;
    s32 vector[3];
    s32 i;
    s32 rotation;
    u16 angle;

    allocation = getCurrentAllocation();

    if (allocation->unk76 == 0) {
        rotation = arg0->unk40;
        angle = rotation + 0x1080;
        arg0->unk40 = rotation + 0x80;
        createYRotationMatrix((Mat3x3Padded *)arg0, angle);
        transformVector2(&D_80090BB0_917B0, arg0, vector);

        arg0->unk14 += vector[0];
        arg0->unk18 += vector[1];
        arg0->unk1C += vector[2];

        if (arg0->unk40 == 0x1080) {
            arg0->unk3C = 0x2C;
            setCallback(&func_80047590_48190);
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_80047590_48190(func_80047590_48190_arg *arg0) {
    Allocation_47D1C *allocation;
    s32 vec[3];
    s32 i;

    allocation = (Allocation_47D1C *)getCurrentAllocation();

    if (allocation->unk76 == 0) {
        createYRotationMatrix((Mat3x3Padded *)arg0, 0);
        transformVector2(D_80090B98_91798, arg0, vec);

        arg0->unk14 += vec[0];
        arg0->unk18 += vec[1];
        arg0->unk1C += vec[2];

        if (arg0->unk3C != 0) {
            arg0->unk3C -= 1;
        } else {
            arg0->unk3C = 0x32;
            setCallback(&func_80047660_48260);
        }
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_80047660_48260(func_80047660_48260_arg *arg0) {
    GameState *alloc;
    s32 stackBuffer[3];
    s32 i;

    alloc = (GameState *)getCurrentAllocation();

    if (alloc->gamePaused == 0) {
        transformVector2(D_80090BBC_917BC, arg0, stackBuffer);

        arg0->unk14 += stackBuffer[0];
        arg0->unk18 += stackBuffer[1];
        arg0->unk1C += stackBuffer[2];

        if (arg0->unk3C != 0) {
            arg0->unk3C--;
        } else {
            func_80069CF8_6A8F8();
        }

        i = 0;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_80047718_48318(func_80047718_48318_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

extern void func_800477E4_483E4(void);
extern void *D_80090BD4_917D4[];
extern void *D_80090BD8_917D8[];
extern s32 D_80090BDC_917DC[];

typedef struct {
    void *unk0;
    u8 _pad[0x8];
    void *unkC;
    void *unk10;
    s16 unk14;
} func_80047A64_48664_arg;

void func_80047A64_48664(func_80047A64_48664_arg *arg0);

void func_80047750_48350(func_80047A64_48664_arg *arg0) {
    s16 index;
    s32 arrayOffset;

    arg0->unkC = dmaRequestAndUpdateStateWithSize(&_3F6670_ROM_START, &_3F6950_ROM_START, 0x388);

    index = arg0->unk14;
    arrayOffset = index * 3;

    arg0->unk10 = dmaRequestAndUpdateStateWithSize(
        D_80090BD4_917D4[arrayOffset],
        D_80090BD8_917D8[arrayOffset],
        D_80090BDC_917DC[arrayOffset]
    );

    arg0->unk0 = 0;
    setCleanupCallback(func_80047A64_48664);
    setCallback(func_800477E4_483E4);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_800477E4_483E4);

INCLUDE_ASM("asm/nonmatchings/46080", func_800478FC_484FC);

void func_80047A64_48664(func_80047A64_48664_arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80047AA8_486A8);

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

extern void func_800482A4_48EA4(Struct_func_800482A4_48EA4 *);

typedef struct {
    void *unk0;
} Struct_func_80047EBC_48ABC;

typedef struct {
    u8 _pad0[0x44];
    s32 unk44;
} AllocationType_46080;

typedef struct {
    loadAssetMetadata_arg asset;
    u8 _pad1C[0x4];
} AssetMetadata_46080;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 _pad14[0xC];
} Element_80047F90;

typedef struct {
    void *unk0;
    AssetMetadata_46080 unk4;
    AssetMetadata_46080 unk24;
    AssetMetadata_46080 unk44;
    Player *unk64;
    s16 unk68;
    s16 unk6A;
} Struct_func_80047EFC_48AFC;

extern void func_800480A8_48CA8(Struct_func_80047EFC_48AFC *);
extern void func_80047EFC_48AFC(Struct_func_80047EFC_48AFC *);
void func_80047F90_48B90(Struct_func_80047EFC_48AFC *);
void func_800481A0_48DA0(Struct_func_80047EFC_48AFC *);

void func_80047EBC_48ABC(Struct_func_80047EBC_48ABC *arg0) {
    arg0->unk0 = loadAsset_34CB50();
    setCleanupCallback(&func_800482A4_48EA4);
    setCallbackWithContinue(&func_80047EFC_48AFC);
}

void func_80047EFC_48AFC(Struct_func_80047EFC_48AFC *arg0) {
    AllocationType_46080 *alloc;

    alloc = (AllocationType_46080 *)getCurrentAllocation();
    loadAssetMetadata(&arg0->unk4.asset, arg0->unk0, 2);
    loadAssetMetadata(&arg0->unk24.asset, arg0->unk0, 3);
    loadAssetMetadata(&arg0->unk44.asset, arg0->unk0, 4);

    arg0->unk4.asset.unk0 = (loadAssetMetadata_arg *)(alloc->unk44 + 0xC0);
    arg0->unk24.asset.unk0 = (loadAssetMetadata_arg *)(alloc->unk44 + 0x100);
    arg0->unk44.asset.unk0 = (loadAssetMetadata_arg *)(alloc->unk44 + 0x140);
    arg0->unk6A = 0;
    setCallbackWithContinue(&func_80047F90_48B90);
}

void func_80047F90_48B90(Struct_func_80047EFC_48AFC *arg0) {
    GameState *allocation;
    s32 heightOffset;
    s32 sinVal;
    s32 i;
    s32 j;
    s16 temp;
    s32 offset;
    Element_80047F90 *elem;

    allocation = (GameState *)getCurrentAllocation();

    if (allocation->gamePaused == 0) {
        temp = arg0->unk6A + 0x40;
        arg0->unk6A = temp;
        if (temp == 0x800) {
            arg0->unk68 = 0x1E;
            loadAssetMetadata(&arg0->unk44.asset, arg0->unk0, 5);
            setCallback(&func_800480A8_48CA8);
        }
    }

    sinVal = approximateSin(arg0->unk6A);
    heightOffset = ((0x2000 - sinVal) << 10) + 0x190000;
    i = 0;
    elem = (Element_80047F90 *)arg0;

loop:
    elem->unk8 = arg0->unk64->worldPosX;
    elem->unkC = arg0->unk64->worldPosY + heightOffset;
    elem->unk10 = arg0->unk64->worldPosZ;
    i += 1;
    elem += 1;
    if (i < 3)
        goto loop;

    i = 0;
    j = 0;
    do {
        offset = 4;
        do {
            func_80066444_67044(i, (func_80066444_67044_arg1 *)((s32)arg0 + offset));
            j++;
            offset += 0x20;
        } while (j < 3);
        i++;
        j = 0;
    } while (i < 4);
}

void func_800480A8_48CA8(Struct_func_80047EFC_48AFC *arg0) {
    GameState *allocation;
    s32 heightOffset;
    s32 i;
    s32 j;
    s32 offset;
    Element_80047F90 *elem;

    allocation = (GameState *)getCurrentAllocation();

    if (allocation->gamePaused == 0) {
        arg0->unk68 -= 1;
        if (arg0->unk68 == 0) {
            loadAssetMetadata(&arg0->unk44.asset, arg0->unk0, 4);
            setCallback(func_800481A0_48DA0);
        }
    }

    i = 0;
    heightOffset = 0x190000;
    elem = (Element_80047F90 *)arg0;

loop:
    elem->unk8 = arg0->unk64->worldPosX;
    elem->unkC = arg0->unk64->worldPosY + heightOffset;
    elem->unk10 = arg0->unk64->worldPosZ;
    i += 1;
    elem += 1;
    if (i < 3)
        goto loop;

    i = 0;
    j = 0;
    do {
        offset = 4;
        do {
            func_80066444_67044(i, (func_80066444_67044_arg1 *)((s32)arg0 + offset));
            j++;
            offset += 0x20;
        } while (j < 3);
        i++;
        j = 0;
    } while (i < 4);
}

void func_800481A0_48DA0(Struct_func_80047EFC_48AFC *arg0) {
    GameState *allocation;
    s32 heightOffset;
    s32 i;
    s32 j;
    s32 offset;
    Element_80047F90 *elem;

    allocation = (GameState *)getCurrentAllocation();

    if (allocation->gamePaused == 0) {
        arg0->unk6A -= 0x40;
        if (arg0->unk6A == 0) {
            func_80069CF8_6A8F8();
        }
    }

    heightOffset = ((0x2000 - approximateSin(arg0->unk6A)) * 5 * 256) + 0x190000;
    i = 0;
    elem = (Element_80047F90 *)arg0;

loop:
    elem->unk8 = arg0->unk64->worldPosX;
    elem->unkC = arg0->unk64->worldPosY + heightOffset;
    elem->unk10 = arg0->unk64->worldPosZ;
    i += 1;
    elem += 1;
    if (i < 3)
        goto loop;

    i = 0;
    j = 0;
    do {
        offset = 4;
        do {
            func_80066444_67044(i, (void *)((s32)arg0 + offset));
            j++;
            offset += 0x20;
        } while (j < 3);
        i++;
        j = 0;
    } while (i < 4);
}

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

typedef struct {
    void *unk0;
    AssetMetadata_46080 elements[5];
    Player *unkA4;
    s16 unkA8;
    s16 unkAA;
    s16 unkAC;
} func_80048350_48F50_arg;

extern void func_80048834_49434(Struct_func_80048834_49434 *arg0);

void func_80048350_48F50(func_80048350_48F50_arg *arg0);
void func_8004841C_4901C(func_80048350_48F50_arg *arg0);

void func_80048310_48F10(Struct_func_80048834_49434 *arg0) {
    arg0->unk0 = loadAsset_34CB50();
    setCleanupCallback(&func_80048834_49434);
    setCallbackWithContinue(&func_80048350_48F50);
}

void func_80048350_48F50(func_80048350_48F50_arg *arg0) {
    AllocationType_46080 *allocation;
    s32 i;
    s32 offset;

    allocation = (AllocationType_46080 *)getCurrentAllocation();

    loadAssetMetadata(&arg0->elements[0].asset, arg0->unk0, 2);
    loadAssetMetadata(&arg0->elements[1].asset, arg0->unk0, 3);

    for (i = 2; i < 5; i++) {
        loadAssetMetadata(&arg0->elements[i].asset, arg0->unk0, 6);
    }

    arg0->unkAA = 0;

    i = 0;
    offset = 0x180;
    for (; i < 5; i++) {
        arg0->elements[i].asset.unk0 = (loadAssetMetadata_arg *)((s32)allocation->unk44 + offset);
        offset += 0x40;
    }

    arg0->unkAC = 0;
    setCallbackWithContinue(&func_8004841C_4901C);
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 _pad14[0xC];
} Element_80048720;

extern void func_80048540_49140(void);

void func_8004841C_4901C(func_80048350_48F50_arg *arg0) {
    Allocation_47D1C *allocation;
    s32 heightOffset;
    s32 sinVal;
    s32 j;
    s32 i;
    Element_80048720 *elem;

    allocation = (Allocation_47D1C *)getCurrentAllocation();

    if (allocation->unk76 == 0) {
        arg0->unkAC += 0x40;
        if ((s16)arg0->unkAC == 0x800) {
            arg0->unkA8 = 0;
            setCallback(&func_80048540_49140);
        }
    }

    sinVal = approximateSin(arg0->unkAC);
    heightOffset = ((0x2000 - sinVal) << 10) + 0x200000;
    j = 0;
    elem = (Element_80048720 *)arg0;

loop:
    elem->unk8 = arg0->unkA4->worldPosX;
    elem->unkC = arg0->unkA4->worldPosY + heightOffset;
    elem->unk10 = arg0->unkA4->worldPosZ;
    j += 1;
    elem += 1;
    if (j < 5)
        goto loop;

    for (i = 0; i < 2; i++) {
        func_80066444_67044(arg0->unkA4->unkBB8, (func_80066444_67044_arg1 *)&arg0->elements[i]);
    }

    for (i = 2; i < 5; i++) {
        func_800670A4_67CA4(arg0->unkA4->unkBB8, (func_80066444_67044_arg1 *)&arg0->elements[i]);
    }
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80048540_49140);

void func_80048720_49320(func_80048350_48F50_arg *arg0) {
    Allocation_47D1C *allocation;
    s32 heightOffset;
    s32 sinVal;
    s32 j;
    s32 i;
    Element_80048720 *elem;

    allocation = (Allocation_47D1C *)getCurrentAllocation();

    if (allocation->unk76 == 0) {
        arg0->unkAC -= 0x40;
        if (arg0->unkAC == 0) {
            func_80069CF8_6A8F8();
        }
    }

    sinVal = approximateSin(arg0->unkAC);
    heightOffset = ((0x2000 - sinVal) << 10) + 0x200000;
    j = 0;
    elem = (Element_80048720 *)arg0;

loop:
    elem->unk8 = arg0->unkA4->worldPosX;
    elem->unkC = arg0->unkA4->worldPosY + heightOffset;
    elem->unk10 = arg0->unkA4->worldPosZ;
    j += 1;
    elem += 1;
    if (j < 5)
        goto loop;

    for (i = 0; i < 2; i++) {
        func_80066444_67044(arg0->unkA4->unkBB8, (func_80066444_67044_arg1 *)&arg0->elements[i]);
    }

    for (i = 2; i < 5; i++) {
        func_800670A4_67CA4(arg0->unkA4->unkBB8, (func_80066444_67044_arg1 *)&arg0->elements[i]);
    }
}

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

typedef struct {
    s16 unk0;
    u8 _pad[14];
} func_80049104_49D04_DataElement;

typedef struct {
    s32 offset;
} func_80049104_49D04_DataBlock;

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    func_80049104_49D04_DataBlock *unkC;
    func_80049104_49D04_DataElement *unk10;
    s16 unk14;
    s16 unk16;
    u8 unk18;
    u8 _pad3[0x3];
    u8 unk1C;
    u8 _pad4[0x3];
    u8 unk20;
    u8 _pad5[0x3];
    u8 unk24;
} func_80048E34_49A34_arg;

s32 func_800488A0_494A0(Player *arg0, u8 *arg1) {
    s32 i;
    s32 randVal;
    u8 index;
    func_80048E34_49A34_arg *element;

    randVal = getRand(*(arg1 + arg0->unkBC4 + 0x24)) & 0xFF;
    index = arg0->unkBC4;

    for (i = 0; i < 5; i++) {
        if (D_80090CA8_918A8[index][i] >= randVal) {
            break;
        }
    }

    element = (func_80048E34_49A34_arg *)(arg1 + index);

    switch (element->unk1C) {
        case 0:
            element->unk24 = element->unk24 + 1;
            break;
        case 1:
            element->unk24 = element->unk24 - 1;
            break;
        case 2:
            element->unk24 = element->unk24 + 3;
            break;
        case 3:
            element->unk24 = element->unk24 - 3;
            break;
        default:
            return i + 1;
    }

    return i + 1;
}

typedef struct {
    u8 padding[0x18];
    u8 unk18;
    u8 _pad[0x7];
    u8 unk20;
} func_8004899C_arg1_element;

s32 func_8004899C_4959C(Player *arg0, u8 *arg1) {
    s32 i;
    s32 randVal;
    u8 index;
    func_8004899C_arg1_element *element;

    if (arg0->unkB84 & 0x1000000) {
        randVal = randB() & 0xFF;
        for (i = 0; i < 9; i++) {
            if (D_80090CE0_918E0[i] >= randVal) {
                break;
            }
        }
        return i + 1;
    }

    randVal = getRand(*(arg1 + arg0->unkBC4 + 0x20)) & 0xFF;
    index = arg0->unkBC4;

    for (i = 0; i < 9; i++) {
        if (D_80090CBC_918BC[index][i] >= randVal) {
            break;
        }
    }

    element = (func_8004899C_arg1_element *)(arg1 + index);

    switch (element->unk18) {
        case 0:
            element->unk20 = element->unk20 + 1;
            break;
        case 1:
            element->unk20 = element->unk20 - 1;
            break;
        case 2:
            element->unk20 = element->unk20 + 3;
            break;
        case 3:
            element->unk20 = element->unk20 - 3;
            break;
        default:
            return i + 1;
    }

    return i + 1;
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80048AE8_496E8);

typedef struct {
    void *unk0;
    void *unk4;
    void *unk8;
    void *unkC;
} func_80049230_49E30_arg;

extern void func_80048F0C_49B0C(func_80048E34_49A34_arg *arg0, s32 arg1);
void func_80049104_49D04(func_80048E34_49A34_arg *arg0);
void func_80049230_49E30(func_80049230_49E30_arg *);

void func_80048E34_49A34(func_80048E34_49A34_arg *arg0) {
    s32 i;
    s32 index;
    func_80048E34_49A34_arg *ptr;

    arg0->unk0 = loadAsset_B7E70();
    arg0->unk4 = loadAsset_216290();

    index = arg0->unk14 * 3;

    arg0->unkC =
        dmaRequestAndUpdateStateWithSize(D_80090CEC_918EC[index], D_80090CF0_918F0[index], D_80090CF4_918F4[index]);
    arg0->unk8 = NULL;

    setCleanupCallback(&func_80049230_49E30);

    i = 0;
loop:
    ptr = (func_80048E34_49A34_arg *)((u8 *)arg0 + i);
    ptr->unk18 = randA() & 3;
    ptr->unk1C = randA() & 3;
    ptr->unk20 = randA();
    ptr->unk24 = randA();
    i++;
    if (i < 4) {
        goto loop;
    }

    setCallback(&func_80049104_49D04);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80048F0C_49B0C);

struct func_800491CC_49DCC_arg;
void func_800491CC_49DCC(struct func_800491CC_49DCC_arg *arg0);

void func_80049104_49D04(func_80048E34_49A34_arg *arg0) {
    func_80049104_49D04_DataElement *ptr;
    func_80049104_49D04_DataElement *base;
    s32 count;
    s32 i;

    arg0->unk10 = (func_80049104_49D04_DataElement *)((s32)arg0->unkC + arg0->unkC->offset);
    arg0->unk16 = 0;
    ptr = arg0->unk10;

    if (ptr->unk0 != -1) {
        base = ptr;
        do {
            arg0->unk16++;
        } while (base[arg0->unk16].unk0 != -1);
    }

    count = arg0->unk16;
    arg0->unk8 = allocateNodeMemory(count * 132);

    for (i = 0; i < arg0->unk16; i++) {
        func_80048F0C_49B0C(arg0, i);
    }

    setCallback(&func_800491CC_49DCC);
}

typedef struct {
    u8 _pad[0x84];
} func_800491CC_49DCC_Element;

typedef struct func_800491CC_49DCC_arg {
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

extern void func_80049430_4A030(func_80049300_49F00_arg *arg0);

void func_8004934C_49F4C(func_80049300_49F00_arg *arg0) {
    GameState *allocation = (GameState *)getCurrentAllocation();
    GameStateUnk44Unk2C0 *entry;
    s32 i;

    if (arg0->unk38 == 8) {
        func_80069CF8_6A8F8();
        return;
    }

    entry = &allocation->unk44->unk2C0[arg0->unk38];
    arg0->unk30 = 0;
    arg0->unk34 = entry;

    for (i = 0; i < 4; i++) {
        debugEnqueueCallback(i, 1, &func_80049430_4A030, arg0);
    }

    if (allocation->gamePaused == 0) {
        arg0->unk38++;
    }
}

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

extern void *D_80094DA0_959A0;
extern void *D_80094DE0_959E0;
extern void *D_80094DF0_959F0;
extern void *D_80094E00_95A00;
extern void *D_80094E10_95A10;
extern void *D_80094EE0_95AE0;
extern void *D_80094EF0_95AF0;
extern void *D_80094FC0_95BC0;
extern void *D_80094FD0_95BD0;
extern void *D_800950B0_95CB0;
extern void *D_80095360_95F60;
extern void *D_80095370_95F70;
extern void *D_80095380_95F80;
extern void *D_80095460_96060;
extern void *D_80095470_96070;
extern void *D_80095480_96080;
extern void *D_800955C0_961C0;
extern void *D_800955D0_961D0;
extern void *D_800955E0_961E0;
extern void *D_80095860_96460;
extern void *D_80095930_96530;

void func_80046D38_47938(void *, s32, s32, s32, s32, s32);

void func_800497FC_4A3FC(s32 poolId) {
    s32 temp;

    switch (poolId) {
        case 0:
            func_800457A0_463A0(poolId);
            func_80046D38_47938(&D_80094DA0_959A0, poolId, 0, 4, 0x12, 0);
            func_80046244_46E44(poolId, 0);
            break;
        case 1:
            func_800457A0_463A0(poolId);
            func_80046D38_47938(&D_80094DE0_959E0, poolId, 4, 0, 0, 0);
            temp = 1;
            func_80046D38_47938(&D_80094DF0_959F0, poolId, 4, 0, temp, 0);
            func_80046D38_47938(&D_80094E00_95A00, poolId, 4, 0, 0, 0);
            func_80046D38_47938(&D_80094E10_95A10, poolId, 0, 1, 2, temp);
            break;
        case 2:
            func_800457A0_463A0(poolId);
            func_80046D38_47938(&D_80094EE0_95AE0, poolId, 0, 4, 0, 0);
            func_80046D38_47938(&D_80094EF0_95AF0, poolId, 1, 0, 2, 0);
            break;
        case 3:
            func_800457A0_463A0(poolId);
            func_80046D38_47938(&D_80094FC0_95BC0, poolId, 0, 4, 0, 0);
            func_80046D38_47938(&D_80094FD0_95BD0, poolId, 1, 0, 2, 0);
            func_80046244_46E44(poolId, 5);
            break;
        case 4:
            func_800457A0_463A0(poolId);
            func_80046D38_47938(&D_800950B0_95CB0, poolId, -4, 0, 0, 0);
            func_80046244_46E44(poolId, 0xA);
            break;
        case 5:
            func_800457A0_463A0(poolId);
            break;
        case 6:
            func_80046D38_47938(&D_80095360_95F60, poolId, 0, 4, 0, 0);
            func_80046D38_47938(&D_80095370_95F70, poolId, 0, 4, 1, 0);
            func_80046D38_47938(&D_80095380_95F80, poolId, 4, 0, 2, 0);
            func_800457A0_463A0(poolId);
            func_80046244_46E44(poolId, 1);
            break;
        case 7:
            func_80046D38_47938(&D_80095460_96060, poolId, 0, 4, 0, 0);
            func_80046D38_47938(&D_80095470_96070, poolId, 0, 4, 1, 0);
            func_80046D38_47938(&D_80095480_96080, poolId, 4, 0, 2, 0);
            func_800457A0_463A0(poolId);
            func_80046244_46E44(poolId, 2);
            break;
        case 8:
            func_800457A0_463A0(poolId);
            func_80046D38_47938(&D_800955C0_961C0, poolId, 4, 0, 1, 0);
            func_80046D38_47938(&D_800955D0_961D0, poolId, 0, 4, 2, 0);
            func_80046D38_47938(&D_800955E0_961E0, poolId, 4, 0, 3, 0);
            break;
        case 9:
            func_800457A0_463A0(poolId);
            func_80046244_46E44(poolId, 0xB);
            break;
        case 10:
            func_800457A0_463A0(poolId);
            func_80046244_46E44(poolId, 8);
            break;
        case 11:
            func_800457A0_463A0(poolId);
            func_80046244_46E44(poolId, 9);
            break;
        case 12:
            func_800457A0_463A0(poolId);
            func_80046D38_47938(&D_80095860_96460, poolId, 0, -4, 0, 0);
            break;
        case 13:
            func_800457A0_463A0(poolId);
            func_80046D38_47938(&D_80095930_96530, poolId, 0, -4, 0, 0);
            break;
        case 14:
        case 15:
            func_800457A0_463A0(poolId);
            func_80046244_46E44(poolId, 7);
            break;
    }
}

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

extern void func_800409B4_415B4(void);
extern void func_800BB2B0(void);
extern void func_800BBA50(s32);
extern void func_800BBA98(void);
extern void func_800BBB34(void);
extern void func_800BBED8(void);
extern void func_800BC0FC(s32);
extern void func_800BC9BC(void);
extern void D_800BB688(void);
extern void D_800BB74C(void);
extern void D_800BB7F0(void);
extern void D_800BB814(void);
extern void D_800BB8E8(void);
extern void D_800BBA28(void);
extern void D_800BBA90(void);
extern void D_800BBAF8(void);
extern void D_800BBE84(void);
extern void D_800BC13C(void);
extern void D_800BC528(void);
extern void D_800BC72C(void);

void func_80049CA8_4A8A8(s32 arg0, s32 arg1) {
    GameState *temp_s1;
    s32 s0;

    temp_s1 = (GameState *)getCurrentAllocation();

    switch (arg0) {
        case 0:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            func_80049BFC_4A7FC();
            scheduleTask(&func_800BB2B0, 0, 0, 0xD3);
            scheduleTask(&D_800BB814, 0, 0, 0xD3);
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            func_80049C38_4A838(arg0);
            func_80049C70_4A870(arg0);
            break;

        case 1:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            func_80049C38_4A838(arg0);
            func_80049C70_4A870(arg0);
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            func_80046244_46E44(arg0, 4);
            func_80049BFC_4A7FC();
            break;

        case 2:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            if (temp_s1->unk7A != 0xB) {
                func_80049C38_4A838(arg0);
                func_80049C70_4A870(arg0);
                func_800BBB34();
                func_80049BFC_4A7FC();
            }
            func_80046244_46E44(arg0, 3);
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            break;

        case 3:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            if (temp_s1->unk7A == 8) {
                func_80049C38_4A838(arg0);
                func_80049C70_4A870(arg0);
            } else {
                func_80045434_46034(1);
            }
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            break;

        case 4:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            scheduleTask(&func_800BB2B0, 0, 0, 0x32);
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            scheduleTask(&D_800BB7F0, 0, 0, 0x5E);
            func_80049C38_4A838(arg0);
            func_80049C70_4A870(arg0);
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            break;

        case 5:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            func_80049C38_4A838(arg0);
            func_80049C70_4A870(arg0);
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            func_80046244_46E44(arg0, 6);
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            scheduleTask(&D_800BBA28, 0, 0, 0x31);
            scheduleTask(&D_800BBAF8, 0, 0, 0xF0);
            func_80049BFC_4A7FC();
            break;

        case 6:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            func_80044538_45138(0);
            func_80044538_45138(1);
            func_80049C38_4A838(arg0);
            func_80049C70_4A870(arg0);
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            func_800BBA98();
            func_80049BFC_4A7FC();
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            break;

        case 7:
            func_80044538_45138(2);
            func_80044538_45138(3);
            if (temp_s1->unk7A != 8) {
                func_80045434_46034(0);
            } else {
                func_80049C38_4A838(arg0);
                func_80049C70_4A870(arg0);
            }
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            break;

        case 8:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            func_80049C38_4A838(arg0);
            func_80049C70_4A870(arg0);
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            scheduleTask(&func_800BB2B0, 0, 0, 0xD3);
            func_800BBED8();
            scheduleTask(&D_800BC528, 0, 0, 0xC7);
            func_800BC9BC();
            func_80049BFC_4A7FC();
            break;

        case 9:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            func_80049C38_4A838(arg0);
            func_80049C70_4A870(arg0);
            scheduleTask(&D_800BB74C, 0, 0, 0xC8);
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            scheduleTask(&D_800BB8E8, 0, 0, 0x32);
            scheduleTask(&D_800BBE84, 0, 0, 0x32);
            func_80044538_45138(6);
            func_80044538_45138(7);
            func_800BC0FC(0);
            func_800BC0FC(1);
            func_800BC0FC(2);
            func_800BC0FC(3);
            func_800BC0FC(4);
            scheduleTask(&D_800BC13C, 0, 0, 0xD3);
            scheduleTask(&D_800BC72C, 0, 0, 0xD3);
            func_80049BFC_4A7FC();
            break;

        case 10:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            func_80044538_45138(4);
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            scheduleTask(&D_800BB688, 0, 0, 0xD3);
            func_80049C38_4A838(arg0);
            func_80049C70_4A870(arg0);
            break;

        case 11:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            if (temp_s1->unk7A == 8) {
                func_80049C38_4A838(arg0);
                func_80049C70_4A870(arg0);
            } else {
                func_80045434_46034(2);
            }
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            func_80041418_42018();
            func_80044538_45138(5);
            break;

        case 12:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            if (temp_s1->unk7A == 8) {
                func_80049C38_4A838(arg0);
                func_80049C70_4A870(arg0);
            } else {
                func_80045434_46034(3);
            }
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            scheduleTask(&func_800BB2B0, 0, 0, 0xD3);
            break;

        case 13:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            if (temp_s1->unk7A == 8) {
                func_80049C38_4A838(arg0);
                func_80049C70_4A870(arg0);
            } else {
                func_800BBA50(arg0);
            }
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            scheduleTask(&D_800BBA90, 0, 0, 0xD3);
            break;

        case 14:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            if (temp_s1->unk7A == 8) {
                func_80049C38_4A838(arg0);
                func_80049C70_4A870(arg0);
            }
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            break;

        case 15:
            for (s0 = 0; s0 < arg1; s0++) {
                temp_s1->unk60 = temp_s1->unk60 + 1;
                func_80045964_46564(s0);
            }
            func_80049C38_4A838(arg0);
            func_80049C70_4A870(arg0);
            scheduleTask(func_80046DCC_479CC, 0, 0, 0xD3);
            scheduleTask(&func_800409B4_415B4, 0, 0, 0xD3);
            break;
    }
}

typedef struct {
    void *unk0;
    void *unk4;
    u8 _pad[0x1E];
    s16 unk26;
    u8 _pad2[0x4];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
} func_8004A634_4B234_arg;

void func_8004A634_4B234(func_8004A634_4B234_arg *arg0);

void func_8004A5E0_4B1E0(void **arg0) {
    *arg0 = dmaRequestAndUpdateStateWithSize(&_3F6670_ROM_START, &_3F6670_ROM_END, 0x388);
    setCleanupCallback(&func_8004A96C_4B56C);
    setCallbackWithContinue(&func_8004A634_4B234);
}

void func_8004A634_4B234(func_8004A634_4B234_arg *arg0) {
    s32 rotatedVector[3];
    GameState *allocation;
    void *temp;
    u8 randomValue;

    allocation = (GameState *)getCurrentAllocation();
    arg0->unk26 = 0;

    temp = (void *)((s32)allocation->unk44 + 0x80);
    arg0->unk4 = temp;

    loadAssetMetadata((void *)((s32)arg0 + 4), arg0->unk0, arg0->unk26);

    randomValue = randA();
    rotateVectorY(&D_80090E20_91A20, (randomValue & 0xFF) << 5, &rotatedVector);

    arg0->unk2C += rotatedVector[0];
    arg0->unk30 += rotatedVector[1];
    arg0->unk34 += rotatedVector[2];

    setCallbackWithContinue(func_8004A6D4_4B2D4);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004A6D4_4B2D4);

typedef struct {
    void *unk0;
    loadAssetMetadata_arg unk4;
    u8 _pad20[0x6];
    s16 unk26;
    s16 unk28;
} func_8004A850_4B450_arg;

void func_8004A850_4B450(func_8004A850_4B450_arg *arg0) {
    GameState *allocation;
    void *s2;
    void *s0;
    s32 i;

    allocation = (GameState *)getCurrentAllocation();
    if (allocation->gamePaused == 0) {
        arg0->unk28--;
    }

    if (arg0->unk28 == 0) {
        func_80069CF8_6A8F8();
    }

    s2 = (void *)((s32)arg0 + 8);
    s0 = func_8005B24C_5BE4C(s2, -1, 0x100000);

    if (s0 != NULL) {
        func_80056B7C_5777C(s2, 7);
        func_80059A48_5A648(s0, 100);
        func_80069CF8_6A8F8();
    }

    if (D_8009ADE0_9B9E0 & 1) {
        arg0->unk26++;
        if ((s16)arg0->unk26 >= 6) {
            arg0->unk26 = 0;
        }
        loadAssetMetadata(&arg0->unk4, arg0->unk0, arg0->unk26);
    }

    if ((s16)arg0->unk28 >= 0x1F || (gFrameCounter & 1)) {
        for (i = 0; i < 4; i++) {
            func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
        }
    }
}

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

typedef struct {
    void *unk0;
    loadAssetMetadata_arg unk4;
    u8 _pad20[0x4];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    Player *unk30;
    u16 unk34;
    u8 _pad36[0x2];
    s16 unk38;
    s16 unk3A;
} func_8004AA90_4B690_arg;

void func_8004AA90_4B690(func_8004AA90_4B690_arg *arg0);

typedef struct func_8004AB50_4B750_arg func_8004AB50_4B750_arg;
extern void func_8004AB50_4B750(func_8004AB50_4B750_arg *arg0);

void func_8004AA50_4B650(s32 *arg0) {
    *arg0 = (s32)load_3ECE40();
    setCleanupCallback(func_8004AE58_4BA58);
    setCallbackWithContinue(&func_8004AA90_4B690);
}

void func_8004AA90_4B690(func_8004AA90_4B690_arg *arg0) {
    GameState *gs;
    Player *player;
    s32 *temp_s1;
    s32 pad[4];

    gs = (GameState *)getCurrentAllocation();
    arg0->unk4.unk0 = (loadAssetMetadata_arg *)((u8 *)gs->unk44 + 0xEC0);
    loadAssetMetadata(&arg0->unk4, arg0->unk0, 0x3F);
    player = arg0->unk30;
    temp_s1 = &arg0->unk4.unk4;
    memcpy(temp_s1, (u8 *)player + 0x31C, 0xC);
    arg0->unk34 = arg0->unk30->unkB94;
    arg0->unk38 = (s16)arg0->unk30->unkBB8;
    memcpy(&arg0->unk24, (u8 *)arg0->unk30 + 0x44C, 0xC);
    arg0->unk3A = 1;
    arg0->unk28 = arg0->unk28 + 0x28000;
    func_80056B7C_5777C(temp_s1, 0x17);
    setCallbackWithContinue(func_8004AB50_4B750);
}

struct func_8004AB50_4B750_arg {
    u8 _pad0[0x4];
    u8 unk4[0x4];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 _pad14[0x10];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    u8 _pad30[0x4];
    u16 unk34;
    s16 unk36;
    s16 unk38;
    s16 unk3A;
};

typedef struct {
    s32 sp18;
    s32 sp1C;
    s32 sp20;
} func_8004AB50_4B750_sp;

typedef struct {
    u8 _pad0[0x4];
    func_80066444_67044_arg1 unk4;
} func_8004AD18_4B918_arg;

void func_8004AD18_4B918(func_8004AD18_4B918_arg *arg0);

void func_8004AB50_4B750(func_8004AB50_4B750_arg *arg0) {
    func_8004AB50_4B750_sp sp;
    GameState_46080 *s0;
    void *temp_s0;
    Vec3s32 *s2;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_v1;
    s32 var_v0;
    s32 var_v0_2;
    void *temp_v0_3;
    s32 i;

    s0 = (GameState_46080 *)getCurrentAllocation();
    if (s0->unk76 == 0) {
        if (arg0->unk3A != 0) {
            arg0->unk3A = 0;
        } else {
            temp_a0 = arg0->unk24;
            var_v0 = temp_a0;
            if (temp_a0 < 0) {
                var_v0 = temp_a0 + 7;
            }
            temp_v1 = arg0->unk28;
            temp_a1 = arg0->unk2C;
            arg0->unk24 = temp_a0 - (var_v0 >> 3);
            var_v0_2 = temp_a1;
            arg0->unk28 = temp_v1 - 0x3000;
            if (temp_a1 < 0) {
                var_v0_2 = temp_a1 + 7;
            }
            arg0->unk2C = temp_a1 - (var_v0_2 >> 3);
            arg0->unk8 = arg0->unk8 + arg0->unk24;
            arg0->unkC = arg0->unkC + arg0->unk28;
            arg0->unk10 = arg0->unk10 + arg0->unk2C;
        }
        temp_s0 = (void *)((u8 *)s0 + 0x30);
        s2 = (Vec3s32 *)((u8 *)arg0 + 8);
        arg0->unk34 = func_80060A3C_6163C(temp_s0, arg0->unk34, s2);
        func_80060CDC_618DC(temp_s0, arg0->unk34, s2, 0x100000, &sp.sp18);
        arg0->unk8 = arg0->unk8 + sp.sp18;
        arg0->unk10 = arg0->unk10 + sp.sp20;
        sp.sp1C = func_80061A64_62664(temp_s0, arg0->unk34, s2);
        temp_v0_3 = func_8005B24C_5BE4C(s2, arg0->unk38, 0xA0000);
        if (temp_v0_3 != NULL) {
            func_80050ECC_51ACC(s2);
            func_80056B7C_5777C(s2, 0xD);
            func_80058924_59524(temp_v0_3);
            func_80069CF8_6A8F8();
            return;
        }
        if (arg0->unkC < sp.sp1C) {
            arg0->unkC = sp.sp1C;
            arg0->unk36 = 0x1518;
            setCallback(func_8004AD18_4B918);
        }
    }
    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} func_8004AD18_4B918_CopyData;

typedef struct {
    u8 padding[0x5B];
    u8 unk5B;
    u8 padding2[0x1A];
    u8 unk76;
} allocation_46080;

void func_8004AD18_4B918(func_8004AD18_4B918_arg *arg0) {
    func_8004AD18_4B918_CopyData sp10;
    func_8004AD18_4B918_CopyData *s0;
    Vec3s32 *s2;
    void *s3;
    s32 i;
    allocation_46080 *alloc;

    alloc = (allocation_46080 *)getCurrentAllocation();

    if (alloc->unk76 == 0) {
        if (alloc->unk5B < 10) {
            if (arg0->unk4.unk36 < 0x1194) {
                if (arg0->unk4.unk36 >= 0x1F) {
                    arg0->unk4.unk36 = 0x1E;
                }
            }
        }
        arg0->unk4.unk36--;
    }

    s2 = &arg0->unk4.unk4;

    if (arg0->unk4.unk36 == 0) {
        func_80069CF8_6A8F8();
    }

    s3 = func_8005B24C_5BE4C(s2, -1, 0xA0000);
    s0 = &sp10;

    if (s3 != NULL) {
        memcpy(s0, s2, 0xC);
        sp10.unk4 = sp10.unk4 + 0xA0000;
        func_80050ECC_51ACC(s0);
        func_80056B7C_5777C(s0, 0xD);
        func_80058924_59524(s3);
        func_80069CF8_6A8F8();
    } else {
        i = 0;
        if (arg0->unk4.unk36 < 0x1F) {
            if ((gFrameCounter & 1) == 0) {
                return;
            }
        }
        for (i = 0; i < 4; i++) {
            func_80066444_67044(i, &arg0->unk4);
        }
    }
}

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

void func_8004AF2C_4BB2C(func_8004AF2C_4BB2C_arg *arg0) {
    GameState *allocation;
    s32 rotatedVector[3];
    u8 randomValue;

    allocation = (GameState *)getCurrentAllocation();

    if (arg0->unk38 == 0) {
        arg0->unk32 = arg0->unk36;
    } else {
        arg0->unk32 = arg0->unk36 + 7;
    }

    arg0->unk4 = (void *)((s32)allocation->unk44 + 0xF80);
    loadAssetMetadata((void *)((s32)arg0 + 4), arg0->unk0, arg0->unk32);

    randomValue = randA();
    rotateVectorY(&D_80090E20_91A20, (randomValue & 0xFF) << 5, &rotatedVector);

    arg0->unk24 += rotatedVector[0];
    arg0->unk28 += rotatedVector[1];
    arg0->unk2C += rotatedVector[2];

    func_80056B7C_5777C((void *)((s32)arg0 + 8), 0x17);
    setCallbackWithContinue(&func_8004AFF8_4BBF8);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004AFF8_4BBF8);

typedef struct {
    void *unk0;
    void *unk4;
    u8 _pad[0x28];
    s16 unk30;
    u8 _pad2[0x2];
    s16 unk34;
    s16 unk36;
    s16 unk38;
} func_8004B130_arg;

void func_8004B130_4BD30(func_8004B130_arg *arg0) {
    allocation_46080 *allocation;
    Player *player;
    s32 i;

    allocation = (allocation_46080 *)getCurrentAllocation();

    if (allocation->unk76 == 0) {
        arg0->unk34--;
    }

    if (arg0->unk34 == 0) {
        if (arg0->unk38 == 1) {
            if (arg0->unk36 == 7) {
                allocation->unk5B--;
            }
        }
        func_80069CF8_6A8F8();
        return;
    }

    player = (Player *)func_8005B24C_5BE4C((void *)&arg0->unk4 + 4, -1, 0x100000);

    if (player != NULL) {
        if (arg0->unk38 == 0) {
            u16 val = (u16)arg0->unk36;
            u8 tmp = player->unkBD8 | 1;
            player->unkBD3 = 3;
            player->unkBD2 = (u8)val;
            player->unkBD8 = tmp;
        } else {
            player->unkBD4 = (u8)arg0->unk36;
            player->unkBD8 |= 2;
        }
        func_80056B7C_5777C((void *)&arg0->unk4 + 4, 8);
        func_80069CF8_6A8F8();
    }

    if (arg0->unk34 < 0x1F) {
        i = 0;
        if ((gFrameCounter & 1) == 0) {
            return;
        }
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
    }
}

void func_8004B264_4BE64(func_8004B264_4BE64_arg *arg0) {
    GameState *allocation;

    allocation = (GameState *)getCurrentAllocation();
    allocation->unk5B++;
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void *func_8004B2A0_4BEA0(void *arg0, u32 arg1, void *arg2, s16 arg3, s32 arg4) {
    GameState *allocation;
    void *task;

    allocation = (GameState *)getCurrentAllocation();

    if (allocation->unk5B == 0) {
        goto end;
    }

    task = scheduleTask(func_8004AED8_4BAD8, 3, 0, 0xEF);

    if (task == NULL) {
        goto end;
    }

    allocation->unk5B--;

    memcpy((u8 *)task + 8, arg0, 0xC);
    memcpy((u8 *)task + 0x24, arg2, 0xC);

    *(s16 *)((u8 *)task + 0x30) = (s16)arg1;
    *(s16 *)((u8 *)task + 0x38) = arg3;
    *(s16 *)((u8 *)task + 0x36) = arg4;

end:
    return task;
}

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

typedef struct {
    u8 _pad0[0x4];
    func_80066444_67044_arg1 unk4;
} func_8004B648_4C248_arg;

void func_8004B648_4C248(func_8004B648_4C248_arg *arg0);

typedef struct {
    s32 sp18;
    s32 sp1C;
    s32 sp20;
} func_8004B4CC_4C0CC_StackLocals;

typedef struct {
    u8 _pad0[0x4];
    u8 unk4[0x4];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 _pad14[0x14];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    u16 unk34;
    s16 unk36;
    s16 unk38;
} func_8004B4CC_4C0CC_arg;

void func_8004B4CC_4C0CC(func_8004B4CC_4C0CC_arg *arg0) {
    func_8004B4CC_4C0CC_StackLocals sp;
    GameState_46080 *s0;
    void *s2;
    s32 i;
    void *temp_s0;

    s0 = getCurrentAllocation();

    if (s0->unk76 != 0) {
        s0 = NULL;
        goto exit_loop;
    }

    s2 = (void *)((u8 *)arg0 + 0x8);

    arg0->unk2C -= 0x4000;
    arg0->unk8 += arg0->unk28;
    arg0->unkC += arg0->unk2C;
    arg0->unk10 += arg0->unk30;

    arg0->unk34 = func_80060A3C_6163C((u8 *)s0 + 0x30, arg0->unk34, s2);

    func_80060CDC_618DC((u8 *)s0 + 0x30, arg0->unk34, s2, 0x100000, &sp.sp18);

    arg0->unk8 += sp.sp18;
    arg0->unk10 += sp.sp20;

    sp.sp1C = func_80061A64_62664((u8 *)s0 + 0x30, arg0->unk34, s2);

    temp_s0 = func_8005B24C_5BE4C(s2, arg0->unk38, 0xA0000);

    if (temp_s0 != NULL) {
        func_80050ECC_51ACC(s2);
        func_80056B7C_5777C(s2, 0xD);
        func_80058924_59524(temp_s0);
        func_80069CF8_6A8F8();
        return;
    }

    if (arg0->unkC < sp.sp1C) {
        arg0->unkC = sp.sp1C;
        arg0->unk36 = 0x1E0;
        setCallback(&func_8004B648_4C248);
    }

    s0 = NULL;

exit_loop:
    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
    }
}

void func_8004B648_4C248(func_8004B648_4C248_arg *arg0) {
    func_8004AD18_4B918_CopyData sp10;
    func_8004AD18_4B918_CopyData *s0;
    Vec3s32 *s2;
    void *s3;
    s32 i;

    if (((GameState *)getCurrentAllocation())->gamePaused == 0) {
        arg0->unk4.unk36--;
    }

    s2 = &arg0->unk4.unk4;

    if (arg0->unk4.unk36 == 0) {
        func_80069CF8_6A8F8();
    }

    s3 = func_8005B24C_5BE4C(s2, -1, 0xA0000);
    s0 = &sp10;

    if (s3 != NULL) {
        memcpy(s0, s2, 0xC);
        sp10.unk4 = sp10.unk4 + 0xA0000;
        func_80050ECC_51ACC(s0);
        func_80056B7C_5777C(s0, 0xD);
        func_80058924_59524(s3);
        func_80069CF8_6A8F8();
    } else {
        if (arg0->unk4.unk36 < 0x1F) {
            i = 0;
            if ((gFrameCounter & 1) == 0) {
                return;
            }
        }
        for (i = 0; i < 4; i++) {
            func_80066444_67044(i, &arg0->unk4);
        }
    }
}

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

void func_8004B834_4C434(func_8004B834_4C434_arg *);

void func_8004B7F4_4C3F4(MemoryAllocatorNode **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(&func_8004BC20_4C820);
    setCallbackWithContinue(&func_8004B834_4C434);
}

void func_8004B834_4C434(func_8004B834_4C434_arg *arg0) {
    GameState *allocation;
    u8 randomValue;
    s32 rotationAngle;
    u8 randomValue3;
    u8 randomValue4;
    s32 *addr;
    s32 pad[4];

    allocation = (GameState *)getCurrentAllocation();
    arg0->unk4 = (void *)((s32)allocation->unk44 + 0x1400);

    randomValue = randA();
    loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->unk0, (randomValue % 3) + 0x6B);

    memcpy(&arg0->unk8, (void *)((s32)arg0->unk24 + 0x4C), 0xC);

    arg0->unkC = arg0->unkC + (s32)0xFFF10000;

    arg0->unk34 = arg0->unk24->unkB94;
    arg0->unk38 = arg0->unk24->unkBB8;

    randomValue = randA();
    rotationAngle = ((randomValue & 0xFF) << 5) + arg0->unk24->unkA94;

    randomValue3 = randA();
    addr = &D_80090E40_91A40;
    *addr = (randomValue3 & 0xFF) * 7 * 256;
    rotateVectorY(addr - 2, (s16)rotationAngle, &arg0->unk28);

    arg0->unk28 = arg0->unk28 + arg0->unk24->unk44C;

    randomValue4 = randA();
    arg0->unk2C = arg0->unk2C + (arg0->unk24->unk450 + (((randomValue4 & 0xFF) * 5) << 9));
    arg0->unk30 = arg0->unk30 + arg0->unk24->unk454;

    setCallbackWithContinue(&func_8004B990_4C590);
}

extern void func_8004BB0C_4C70C(func_8004BB0C_4C70C_arg *);

typedef struct {
    s32 sp18;
    s32 sp1C;
    s32 sp20;
} func_8004B990_4C590_StackLocals;

void func_8004B990_4C590(func_8004B990_4C590_arg *arg0) {
    func_8004B990_4C590_StackLocals sp;
    GameState_46080 *alloc;
    void *s0;
    Vec3s32 *s2;
    void *temp_s0;
    s32 i;

    alloc = (GameState_46080 *)getCurrentAllocation();

    if (alloc->unk76 != 0) {
        s0 = NULL;
    } else {
        s0 = &alloc->unk30;
        s2 = &arg0->unk4.unk4;

        arg0->unk4.unk28 -= 0x6000;
        arg0->unk4.unk4.unk0 += arg0->unk4.unk24;
        arg0->unk4.unk4.unk4 += arg0->unk4.unk28;
        arg0->unk4.unk4.unk8 += arg0->unk4.unk2C;

        arg0->unk4.unk30 = func_80060A3C_6163C(s0, arg0->unk4.unk30, s2);

        func_80060CDC_618DC(s0, arg0->unk4.unk30, s2, 0x100000, &sp.sp18);

        arg0->unk4.unk4.unk0 += sp.sp18;
        arg0->unk4.unk4.unk8 += sp.sp20;

        sp.sp1C = func_80061A64_62664(s0, arg0->unk4.unk30, s2);

        temp_s0 = func_8005B24C_5BE4C(s2, arg0->unk38, 0xA0000);

        if (temp_s0 != NULL) {
            func_80050ECC_51ACC(s2);
            func_80056B7C_5777C(s2, 0xD);
            func_80058924_59524(temp_s0);
            func_80069CF8_6A8F8();
            return;
        }

        if (arg0->unk4.unk4.unk4 < sp.sp1C) {
            arg0->unk4.unk4.unk4 = sp.sp1C;
            arg0->unk4.unk36 = 0x96;
            setCallback(&func_8004BB0C_4C70C);
        }

        s0 = NULL;
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, &arg0->unk4);
    }
}

void func_8004BB0C_4C70C(func_8004BB0C_4C70C_arg *arg0) {
    func_8004AD18_4B918_CopyData sp10;
    func_8004AD18_4B918_CopyData *s0;
    Vec3s32 *s2;
    void *s3;
    s32 i;

    if (((GameState *)getCurrentAllocation())->gamePaused == 0) {
        arg0->unk4.unk36--;
    }

    if (arg0->unk4.unk36 == 0) {
        func_80069CF8_6A8F8();
    }

    s2 = &arg0->unk4.unk4;
    s3 = func_8005B24C_5BE4C(s2, arg0->unk38, 0xA0000);
    s0 = &sp10;

    if (s3 != NULL) {
        memcpy(s0, s2, 0xC);
        sp10.unk4 = sp10.unk4 + 0xA0000;
        func_80050ECC_51ACC(s0);
        func_80056B7C_5777C(s0, 0xD);
        func_80058924_59524(s3);
        func_80069CF8_6A8F8();
    } else {
        if (arg0->unk4.unk36 < 0x1F) {
            i = 0;
            if ((gFrameCounter & 1) == 0) {
                return;
            }
        }
        for (i = 0; i < 4; i++) {
            func_80066444_67044(i, &arg0->unk4);
        }
    }
}

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

typedef struct {
    u8 _pad0[0x4];
    u8 unk4[0x4];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 _pad14[0x14];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    u16 unk34;
    s16 unk36;
    s16 unk38;
} func_8004BE40_4CA40_arg;

void func_8004BCFC_4C8FC(func_8004B834_4C434_arg *arg0);
void func_8004BE40_4CA40(func_8004BE40_4CA40_arg *arg0);

void func_8004BCBC_4C8BC(MemoryAllocatorNode **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(&func_8004C0D0_4CCD0);
    setCallbackWithContinue(&func_8004BCFC_4C8FC);
}

void func_8004BCFC_4C8FC(func_8004B834_4C434_arg *arg0) {
    GameState *allocation;
    void *temp_s2;
    s32 rotationAngle;
    s32 *addr;
    s32 pad[4];

    allocation = (GameState *)getCurrentAllocation();
    arg0->unk4 = (void *)((s32)allocation->unk44 + 0xEC0);
    loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->unk0, 0x3F);
    arg0->unk34 = arg0->unk24->unkB94;
    temp_s2 = (void *)((s32)arg0 + 8);
    transformVector(&D_80090E50_91A50, (s16 *)((s32)arg0->unk24 + 0x164), temp_s2);
    arg0->unk38 = arg0->unk24->unkBB8;
    rotationAngle = ((randA() & 0xFF) << 5) + arg0->unk24->unkA94;
    addr = &D_80090E4C_91A4C;
    *addr = (randA() & 0xFF) * 0x580;
    rotateVectorY(addr - 2, (s16)rotationAngle, (void *)((s32)arg0 + 0x28));
    arg0->unk28 = arg0->unk28 + arg0->unk24->unk44C;
    arg0->unk2C = arg0->unk2C + (arg0->unk24->unk450 + ((randA() & 0xFF) * 0x600));
    arg0->unk30 = arg0->unk30 + arg0->unk24->unk454;
    func_80056B7C_5777C(temp_s2, 0x17);
    setCallbackWithContinue(&func_8004BE40_4CA40);
}

typedef struct {
    u8 _pad0[0x4];
    func_80066444_67044_arg1 unk4;
    s16 unk38;
} func_8004BFBC_4CBBC_arg;

void func_8004BFBC_4CBBC(func_8004BFBC_4CBBC_arg *arg0);

void func_8004BE40_4CA40(func_8004BE40_4CA40_arg *arg0) {
    func_8004B4CC_4C0CC_StackLocals sp;
    GameState_46080 *s0;
    void *s2;
    s32 i;
    void *temp_s0;

    s0 = getCurrentAllocation();

    if (s0->unk76 != 0) {
        i = 0;
        goto exit_loop;
    }

    s2 = (void *)((u8 *)arg0 + 0x8);

    arg0->unk2C -= 0x4000;
    arg0->unk8 += arg0->unk28;
    arg0->unkC += arg0->unk2C;
    arg0->unk10 += arg0->unk30;

    arg0->unk34 = func_80060A3C_6163C((u8 *)s0 + 0x30, arg0->unk34, s2);

    func_80060CDC_618DC((u8 *)s0 + 0x30, arg0->unk34, s2, 0x100000, &sp.sp18);

    arg0->unk8 += sp.sp18;
    arg0->unk10 += sp.sp20;

    sp.sp1C = func_80061A64_62664((u8 *)s0 + 0x30, arg0->unk34, s2);

    temp_s0 = func_8005B24C_5BE4C(s2, arg0->unk38, 0xA0000);

    if (temp_s0 != NULL) {
        func_80050ECC_51ACC(s2);
        func_80056B7C_5777C(s2, 0xD);
        func_80058924_59524(temp_s0);
        func_80069CF8_6A8F8();
        return;
    }

    if (arg0->unkC < sp.sp1C) {
        arg0->unkC = sp.sp1C;
        arg0->unk36 = 0xF0;
        setCallback(&func_8004BFBC_4CBBC);
    }

    i = 0;

exit_loop:
    do {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
        i++;
    } while (i < 4);
}

void func_8004BFBC_4CBBC(func_8004BFBC_4CBBC_arg *arg0) {
    func_8004AD18_4B918_CopyData sp10;
    func_8004AD18_4B918_CopyData *s0;
    Vec3s32 *s2;
    void *s3;
    s32 i;

    if (((GameState *)getCurrentAllocation())->gamePaused == 0) {
        arg0->unk4.unk36--;
    }

    if (arg0->unk4.unk36 == 0) {
        func_80069CF8_6A8F8();
    }

    s2 = &arg0->unk4.unk4;
    s3 = func_8005B24C_5BE4C(s2, arg0->unk38, 0xA0000);
    s0 = &sp10;

    if (s3 != NULL) {
        memcpy(s0, s2, 0xC);
        sp10.unk4 = sp10.unk4 + 0xA0000;
        func_80050ECC_51ACC(s0);
        func_80056B7C_5777C(s0, 0xD);
        func_80058924_59524(s3);
        func_80069CF8_6A8F8();
    } else {
        if (arg0->unk4.unk36 < 0x1F) {
            i = 0;
            if ((gFrameCounter & 1) == 0) {
                return;
            }
        }
        for (i = 0; i < 4; i++) {
            func_80066444_67044(i, &arg0->unk4);
        }
    }
}

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
