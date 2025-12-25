#include "56910.h"
#include "common.h"
#include "displaylist.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    u8 _pad[0x20];
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 _pad2[0x4C - 0x30];
    s16 unk4C;
} func_800BB814_B5114_arg;

typedef struct {
    u8 _pad[0x3C];
    void *unk3C;
    void *unk40;
    u8 _pad2[0x10];
    void *unk54;
} func_800BB7D0_arg;

extern u8 D_800BBB68_B5468[];
extern u8 D_800BBB6C_B546C[];
extern u8 D_800BBB88_B5488[];
extern u8 D_800BBB8C_B548C[];

void func_800BBB18_B5418(func_800BB814_B5114_arg *arg0);

INCLUDE_ASM("asm/nonmatchings/levels/sunny_mountain", func_800BB2B0_B4BB0);

extern void func_800BB488_B4D88(void);

void func_800BB3D8_B4CD8(s32 *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();
    s32 i;

    for (i = 0; i < state->unk5F; i++) {
        if (state->players[i].unkB94 >= 0x3D) {
            arg0[0] = 0x1C84A5B9;
            arg0[1] = 0x0B1F0000;
            arg0[2] = 0x02C792C4;
            arg0[3] = 0x1CE44F4D;
            arg0[4] = 0x0B1F0000;
            arg0[5] = 0x02E17D96;
            setCallbackWithContinue(func_800BB488_B4D88);
            return;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/levels/sunny_mountain", func_800BB488_B4D88);

void func_800BB7D0_B50D0(func_800BB7D0_arg *arg0) {
    arg0->unk3C = freeNodeMemory(arg0->unk3C);
    arg0->unk40 = freeNodeMemory(arg0->unk40);
    arg0->unk54 = freeNodeMemory(arg0->unk54);
}

extern u16 D_800BBBA8_B54A8[];
extern s16 identityMatrix[];

typedef struct {
    DisplayListObject unk0;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s16 unk4C;
    u16 unk4E;
} func_800BBA34_B5334_arg;

void func_800BB890_B5190(func_800BBA34_B5334_arg *arg0);
void func_800BBA34_B5334(func_800BBA34_B5334_arg *arg0);

void func_800BB814_B5114(func_800BB814_B5114_arg *arg0) {
    GameState *state = (GameState *)getCurrentAllocation();

    arg0->unk20 = (void *)((u32)func_80055E68_56A68(state->memoryPoolId) + 0xB0);
    arg0->unk24 = func_80055DC4_569C4(state->memoryPoolId);
    arg0->unk28 = func_80055DF8_569F8(state->memoryPoolId);
    arg0->unk2C = 0;
    arg0->unk4C = 0x3C;
    setCleanupCallback(func_800BBB18_B5418);
    setCallback(func_800BB890_B5190);
}

void func_800BB890_B5190(func_800BBA34_B5334_arg *arg0) {
    s32 offset;
    s32 dx;
    s32 dy;
    s32 endX;
    s32 endY;
    s32 startX;
    s32 startY;
    s32 temp;

    if (arg0->unk4C != 0) {
        arg0->unk4C = arg0->unk4C - 1;
        return;
    }

    memcpy(arg0, identityMatrix, 0x20);

    offset = (randA() & 3) * 8;

    endX = *(s32 *)(D_800BBB88_B5488 + offset);
    startX = *(s32 *)(D_800BBB68_B5468 + offset);
    arg0->unk40 = (endX - startX) / 60;

    endY = *(s32 *)(D_800BBB8C_B548C + offset);
    startY = *(s32 *)(D_800BBB6C_B546C + offset);
    arg0->unk44 = (endY - startY) / 60;

    arg0->unk4E = func_8006D21C_6DE1C(*(s32 *)(D_800BBB88_B5488 + offset), *(s32 *)(D_800BBB8C_B548C + offset),
                                       *(s32 *)(D_800BBB68_B5468 + offset), *(s32 *)(D_800BBB6C_B546C + offset));

    dx = arg0->unk40;
    dy = arg0->unk44;

    temp = *(s32 *)(D_800BBB68_B5468 + offset);
    arg0->unk0.unk10.position.y = 0x243D1AC3;
    arg0->unk0.unk10.position.x = temp;
    arg0->unk0.unk10.position.z = *(s32 *)(D_800BBB6C_B546C + offset);

    arg0->unk48 = isqrt64((s64)dx * (s64)dx + (s64)dy * (s64)dy);
    arg0->unk3C = 0x300000;
    setCallbackWithContinue(func_800BBA34_B5334);
}

void func_800BBA34_B5334(func_800BBA34_B5334_arg *arg0) {
    int new_var;
    s16 angle;
    s32 temp_a2;
    s32 temp_a0;
    s32 temp_a3;
    s32 temp_v1;
    s32 i;
    s32 temp_a1;
    s32 temp_v0;

    angle = atan2Fixed(arg0->unk3C, -arg0->unk48);

    createCombinedRotationMatrix(arg0, angle, arg0->unk4E);

    temp_v0 = arg0->unk0.unk10.position.x;
    temp_a2 = arg0->unk40;
    temp_a0 = arg0->unk0.unk10.position.z;
    temp_a3 = arg0->unk44;
    temp_a1 = arg0->unk0.unk10.position.y;

    arg0->unk0.unk10.position.x = temp_v0 + temp_a2;
    arg0->unk0.unk10.position.z = temp_a0 + temp_a3;
    arg0->unk0.unk10.position.y = temp_a1 + arg0->unk3C;

    if ((arg0->unk3C = arg0->unk3C + 0xFFFE6667) < ((s32)0xFFD00000)) {
        new_var = randA() & 7;
        arg0->unk4C = D_800BBBA8_B54A8[new_var];
        setCallbackWithContinue(func_800BB890_B5190);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->unk0);
    }
}

void func_800BBB18_B5418(func_800BB814_B5114_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}
