#include "36BE0.h"
#include "5E590.h"
#include "6E840.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "rand.h"
#include "task_scheduler.h"

extern u8 D_80090E70_91A70[];
extern s16 D_80090EB0_91AB0[];

typedef struct {
    void *unk0;
    loadAssetMetadata_arg unk4;
    u8 padding2[0x10];
    s32 unk30;
    s32 unk34;
    s32 unk44;
} func_800504A0_510A0_arg;

typedef struct {
    s32 unk0;
    loadAssetMetadata_arg unk4;
    s32 padding2;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 padding3;
    s32 unk34;
} func_80050504_51104_arg;

typedef struct {
    loadAssetMetadata_arg lam;
    u8 padding[0x4];
} AssetWrapper;

typedef struct {
    MemoryAllocatorNode *unk0;
    AssetWrapper assets[2];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s16 unk50;
    s16 unk52;
    s16 unk54;
} func_80050740_51340_arg;

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
    u16 unk14;
    s16 unk16;
} func_80050F18_51B18_arg;

typedef struct {
    u8 padding[0x164];
    s16 unk164[3];
} func_800516F4_522F4_arg_unk0;

typedef struct {
    func_800516F4_522F4_arg_unk0 *unk0;
    void *unk4;
    loadAssetMetadata_arg unk8;
} func_800516F4_522F4_arg;

typedef struct {
    s32 unk0;
    void *unk4;
} func_80051688_52288_arg;

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

typedef struct {
    void *unk0;
} func_80050864_51464_arg;

typedef struct {
    u8 padding[0xC4];
    void *unkC4;
    u8 padding2[0xC];
    u8 unkD4;
    u8 padding3;
    u8 unkD6;
} func_80051BB8_527B8_task;

typedef struct {
    u8 padding[0x12C0];
    void *unk12C0;
} GameStateUnk44_Ext;

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 *unk14;
    /* 0x18 */ void *unk18;
    /* 0x1C */ s8 unk1C;
    /* 0x1D */ s8 unk1D;
    /* 0x1E */ u8 unk1E;
    /* 0x1F */ u8 padding_1F;
    /* 0x20 */ u8 padding_20[0x4];
    /* 0x24 */ s16 unk24;
    /* 0x26 */ u16 unk26;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ void *unk34;
    /* 0x38 */ s16 unk38;
} func_80050C00_51800_Task;

typedef struct {
    void *unk0;
    void *unk4;
    u8 padding[0x1C];
    s32 unk24;
    s32 unk28;
} func_80050DB0_519B0_arg;

typedef struct {
    loadAssetMetadata_arg unk0;
    u8 _pad2[0x4];
} func_800518AC_524AC_element;

typedef struct {
    func_800518AC_524AC_element elements[6];
    void *unkC0;
    u8 _padC4[0x4];
    u8 unkC8[0xC];
    s8 unkD4;
    u8 unkD5;
    u8 unkD6;
} func_800518AC_524AC_arg;

void func_800504A0_510A0(func_800504A0_510A0_arg *);
void func_800505D8_511D8(s32 **arg0);

void func_80050740_51340(func_80050740_51340_arg *);
void func_80050864_51464(func_80050864_51464_arg *);
void func_800509CC_515CC(void);
void func_80050DB0_519B0(func_80050DB0_519B0_arg *);
void func_80050E08_51A08(func_80050DB0_519B0_arg *);
void func_80050EA0_51AA0(void **);
void func_80050F64_51B64(func_80050F18_51B18_arg *);
void func_80050FE0_51BE0(func_80050F18_51B18_arg *);
void func_80051124_51D24(void);
void func_80051250_51E50(func_800506B4_512B4_arg *);
void func_80051760_52360(func_800516F4_522F4_arg *);
void func_80051800_52400(func_800516F4_522F4_arg *);
void func_800518AC_524AC(func_800518AC_524AC_arg *);
void func_80051B8C_5278C(func_8005186C_5246C_arg *);
void func_80050BD4_517D4(s32 **);
void func_80050504_51104(func_80050504_51104_arg *);

