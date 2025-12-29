#include "3E160.h"
#include "42170.h"
#include "52880.h"
#include "56910.h"
#include "5AA90.h"
#include "5DBC0.h"
#include "5E590.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "rand.h"
#include "task_scheduler.h"

typedef struct {
    void *unk00;
    void *unk04;
    s8 unk08[0x16];
    u8 unk1E;
    s8 unk1F[0x5];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s16 unk3C;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    s16 unk44;
    u8 unk46;
} func_800BB388_AF078_arg;

typedef struct {
    s32 unk00;
    s32 unk04;
    s32 unk08;
} Vec3;

typedef struct {
    s16 unk0;
    s16 unk2;
} func_800BB74C_AF43C_arg;

typedef struct {
    void *ghostSlotData;
    void *unk4;
} GhostManager;

typedef struct {
    u8 pad[0x30];
    void *unk30;
} AllocationUnk30;

typedef struct {
    u8 pad[0x10];
    Player *unk10;
    u8 pad14[0x1C];
    void *unk30;
    u8 pad34[0x2A];
    u8 unk5E;
    u8 pad5F[0x17];
    u8 unk76;
} Allocation;

typedef struct {
    u8 pad[0x24];
    s32 unk24;
} Task;

typedef struct {
    u8 _pad[0x24];
    void *unk24;
    void *unk28;
} func_800BBC2C_AF91C_arg;

typedef struct {
    /* 0x00 */ u8 pad[0x14];
    /* 0x14 */ Vec3i unk14;
    /* 0x20 */ void *unk20;
    /* 0x24 */ void *unk24;
    /* 0x28 */ void *unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ u8 pad30[0xC];
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s16 unk48;
    /* 0x4A */ s16 unk4A;
    /* 0x4C */ s16 unk4C;
    /* 0x4E */ s16 unk4E;
    /* 0x50 */ s16 unk50;
} func_800BB8E8_AF5D8_arg;

typedef struct {
    void *unk0;
    void *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad[0xA];
    u8 unk1E;
    u8 pad2[0x5];
    s32 unk24;
    s16 unk28;
} func_800BBC64_AF954_arg;

typedef struct {
    s8 unk0;
    s8 unk1;
} AnimationData;

typedef struct {
    void *matrixData;
    void *textureTable;
    u8 textureIndices[8];
} GhostRenderState;

extern s32 D_8009A8A4_9B4A4;
extern void *D_800BC7F0_B04E0;
extern Vec3 D_800BCAA0_B0790;
extern AnimationData D_800BC830_B0520[];
extern void *D_800BC8C8_B05B8;
extern s8 D_800BC908_B05F8[12];
extern void *D_800BC920_B0610;
extern void *D_800BC960_B0650;
extern s32 D_800BC9A0_B0690[];
extern s16 D_800BC9DC_B06CC[];
extern s16 D_800BC9E8_B06D8[];
extern s16 D_800BC9F4_B06E4[];
extern Vec3i D_800BCA00_B06F0[];
extern s32 D_800BC914_B0604;
extern s32 D_800BC918_B0608;
extern s32 D_800BC91C_B060C;
extern Vec3i D_800BCA00_B06F0[];
extern Gfx D_8009A780_9B380[];
extern Gfx D_800BCA60_B0750[];
extern s32 D_800A8B14_9FE84;
extern s16 gGraphicsMode;
extern Gfx *gRegionAllocPtr;

void func_800BB45C_AF14C(void **);
void func_800BB5B0_AF2A0(func_800BB388_AF078_arg *);
void func_800BB620_AF310(func_800BB388_AF078_arg *);
void func_800BB6F4_AF3E4(func_800BB388_AF078_arg *);
void func_800BB778_AF468(void);
void func_800BB9A4_AF694(func_800BB8E8_AF5D8_arg *);
void func_800BBC2C_AF91C(func_800BBC2C_AF91C_arg *);
void func_800BBC64_AF954(func_800BBC64_AF954_arg *);
void func_800BBEAC_AFB9C(s16 *);
void func_800BBCE8_AF9D8(void **);
void func_800BBD14_AFA04(func_800BBC64_AF954_arg *);
void func_800BC184_AFE74(GhostManager *);
void func_800BC220_AFF10(u8 *ghostSlots);
void func_800BC340_B0030(GhostManager *);
void func_800BC750_B0440(s16 *);
void func_800BB2B0_AEFA0(func_800BB388_AF078_arg *);
void func_800BC378_B0068(GhostRenderState *);

