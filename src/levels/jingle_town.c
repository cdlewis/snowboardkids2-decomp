#include "56910.h"
#include "594E0.h"
#include "5AA90.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

extern s32 D_800BBB90_B4B20[];
extern s32 D_800BBB94_B4B24[];
extern s32 D_800BBB98_B4B28[];
extern s32 D_800BBB9C_B4B2C[];
extern s32 D_800BBBA0_B4B30[];
extern void *D_800BBC14_B4BA4;
extern Vec3i D_800BBC08_B4B98;

typedef struct {
    u8 _pad0[0x30];
    void *unk30;
    u8 _pad34[0x42];
    u8 unk76;
} AllocationStruct;

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

    createYRotationMatrix((Transform3D *)arg0, arg0->rotation);
    memcpy(&arg0->node.unk10.position.x, &arg0->unk3C, 0xC);
    arg0->node.unk10.position.y += arg0->unk48;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, &arg0->node);
    }
}

void func_800BB320_B42B0(B4240FuncArg *arg0) {
    GameState *gs;
    Vec3i pos;
    s32 target[3];
    s32 i;
    Player *player;

    gs = getCurrentAllocation();
    memcpy(&pos, &arg0->unk3C, 0xC);
    pos.y += 0xC0000 + arg0->unk48;

    for (i = 0; i < gs->numPlayers; i++) {
        player = &gs->players[i];
        if (isPlayerInRangeAndPull(&pos, 0xC0000, player) != 0) {
            target[0] = ((player->worldPos.x + player->unkAD4[0] - pos.x) / 2) + pos.x;
            target[1] = ((player->worldPos.y + player->unkAD4[1] - pos.x) / 2) + pos.y;
            target[2] = ((player->worldPos.z + player->unkAD4[2] - pos.x) / 2) + pos.z;
            func_800589CC_595CC(&gs->players[i], &target[0]);
        }
    }
}