extern void func_80010240_10E40(void);
extern void func_80010924_11524(void);
extern void func_80066444_67044(s32, void *);
extern void func_80051978_52578(void);

extern loadAssetMetadata_arg D_80090EC0_91AC0;
extern loadAssetMetadata_arg D_80090F00_91B00;
extern void *D_80090F40_91B40;
extern void *D_80090F4C_91B4C;

void func_80050460_51060(void **node) {
    *node = load_3ECE40();
    setCleanupCallback(&func_800505D8_511D8);
    setCallbackWithContinue(&func_800504A0_510A0);
}

void func_800504A0_510A0(func_800504A0_510A0_arg *arg0) {
    GameState *gs = (GameState *)getCurrentAllocation();
    loadAssetMetadata(&arg0->unk4, arg0->unk0, arg0->unk30);
    arg0->unk4.unk1A = 0xE0;
    arg0->unk4.unk0 = &gs->unk44->unkFC0->asset;
    arg0->unk34 = 0;
    setCallbackWithContinue(&func_80050504_51104);
}

void func_80050504_51104(func_80050504_51104_arg *arg0) {
    GameState *gs;
    s32 i;
    gs = (GameState *)getCurrentAllocation();
    i = 0;

    if (gs->gamePaused == 0) {
        GameStateUnk44 *base;

        base = gs->unk44;
        arg0->unk4.unk0 = &base->unkFC0[arg0->unk34].asset;

        arg0->unk34 = arg0->unk34 + 1;
        if (arg0->unk34 == 4) {
            func_80069CF8_6A8F8();
            return;
        }
        arg0->unk4.unk1A = arg0->unk4.unk1A - 0x30;
        arg0->unk4.unk4 += arg0->unk24;
        arg0->unk4.unk8 += arg0->unk28;
        arg0->unk4.unkC += arg0->unk2C;
    }

    for (i = 0; i < 4; i++) {
        func_800677C0_683C0(i, &arg0->unk4);
    }
}

void func_800505D8_511D8(s32 **arg0) {
    *arg0 = freeNodeMemory(*arg0);
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
    arg0->unk4 = &temp_s1->unk44->unk1080[arg0->unk52];
    arg0->unk1E = (u8)((randA() & 0x1F) + 0x70);
    arg0->unk50 = 0;
    arg0->unk24 = arg0->unk4;
    arg0->unk3E = (u8)arg0->unk1E;
    setCleanupCallback(&func_80050864_51464);
    setCallbackWithContinue(&func_80050740_51340);
}

void func_80050740_51340(func_80050740_51340_arg *arg0) {
    GameState *gs;
    s32 i;

    gs = (GameState *)getCurrentAllocation();
    loadAssetMetadata(&arg0->assets[0].lam, arg0->unk0, arg0->unk54 + arg0->unk50);

    arg0->assets[1].lam.data_ptr = arg0->assets[0].lam.data_ptr;
    arg0->assets[1].lam.index_ptr = arg0->assets[0].lam.index_ptr;
    arg0->assets[1].lam.unk18 = arg0->assets[0].lam.unk18;
    arg0->assets[1].lam.unk19 = arg0->assets[0].lam.unk19;

    for (i = 0; i < 4; i++) {
        func_80067EDC_68ADC(i, &arg0->assets[0].lam);
        func_80067EDC_68ADC(i, &arg0->assets[1].lam);
    }

    if (gs->gamePaused == 0) {
        if (arg0->unk50 != 0) {
            for (i = 0; i < 2; i++) {
                arg0->assets[i].lam.unk4 += arg0->unk44;
                arg0->assets[i].lam.unk8 += arg0->unk48;
                arg0->assets[i].lam.unkC += arg0->unk4C;
            }
        }

        arg0->unk50++;
        if ((s16)arg0->unk50 == 5) {
            func_80069CF8_6A8F8();
        }
    }
}

