#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "task_scheduler.h"
#include "rand.h"
#include "594E0.h"

extern s32 isPlayerInRangeAndPull(void *a0, s32 a1, Player *a2);
extern void *func_80055E68_56A68(u8);
extern void *func_80055DC4_569C4(u8);
extern void *func_80055DF8_569F8(u8);

extern s32 D_800BBB90_B4B20[];
extern s32 D_800BBB94_B4B24[];
extern s32 D_800BBB98_B4B28[];
extern s32 D_800BBB9C_B4B2C[];
extern s32 D_800BBBA0_B4B30[];

typedef struct {
    u8 _pad0[0x24];
    void *unk24;
    void *unk28;
} B4240Struct;

typedef struct {
    u8 _pad0[0x30];
    void *unk30;
    u8 _pad34[0x28];
    u8 unk5C;
    u8 _pad5D[0x19];
    u8 unk76;
} B4240AllocationStruct;

typedef struct {
    DisplayListObject node;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s16 unk50;
    s16 unk52;
    u16 rotation;
    s16 unk56;
    s16 unk58;
    s16 unk5A;
} B4240FuncArg;

void func_800BB454_B43E4(B4240FuncArg *);
void func_800BB598_B4528(B4240FuncArg *);
void func_800BB63C_B45CC(B4240FuncArg *);
void func_800BB89C_B482C(B4240FuncArg *);
void func_800BBAFC_B4A8C(B4240Struct *);

void func_800BB2B0_B4240(B4240FuncArg *arg0) {
    s32 i;

    createYRotationMatrix((Mat3x3Padded *)arg0, arg0->rotation);
    memcpy(&arg0->node.unk10.position.X, &arg0->unk3C, 0xC);
    arg0->node.unk10.position.Y += arg0->unk48;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->node);
    }
}

void func_800BB320_B42B0(B4240FuncArg *arg0) {
    GameState *gs;
    s32 pos[3];
    s32 target[3];
    s32 i;
    Player *player;

    gs = getCurrentAllocation();
    memcpy(&pos[0], &arg0->unk3C, 0xC);
    pos[1] += 0xC0000 + arg0->unk48;

    for (i = 0; i < gs->numPlayers; i++) {
        player = &gs->players[i];
        if (isPlayerInRangeAndPull(&pos[0], 0xC0000, player) != 0) {
            target[0] = ((player->worldPosX + player->unkAD4[0] - pos[0]) / 2) + pos[0];
            target[1] = ((player->worldPosY + player->unkAD4[1] - pos[0]) / 2) + pos[1];
            target[2] = ((player->worldPosZ + player->unkAD4[2] - pos[0]) / 2) + pos[2];
            func_800589CC_595CC(&gs->players[i], &target[0]);
        }
    }
}

void func_800BB454_B43E4(B4240FuncArg *arg0) {
    B4240AllocationStruct *alloc;
    void *temp;
    s32 temp2;
    s32 temp3;
    s16 temp4;

    alloc = getCurrentAllocation();
    temp = func_80055E68_56A68(alloc->unk5C);
    arg0->node.unk20 = (DisplayLists *)((u8 *)temp + 0x90);
    arg0->node.unk24 = func_80055DC4_569C4(alloc->unk5C);
    arg0->node.unk28 = func_80055DF8_569F8(alloc->unk5C);
    arg0->node.unk2C = 0;
    arg0->unk52 = 1;

    temp2 = D_800BBB90_B4B20[arg0->unk50 * 5];
    arg0->unk56 = temp2;
    temp3 = D_800BBB94_B4B24[arg0->unk50 * 5];
    arg0->unk3C = temp3;
    arg0->unk44 = D_800BBB98_B4B28[arg0->unk50 * 5];

    arg0->unk40 = func_80061A64_62664(&alloc->unk30, arg0->unk56, &arg0->unk3C);

    temp4 = arg0->unk50;
    arg0->rotation =
        func_8006D21C_6DE1C(D_800BBB9C_B4B2C[temp4 * 5], D_800BBBA0_B4B30[temp4 * 5], arg0->unk3C, arg0->unk44);

    arg0->unk48 = 0;
    arg0->unk4C = 0;

    arg0->unk58 = ((randA() & 0xFF) >> 1) + 0xA;

    setCleanupCallback(func_800BBAFC_B4A8C);
    setCallbackWithContinue(func_800BB598_B4528);
}

void func_800BB598_B4528(B4240FuncArg *arg0) {
    B4240AllocationStruct *alloc;
    void (*callback)(B4240FuncArg *);
    s16 val;

    alloc = getCurrentAllocation();

    if (arg0->unk58 == 0) {
        if (randA() & 1) {
            callback = func_800BB63C_B45CC;
            arg0->unk4C = 0x30000;
            val = 3;
        } else {
            callback = func_800BB89C_B482C;
            arg0->unk4C = 0x20000;
            val = 6;
        }
        arg0->unk48 = 0;
        arg0->unk5A = val;
        setCallback(callback);
    }

    if (alloc->unk76 == 0) {
        arg0->unk58--;
        func_800BB320_B42B0(arg0);
    }

    func_800BB2B0_B4240(arg0);
}

INCLUDE_ASM("asm/nonmatchings/levels/jingle_town", func_800BB63C_B45CC);

INCLUDE_ASM("asm/nonmatchings/levels/jingle_town", func_800BB89C_B482C);

void func_800BBAFC_B4A8C(B4240Struct *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800BBB34(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        B4240FuncArg *task = scheduleTask(func_800BB454_B43E4, 0, 0, 0x32);
        if (task != NULL) {
            task->unk50 = i;
        }
    }
}