void func_800BB2B0_AEFA0(func_800BB388_AF078_arg *arg0) {
    s32 i;

    arg0->unk40--;

    if (arg0->unk40 == 0) {
        loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk04, arg0->unk00, D_800BC830_B0520[arg0->unk44].unk1);

        arg0->unk40 = D_800BC830_B0520[arg0->unk44].unk0;
        arg0->unk44++;

        if (D_800BC830_B0520[arg0->unk44].unk0 == 0) {
            arg0->unk44 = 0;
        }
    }

    memcpy(&arg0->unk08, &arg0->unk24, 0xC);

    for (i = 0; i < 4; i++) {
        func_800677C0_683C0(i, (loadAssetMetadata_arg *)&arg0->unk04);
    }
}

void func_800BB388_AF078(func_800BB388_AF078_arg *entity) {
    Vec3 vector;
    Vec3 *vector_ptr;
    s32 random_value;
    s32 rotation_angle;

    vector_ptr = &vector;
    memcpy(vector_ptr, &D_800BCAA0_B0790, 12);

    getCurrentAllocation();

    entity->unk00 = load_3ECE40();
    entity->unk04 = &D_800BC7F0_B04E0;
    entity->unk1E = 0;

    random_value = randA() & 0xFF;
    random_value = random_value << 10;
    vector.unk08 = random_value + 0x20000;

    rotation_angle = randA() & 0xFF;
    rotation_angle = rotation_angle << 5;

    rotateVectorY(vector_ptr, rotation_angle, &entity->unk30);

    if (entity->unk3C == 0) {
        entity->unk3E = 0x1A;
    } else {
        entity->unk3E = 0x58;
    }

    entity->unk40 = 1;
    entity->unk44 = 0;
    entity->unk42 = 0x3C;

    setCleanupCallback(func_800BB45C_AF14C);
    setCallbackWithContinue(func_800BB5B0_AF2A0);
}

