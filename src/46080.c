#include "5E590.h"
#include "common.h"
#include "task_scheduler.h"

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

extern void func_8004AA90_4B690();
extern void func_80049300_49F00();
extern void func_80049404_4A004();

extern s32 D_3F6670;
extern s32 D_3F6950;

void func_8004AE58_4BA58(s32 **);
void func_8004A634_4B234(void *);
void func_8004A96C_4B56C(s32 **);

INCLUDE_ASM("asm/nonmatchings/46080", func_80045480_46080);

INCLUDE_ASM("asm/nonmatchings/46080", func_80045564_46164);

INCLUDE_ASM("asm/nonmatchings/46080", func_800455BC_461BC);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004562C_4622C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80045768_46368);

INCLUDE_ASM("asm/nonmatchings/46080", func_800457A0_463A0);

INCLUDE_ASM("asm/nonmatchings/46080", func_800457E0_463E0);

INCLUDE_ASM("asm/nonmatchings/46080", func_80045878_46478);

INCLUDE_ASM("asm/nonmatchings/46080", func_800458AC_464AC);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004592C_4652C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80045964_46564);

INCLUDE_ASM("asm/nonmatchings/46080", func_800459A4_465A4);

INCLUDE_ASM("asm/nonmatchings/46080", func_80045A28_46628);

INCLUDE_ASM("asm/nonmatchings/46080", func_80045B3C_4673C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80045C84_46884);

INCLUDE_ASM("asm/nonmatchings/46080", func_80045CC8_468C8);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046244_46E44);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046298_46E98);

INCLUDE_RODATA("asm/nonmatchings/46080", jtbl_8009E798_9F398);

const char pushStartButtonText[] = "PUSH START BUTTON";

INCLUDE_ASM("asm/nonmatchings/46080", func_800462D8_46ED8);

INCLUDE_ASM("asm/nonmatchings/46080", func_800463F4_46FF4);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046464_47064);

INCLUDE_ASM("asm/nonmatchings/46080", func_800464F4_470F4);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004657C_4717C);

INCLUDE_ASM("asm/nonmatchings/46080", func_800465A8_471A8);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046628_47228);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046708_47308);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004674C_4734C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046CB4_478B4);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046CE0_478E0);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046D0C_4790C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046D38_47938);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046DCC_479CC);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046F44_47B44);

INCLUDE_ASM("asm/nonmatchings/46080", func_80046FEC_47BEC);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047024_47C24);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004711C_47D1C);

INCLUDE_ASM("asm/nonmatchings/46080", func_800471D0_47DD0);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004728C_47E8C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047330_47F30);

INCLUDE_ASM("asm/nonmatchings/46080", func_800473F4_47FF4);

INCLUDE_ASM("asm/nonmatchings/46080", func_800474B4_480B4);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047590_48190);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047660_48260);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047718_48318);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047750_48350);

INCLUDE_ASM("asm/nonmatchings/46080", func_800477E4_483E4);

INCLUDE_ASM("asm/nonmatchings/46080", func_800478FC_484FC);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047A64_48664);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047AA8_486A8);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047E7C_48A7C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047EBC_48ABC);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047EFC_48AFC);

INCLUDE_ASM("asm/nonmatchings/46080", func_80047F90_48B90);

INCLUDE_ASM("asm/nonmatchings/46080", func_800480A8_48CA8);

INCLUDE_ASM("asm/nonmatchings/46080", func_800481A0_48DA0);

INCLUDE_ASM("asm/nonmatchings/46080", func_800482A4_48EA4);

INCLUDE_ASM("asm/nonmatchings/46080", func_800482D0_48ED0);

INCLUDE_ASM("asm/nonmatchings/46080", func_80048310_48F10);

INCLUDE_ASM("asm/nonmatchings/46080", func_80048350_48F50);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004841C_4901C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80048540_49140);

INCLUDE_ASM("asm/nonmatchings/46080", func_80048720_49320);

INCLUDE_ASM("asm/nonmatchings/46080", func_80048834_49434);

INCLUDE_ASM("asm/nonmatchings/46080", func_80048860_49460);

