#include "common.h"
#include "task_scheduler.h"

void func_8003C170_3CD70(void);

INCLUDE_ASM("asm/nonmatchings/3CD70", func_8003C170_3CD70);

INCLUDE_ASM("asm/nonmatchings/3CD70", func_8003C2BC_3CEBC);

void func_8003CEC4_3DAC4(u8 arg0) {
    Node *node = scheduleTask(func_8003C170_3CD70, 1, 0, 0xD2);
    if (node != NULL) {
        node->unkC = arg0;
        node->field_D = arg0;
    }
}

void func_8003CF40_3DB40(void);
void func_8003D0F4_3DCF4(NodeExt *arg0);
void func_8003D210_3DE10(void);

typedef struct {
    s16 unk0;
} func_8003CF0C_arg;

void func_8003CF0C_3DB0C(func_8003CF0C_arg *arg0) {
    getCurrentAllocation();
    arg0->unk0 = 0;
    setCallbackWithContinue(func_8003CF40_3DB40);
}

INCLUDE_ASM("asm/nonmatchings/3CD70", func_8003CF40_3DB40);

void func_8003D0C8_3DCC8(void) {
    scheduleTask(func_8003CF0C_3DB0C, 1, 0, 0xF0);
}

extern u8 D_800901CC_90DCC[];
extern u8 D_800901CE_90DCE[];
extern u8 D_800901D0_90DD0[];
extern u8 D_800901D4_90DD4[];

void func_8003D0F4_3DCF4(NodeExt *arg0) {
    s8 temp_v1;
    s32 offset;

    temp_v1 = arg0->unk26;
    offset = temp_v1 * 12;
    arg0->unk18 = *(void **)(D_800901D0_90DD0 + offset);

    temp_v1 = arg0->unk26;
    offset = temp_v1 * 12;
    arg0->unk1C = *(void **)(D_800901D4_90DD4 + offset);

    temp_v1 = arg0->unk26;
    offset = temp_v1 * 12;
    arg0->unk24 = *(u16 *)(D_800901CC_90DCC + offset);

    temp_v1 = arg0->unk26;
    offset = temp_v1 * 12;
    arg0->unk25 = *(u16 *)(D_800901CE_90DCE + offset);

    memcpy(arg0, (u8 *)arg0->unk18 + 4, 0xC);

    arg0->unk20 = *(u16 *)arg0->unk18;
    if (arg0->unk20 == 0) {
        arg0->unk18 = (u8 *)arg0->unk18 + 0x10;
        arg0->unk20 = *(u16 *)arg0->unk18;
    }

    if (arg0->unk1C != NULL) {
        memcpy(&arg0->unkC, (u8 *)arg0->unk1C + 4, 0xC);
        arg0->unk22 = *(u16 *)arg0->unk1C;
        if (arg0->unk22 == 0) {
            arg0->unk1C = (u8 *)arg0->unk1C + 0x10;
            arg0->unk22 = *(u16 *)arg0->unk1C;
        }
    }

    setCallbackWithContinue(func_8003D210_3DE10);
}

INCLUDE_ASM("asm/nonmatchings/3CD70", func_8003D210_3DE10);

void func_8003D51C_3E11C(u8 arg0) {
    NodeExt *task = (NodeExt *)scheduleTask(func_8003D0F4_3DCF4, 0, 2, 0xF0);
    if (task != NULL) {
        task->unk26 = arg0;
    }
}