void func_800BB45C_AF14C(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

s32 func_800BB488_AF178(func_800BB388_AF078_arg *arg0) {
    s32 sp18[3];
    Allocation *allocation;
    void *allocationUnk30;
    s32 *argPos;
    s32 result;
    u16 temp;

    result = 0;
    allocation = (Allocation *)getCurrentAllocation();

    if (allocation->unk76 == 0) {
        allocationUnk30 = &allocation->unk30;

        arg0->unk24 = arg0->unk24 + arg0->unk30;
        arg0->unk2C = arg0->unk2C + arg0->unk38;

        argPos = &arg0->unk24;

        temp = func_80060A3C_6163C(allocationUnk30, arg0->unk3E, argPos);
        arg0->unk3E = temp;

        func_80060CDC_618DC(allocationUnk30, temp, argPos, 0x80000, sp18);

        arg0->unk28 = func_8005CFC0_5DBC0(allocationUnk30, (u16)arg0->unk3E, argPos, 0x100000) + 0x180000;

        if (arg0->unk3C == 0) {
            result = (arg0->unk3E != 0x1A);
        } else {
            u16 v = arg0->unk3E;
            if ((u16)(v - 0x58) >= 2) {
                result = 1;
            }
        }

        if (sp18[0] != 0) {
            result = 1;
        } else if (sp18[2] != 0) {
            result = 1;
        }

        if (arg0->unk42 == 0) {
            result = 1;
        } else {
            arg0->unk42 = arg0->unk42 - 1;
        }
    }

    return result;
}

void func_800BB5B0_AF2A0(func_800BB388_AF078_arg *arg0) {
    arg0->unk1E += 0x10;

    if (arg0->unk1E == 0xE0) {
        arg0->unk46 = 1;
        setCallback(func_800BB620_AF310);
    }

    if (func_800BB488_AF178(arg0)) {
        setCallback(func_800BB6F4_AF3E4);
    }

    func_800BB2B0_AEFA0(arg0);
}

void func_800BB620_AF310(func_800BB388_AF078_arg *arg0) {
    u8 temp;
    Player *temp_a;

    if (arg0->unk46 != 0) {
        arg0->unk1E -= 0x10;
        if (arg0->unk1E < 0x41) {
            arg0->unk46 = 0;
        }
    } else {
        arg0->unk1E += 0x10;
        if (arg0->unk1E >= 0xE0) {
            arg0->unk46 = 1;
        }
    }

    if (func_800BB488_AF178(arg0) != 0) {
        setCallback(func_800BB6F4_AF3E4);
    }

    temp_a = func_8005B548_5C148(&arg0->unk24, -1, 0x100000);

    if (temp_a != NULL) {
        if (temp_a->unkBCF < 3) {
            func_80042FC0_43BC0(temp_a);
        }

        setCallback(func_800BB6F4_AF3E4);
    }

    func_800BB2B0_AEFA0(arg0);
}

void func_800BB6F4_AF3E4(func_800BB388_AF078_arg *arg0) {
    arg0->unk1E -= 0x10;

    if (arg0->unk1E == 0) {
        func_80069CF8_6A8F8();
    } else {
        func_800BB488_AF178(arg0);
        func_800BB2B0_AEFA0(arg0);
    }
}

void func_800BB74C_AF43C(func_800BB74C_AF43C_arg *arg0) {
    arg0->unk0 = 0;
    arg0->unk2 = 0x14;
    setCallback(func_800BB778_AF468);
}

INCLUDE_ASM("asm/nonmatchings/levels/haunted_house", func_800BB778_AF468);

void func_800BB8E8_AF5D8(func_800BB8E8_AF5D8_arg *arg0) {
    func_80055E68_56A68_result *temp_v0_3;
    u16 temp_a1;

    arg0->unk24 = func_80055DC4_569C4(9);
    arg0->unk28 = func_80055DF8_569F8(9);
    arg0->unk2C = 0;

    temp_v0_3 = func_80055E68_56A68(9);
    arg0->unk20 = &temp_v0_3->unk90;

    arg0->unk4E = 0xF;
    arg0->unk3C = 0xFD71ABE1;
    arg0->unk40 = 0x1E37049C;
    arg0->unk44 = 0xF61F3676;
    arg0->unk4C = 0;
    arg0->unk48 = 0;
    arg0->unk4A = 0x400;
    arg0->unk50 = 0x1E;

    temp_a1 = arg0->unk4A + 0x2A0;
    createYRotationMatrix((Transform3D *)arg0, temp_a1 & 0xFFFF);
    setCleanupCallback(func_800BBC2C_AF91C);
    setCallback(func_800BB9A4_AF694);
}

void func_800BB9A4_AF694(func_800BB8E8_AF5D8_arg *arg0) {
    Allocation *allocation;
    s32 i;
    s32 offset;
    Vec3i *pos;
    Player *player;

    allocation = (Allocation *)getCurrentAllocation();

    if (allocation->unk76 == 0) {
        switch (arg0->unk48) {
            case 0:
                arg0->unk4E--;
                if (arg0->unk4E != 0) {
                    break;
                }
                arg0->unk48++;
                /* fallthrough */
            case 1:
                arg0->unk50--;
                arg0->unk4A -= 0x10;
                if ((s16)arg0->unk4A != -0x400) {
                    break;
                }
                arg0->unk4E = 0xF;
                arg0->unk48++;
                /* fallthrough */
            case 2:
                arg0->unk4E--;
                if (arg0->unk4E != 0) {
                    break;
                }
                arg0->unk48++;
                /* fallthrough */
            case 3:
                arg0->unk50--;
                arg0->unk4A += 0x10;
                if ((s16)arg0->unk4A != 0x400) {
                    break;
                }
                arg0->unk48 = 0;
                arg0->unk4E = 0xF;
                break;
        }

        arg0->unk4C += 0x40;

        i = 0;
        if (allocation->unk5E > i) {
            do {
                isPlayerInRangeAndPull(&arg0->unk14, 0x200000, allocation->unk10 + i);
                i += 1;
            } while (i < allocation->unk5E);
        }

        createYRotationMatrix((Transform3D *)arg0, (arg0->unk4A + 0x2A0) & 0xFFFF);

        if (arg0->unk50 == 0) {
            arg0->unk50 = (randA() & 0xF) + 0xF;

            for (i = 0; i < allocation->unk5E; i++) {
                player = allocation->unk10 + i;
                if ((u16)(player->unkB94 - 0x38) < 5) {
                    break;
                }
            }

            if (i < allocation->unk5E) {
                pos = &arg0->unk14;
                func_80055C80_56880(0, (s16)(arg0->unk4A + 0x2A0), pos);
                func_80055C80_56880(1, (s16)(arg0->unk4A + 0x2A0), pos);
            }
        }
    }

    memcpy(&arg0->unk14, &arg0->unk3C, 0xC);
    arg0->unk14.y = arg0->unk14.y + approximateSin(arg0->unk4C) * 0x1C0;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_800BBC2C_AF91C(func_800BBC2C_AF91C_arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800BBC64_AF954(func_800BBC64_AF954_arg *arg0) {
    s32 temp_v1;

    arg0->unk0 = func_80055D7C_5697C(9);
    arg0->unk4 = &D_800BC8C8_B05B8;
    arg0->unk1E = 0xFF;
    memcpy(&arg0->unk8, &D_800BC908_B05F8, 12);
    temp_v1 = arg0->unkC + arg0->unk24;
    arg0->unk28 = 0xB4;
    arg0->unkC = temp_v1;
    setCleanupCallback(func_800BBCE8_AF9D8);
    setCallbackWithContinue(func_800BBD14_AFA04);
}

void func_800BBCE8_AF9D8(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void func_800BBD14_AFA04(func_800BBC64_AF954_arg *arg0) {
    Allocation *allocation;
    s32 i;
    s32 *ptr8;
    s32 *ptrC;
    s32 *ptr10;

    allocation = getCurrentAllocation();

    if (allocation->unk76 == 0) {
        arg0->unk28 -= 1;

        if (arg0->unk28 == 0) {
            func_80069CF8_6A8F8();
            return;
        }

        ptr8 = &arg0->unk8;
        *ptr8 += (D_800BC914_B0604 - *ptr8) / arg0->unk28;

        ptrC = &arg0->unkC;
        *ptrC += (D_800BC918_B0608 - (*ptrC + arg0->unk24)) / arg0->unk28;

        ptr10 = &arg0->unk10;
        *ptr10 += (D_800BC91C_B060C - *ptr10) / arg0->unk28;
    }

    loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->unk0, 5);

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
    }
}

void func_800BBE84_AFB74(s16 *arg0) {
    *arg0 = 0xB4;
    setCallback(func_800BBEAC_AFB9C);
}

void func_800BBEAC_AFB9C(s16 *arg0) {
    Allocation *allocation;
    Task *task;
    u8 rand;
    s32 value;

    allocation = (Allocation *)getCurrentAllocation();
    if (allocation->unk76 != 0) {
        return;
    }

    *arg0 -= 1;
    if (*arg0 != 0) {
        return;
    }

    task = (Task *)scheduleTask(func_800BBC64_AF954, 0, 0, 0xC8);
    if (task != NULL) {
        rand = randA() & 0xFF;
        value = (rand * 5) << 13;
        task->unk24 = value;
    }

    rand = randA() & 0x1F;
    *arg0 = rand + 0xB4;
}

typedef struct {
    void *unk0;
    void *unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad14[0x10];
    s16 unk24;
    s16 unk26;
} func_800BBF4C_AFC3C_arg;

extern void func_800BC0D0_AFDC0(void **);
void func_800BBFC8_AFCB8(func_800BBF4C_AFC3C_arg *);

void func_800BBF4C_AFC3C(func_800BBF4C_AFC3C_arg *arg0) {
    s16 index;

    arg0->unk0 = func_80055D7C_5697C(9);
    arg0->unk4 = &D_800BC920_B0610;

    index = arg0->unk24;
    if (D_800BC9DC_B06CC[index] == 7) {
        arg0->unk4 = &D_800BC960_B0650;
    }

    setCleanupCallback(func_800BC0D0_AFDC0);
    setCallback(func_800BBFC8_AFCB8);
}

void func_800BBFC8_AFCB8(func_800BBF4C_AFC3C_arg *arg0) {
    Allocation *allocation;
    s32 i;
    s16 index;
    s32 sinResult;
    s16 multiplier;
    s32 *src;

    allocation = (Allocation *)getCurrentAllocation();

    if (allocation->unk76 == 0) {
        index = arg0->unk24;
        arg0->unk26 += D_800BC9F4_B06E4[index];
    }

    index = arg0->unk24;
    src = D_800BC9A0_B0690;
    memcpy(&arg0->unk8, &src[index * 3], 0xC);

    sinResult = approximateSin(arg0->unk26);

    index = arg0->unk24;
    multiplier = D_800BC9E8_B06D8[index];
    arg0->unkC += sinResult * multiplier;

    index = arg0->unk24;
    loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->unk0, D_800BC9DC_B06CC[index]);

    func_8005BCB8_5C8B8(&arg0->unk8, 0x140000, 0x300000);

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, (func_80066444_67044_arg1 *)&arg0->unk4);
    }
}