void func_80050864_51464(func_80050864_51464_arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

void func_80050890_51490(void *arg0, void *arg1, Vec3i *arg2, s16 arg3, s32 arg4) {
    void *task;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    u32 temp_v1;
    u32 temp_v1_2;
    u32 temp_v1_3;
    u8 temp_v0_4;

    if (D_80090E70_91A70[arg4] == 0xFF) {
        return;
    }

    task = scheduleTask(&func_800506B4_512B4, 2, 0, 0xDD);
    if (task == NULL) {
        return;
    }

    memcpy((u8 *)task + 0x8, arg0, 0xC);
    memcpy((u8 *)task + 0x28, arg1, 0xC);

    temp_v0_4 = D_80090E70_91A70[arg4];
    *(s16 *)((u8 *)task + 0x52) = arg3;
    *(s16 *)((u8 *)task + 0x54) = temp_v0_4;

    temp_v0 = arg2->x;
    temp_v1 = (u32)temp_v0 >> 31;
    temp_v0 += temp_v1;
    temp_v0 >>= 1;
    *(s32 *)((u8 *)task + 0x44) = temp_v0;

    temp_v0_2 = arg2->y;
    temp_v1_2 = (u32)temp_v0_2 >> 31;
    temp_v0_2 += temp_v1_2;
    temp_v0_2 >>= 1;
    *(s32 *)((u8 *)task + 0x48) = temp_v0_2;

    temp_v0_3 = arg2->z;
    temp_v1_3 = (u32)temp_v0_3 >> 31;
    temp_v0_3 += temp_v1_3;
    temp_v0_3 >>= 1;
    *(s32 *)((u8 *)task + 0x4C) = temp_v0_3;
}

void func_8005098C_5158C(MemoryAllocatorNode **node) {
    *node = load_3ECE40();
    setCleanupCallback(&func_80050BD4_517D4);
    setCallbackWithContinue(&func_800509CC_515CC);
}

INCLUDE_ASM("asm/nonmatchings/51060", func_800509CC_515CC);

void func_80050AA8_516A8(func_80050C00_51800_Task *arg0) {
    GameState *gs;
    s32 i;
    s16 temp;

    gs = (GameState *)getCurrentAllocation();

    if (gs->gamePaused == 0) {
        temp = arg0->unk26;

        if (temp % 3 == 0) {
            loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->unk0, arg0->unk24 + (temp >> 2));
        }

        arg0->unk26++;
        if ((s16)arg0->unk26 >= 0xF) {
            func_80069CF8_6A8F8();
        }

        arg0->unk8 += arg0->unk28;
        arg0->unkC += arg0->unk2C;
        arg0->unk10 += arg0->unk30;
    }

    i = 0;
    if (arg0->unk1E == 0xFF) {
        do {
            func_80066444_67044(i, &arg0->unk4);
            i++;
        } while (i < 4);
    } else {
        do {
            func_800677C0_683C0(i, (loadAssetMetadata_arg *)&arg0->unk4);
            i++;
        } while (i < 4);
    }
}

void func_80050BD4_517D4(s32 **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void func_80050C00_51800(void *arg0) {
    GameState *allocation;
    func_80050C00_51800_Task *task;

    allocation = (GameState *)getCurrentAllocation();
    task = (func_80050C00_51800_Task *)scheduleTask(&func_8005098C_5158C, 2, 0, 0xEA);
    if (task != NULL) {
        task->unk24 = 0x35;
        task->unk34 = arg0;
        task->unk1E = 0xFF;
        task->unk28 = 0;
        task->unk2C = 0;
        task->unk30 = 0;
        task->unk38 = 0;
        task->unk4 = &allocation->unk44->unkFC0;
    }
}

INCLUDE_ASM("asm/nonmatchings/51060", func_80050C80_51880);

void func_80050D70_51970(MemoryAllocatorNode **node) {
    *node = load_3ECE40();
    setCleanupCallback(&func_80050EA0_51AA0);
    setCallbackWithContinue(&func_80050DB0_519B0);
}

void func_80050DB0_519B0(func_80050DB0_519B0_arg *arg0) {
    GameState *allocation;
    void *temp;

    allocation = getCurrentAllocation();
    temp = &((GameStateUnk44_Ext *)allocation->unk44)->unk12C0;

    arg0->unk24 = 0x40;
    arg0->unk4 = temp;
    arg0->unk28 = 0;

    loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->unk0, arg0->unk24);

    setCallbackWithContinue(&func_80050E08_51A08);
}

