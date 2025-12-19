#include "6E840.h"
#include "common.h"
#include "geometry.h"
#include "graphics.h"
#include "task_scheduler.h"

typedef struct {
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
} CameraData;

typedef struct {
    s32 unk0;          /* 0x00 */
    s32 unk4;          /* 0x04 */
    s32 unk8;          /* 0x08 */
    s32 unkC;          /* 0x0C */
    s32 unk10;         /* 0x10 */
    s32 unk14;         /* 0x14 */
    CameraData *unk18; /* 0x18 */
    CameraData *unk1C; /* 0x1C */
    s16 unk20;         /* 0x20 */
    s16 unk22;         /* 0x22 */
    s8 unk24;          /* 0x24 */
    s8 unk25;          /* 0x25 */
} func_8003D210_3DE10_arg;

typedef struct {
    s16 unk0;
} func_8003CF0C_arg;

typedef struct {
    s32 x;        /* 0x00 */
    s32 y;        /* 0x04 */
    s32 z;        /* 0x08 */
    u8 padC[0x4]; /* 0x0C */
    s32 unk10;    /* 0x10 */
    s32 unk14;    /* 0x14 */
    s32 unk18;    /* 0x18 */
    s32 unk1C;    /* 0x1C */
    s32 unk20;    /* 0x20 */
    s32 unk24;    /* 0x24 */
} NodeState;

typedef struct {
    u8 padding[0x434];
    void *unk434;
    u8 padding1[0xBCA - 0x438];
    u8 unkBCA;
    u8 padding2[0x101C - 0xBCC];
    void *unk101C;
    u8 padding3[0x1C04 - 0x1020];
    void *unk1C04;
} AllocationUnk10;

typedef struct {
    u8 pad0[0x10];
    /* 0x10 */ AllocationUnk10 *players;
} Allocation;

typedef struct {
    Node_70B00 *unk0;
    u8 pad4[0xC];
    u8 *unk10;
    u8 pad14[0x65];
    u8 unk79;
} func_8003CF40_Allocation;

extern void func_8003C2BC_3CEBC(void);
extern s32 D_8008FEB0_90AB0;
extern u8 identityMatrix[];
void func_8003CF40_3DB40(s16 *arg0);
void func_8003D0F4_3DCF4(NodeExt *arg0);
void func_8003D210_3DE10(func_8003D210_3DE10_arg *);

void func_8003C170_3CD70(NodeState *arg0) {
    Allocation *allocation;
    u8 playerIdx;
    s32 tempVec[3];
    s32 outputVec[3];
    Mat3x3Padded mtx;

    allocation = (Allocation *)getCurrentAllocation();

    // Initialize input vector on stack
    tempVec[1] = 0;
    tempVec[0] = 0;
    tempVec[2] = 0xFFC00000;

    // Get player index from offset 0xC
    playerIdx = *(u8 *)((u8 *)arg0 + 0xC);

    // Read rotation at offset 0xA94 from players[playerIdx]
    createYRotationMatrix(&mtx, *(u16 *)((u8 *)allocation->players + (playerIdx * 0xBE8) + 0xA94));

    // Transform vector
    transformVector2(tempVec, &mtx, outputVec);

    // Read player index again
    playerIdx = *(u8 *)((u8 *)arg0 + 0xC);

    // Set position X with transformed vector offset
    arg0->x = *(s32 *)((u8 *)allocation->players + (playerIdx * 0xBE8) + 0x434) + outputVec[0];

    // Read player index again
    playerIdx = *(u8 *)((u8 *)arg0 + 0xC);

    // Set position Y with transformed vector offset
    arg0->y = *(s32 *)((u8 *)allocation->players + (playerIdx * 0xBE8) + 0x438) + outputVec[1];

    // Read player index again
    playerIdx = *(u8 *)((u8 *)arg0 + 0xC);

    // Set position Z with transformed vector offset
    arg0->z = *(s32 *)((u8 *)allocation->players + (playerIdx * 0xBE8) + 0x43C) + outputVec[2];

    // Set various node fields
    arg0->unk10 = 0x600000;
    arg0->unk1C = 0x20000;
    arg0->unk18 = 0x2C0000;
    arg0->unk20 = 0;
    arg0->unk24 = 0;
    arg0->unk14 = 0x1E0000;

    // Set callback
    setCallbackWithContinue(func_8003C2BC_3CEBC);
}

INCLUDE_ASM("asm/nonmatchings/3CD70", func_8003C2BC_3CEBC);

void func_8003CEC4_3DAC4(u8 arg0) {
    Node *node = scheduleTask(func_8003C170_3CD70, 1, 0, 0xD2);
    if (node != NULL) {
        node->unkC = arg0;
        node->field_D = arg0;
    }
}

