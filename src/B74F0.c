#include "69EF0.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"

typedef struct {
    u8 padding[0x3C];
    s32 unk3C;
    u8 padding2[0x38];
    s32 unk78;
    u8 padding3[0x1C];
    s32 unk98;
    u8 padding4[0x8];
    u16 unkA4;
} func_800BB3B8_B75F8_arg;

typedef struct {
    u8 padding[0x24];
    s32 unk24;
    s32 unk28;
} func_800BBA60_B7CA0_arg;

typedef struct {
    u8 padding[0x90];
    s32 unk90;
    s32 padding2[3];
    s32 unkA0;
} func_80055E68_56A68_result;

typedef struct {
    Mat3x3 matrix;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 PAD[0xC];
    s16 unk3C;
    s16 unk3E;
} func_800BB808_B7A48_arg;

typedef struct {
    u16 unk0;
    u16 unk2;
    s32 unk4[3];
} D_800BBB90_Entry;

extern D_800BBB90_Entry D_800BBB90_B7DD0[];

typedef struct
{
    s8 pad_0[0x20];
    void *unk20;
    void *unk24;
    void *unk28;
    void *unk2C;
    s8 pad_30[0x2C];
    void *unk5C;
    void *unk60;
    void *unk64;
    void *unk68;
    s8 pad_6C[0xC];
    s16 unk78;
    s8 pad_7A[0x12];
    s16 unk8C[6];
    s32 unk98;
    s32 unk9C;
    s32 unkA0;
    s16 unkA4;
    s16 unkA6;
} func_800BB2B0_B74F0_arg;

typedef struct {
    u8 padding[0x78];
    s32 unk78;
    u8 padding2[0x10];
    s32 unk8C[3];
    u8 padding3[0xE];
    s16 unkA6;
    s16 unkA8;
} func_800BB71C_B795C_arg;

typedef struct {
    Node n;
    u8 padding[0x7A];
    s16 unkA6;
} NodeWithPayload;

extern func_80055E68_56A68_result *func_80055E68_56A68(u8);
extern s32 func_800AFF44_9FE04(void);
extern s32 D_8009A8A4_9B4A4;
extern void *func_80055DC4_569C4(u8);
extern void *func_80055DF8_569F8(u8);
extern void func_8006A724_6B324(void *, u16, u16);
extern void func_8006B084_6BC84(void *, void *, void *);
extern void func_800BB468_B76A8(void);
extern void func_8005C60C_5D20C(void *, void *, u8 *);
extern void func_800BB8B8_B7AF8;

void func_800BBA60_B7CA0(func_800BBA60_B7CA0_arg *arg0);
void func_80063788_64388(s32, void *);
void func_800BB7D0_B7A10(func_800BBA60_B7CA0_arg *);
void func_800BB71C_B795C(func_800BB71C_B795C_arg *);

void func_800BB2B0_B74F0(func_800BB2B0_B74F0_arg *arg0) {
    GameState *gameState;
    s32 temp;
    u16 var_v1;
    func_80055E68_56A68_result *result;

    gameState = GameStateGet();
    temp = func_800AFF44_9FE04();

    arg0->unkA6 += (temp & 1);
    result = func_80055E68_56A68(gameState->unk5C);
    arg0->unk20 = &result->unk90;
    result = func_80055E68_56A68(gameState->unk5C);
    arg0->unk5C = &result->unkA0;
    arg0->unk24 = func_80055DC4_569C4(gameState->unk5C);
    arg0->unk28 = func_80055DF8_569F8(gameState->unk5C);
    arg0->unk2C = 0;
    arg0->unk98 = 0;
    arg0->unk9C = 0;
    arg0->unkA0 = 0;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    arg0->unk68 = arg0->unk2C;
    memcpy(&arg0->unk8C, &D_800BBB90_B7DD0[arg0->unkA6].unk4, 0xC);
    func_8006A724_6B324(&arg0->unk78, D_800BBB90_B7DD0[arg0->unkA6].unk0, D_800BBB90_B7DD0[arg0->unkA6].unk2);
    arg0->unkA4 = 0;

    setCleanupCallback((void (*)(void *))func_800BB7D0_B7A10);
    setCallback((void (*)(void *))func_800BB468_B76A8);
}

void func_800BB3B8_B75F8(func_800BB3B8_B75F8_arg *arg0) {
    s32 matrix[8];  // this should be matrix[9] but it causes stack issues
    s32 i;

    memcpy(&D_8009A8A4_9B4A4, &arg0->unk98, 0xC);
    func_8006B084_6BC84(&D_8009A8A4_9B4A4 - 5, &arg0->unk78, arg0);
    createXRotationMatrix(&matrix, arg0->unkA4);

    matrix[6] = 0x3b333;
    matrix[5] = 0;
    matrix[7] = 0x170000;

    func_8006B084_6BC84(&matrix[0], arg0, &arg0->unk3C);

    for (i = 0; i < 4; i++) {
        func_80063788_64388(i, arg0);
        func_80063788_64388(i, &arg0->unk3C);
    }
}

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB468_B76A8);