void func_800BB454_B43E4(B4240FuncArg *arg0) {
    B4240AllocationStruct *alloc;
    func_80055E68_56A68_result *temp;
    s32 temp2;
    s32 temp3;
    s16 temp4;

    alloc = getCurrentAllocation();
    temp = func_80055E68_56A68(alloc->unk5C);
    arg0->node.unk20 = &temp->unk90;
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

void func_800BB63C_B45CC(B4240FuncArg *arg0) {
    B4240AllocationStruct *alloc;
    void *terrainPtr;
    s32 *posPtr;
    Vec3i rotResult;
    s32 temp;
    s16 angleDiff;
    s16 newRotation;
    u16 newUnk56;

    alloc = getCurrentAllocation();
    if (alloc->unk76 == 0) {
        temp = (arg0->unk52 << 3) + (arg0->unk50 * 5 << 2);
        angleDiff =
            func_8006D21C_6DE1C(D_800BBB94_B4B24[temp >> 2], D_800BBB98_B4B28[temp >> 2], arg0->unk3C, arg0->unk44);
        angleDiff = (angleDiff - arg0->rotation) & 0x1FFF;
        if (angleDiff >= 0x1001) {
            angleDiff = angleDiff | 0xE000;
        }
        if (angleDiff >= 0x81) {
            angleDiff = 0x80;
        }
        if (angleDiff < -0x80) {
            angleDiff = -0x80;
        }
        newRotation = arg0->rotation + angleDiff;
        arg0->rotation = newRotation;
        rotateVectorY(&D_800BBC08_B4B98, newRotation, &rotResult);
        terrainPtr = &alloc->unk30;
        arg0->unk3C += rotResult.x;
        posPtr = &arg0->unk3C;
        arg0->unk44 += rotResult.z;
        newUnk56 = func_80060A3C_6163C(terrainPtr, arg0->unk56, posPtr);
        arg0->unk56 = newUnk56;
        arg0->unk40 = func_80061A64_62664(terrainPtr, newUnk56, posPtr);
        arg0->unk48 += arg0->unk4C;
        arg0->unk4C -= 0x8000;
        if (arg0->unk48 == 0) {
            arg0->unk5A--;
            if (arg0->unk5A == 0) {
                arg0->unk58 = ((randA() & 0xFF) >> 1) + 0xA;
                setCallback(func_800BB598_B4528);
            } else {
                arg0->unk4C = 0x30000;
            }
        }
        rotResult.x = D_800BBB94_B4B24[((arg0->unk52 << 3) + (arg0->unk50 * 5 << 2)) >> 2] - arg0->unk3C;
        rotResult.y = D_800BBB98_B4B28[((arg0->unk52 << 3) + (arg0->unk50 * 5 << 2)) >> 2] - arg0->unk44;
        if (((u32)(rotResult.x + 0xFFFFF) <= 0x1FFFFEUL) && ((u32)(rotResult.z + 0xFFFFF) <= 0x1FFFFEUL)) {
            arg0->unk52 = (arg0->unk52 + 1) & 1;
        }
        func_800BB320_B42B0(arg0);
    }
    func_800BB2B0_B4240(arg0);
}

void func_800BB89C_B482C(B4240FuncArg *arg0) {
    AllocationStruct *alloc;
    Vec3i rotatedVec;
    s32 idx;
    s16 delta;
    s16 var_v1;
    void *allocUnk30;
    s32 *pos;

    alloc = getCurrentAllocation();
    if (alloc->unk76 == 0) {
        idx = (arg0->unk52 * 8) + (arg0->unk50 * 0x14);
        delta = (func_8006D21C_6DE1C(
                     *(s32 *)((u8 *)D_800BBB94_B4B24 + idx),
                     *(s32 *)((u8 *)D_800BBB98_B4B28 + idx),
                     arg0->unk3C,
                     arg0->unk44
                 ) -
                 arg0->rotation) &
                0x1FFF;
        var_v1 = delta;
        if (delta >= 0x1001) {
            var_v1 = 0xE000;
            var_v1 = delta | var_v1;
        }
        if ((s16)var_v1 >= 0x81) {
            var_v1 = 0x80;
        }
        if ((s16)var_v1 < -0x80) {
            var_v1 = -0x80;
        }
        arg0->rotation = arg0->rotation + var_v1;
        rotateVectorY(&D_800BBC14_B4BA4, (s16)arg0->rotation, &rotatedVec);

        allocUnk30 = (u8 *)alloc + 0x30;
        arg0->unk3C += rotatedVec.x;
        pos = &arg0->unk3C;
        arg0->unk44 += rotatedVec.z;

        arg0->unk56 = func_80060A3C_6163C(allocUnk30, arg0->unk56, pos);
        arg0->unk40 = func_80061A64_62664(allocUnk30, (u16)arg0->unk56, pos);

        arg0->unk48 += arg0->unk4C;
        arg0->unk4C += -0x8000;

        if (arg0->unk48 == 0) {
            arg0->unk5A -= 1;
            if (arg0->unk5A == 0) {
                arg0->unk58 = ((randA() & 0xFF) >> 1) + 0xA;
                setCallback(func_800BB598_B4528);
            } else {
                arg0->unk4C = 0x20000;
            }
        }

        rotatedVec.x = *(s32 *)((u8 *)D_800BBB94_B4B24 + (arg0->unk52 * 8) + (arg0->unk50 * 0x14)) - arg0->unk3C;
        rotatedVec.y = *(s32 *)((u8 *)D_800BBB98_B4B28 + (arg0->unk52 * 8) + (arg0->unk50 * 0x14)) - arg0->unk44;

        if ((u32)(rotatedVec.x + 0xFFFFF) <= 0x1FFFFEU && (u32)(rotatedVec.z + 0xFFFFF) <= 0x1FFFFEU) {
            arg0->unk52 = (arg0->unk52 + 1) & 1;
        }

        func_800BB320_B42B0(arg0);
    }
    func_800BB2B0_B4240(arg0);
}

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