void func_800BC0D0_AFDC0(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

typedef struct {
    u8 _pad[0x24];
    s16 unk24;
} func_800BC0FC_Task;

void func_800BC0FC(s16 arg0) {
    func_800BC0FC_Task *task = (func_800BC0FC_Task *)scheduleTask(func_800BBF4C_AFC3C, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk24 = arg0;
    }
}

void func_800BC13C_AFE2C(GhostManager *ghostManager) {
    ghostManager->unk4 = func_80055D7C_5697C(9);
    ghostManager->ghostSlotData = 0;
    setCleanupCallback(func_800BC340_B0030);
    setCallback(func_800BC184_AFE74);
}

void func_800BC184_AFE74(GhostManager *ghostManager) {
    s32 ghostIndex;
    s32 *tempPosition;
    Vec3i *spawnPositions;

    ghostManager->ghostSlotData = allocateNodeMemory(0x200);

    ghostIndex = 0;
    tempPosition = &D_8009A8A4_9B4A4;
    spawnPositions = D_800BCA00_B06F0;

    while (ghostIndex < 8) {
        memcpy(tempPosition, spawnPositions, 0xC);
        func_8006BFB8_6CBB8(tempPosition - 5, (void *)((u8 *)ghostManager->ghostSlotData + (ghostIndex << 6)));
        spawnPositions++;
        ghostIndex++;
    }

    setCallback(func_800BC220_AFF10);
}

void func_800BC220_AFF10(u8 *ghostSlots) {
    GameState *gameState;
    s32 i;
    s32 maxRaceState;
    s32 activeGhostCount;
    s32 playerCount;
    Player *player;
    s32 numPlayersToCheck;
    s32 raceState;
    s32 pad[4];

    (void)pad;

    gameState = getCurrentAllocation();
    numPlayersToCheck = gameState->numPlayers;
    maxRaceState = 0;
    i = 0;

    if (numPlayersToCheck > 0) {
        playerCount = numPlayersToCheck;
        player = gameState->players;
        do {
            raceState = player->unkB94;
            if ((u32)(raceState - 7) < 5U) {
                raceState &= 0xFFFF;
                if (maxRaceState < raceState) {
                    maxRaceState = raceState;
                }
            }
            i++;
            player = (Player *)((u8 *)player + sizeof(Player));
        } while (i < playerCount);
    }

    activeGhostCount = ((maxRaceState < 7) ^ 1) * 2;
    if (maxRaceState >= 8) {
        activeGhostCount = 4;
    }
    if (maxRaceState >= 9) {
        activeGhostCount = 6;
    }
    if (maxRaceState >= 10) {
        activeGhostCount = 8;
    }

    i = 0;
    if (activeGhostCount != 0) {
        do {
            (ghostSlots + i)[8] = 8;
            i++;
        } while (i < activeGhostCount);
    }

    i = activeGhostCount;
    while (i < 8) {
        (ghostSlots + i)[8] = 9;
        i++;
    }

    for (i = 0; i < 4; i++) {
        debugEnqueueCallback((u16)i, 4, func_800BC378_B0068, ghostSlots);
    }
}

void func_800BC340_B0030(GhostManager *ghostManager) {
    ghostManager->ghostSlotData = freeNodeMemory(ghostManager->ghostSlotData);
    ghostManager->unk4 = freeNodeMemory(ghostManager->unk4);
}

void func_800BC378_B0068(GhostRenderState *state) {
    OutputStruct_19E80 tableEntry;
    s32 prevTextureIndex;
    s32 i;

    prevTextureIndex = -1;
    gGraphicsMode = -1;
    gSPDisplayList(gRegionAllocPtr++, D_8009A780_9B380);

    for (i = 0; i < 8; i++) {
        if (isObjectCulled(&D_800BCA00_B06F0[i]) == 0) {
            u8 textureIndex = state->textureIndices[i];

            if (textureIndex != prevTextureIndex) {
                prevTextureIndex = textureIndex;
                getTableEntryByU16Index(state->textureTable, (u16)prevTextureIndex, &tableEntry);

                gDPLoadMultiBlock_4b(
                    gRegionAllocPtr++,
                    tableEntry.data_ptr,
                    0,
                    G_TX_RENDERTILE,
                    G_IM_FMT_CI,
                    tableEntry.field1,
                    tableEntry.field2,
                    0,
                    G_TX_CLAMP,
                    G_TX_CLAMP,
                    G_TX_NOMASK,
                    G_TX_NOMASK,
                    G_TX_NOLOD,
                    G_TX_NOLOD
                );

                gDPLoadTLUT_pal16(gRegionAllocPtr++, 0, tableEntry.index_ptr);
            }

            gSPMatrix(
                gRegionAllocPtr++,
                (u8 *)state->matrixData + (i << 6),
                G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW
            );

            gSPMatrix(gRegionAllocPtr++, D_800A8B14_9FE84, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);

            gSPVertex(gRegionAllocPtr++, D_800BCA60_B0750, 4, 0);

            gSP2Triangles(gRegionAllocPtr++, 0, 3, 2, 0, 2, 1, 0, 0);
        }
    }
}

void func_800BC72C_B041C(s16 *arg0) {
    *arg0 = 0;
    setCallback(func_800BC750_B0440);
}

void func_800BC750_B0440(s16 *targetLap) {
    s32 i;
    GameState *gameState;
    Player *player;
    u32 playerOffset;
    s16 lapValue;

    gameState = getCurrentAllocation();

    for (i = 0; i < gameState->unk5F; i++) {
        playerOffset = (u32)(i * sizeof(Player));
        player = (Player *)((u32)playerOffset + (u32)gameState->players);
        lapValue = *targetLap;

        if (player->unkBC5 == lapValue) {
            if ((u32)(player->unkB94 - 7) < 4) {
                *targetLap = lapValue + 1;
                func_80058530_59130(0xC, 6);
            }
        }
    }
}