void func_8003CF0C_3DB0C(func_8003CF0C_arg *arg0) {
    getCurrentAllocation();
    arg0->unk0 = 0;
    setCallbackWithContinue(func_8003CF40_3DB40);
}

void func_8003CF40_3DB40(s16 *arg0) {
    func_8003CF40_Allocation *s1;
    Mat3x3Padded sp10;
    Mat3x3Padded sp30;
    Mat3x3Padded sp50;
    s32 sp70[3];
    s16 temp_v0;
    s16 temp_v1;
    Mat3x3Padded *s2;

    s1 = getCurrentAllocation();
    s2 = &sp10;
    memcpy(s2, identityMatrix, 0x20);
    sp10.unk18 = 0x1E0000;
    sp10.unk1C = 0x870000;
    temp_v0 = *arg0 + 0x58;
    *arg0 = temp_v0;
    if (temp_v0 == 0x12E8) {
        func_8006FE28_70A28(s1->unk0, 0xFF, 0xFF, 0xFF);
        func_8006FDA0_709A0(s1->unk0, 0xFF, 0x20);
    }
    temp_v1 = *arg0;
    if (temp_v1 == 0x1EF0) {
        s1->unk79 = s1->unk79 - 1;
        func_80069CF8_6A8F8();
        func_8006FDA0_709A0(s1->unk0, 0, 0x10);
        return;
    }
    createYRotationMatrix(&sp30, (temp_v1 + 0x1000) & 0xFFFF);
    memcpy(&sp30.unk14, (u8 *)identityMatrix + 0x14, 0xC);
    func_8006B084_6BC84(s2, &sp30, &sp50);
    func_8006B084_6BC84(&sp50, s1->unk10 + 0x950, s2);
    transformVector2(&D_8008FEB0_90AB0, s1->unk10 + 0x950, sp70);
    sp10.unk14 = sp10.unk14 + sp70[0];
    sp10.unk18 = sp10.unk18 + sp70[1];
    sp10.unk1C = sp10.unk1C + sp70[2];
    func_8006FD3C_7093C(0x64, s2);
}

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

void func_8003D210_3DE10(func_8003D210_3DE10_arg *arg0) {
    Allocation *allocation;
    u8 sp10[0x30];

    allocation = getCurrentAllocation();

    if (arg0->unk24 == 0) {
        arg0->unk0 += ((arg0->unk18->unk8 - arg0->unk0) / arg0->unk20);
        arg0->unk4 += ((arg0->unk18->unkC - arg0->unk4) / arg0->unk20);
        arg0->unk8 += ((arg0->unk18->unk10 - arg0->unk8) / arg0->unk20);
        arg0->unk20--;
        if (arg0->unk20 == 0) {
            arg0->unk18 = (CameraData *)((u8 *)arg0->unk18 + 0x10);
            arg0->unk20 = *(s16 *)arg0->unk18;
        }
    }

    switch (arg0->unk25) {
        case 0:
            memcpy(&arg0->unkC, &allocation->players->unk434, 0xC);
            arg0->unk10 += 0x200000;
            break;
        case 1:
            arg0->unkC += ((arg0->unk1C->unk8 - arg0->unkC) / arg0->unk22);
            arg0->unk10 += ((arg0->unk1C->unkC - arg0->unk10) / arg0->unk22);
            arg0->unk14 += ((arg0->unk1C->unk10 - arg0->unk14) / arg0->unk22);
            arg0->unk22--;
            if (arg0->unk22 == 0) {
                arg0->unk1C = (CameraData *)((u8 *)arg0->unk1C + 0x10);
                arg0->unk22 = *(s16 *)arg0->unk1C;
            }
            break;
        case 2:
            memcpy(&arg0->unkC, &allocation->players->unk1C04, 0xC);
            arg0->unk10 += 0x200000;
            break;
        case 3:
            memcpy(&arg0->unkC, &allocation->players->unk101C, 0xC);
            arg0->unk10 += 0x200000;
            break;
    }

    computeLookAtMatrix(arg0, &arg0->unkC, sp10);
    func_8006FD3C_7093C(0x64, sp10);

    if (allocation->players->unkBCA == 0) {
        func_800569A4_575A4(sp10, 0);
    } else {
        setBufferData(sp10, 0x30, 0);
    }
}

void func_8003D51C_3E11C(u8 arg0) {
    NodeExt *task = (NodeExt *)scheduleTask(func_8003D0F4_3DCF4, 0, 2, 0xF0);
    if (task != NULL) {
        task->unk26 = arg0;
    }
}