typedef struct {
    u8 padding[0x50];
    s32 unk50;
    u8 padding2[0x48];
    s32 unk9C;
    s32 unkA0;
    s16 unkA4;
    s16 unkA6;
    s16 unkA8;
} func_800BB658_B7898_arg;
void func_800BB658_B7898(func_800BB658_B7898_arg *arg0) {
    GameState *gs;
    s32 i;

    gs = GameStateGet();
    if (arg0->unk9C != 0xFFF00000) {
        if (gs->PAD_6[0x17] == 0) {
            arg0->unk9C = arg0->unk9C - 0x8000;
        }
    } else {
        arg0->unkA8 = 0x12C;
        setCallback((void (*)(void *))&func_800BB71C_B795C);
    }
    arg0->unkA4 = 0;
    func_800BB3B8_B75F8(arg0);

    for (i = 0; i < gs->unk5E; i++) {
        func_8005C60C_5D20C(&arg0->unk50, (void *)0x12A000, &gs->unk10[i]);
    }
}

void func_800BB71C_B795C(func_800BB71C_B795C_arg *arg0) {
    u8 temp;

    if (GameStateGet()->PAD_6[0x17] == 0) {
        arg0->unkA8--;
    }

    if (!arg0->unkA8) {
        temp = func_800AFF44_9FE04() & 1;
        arg0->unkA6 = temp + (arg0->unkA6 & 0xFE);
        memcpy(&arg0->unk8C, &D_800BBB90_B7DD0[arg0->unkA6].unk4, 0xC);
        func_8006A724_6B324(&arg0->unk78, D_800BBB90_B7DD0[arg0->unkA6].unk0, D_800BBB90_B7DD0[arg0->unkA6].unk2);
        setCallback((void (*)(void *))&func_800BB468_B76A8);
    }
}

void func_800BB7D0_B7A10(func_800BBA60_B7CA0_arg *arg0) {
    arg0->unk24 = freeGameStateMemory(arg0->unk24);
    arg0->unk28 = freeGameStateMemory(arg0->unk28);
}

void func_800BB808_B7A48(func_800BB808_B7A48_arg *arg0) {
    GameState *gs = GameStateGet();
    arg0->unk24 = func_80055DC4_569C4(gs->unk5C);
    arg0->unk28 = func_80055DF8_569F8(gs->unk5C);
    arg0->unk2C = 0;
    createYRotationMatrix(&arg0->matrix, 0x6C0);
    arg0->unk14 = 0xDD196FEA;
    arg0->unk18 = 0x0ABD4CA3;
    arg0->unk1C = 0xE270649E;
    arg0->unk3C = 0x12C;
    arg0->unk3E = 0;
    arg0->unk20 = (s32)(func_80055E68_56A68(gs->unk5C)) + 0xB0;
    setCleanupCallback((void (*)(void *))&func_800BBA60_B7CA0);
    setCallback(&func_800BB8B8_B7AF8);
}

INCLUDE_ASM("asm/nonmatchings/B74F0", func_800BB8B8_B7AF8);

void func_800BBA60_B7CA0(func_800BBA60_B7CA0_arg *arg0) {
    arg0->unk24 = freeGameStateMemory(arg0->unk24);
    arg0->unk28 = freeGameStateMemory(arg0->unk28);
}

void func_800BBA98(void) {
    s32 i;
    NodeWithPayload *temp;

    temp = (NodeWithPayload *)scheduleTask(&func_800BB2B0_B74F0, 0, 0, 0x32);
    if (temp != NULL) {
        temp->unkA6 = 0;
    }

    temp = (NodeWithPayload *)scheduleTask(&func_800BB2B0_B74F0, 0, 0, 0x32);
    if (temp != NULL) {
        temp->unkA6 = 2;
    }

    temp = (NodeWithPayload *)scheduleTask(&func_800BB2B0_B74F0, 0, 0, 0x32);
    if (temp != NULL) {
        temp->unkA6 = 4;
    }

    temp = (NodeWithPayload *)scheduleTask(&func_800BB2B0_B74F0, 0, 0, 0x32);
    if (temp != NULL) {
        temp->unkA6 = 6;
    }

    temp = scheduleTask(&func_800BB2B0_B74F0, 0, 0, 0x32);
    if (temp != NULL) {
        temp->unkA6 = 8;
    }

    scheduleTask(&func_800BB808_B7A48, 0, 0, 0xC8);
}
