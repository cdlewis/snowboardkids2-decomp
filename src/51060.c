#include "5E590.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "task_scheduler.h"

typedef struct {
    void *unk0;
    loadAssetMetadata_arg *unk4;
    u8 padding[0x16];
    u8 unk1E;
    u8 padding2[0x10];
    s32 unk30;
    s32 unk34;
    s32 unk44;
} func_800504A0_510A0_arg;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 padding[0xA];
    s8 unk1E;
    s32 padding2;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 padding3;
    s32 unk34;
} func_80050504_51104_arg;

typedef struct {
    MemoryAllocatorNode *unk0;
    void *unk4;
    u8 padding[0x16];
    s8 unk1E;
    u8 padding2[0x2];
    void *unk24;
    u8 padding3[0x16];
    s8 unk3E;
    u8 padding4[0x10];
    s16 unk50;
    s16 unk52;
} func_800506B4_512B4_arg;

typedef struct {
    s32 unk0;
    MemoryAllocatorNode *unk4;
    s16 unk8;
    s32 unkC;
    s32 unk10;
} func_80050F18_51B18_arg;

typedef struct {
    s32 unk0;
    void *unk4;
    loadAssetMetadata_arg *unk8;
    s32 unkC;
    s32 unk10;
    u8 padding[0xE];
    u8 unk22;
} func_800516F4_522F4_arg;

typedef struct {
    u8 padding[0xC0];
    MemoryAllocatorNode *unkC0;
} func_8005186C_5246C_arg;

typedef struct {
    Node n;
    s32 unk2C;
    s32 unk30;
} NodeWithPayload;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} func_80050604_51204_arg;

void func_800504A0_510A0(func_800504A0_510A0_arg *);
void func_800505D8_511D8(s32 **arg0);

extern void func_80050740_51340();
extern void func_80050864_51464();
extern void func_800509CC_515CC();
extern void func_80050DB0_519B0();
extern void func_80050EA0_51AA0();
extern void func_80050F64_51B64();
extern void func_80050FE0_51BE0();
extern void func_80051124_51D24();
extern void func_80051250_51E50();
extern void func_80051760_52360();
extern void func_80051800_52400();
extern void func_800518AC_524AC();
extern void func_80051B8C_5278C();

extern s32 randA();
extern s32 *D_80090EC0_91AC0;
void func_80050BD4_517D4(s32 **);
void func_80050504_51104();
void func_800677C0_683C0(s32, void *);
void func_80069CF8_6A8F8();

void func_80050460_51060(void **node) {
    *node = load_3ECE40();
    setCleanupCallback((void (*)(void *))&func_800505D8_511D8);
    setCallbackWithContinue((void (*)(void *))&func_800504A0_510A0);
}

void func_800504A0_510A0(func_800504A0_510A0_arg *arg0) {
    GameState *gs = (GameState *)getCurrentAllocation();
    loadAssetMetadata(&arg0->unk4, arg0->unk0, arg0->unk30);
    arg0->unk1E = 0xE0;
    arg0->unk4 = &gs->unk44[0xFC0];
    arg0->unk34 = 0;
    setCallbackWithContinue(&func_80050504_51104);
}

void func_80050504_51104(func_80050504_51104_arg *arg0) {
    GameState *gs;
    s32 i;
    gs = (GameState *)getCurrentAllocation();
    i = 0;
    if (gs->PAD_6[0x16] == 0) {
        arg0->unk4 = (s32)(gs->unk44 + ((arg0->unk34 << 6) + 0xFC0));
        arg0->unk34 = arg0->unk34 + 1;
        if (arg0->unk34 == 4) {
            func_80069CF8_6A8F8();
            return;
        }
        arg0->unk1E = arg0->unk1E - 0x30;
        arg0->unk8 = arg0->unk8 + arg0->unk24;
        arg0->unkC = arg0->unkC + arg0->unk28;
        arg0->unk10 = arg0->unk10 + arg0->unk2C;
    }
    for (i = 0; i < 4; i++) {
        func_800677C0_683C0(i, &arg0->unk4);
    }
}

void func_800505D8_511D8(s32 **arg0) {
    *arg0 = (s32 *)freeNodeMemory(*arg0);
}

void func_80050604_51204(void *arg0, func_80050604_51204_arg *arg1, s32 arg2) {
    NodeWithPayload *task = (NodeWithPayload *)scheduleTask(&func_80050460_51060, 2, 0, 0xDC);
    if (task != NULL) {
        memcpy((void *)&task->n.freeNext, arg0, 0xC);
        task->unk30 = arg2;
        task->n.cleanupCallback = (void *)(arg1->unk0 / 2);
        task->n.payload = (void *)(arg1->unk4 / 2);
        task->unk2C = ((s32)arg1->unk8 / 2);
    }
}

void func_800506B4_512B4(func_800506B4_512B4_arg *arg0) {
    GameState *temp_s1 = (GameState *)getCurrentAllocation();
    arg0->unk0 = load_3ECE40();
    arg0->unk4 = (void *)(temp_s1->unk44 + ((arg0->unk52 << 6) + 0x1080));
    arg0->unk1E = (u8)((randA() & 0x1F) + 0x70);
    arg0->unk50 = 0;
    arg0->unk24 = (void *)arg0->unk4;
    arg0->unk3E = (u8)arg0->unk1E;
    setCleanupCallback(&func_80050864_51464);
    setCallbackWithContinue(&func_80050740_51340);
}