void func_80050E08_51A08(func_80050DB0_519B0_arg *arg0) {
    GameState *alloc;
    s32 i;

    alloc = getCurrentAllocation();
    if (alloc->gamePaused == 0) {
        if ((arg0->unk28 & 1) == 0) {
            loadAssetMetadata((loadAssetMetadata_arg *)&arg0->unk4, arg0->unk0, arg0->unk24 + (arg0->unk28 >> 1));
        }
        arg0->unk28++;
        if (arg0->unk28 >= 0xA) {
            func_80069CF8_6A8F8();
        }
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, &arg0->unk4);
    }
}

void func_80050EA0_51AA0(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void func_80050ECC_51ACC(void *arg0) {
    Node *task;

    task = scheduleTask(&func_80050D70_51970, 2, 0, 0xFA);
    if (task != NULL) {
        memcpy(&task->freeNext, arg0, 0xC);
    }
}

void func_80050F18_51B18(func_80050F18_51B18_arg *arg0) {
    arg0->unk4 = load_3ECE40();
    arg0->unk8 = 0x45;
    arg0->unk10 = 0;
    setCleanupCallback(&func_80050FE0_51BE0);
    setCallbackWithContinue(&func_80050F64_51B64);
}

void func_80050F64_51B64(func_80050F18_51B18_arg *arg0) {
    arg0->unk8 = (arg0->unk10 >> 1) + 0x45;
    arg0->unk10 = arg0->unk10 + 1;

    if (arg0->unk10 == 0x10) {
        func_80069CF8_6A8F8();
    }

    if (arg0->unk16 == 0) {
        debugEnqueueCallback(arg0->unk14, 1, func_80010240_10E40, arg0);
    } else {
        debugEnqueueCallback(arg0->unk14, 1, func_80010924_11524, arg0);
    }
}

void func_80050FE0_51BE0(func_80050F18_51B18_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

INCLUDE_ASM("asm/nonmatchings/51060", func_8005100C_51C0C);

void func_800510A4_51CA4(func_800506B4_512B4_arg *arg0) {
    GameState *temp_s1 = (GameState *)getCurrentAllocation();
    arg0->unk0 = load_3ECE40();
    arg0->unk4 = &temp_s1->unk44->unk1340;
    arg0->unk1E = (u8)((randA() & 0x1F) + 0x70);
    arg0->unk50 = 0;
    arg0->unk24 = (u8 *)arg0->unk4;
    arg0->unk3E = (u8)arg0->unk1E;
    setCleanupCallback(&func_80051250_51E50);
    setCallbackWithContinue(&func_80051124_51D24);
}

INCLUDE_ASM("asm/nonmatchings/51060", func_80051124_51D24);

void func_80051250_51E50(func_800506B4_512B4_arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
}

INCLUDE_ASM("asm/nonmatchings/51060", func_8005127C_51E7C);

INCLUDE_ASM("asm/nonmatchings/51060", func_80051348_51F48);

INCLUDE_ASM("asm/nonmatchings/51060", func_8005152C_5212C);

void func_80051688_52288(func_80051688_52288_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_80051348_51F48(void);

void func_800516B4_522B4(void *arg0) {
    void *result = scheduleTask(&func_80051348_51F48, 2, 0, 0xDD);
    if (result != NULL) {
        *(void **)result = arg0;
    }
}

void func_800516F4_522F4(func_800516F4_522F4_arg *arg0) {
    getCurrentAllocation();
    arg0->unk4 = load_3ECE40();
    arg0->unk8.unk0 = &D_80090EC0_91AC0;
    arg0->unk8.unk1A = 0xFF;
    loadAssetMetadata(&arg0->unk8, arg0->unk4, 0x6A);
    setCleanupCallback(&func_80051800_52400);
    setCallbackWithContinue(&func_80051760_52360);
}

void func_80051760_52360(func_800516F4_522F4_arg *arg0) {
    s32 i;
    GameState *gs;

    gs = (GameState *)getCurrentAllocation();
    transformVector((s16 *)&D_80090EB0_91AB0, (s16 *)&arg0->unk0->unk164, &arg0->unk8.unk4);

    for (i = 0; i < 4; i++) {
        func_80067EDC_68ADC(i, &arg0->unk8);
    }

    if (gs->gamePaused == 0) {
        arg0->unk8.unk1A -= 0x10;
        if ((u8)arg0->unk8.unk1A < 0x40) {
            func_80069CF8_6A8F8();
        }
    }
}

void func_80051800_52400(func_800516F4_522F4_arg *arg0) {
    arg0->unk4 = freeNodeMemory(arg0->unk4);
}

void func_8005182C_5242C(void *arg0) {
    void *result = scheduleTask(&func_800516F4_522F4, 2, 0, 0xDD);
    if (result != NULL) {
        *(void **)result = arg0;
    }
}

void func_8005186C_5246C(func_8005186C_5246C_arg *arg0) {
    arg0->unkC0 = load_3ECE40();
    setCleanupCallback(&func_80051B8C_5278C);
    setCallbackWithContinue(&func_800518AC_524AC);
}

void func_800518AC_524AC(func_800518AC_524AC_arg *arg0) {
    s32 i;

    for (i = 0; i < 6; i++) {
        loadAssetMetadata(&arg0->elements[i].unk0, arg0->unkC0, arg0->unkD4);
        arg0->elements[i].unk0.unk0 = &D_80090F00_91B00;

        if (arg0->unkD6 == 0) {
            memcpy(&arg0->unkC8, &D_80090F40_91B40, 0xC);
            arg0->elements[i].unk0.unk1A = 0x90;
        } else {
            memcpy(&arg0->unkC8, &D_80090F4C_91B4C, 0xC);
            arg0->elements[i].unk0.unk1A = 0xF0;
        }
    }

    arg0->unkD5 = 0;
    setCallbackWithContinue(&func_80051978_52578);
}

INCLUDE_ASM("asm/nonmatchings/51060", func_80051978_52578);

void func_80051B8C_5278C(func_8005186C_5246C_arg *arg0) {
    arg0->unkC0 = freeNodeMemory(arg0->unkC0);
}

void func_80051BB8_527B8(void *arg0) {
    func_80051BB8_527B8_task *task = (func_80051BB8_527B8_task *)scheduleTask(&func_8005186C_5246C, 2, 0, 0xE7);
    if (task != NULL) {
        task->unkC4 = arg0;
        task->unkD4 = 0x12;
        task->unkD6 = 0;
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
void func_80051C08_52808(void *arg0, s32 arg1) {
    s32 temp_s0;
    func_80051BB8_527B8_task *task;

    temp_s0 = D_80090E70_91A70[arg1];
    if (temp_s0 == 0xFF) {
        temp_s0 = 0xD;
    }

    task = (func_80051BB8_527B8_task *)scheduleTask(&func_8005186C_5246C, 2, 0, 0xE7);
    if (task != NULL) {
        task->unkC4 = arg0;
        task->unkD4 = temp_s0;
        task->unkD6 = 1;
    }
}
#pragma GCC diagnostic pop