INCLUDE_ASM("asm/nonmatchings/46080", func_800488A0_494A0);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004899C_4959C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80048AE8_496E8);

INCLUDE_ASM("asm/nonmatchings/46080", func_80048E34_49A34);

INCLUDE_ASM("asm/nonmatchings/46080", func_80048F0C_49B0C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80049104_49D04);

INCLUDE_ASM("asm/nonmatchings/46080", func_800491CC_49DCC);

INCLUDE_ASM("asm/nonmatchings/46080", func_80049230_49E30);

INCLUDE_ASM("asm/nonmatchings/46080", func_80049280_49E80);

void func_800492C0_49EC0(void **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(&func_80049404_4A004);
    setCallbackWithContinue(&func_80049300_49F00);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_80049300_49F00);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004934C_49F4C);

INCLUDE_ASM("asm/nonmatchings/46080", func_80049404_4A004);

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

INCLUDE_ASM("asm/nonmatchings/46080", func_80049BFC_4A7FC);

INCLUDE_ASM("asm/nonmatchings/46080", func_80049C38_4A838);

INCLUDE_ASM("asm/nonmatchings/46080", func_80049C70_4A870);

INCLUDE_ASM("asm/nonmatchings/46080", func_80049CA8_4A8A8);

void func_8004A5E0_4B1E0(void **arg0) {
    *arg0 = dmaRequestAndUpdateStateWithSize(&D_3F6670, &D_3F6950, 0x388);
    setCleanupCallback((void (*)(void *))&func_8004A96C_4B56C);
    setCallbackWithContinue(&func_8004A634_4B234);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004A634_4B234);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004A6D4_4B2D4);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004A850_4B450);

void func_8004A96C_4B56C(s32 **arg0) {
    GameState *temp_v0;

    temp_v0 = GameStateGet();
    temp_v0->PAD_5[0xB] += 1;
    *arg0 = (s32 *)freeGameStateMemory(*arg0);
}

void func_8004A9A8_4B5A8(void *arg0, s32 arg1, void *arg2) {
    GameState *gs;
    func_8004A9A8_4B5A8_node *node;

    gs = GameStateGet();
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
    setCleanupCallback((void (*)(void *))func_8004AE58_4BA58);
    setCallbackWithContinue(&func_8004AA90_4B690);
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004AA90_4B690);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004AB50_4B750);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004AD18_4B918);

void func_8004AE58_4BA58(s32 **arg0) {
    GameState *temp_v0;

    temp_v0 = GameStateGet();
    temp_v0->PAD_5[0xB] += 1;
    *arg0 = (s32 *)freeGameStateMemory(*arg0);
}

void func_8004AE94_4BA94(s32 arg0) {
    NodeWithPayload *temp_v0;

    GameStateGet();
    temp_v0 = (NodeWithPayload *)scheduleTask(&func_8004AA50_4B650, 3, 0, 0xEFU);
    if (temp_v0 != NULL) {
        temp_v0->unk30 = arg0;
    }
}

INCLUDE_ASM("asm/nonmatchings/46080", func_8004AED8_4BAD8);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004AF2C_4BB2C);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004AFF8_4BBF8);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B130_4BD30);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B264_4BE64);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B2A0_4BEA0);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B36C_4BF6C);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B3B0_4BFB0);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B4CC_4C0CC);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B648_4C248);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B758_4C358);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B794_4C394);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B7F4_4C3F4);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B834_4C434);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004B990_4C590);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004BB0C_4C70C);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004BC20_4C820);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004BC5C_4C85C);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004BCBC_4C8BC);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004BCFC_4C8FC);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004BE40_4CA40);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004BFBC_4CBBC);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004C0D0_4CCD0);

INCLUDE_ASM("asm/nonmatchings/46080", func_8004C10C_4CD0C);

INCLUDE_RODATA("asm/nonmatchings/46080", D_8009E868_9F468);

INCLUDE_RODATA("asm/nonmatchings/46080", D_8009E870_9F470);

INCLUDE_RODATA("asm/nonmatchings/46080", D_8009E878_9F478);