INCLUDE_ASM("asm/nonmatchings/51060", func_80050740_51340);

INCLUDE_ASM("asm/nonmatchings/51060", func_80050864_51464);

INCLUDE_ASM("asm/nonmatchings/51060", func_80050890_51490);

void func_8005098C_5158C(MemoryAllocatorNode **node) {
    *node = load_3ECE40();
    setCleanupCallback((void (*)(void *))&func_80050BD4_517D4);
    setCallbackWithContinue(&func_800509CC_515CC);
}

INCLUDE_ASM("asm/nonmatchings/51060", func_800509CC_515CC);

INCLUDE_ASM("asm/nonmatchings/51060", func_80050AA8_516A8);

void func_80050BD4_517D4(s32 **arg0) {
    *arg0 = (s32 *)freeNodeMemory(*arg0);
}

INCLUDE_ASM("asm/nonmatchings/51060", func_80050C00_51800);

INCLUDE_ASM("asm/nonmatchings/51060", func_80050C80_51880);

void func_80050D70_51970(MemoryAllocatorNode **node) {
    *node = load_3ECE40();
    setCleanupCallback(&func_80050EA0_51AA0);
    setCallbackWithContinue(&func_80050DB0_519B0);
}

INCLUDE_ASM("asm/nonmatchings/51060", func_80050DB0_519B0);

INCLUDE_ASM("asm/nonmatchings/51060", func_80050E08_51A08);

INCLUDE_ASM("asm/nonmatchings/51060", func_80050EA0_51AA0);

INCLUDE_ASM("asm/nonmatchings/51060", func_80050ECC_51ACC);

void func_80050F18_51B18(func_80050F18_51B18_arg *arg0) {
    arg0->unk4 = load_3ECE40();
    arg0->unk8 = 0x45;
    arg0->unk10 = 0;
    setCleanupCallback(&func_80050FE0_51BE0);
    setCallbackWithContinue(&func_80050F64_51B64);
}

INCLUDE_ASM("asm/nonmatchings/51060", func_80050F64_51B64);

INCLUDE_ASM("asm/nonmatchings/51060", func_80050FE0_51BE0);

INCLUDE_ASM("asm/nonmatchings/51060", func_8005100C_51C0C);

void func_800510A4_51CA4(func_800506B4_512B4_arg *arg0) {
    GameState *temp_s1 = (GameState *)getCurrentAllocation();
    arg0->unk0 = load_3ECE40();
    arg0->unk4 = (u8 *)(temp_s1->unk44 + 0x1340);
    arg0->unk1E = (u8)((randA() & 0x1F) + 0x70);
    arg0->unk50 = 0;
    arg0->unk24 = (u8 *)arg0->unk4;
    arg0->unk3E = (u8)arg0->unk1E;
    setCleanupCallback(&func_80051250_51E50);
    setCallbackWithContinue(&func_80051124_51D24);
}

INCLUDE_ASM("asm/nonmatchings/51060", func_80051124_51D24);

INCLUDE_ASM("asm/nonmatchings/51060", func_80051250_51E50);

INCLUDE_ASM("asm/nonmatchings/51060", func_8005127C_51E7C);

INCLUDE_ASM("asm/nonmatchings/51060", func_80051348_51F48);

INCLUDE_ASM("asm/nonmatchings/51060", func_8005152C_5212C);

INCLUDE_ASM("asm/nonmatchings/51060", func_80051688_52288);

INCLUDE_ASM("asm/nonmatchings/51060", func_800516B4_522B4);

void func_800516F4_522F4(func_800516F4_522F4_arg *arg0) {
    (GameState *)getCurrentAllocation();
    arg0->unk4 = load_3ECE40();
    arg0->unk8 = &D_80090EC0_91AC0;
    arg0->unk22 = 0xFF;
    loadAssetMetadata(&arg0->unk8, arg0->unk4, 0x6A);
    setCleanupCallback(&func_80051800_52400);
    setCallbackWithContinue(&func_80051760_52360);
}

INCLUDE_ASM("asm/nonmatchings/51060", func_80051760_52360);

INCLUDE_ASM("asm/nonmatchings/51060", func_80051800_52400);

INCLUDE_ASM("asm/nonmatchings/51060", func_8005182C_5242C);

void func_8005186C_5246C(func_8005186C_5246C_arg *arg0) {
    arg0->unkC0 = load_3ECE40();
    setCleanupCallback(&func_80051B8C_5278C);
    setCallbackWithContinue(&func_800518AC_524AC);
}

INCLUDE_ASM("asm/nonmatchings/51060", func_800518AC_524AC);

INCLUDE_ASM("asm/nonmatchings/51060", func_80051978_52578);

INCLUDE_ASM("asm/nonmatchings/51060", func_80051B8C_5278C);

INCLUDE_ASM("asm/nonmatchings/51060", func_80051BB8_527B8);

INCLUDE_ASM("asm/nonmatchings/51060", func_80051C08_52808);
