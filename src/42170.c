#include "3E160.h"
#include "56910.h"
#include "common.h"
#include "displaylist.h"
#include "gamestate.h"
#include "geometry.h"
#include "graphics.h"
#include "overlay.h"
#include "task_scheduler.h"

USE_ASSET(_3F3EF0);

extern void *D_8009A6B0_9B2B0;
extern void *D_8009A6C0_9B2C0;
extern void *D_8009A6D0_9B2D0;
extern void *D_8009A6E0_9B2E0;
extern void *D_8009A6F0_9B2F0;
extern void *D_8009A700_9B300;
extern void *D_8009A710_9B310;
extern void *D_8009A720_9B320;
extern void *D_8009A760_9B360;
extern void *D_8009A770_9B370;
extern Mat3x3Padded D_8009A8B0_9B4B0;
extern s32 gFrameCounter;
extern void func_80041810_42410(void *);
extern void func_80042160_42D60(void);

typedef struct {
    u8 _pad0[0x1F0];   /* 0x00 */
    s32 pos1F0;        /* 0x1F0 */
    s32 pos1F4;        /* 0x1F4 */
    s32 pos1F8;        /* 0x1F8 */
    u8 _pad1FC[0x98C]; /* 0x1FC */
    s32 unkB88;        /* 0xB88 */
} Func41570StateUnk3C;

typedef struct {
    s16 matrix[3][3];           /* 0x00 (0x12 bytes: 9 * 2), offset 0x0E is matrix[2][1] */
    u8 pad12[0x2];              /* 0x12 */
    s32 unk14;                  /* 0x14 */
    s32 unk18;                  /* 0x18 */
    s32 unk1C;                  /* 0x1C */
    void *unk20;                /* 0x20 */
    void *unk24;                /* 0x24 */
    void *unk28;                /* 0x28 */
    s32 unk2C;                  /* 0x2C */
    u8 pad30[0xC];              /* 0x30 */
    Func41570StateUnk3C *unk3C; /* 0x3C */
    s32 unk40;                  /* 0x40 */
    s32 unk44;                  /* 0x44 */
} Func41570State;

typedef struct {
    u8 pad0[0x20];
    void *unk20;
    void *unk24;
    void *unk28;
    s32 unk2C;
    u8 pad30[0xC];
    void *unk3C;
    s32 unk40;
} Func4179CArg;

typedef struct {
    u8 pad0[0x14];  /* 0x00 */
    s32 posX;       /* 0x14 */
    s32 posY;       /* 0x18 */
    s32 posZ;       /* 0x1C */
    u8 pad20[0x1C]; /* 0x20 */
    Player *unk3C;  /* 0x3C */
} Func41A60Arg;

void func_800419AC_425AC(Func4179CArg *);

typedef struct {
    u8 pad0[0x14]; /* 0x00 */
    u8 unk14[0xC]; /* 0x14 */
    void *unk20;   /* 0x20 */
    void *unk24;   /* 0x24 */
    void *unk28;   /* 0x28 */
    s32 unk2C;     /* 0x2C */
    u8 pad30[0xB]; /* 0x30 */
    u8 unk3B;      /* 0x3B */
    u8 pad3C[0x4]; /* 0x3C */
    s16 unk40;     /* 0x40 */
    u16 unk42;     /* 0x42 */
    s16 unk44;     /* 0x44 */
    s16 unk46;     /* 0x46 */
} Func420E8State;

void func_80042308_42F08(Func420E8State *);

typedef struct {
    u8 pad0[0x14]; /* 0x00 */
    s32 unk14;     /* 0x14 */
    s32 unk18;     /* 0x18 - corresponds to DisplayListObject.unk10.position.Y */
    s32 unk1C;     /* 0x1C */
    void *unk20;   /* 0x20 */
    void *unk24;   /* 0x24 */
    void *unk28;   /* 0x28 */
    s32 unk2C;     /* 0x2C */
    u8 pad30[0xC]; /* 0x30 */
    void *unk3C;   /* 0x3C */
    s32 unk40;     /* 0x40 */
    s16 unk44;     /* 0x44 */
    s16 unk46;     /* 0x46 */
} Func43DC0State;

void func_80044018_44C18(Func43DC0State *);
void func_80043E24_44A24(Func43DC0State *);
void func_80043F8C_44B8C(Func43DC0State *);

typedef struct {
    u8 pad0[0x14];    /* 0x00 */
    u8 unk14[0xC];    /* 0x14 */
    void *unk20;      /* 0x20 */
    void *unk24;      /* 0x24 */
    void *unk28;      /* 0x28 */
    s32 unk2C;        /* 0x2C */
    u8 pad30[0x0C];   /* 0x30 */
    Player *unk3C;    /* 0x3C */
    s16 matrix[3][3]; /* 0x40 - just the 9 s16 values (0x12 bytes) */
    u8 pad52[2];      /* 0x52 - padding to 0x54 */
    s32 unk54;        /* 0x54 */
    s32 unk58;        /* 0x58 */
    s32 unk5C;        /* 0x5C */
    s32 unk60;        /* 0x60 */
} Func41F38State;

typedef struct {
    u8 _pad0[0x3C]; /* 0x00 */
    s16 unk3C;      /* 0x3C */
} Func44538TaskMem;

typedef struct {
    u8 pad0[0x14]; /* 0x00 */
    s32 unk14;     /* 0x14 - position X */
    s32 unk18;     /* 0x18 - position Y */
    s32 unk1C;     /* 0x1C - position Z */
    void *unk20;   /* 0x20 */
    void *unk24;   /* 0x24 */
    void *unk28;   /* 0x28 */
    s32 unk2C;     /* 0x2C */
    u8 pad30[0xC]; /* 0x30 */
    Player *unk3C; /* 0x3C */
    s16 unk40;     /* 0x40 */
    s16 unk42;     /* 0x42 - scale factor */
    u16 unk44;     /* 0x44 - rotation angle */
    s16 unk46;     /* 0x46 */
} Func42410State;

void func_80041724_42324(Func41570State *);

typedef struct {
    u8 _pad0[0x44];
    void *unk44; /* 0x44 */
    u8 _pad48[0x2E];
    u8 unk76; /* 0x76 */
} Func43CA4GameState;

typedef struct {
    u8 _pad0[0xB44];
    u8 unkB44[0xC];   /* 0xB44 */
    u8 _padB50[0x24]; /* 0xB50 */
    s16 unkB74;       /* 0xB74 */
    u8 _padB76[0x58]; /* 0xB76 to 0xBCE */
    u8 unkBCE;        /* 0xBCE */
} Func44BBCPointerTarget;

typedef struct {
    u8 _pad0[0x14];                /* 0x00 */
    s32 unk14;                     /* 0x14 */
    s32 unk18;                     /* 0x18 */
    s32 unk1C;                     /* 0x1C */
    void *unk20;                   /* 0x20 */
    void *unk24;                   /* 0x24 */
    void *unk28;                   /* 0x28 */
    s32 unk2C;                     /* 0x2C */
    u8 _pad30[0xC];                /* 0x30 */
    DisplayListObject unk3C;       /* 0x3C */
    DisplayListObject unk78;       /* 0x78 */
    Func44BBCPointerTarget *unkB4; /* 0xB4 */
    u8 unkB8[0x4];                 /* 0xB8 */
    s32 unkBC;                     /* 0xBC */
    s32 unkC0;                     /* 0xC0 */
    s16 unkC4;                     /* 0xC4 - counter */
    s16 unkC6;                     /* 0xC6 - value copied to unkB74 */
    u16 unkC8;                     /* 0xC8 */
    u16 unkCA;                     /* 0xCA */
} Func44BBCArg;

void func_80044578_45178(Func44BBCArg *);
void func_80044684_45284(Func44BBCArg *);
void func_800447D4_453D4(Func44BBCArg *);
void func_80044888_45488(Func44BBCArg *);
void func_80044990_45590(Func44BBCArg *);
void func_80044C38_45838(Func44BBCArg *);

typedef struct {
    u8 _pad0[0x9F0];
    s16 unk9F0[3];
    u8 _pad9F6[0x1C5];
    u8 unkBBB;
    u8 _padBBC[0x13];
    u8 unkBCF;
    u8 _padBD0[0x9];
    u8 unkBD9;
} Func43CA4Unk28;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    u8 _pad10[0x18];
    Func43CA4Unk28 *unk28;
    s16 unk2C[3];
    u8 _pad32[0xE];
    s16 unk40;
    s16 unk42;
} Func43CA4Arg;

typedef struct {
    s32 unk0;              /* 0x00 */
    void *unk4;            /* 0x04 */
    s32 unk8;              /* 0x08 */
    s32 unkC;              /* 0x0C */
    u8 _pad10[0x14];       /* 0x10 */
    Func43CA4Unk28 *unk24; /* 0x24 */
    Func43CA4Unk28 *unk28; /* 0x28 */
    s32 unk2C;             /* 0x2C */
    s32 unk30;             /* 0x30 */
    s32 unk34;             /* 0x34 */
    s32 unk38;             /* 0x38 */
    s32 unk3C;             /* 0x3C */
    s16 unk40;             /* 0x40 */
    s16 unk42;             /* 0x42 */
    u8 _pad44[0x2];        /* 0x44 */
    s16 unk46;             /* 0x46 */
    s16 unk48;             /* 0x48 */
    u8 _pad4A[0x2];        /* 0x4A */
    s32 unk4C;             /* 0x4C */
} Func4393CArg;

void func_80043CA4_448A4(Func43CA4Arg *);
void func_800439F4_445F4(Func4393CArg *);

extern s32 D_80090964_91564;
extern s32 D_80090974_91574;
extern s32 D_8009093C_9153C;
extern s8 D_80090958_91558;
extern s8 D_8009095C_9155C;
extern s32 D_80090AA0_916A0;
extern s32 D_80090AAC_916AC;
extern s8 D_80090950_91550;
extern void func_80066444_67044(s32, void *);

void func_800415E8_421E8(Func41570State *);
void func_8004168C_4228C(Func41570State *);

void func_80041570_42170(Func41570State *arg0) {
    getCurrentAllocation();
    createXRotationMatrix(arg0->matrix, 0x800);
    arg0->unk20 = &D_8009A6B0_9B2B0;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk40 = 0x2000;
    arg0->unk2C = 0;
    arg0->unk44 = 1;
    setCleanupCallback(func_80041724_42324);
    setCallbackWithContinue(func_800415E8_421E8);
}

void func_800415E8_421E8(Func41570State *arg0) {
    s32 i;

    arg0->unk14 = arg0->unk3C->pos1F0;
    arg0->unk18 = arg0->unk3C->pos1F4;
    arg0->unk1C = arg0->unk3C->pos1F8;

    if ((arg0->unk3C->unkB88 & 0x20) == 0) {
        setCallback(func_8004168C_4228C);
    }

    if (arg0->unk44 != 0) {
        arg0->unk44 = 0;
        func_80056B7C_5777C(&arg0->unk14, 0xE);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_8004168C_4228C(Func41570State *arg0) {
    Func43CA4GameState *allocation;
    s32 i;

    allocation = (Func43CA4GameState *)getCurrentAllocation();
    if (allocation->unk76 == 0) {
        arg0->unk40 -= 0x200;
    }

    if (arg0->unk40 == 0x200) {
        func_80069CF8_6A8F8();
    }

    arg0->matrix[2][1] = -arg0->unk40;
    arg0->unk18 += 0xFFFF0000;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_80041724_42324(Func41570State *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

typedef struct {
    u8 _pad0[0x3C];
    void *unk3C;
} Func4175CTask;

void func_8004175C_4235C(void *arg0) {
    Func4175CTask *task;

    task = (Func4175CTask *)scheduleTask(&func_80041570_42170, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk3C = arg0;
    }
}

void func_8004179C_4239C(Func4179CArg *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A6C0_9B2C0;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->unk40 = 0x400;
    func_80056B7C_5777C((u8 *)arg0->unk3C + 0x434, 0x13);
    setCleanupCallback(func_800419AC_425AC);
    setCallbackWithContinue(func_80041810_42410);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80041810_42410);

void func_800419AC_425AC(Func4179CArg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800419E4_425E4(void *arg0) {
    Func4179CArg *task;

    task = scheduleTask(&func_8004179C_4239C, 0, 0, 0x63);
    if (task != NULL) {
        task->unk3C = arg0;
    }
}

void func_80041A24_42624(Func41A60Arg *arg0) {
    arg0->posX = arg0->unk3C->worldPosX;
    arg0->posY = arg0->unk3C->worldPosY;
    arg0->posZ = arg0->unk3C->worldPosZ;
    func_800634E8_640E8((DisplayListObject *)arg0);
}

void func_80041A60_42660(Func41A60Arg *arg0) {
    arg0->posX = arg0->unk3C->worldPosX;
    arg0->posY = arg0->unk3C->worldPosY;
    arg0->posZ = arg0->unk3C->worldPosZ;
    func_80063534_64134((s32)arg0);
}

void func_80041A9C_4269C(Func41A60Arg *arg0) {
    arg0->posX = arg0->unk3C->worldPosX;
    arg0->posY = arg0->unk3C->worldPosY;
    arg0->posZ = arg0->unk3C->worldPosZ;
    func_80063580_64180((s32)arg0);
}

typedef struct {
    u8 pad0[0xD8];
    void *unkD8;
} Func41AD8Arg;

extern void *load_3ECE40(void);
void func_80041B18_42718(void);
void func_80041DE4_429E4(Func41AD8Arg *);

void func_80041AD8_426D8(Func41AD8Arg *arg0) {
    arg0->unkD8 = load_3ECE40();
    setCleanupCallback(func_80041DE4_429E4);
    setCallbackWithContinue(func_80041B18_42718);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80041B18_42718);

INCLUDE_ASM("asm/nonmatchings/42170", func_80041C28_42828);

void func_80041DE4_429E4(Func41AD8Arg *arg0) {
    arg0->unkD8 = freeNodeMemory(arg0->unkD8);
}

void func_80041E10_42A10(s32 *arg0) {
    u8 *task;
    s32 i;
    u8 *ptr;

    task = (u8 *)scheduleTask(func_80041AD8_426D8, 2, 0, 0xE7);
    if (task != NULL) {
        i = 0;
        ptr = task;
        do {
            memcpy(ptr + 4, arg0, 0xC);
            i++;
            ptr += 0x24;
        } while (i < 6);
        task[0xDC] = 0x2B;
        func_80056B7C_5777C(arg0, 0x16);
    }
}

void func_80041EA4_42AA4(s32 *arg0) {
    u8 *task;
    s32 i;
    u8 *ptr;

    task = (u8 *)scheduleTask(func_80041AD8_426D8, 2, 0, 0xE7);
    if (task != NULL) {
        i = 0;
        ptr = task;
        do {
            memcpy(ptr + 4, arg0, 0xC);
            i++;
            ptr += 0x24;
        } while (i < 6);
        task[0xDC] = 0x2F;
        func_80056B7C_5777C(arg0, 0xE);
    }
}

void func_80041FB4_42BB4(Func41F38State *);
void func_80042070_42C70(Func41F38State *);

void func_80041F38_42B38(Func41F38State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A6D0_9B2D0;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk2C = 0;
    createYRotationMatrix((Mat3x3Padded *)arg0->matrix, 0xF800);
    arg0->unk54 = 0;
    arg0->unk58 = 0;
    arg0->unk5C = 0;
    arg0->unk60 = 1;
    setCleanupCallback(func_80042070_42C70);
    setCallbackWithContinue(func_80041FB4_42BB4);
}

void func_80041FB4_42BB4(Func41F38State *arg0) {
    s32 pos[3];
    s32 i;

    func_8006B084_6BC84(arg0->matrix, &arg0->unk3C->unk3F8, arg0);

    if ((arg0->unk3C->unkB88 & 0x80) == 0) {
        pos[0] = arg0->unk3C->worldPosX;
        pos[1] = arg0->unk3C->worldPosY + 0x100000;
        pos[2] = arg0->unk3C->worldPosZ;
        func_80041EA4_42AA4(pos);
        func_80069CF8_6A8F8();
    }

    if (arg0->unk60 != 0) {
        arg0->unk60 = 0;
        func_80056B7C_5777C(&arg0->unk14, 0x12);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_80042070_42C70(Func41F38State *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_800420A8_42CA8(void *arg0) {
    Func41F38State *task;

    task = scheduleTask(&func_80041F38_42B38, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk3C = arg0;
    }
}

void func_800420E8_42CE8(Func420E8State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A6E0_9B2E0;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->unk40 = 0;
    arg0->unk42 = 0;
    arg0->unk44 = 0xFF;
    func_80056B7C_5777C(&arg0->unk14, 0x14);
    setCleanupCallback(func_80042308_42F08);
    setCallbackWithContinue(func_80042160_42D60);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80042160_42D60);

void func_80042254_42E54(Func420E8State *arg0) {
    Func43CA4GameState *gameState;
    s32 i;
    s32 pad[3];

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        arg0->unk44 -= 0x18;
    }

    if (arg0->unk44 < 0x18) {
        func_80069CF8_6A8F8();
        return;
    }

    arg0->unk42 += 0x300;
    createYRotationMatrix((Mat3x3Padded *)arg0, arg0->unk42);
    scaleMatrix((Mat3x3Padded *)arg0, arg0->unk40, arg0->unk40, arg0->unk40);
    arg0->unk3B = (u8)arg0->unk44;

    for (i = 0; i < 4; i++) {
        func_800639F8_645F8(i, (DisplayListObject *)arg0);
    }
}

void func_80042308_42F08(Func420E8State *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

Func420E8State *func_80042340_42F40(void *arg0) {
    Func420E8State *task;

    task = (Func420E8State *)scheduleTask(func_800420E8_42CE8, 0, 0, 0xC8);
    if (task != NULL) {
        memcpy(task->unk14, arg0, 0xC);
        task->unk46 = -1;
    }
    return task;
}

Func420E8State *func_800423A4_42FA4(void *arg0, s32 arg1) {
    Func420E8State *task;

    task = (Func420E8State *)scheduleTask(&func_800420E8_42CE8, 0, 0, 0xC8);
    if (task != NULL) {
        memcpy(task->unk14, arg0, 0xC);
        task->unk46 = arg1;
    }
    return task;
}

void func_80042638_43238(Func42410State *);
void func_8004247C_4307C(Func42410State *);
void func_8004256C_4316C(Func42410State *);

void func_80042410_43010(Func42410State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A6F0_9B2F0;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    *(s32 *)&arg0->unk40 = 0x200;
    arg0->unk2C = 0;
    arg0->unk46 = 1;
    setCleanupCallback(func_80042638_43238);
    setCallbackWithContinue(func_8004247C_4307C);
}

void func_8004247C_4307C(Func42410State *arg0) {
    Func43CA4GameState *gameState;
    Player *player;
    s32 *pos;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        if (*(s32 *)&arg0->unk40 != 0x2000) {
            *(s32 *)&arg0->unk40 += 0x200;
        }
    }

    arg0->unk44 += 0x300;
    createYRotationMatrix((Mat3x3Padded *)arg0, arg0->unk44);
    scaleMatrix((Mat3x3Padded *)arg0, arg0->unk42, arg0->unk42, arg0->unk42);
    player = arg0->unk3C;
    pos = &arg0->unk14;
    memcpy(pos, &player->worldPosX, 0xC);
    arg0->unk18 += 0xFFEC0000;

    if (arg0->unk46 != 0) {
        func_80056B7C_5777C(pos, 0x15);
        arg0->unk46 = 0;
    }

    if ((arg0->unk3C->unkB88 & 0x400) == 0) {
        setCallback(func_8004256C_4316C);
    }

    i = 0;
    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        i++;
    } while (i < 4);
}

void func_8004256C_4316C(Func42410State *arg0) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        *(s32 *)&arg0->unk40 -= 0x200;
    }

    if (*(s32 *)&arg0->unk40 == 0x200) {
        func_80069CF8_6A8F8();
    }

    arg0->unk44 += 0x12C;
    createYRotationMatrix((Mat3x3Padded *)arg0, arg0->unk44);
    scaleMatrix((Mat3x3Padded *)arg0, arg0->unk42, arg0->unk42, arg0->unk42);
    memcpy(&arg0->unk14, &arg0->unk3C->worldPosX, 0xC);
    arg0->unk18 += 0xFFEC0000;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_80042638_43238(Func42410State *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_80042670_43270(Player *arg0) {
    Func42410State *task;

    task = (Func42410State *)scheduleTask(func_80042410_43010, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk3C = arg0;
    }
}

typedef struct {
    u8 pad0[0x434]; /* 0x00 */
    s32 pos434[3];  /* 0x434 - position data (12 bytes) */
} Func426B0Unk3C;

typedef struct {
    s16 matrix[3][3];      /* 0x00 (0x12 bytes: 9 * s16) */
    u8 pad12[0x2];         /* 0x12 */
    s32 unk14;             /* 0x14 */
    s32 unk18;             /* 0x18 */
    s32 unk1C;             /* 0x1C */
    void *unk20;           /* 0x20 */
    void *unk24;           /* 0x24 */
    void *unk28;           /* 0x28 */
    s32 unk2C;             /* 0x2C */
    u8 pad30[0xC];         /* 0x30 */
    Func426B0Unk3C *unk3C; /* 0x3C */
    Player *unk40;         /* 0x40 */
    s32 unk44;             /* 0x44 */
    s32 unk48;             /* 0x48 */
    s32 unk4C;             /* 0x4C */
    s32 unk50;             /* 0x50 */
} Func426B0State;

typedef struct {
    u8 pad0[0x24];
    void *unk24;
    void *unk28;
} Func429C4Arg;

void func_800429C4_435C4(Func429C4Arg *);
void func_8004273C_4333C(Func426B0State *);

void func_800426B0_432B0(Func426B0State *arg0) {
    if (arg0->unk44 == 0) {
        getCurrentAllocation();
        arg0->unk20 = &D_8009A700_9B300;
        arg0->unk24 = loadAsset_B7E70();
        arg0->unk28 = loadAsset_216290();
        arg0->unk48 = 0x400;
        arg0->unk2C = 0;
        arg0->unk4C = 0x100000;
        arg0->unk50 = 0;
        setCleanupCallback(func_800429C4_435C4);
        setCallbackWithContinue(func_8004273C_4333C);
    } else {
        arg0->unk44 = arg0->unk44 - 1;
    }
}

void func_8004290C_4350C(Func426B0State *);
void func_80042820_43420(Func426B0State *);

void func_8004273C_4333C(Func426B0State *arg0) {
    GameState *gameState;
    s16 scale;
    s32 i;

    gameState = getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        if (arg0->unk48 != 0x2000) {
            arg0->unk48 += 0x400;
        }

        arg0->unk50 += 0x20000;
        arg0->unk4C += arg0->unk50;

        if (arg0->unk4C > 0x500000) {
            setCallback(func_80042820_43420);
        }
    }

    createXRotationMatrix(arg0->matrix, 0);
    scale = (s16)arg0->unk48;
    scaleMatrix((Mat3x3Padded *)arg0, scale, scale, scale);
    memcpy(&arg0->unk14, arg0->unk3C->pos434, 0xC);
    arg0->unk18 += arg0->unk4C;

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_80042820_43420(Func426B0State *arg0) {
    GameState *gameState;
    s32 i;

    gameState = getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        arg0->unk40->unkBDE |= 4;
        arg0->unk50 += 0xFFFE0000;
        arg0->unk4C += arg0->unk50;

        if (arg0->unk4C == 0x220000) {
            func_80058B30_59730(arg0->unk40);
            func_80056B7C_5777C(&arg0->unk14, 0x19);
        }

        if (arg0->unk4C <= 0x10000) {
            arg0->unk50 = 0x50000;
            setCallback(func_8004290C_4350C);
        }
    }

    memcpy(&arg0->unk14, &arg0->unk40->worldPosX, 0xC);
    arg0->unk18 += arg0->unk4C;

    i = 0;
    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        i++;
    } while (i < 4);
}

void func_8004290C_4350C(Func426B0State *arg0) {
    GameState *gameState;
    s32 i;

    gameState = getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        arg0->unk50 = arg0->unk50 + (-0x8000);
        arg0->unk4C = arg0->unk4C + arg0->unk50;

        if (arg0->unk4C < (s32)0xFFF00000 && arg0->unk50 < 0) {
            func_80069CF8_6A8F8();
        }

        arg0->unk14 = arg0->unk14 + 0x20000;
        arg0->unk18 = arg0->unk18 + arg0->unk50;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_800429C4_435C4(Func429C4Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

Func426B0State *func_800429FC_435FC(void *arg0, Player *arg1, s16 arg2) {
    Func426B0State *task;

    task = (Func426B0State *)scheduleTask(func_800426B0_432B0, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk3C = arg0;
        task->unk40 = arg1;
        task->unk44 = arg2;
    }
    return task;
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80042A6C_4366C);

void func_80042F2C_43B2C(void **);
extern void func_80042A6C_4366C(void *);

typedef struct {
    void *unk0;            /* 0x00 */
    void *unk4;            /* 0x04 */
    s32 unk8;              /* 0x08 */
    u8 _padC[0x12];        /* 0x0C */
    u8 unk1E;              /* 0x1E */
    u8 _pad1F[0x1];        /* 0x1F */
    void *unk20;           /* 0x20 */
    void *unk24;           /* 0x24 */
    Func43CA4Unk28 *unk28; /* 0x28 */
    s32 unk2C;             /* 0x2C */
    s32 unk30;             /* 0x30 */
    s32 unk34;             /* 0x34 */
    s16 unk38;             /* 0x38 */
    s16 unk3A;             /* 0x3A */
    s16 unk3C;             /* 0x3C */
    s16 unk3E;             /* 0x3E */
    s16 unk40;             /* 0x40 */
    u8 unk42;              /* 0x42 */
    u8 unk43;              /* 0x43 */
} Func42BA4Arg;

void func_80042BA4_437A4(Func42BA4Arg *);
struct Func42C98Arg;
struct Func42E40Arg;
void func_80042C98_43898(struct Func42C98Arg *);
void func_80042E40_43A40(struct Func42E40Arg *);

void func_80042B64_43764(void **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(func_80042F2C_43B2C);
    setCallbackWithContinue(func_80042BA4_437A4);
}

void func_80042BA4_437A4(Func42BA4Arg *arg0) {
    Func43CA4GameState *gameState;
    s16 temp;
    void *ptr;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    temp = arg0->unk3A;

    if (temp == 0) {
        arg0->unk3A = 1;
        arg0->unk3E = 0;
        arg0->unk1E = 0;
        ptr = gameState->unk44;
        arg0->unk30 = 0x200000;
        arg0->unk2C = 0;
        arg0->unk34 = 0;
        arg0->unk38 = 0;
        arg0->unk42 = 1;
        arg0->unk4 = (void *)((u8 *)ptr + 0xF00);
        func_80042A6C_4366C(arg0);

        if (arg0->unk43 != 0) {
            arg0->unk28->unkBCF++;

            if (arg0->unk28->unkBBB == 0xC) {
                arg0->unk3C = 0x1E;
            } else {
                arg0->unk3C = 0x12C;
            }

            arg0->unk2C = 0x140000;
            arg0->unk30 = 0x190000;
            setCallbackWithContinue(func_80042E40_43A40);
        } else {
            setCallbackWithContinue(func_80042C98_43898);
        }
    } else {
        if (gameState->unk76 == 0) {
            arg0->unk3A = temp - 1;
        }
    }
}

struct Func42C98Arg {
    u8 _pad0[0x4];              /* 0x00 - 0x04 */
    loadAssetMetadata_arg unk4; /* 0x04 - 0x20 (unk4.unk4 is at 0x08) */
    u8 _pad20[0x4];             /* 0x20 - 0x24 */
    Func43CA4Unk28 *unk24;      /* 0x24 - 0x28 */
    u8 _pad28[0x4];             /* 0x28 - 0x2C */
    s16 unk2C[2];               /* 0x2C - 0x30 (4 bytes) */
    s32 unk30;                  /* 0x30 - 0x34 */
    u8 _pad34[0xE];             /* 0x34 - 0x42 */
    u8 unk42;                   /* 0x42 */
};

typedef struct Func42D54Arg Func42D54Arg;
void func_80042D54_43954(Func42D54Arg *);

void func_80042C98_43898(struct Func42C98Arg *arg0) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        arg0->unk30 += 0x10000;
        if (arg0->unk30 > 0x2FFFFF) {
            setCallback(func_80042D54_43954);
        }

        transformVector(arg0->unk2C, arg0->unk24->unk9F0, &arg0->unk4.unk4);

        if (arg0->unk42 != 0) {
            arg0->unk42 = 0;
            func_80056B7C_5777C(&arg0->unk4.unk4, 0x1A);
        }

        func_80042A6C_4366C(arg0);
    }

    for (i = 0; i < 4; i++) {
        func_800677C0_683C0(i, &arg0->unk4);
    }
}

struct Func42D54Arg {
    u8 _pad0[0x4];              /* 0x00 */
    loadAssetMetadata_arg unk4; /* 0x04 - 0x1F */
    void *unk20;                /* 0x20 */
    Func43CA4Unk28 *unk24;      /* 0x24 */
    Func43CA4Unk28 *unk28;      /* 0x28 */
    s32 unk2C;                  /* 0x2C */
    s32 unk30;                  /* 0x30 */
    s32 unk34;                  /* 0x34 */
    s32 unk38;                  /* 0x38 */
    s16 unk3C;                  /* 0x3C */
    u8 _pad3E[0x4];             /* 0x3E */
    u8 unk42;                   /* 0x42 */
};

void func_80042D54_43954(Func42D54Arg *arg0) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        func_80042A6C_4366C(arg0);
        transformVector((s16 *)&arg0->unk2C, arg0->unk24->unk9F0, &arg0->unk4.unk4);

        if (arg0->unk4.unk1A == 0x40) {
            if (arg0->unk28->unkBD9 != 0) {
                arg0->unk2C = 0x300000;
                arg0->unk30 = 0x300000;
            } else {
                arg0->unk2C = 0x140000;
                arg0->unk30 = 0x190000;
            }

            if (arg0->unk28->unkBBB == 0xC) {
                arg0->unk3C = 0x1E;
            } else {
                arg0->unk3C = 0x12C;
            }

            arg0->unk28->unkBCF++;
            arg0->unk42 = 1;
            setCallback(func_80042E40_43A40);
        }
    }

    for (i = 0; i < 4; i++) {
        func_800677C0_683C0(i, &arg0->unk4);
    }
}

struct Func42E40Arg {
    void *unk0;            /* 0x00 */
    s32 unk4;              /* 0x04 */
    s32 unk8;              /* 0x08 */
    s32 unkC;              /* 0x0C */
    u8 *unk10;             /* 0x10 */
    void *unk14;           /* 0x14 */
    s8 unk18;              /* 0x18 */
    s8 unk19;              /* 0x19 */
    u8 unk1A;              /* 0x1A */
    u8 _pad1B[0x1];        /* 0x1B */
    u8 _pad1C[0x2];        /* 0x1C */
    u8 unk1E;              /* 0x1E */
    u8 _pad1F[0x1];        /* 0x1F */
    void *unk20;           /* 0x20 */
    void *unk24;           /* 0x24 */
    Func43CA4Unk28 *unk28; /* 0x28 */
    s16 unk2C[3];          /* 0x2C - 0x32 */
    u8 _pad32[0xA];        /* 0x32 - 0x3C */
    s16 unk3C;             /* 0x3C */
    u8 _pad3E[0x2];        /* 0x3E */
    u16 unk40;             /* 0x40 */
    u8 unk42;              /* 0x42 */
};

void func_80042E40_43A40(struct Func42E40Arg *arg0) {
    Func43CA4GameState *gameState;
    s32 i;
    s32 pad;
    s32 rotated[3];

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        func_80042A6C_4366C(arg0);
        arg0->unk40 += 0x100;
        rotateVectorY(arg0->unk2C, (s16)arg0->unk40, rotated);
        transformVector((s16 *)rotated, arg0->unk28->unk9F0, &arg0->unk8);
        if (arg0->unk42 != 0) {
            arg0->unk42 = 0;
            func_80056B7C_5777C(&arg0->unk8, 0x1A);
        }
        if (arg0->unk3C != 0) {
            arg0->unk3C--;
        } else if (arg0->unk1E == 0x40) {
            arg0->unk28->unkBCF--;
            func_80069CF8_6A8F8();
        }
    }

    for (i = 0; i < 4; i++) {
        func_800677C0_683C0(i, (loadAssetMetadata_arg *)&arg0->unk4);
    }
}

void func_80042F2C_43B2C(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

typedef struct {
    u8 _pad0[0x24]; /* 0x00 */
    void *unk24;    /* 0x24 */
    void *unk28;    /* 0x28 */
    u8 _pad2C[0xE]; /* 0x2C */
    s16 unk3A;      /* 0x3A */
    u8 _pad3C[0x7]; /* 0x3C */
    u8 unk43;       /* 0x43 */
} Func42FC0TaskMem;

void func_80042F58_43B58(void *arg0, void *arg1, s16 arg2) {
    Func42FC0TaskMem *task;

    task = (Func42FC0TaskMem *)scheduleTask(func_80042B64_43764, 0, 0, 0xDC);
    if (task != NULL) {
        task->unk24 = arg0;
        task->unk28 = arg1;
        task->unk3A = arg2;
        task->unk43 = 0;
    }
}

Func42FC0TaskMem *func_80042FC0_43BC0(void *arg0) {
    Func42FC0TaskMem *task;

    task = (Func42FC0TaskMem *)scheduleTask(func_80042B64_43764, 0, 0, 0xDC);
    if (task != NULL) {
        task->unk24 = arg0;
        task->unk28 = arg0;
        task->unk3A = 0;
        task->unk43 = 1;
    }
    return task;
}

typedef struct {
    u8 pad0[0x24];
    void *unk24;
    void *unk28;
} Func432D8Arg;

void func_800432D8_43ED8(Func432D8Arg *);

typedef struct {
    u8 pad0[0x18];  /* 0x00 */
    s32 unk18;      /* 0x18 */
    u8 pad1C[0x4];  /* 0x1C */
    void *unk20;    /* 0x20 */
    void *unk24;    /* 0x24 */
    void *unk28;    /* 0x28 */
    s32 unk2C;      /* 0x2C */
    u8 pad30[0x2C]; /* 0x30 */
    void *unk5C;    /* 0x5C */
    void *unk60;    /* 0x60 */
    void *unk64;    /* 0x64 */
    s32 unk68;      /* 0x68 */
    u8 pad6C[0xC];  /* 0x6C */
    Player *unk78;  /* 0x78 */
    s32 unk7C;      /* 0x7C */
    u16 unk80;      /* 0x80 */
    u16 unk82;      /* 0x82 */
    s16 unk84;      /* 0x84 */
} Func43018State;

void func_8004309C_43C9C(Func43018State *arg0);
void func_8004320C_43E0C(Func43018State *arg0);

void func_80043018_43C18(Func43018State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A710_9B310;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk5C = &D_8009A720_9B320;
    arg0->unk2C = 0;
    arg0->unk68 = 0;
    arg0->unk84 = 0x200;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    setCleanupCallback(func_800432D8_43ED8);
    setCallbackWithContinue(func_8004309C_43C9C);
}

void func_8004309C_43C9C(Func43018State *arg0) {
    Func43CA4GameState *gameState;
    s32 i;
    Mat3x3Padded matrix;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    createYRotationMatrix(&D_8009A8B0_9B4B0, arg0->unk80);
    func_8006B084_6BC84(&D_8009A8B0_9B4B0, (u8 *)arg0->unk78 + 0x3F8, arg0);
    scaleMatrix((Mat3x3Padded *)arg0, arg0->unk84, arg0->unk84, arg0->unk84);

    arg0->unk82 += 0x300;
    createZRotationMatrix(&matrix, arg0->unk82);

    matrix.unk14 = 0;
    matrix.unk18 = 0xBB333;
    matrix.unk1C = 0xFFEA0000;

    func_8006B084_6BC84(&matrix, arg0, (u8 *)arg0 + 0x3C);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)((u8 *)arg0 + 0x3C));
    }

    if (arg0->unk78->unkB84 & 0x80000) {
        arg0->unk78->unkB9A = 0;
    }

    if (arg0->unk78->unkB9A != 0) {
        if (gameState->unk76 == 0) {
            arg0->unk78->unkB9A--;
            if (arg0->unk78->unkB9A == 0) {
                if (arg0->unk78->unkBBB == 0x10) {
                    arg0->unk78->unkB9A++;
                }
            }
            if (arg0->unk84 != 0x2000) {
                arg0->unk84 += 0x200;
            }
        }
    } else {
        arg0->unk78->unkBD0 = 0;
        arg0->unk7C = 0x40000;
        setCallback(func_8004320C_43E0C);
    }
}

void func_8004320C_43E0C(Func43018State *arg0) {
    Func43CA4GameState *gameState;
    s32 i;
    Mat3x3Padded matrix;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        arg0->unk7C -= 0x8000;
        if (arg0->unk7C <= (s32)0xFFF80000) {
            func_80069CF8_6A8F8();
        }
        arg0->unk18 += arg0->unk7C;
        createZRotationMatrix(&matrix, arg0->unk82);
        matrix.unk14 = 0;
        matrix.unk18 = 0xBB333;
        matrix.unk1C = 0xFFEA0000;
        func_8006B084_6BC84(&matrix, arg0, (u8 *)arg0 + 0x3C);
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)((u8 *)arg0 + 0x3C));
    }
}

void func_800432D8_43ED8(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void *func_80043310_43F10(Player *arg0) {
    Func43018State *task;

    task = (Func43018State *)scheduleTask(func_80043018_43C18, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk78 = arg0;
        task->unk80 = 0;
        if (arg0->unkB84 & 2) {
            task->unk80 = 0x1000;
        }
    }
    return task;
}

extern void *D_8009A730_9B330;
extern void func_800433EC_43FEC(void);
void func_8004367C_4427C(Func432D8Arg *);

typedef struct {
    u8 pad0[0x18];  /* 0x00 */
    s32 unk18;      /* 0x18 */
    u8 pad1C[0x4];  /* 0x1C */
    void *unk20;    /* 0x20 */
    void *unk24;    /* 0x24 */
    void *unk28;    /* 0x28 */
    s32 unk2C;      /* 0x2C */
    u8 pad30[0x30]; /* 0x30 */
    void *unk60;    /* 0x60 */
    void *unk64;    /* 0x64 */
    s32 unk68;      /* 0x68 */
    u8 pad6C[0xC];  /* 0x6C */
    Player *unk78;  /* 0x78 */
    s32 unk7C;      /* 0x7C */
    s16 unk80;      /* 0x80 */
    s16 unk82;      /* 0x82 */
} Func43374State;

void func_80043374_43F74(Func43374State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A730_9B330;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->unk68 = 0;
    arg0->unk82 = 0x200;
    arg0->unk60 = arg0->unk24;
    arg0->unk64 = arg0->unk28;
    setCleanupCallback(func_8004367C_4427C);
    setCallbackWithContinue(func_800433EC_43FEC);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_800433EC_43FEC);

void func_800435F0_441F0(Func43374State *arg0) {
    GameState *gameState;
    s32 i;
    s32 pad[7];

    gameState = (GameState *)getCurrentAllocation();
    if (gameState->gamePaused == 0) {
        arg0->unk7C -= 0x8000;
        if (arg0->unk7C <= (s32)0xFFF80000) {
            func_80069CF8_6A8F8();
        }
        arg0->unk18 += arg0->unk7C;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_8004367C_4427C(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

Func43374State *func_800436B4_442B4(Player *arg0) {
    Func43374State *task;

    task = (Func43374State *)scheduleTask(func_80043374_43F74, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk78 = arg0;
        task->unk80 = 0;
        if (arg0->unkB84 & 2) {
            task->unk80 = 0x1000;
        }
    }
    return task;
}

s32 func_80043718_44318(void *arg0_void, void *arg1_void) {
    Func4393CArg *arg0 = (Func4393CArg *)arg0_void;
    u8 *arg1 = (u8 *)arg1_void;
    s8 check;

    arg0->unk42--;

    if ((s16)arg0->unk42 != 0) {
        return 0;
    }

    loadAssetMetadata((void *)((s32)arg0 + 4), (void *)arg0->unk0, (s8)arg1[(s16)arg0->unk40 * 2 + 1]);

    arg0->unk42 = (s8)arg1[(s16)arg0->unk40 * 2];
    arg0->unk40++;

    check = arg1[(s16)arg0->unk40 * 2];
    if (check != 0) {
        return 0;
    }

    arg0->unk40--;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/42170", func_800437C4_443C4);

void func_80043D30_44930(void **);
void func_8004393C_4453C(Func4393CArg *);
void func_800438A0_444A0(Func4393CArg *);
extern void func_800437C4_443C4(void *, void *);
void func_80043AB4_446B4(Func4393CArg *);

void func_80043860_44460(void **arg0) {
    *arg0 = load_3ECE40();
    setCleanupCallback(func_80043D30_44930);
    setCallbackWithContinue(func_800438A0_444A0);
}

void func_800438A0_444A0(Func4393CArg *arg0) {
    Func43CA4GameState *gameState;
    s16 temp;
    void *ptr;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    temp = arg0->unk42;

    if (temp == 0) {
        arg0->unk42 = 1;
        arg0->unk46 = 0;
        ptr = (void *)((u8 *)gameState->unk44 + 0xF40);
        arg0->unk30 = 0x300000;
        arg0->unk2C = 0;
        arg0->unk34 = 0;
        arg0->unk40 = 0;
        arg0->unk4C = 1;
        arg0->unk4 = ptr;
        func_80043718_44318(arg0, &D_8009093C_9153C);
        setCallbackWithContinue(func_8004393C_4453C);
    } else {
        if (gameState->unk76 == 0) {
            arg0->unk42 = temp - 1;
        }
    }
}

void func_8004393C_4453C(Func4393CArg *arg0) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        if (func_80043718_44318(arg0, &D_8009093C_9153C) != 0) {
            arg0->unk38 = 0;
            arg0->unk40 = 0;
            arg0->unk42 = 1;
            setCallback(func_800439F4_445F4);
        }
    }

    transformVector((s16 *)&arg0->unk2C, arg0->unk24->unk9F0, &arg0->unk8);

    if (arg0->unk4C != 0) {
        arg0->unk4C = 0;
        func_80056B7C_5777C(&arg0->unk8, 0x12);
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, &arg0->unk4);
    }
}

void func_800439F4_445F4(Func4393CArg *arg0) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        func_800437C4_443C4(arg0, &D_80090950_91550);
        arg0->unk38 = arg0->unk38 + 0x8000;
        arg0->unk30 = arg0->unk30 + arg0->unk38;
        if (arg0->unk30 > 0x600000) {
            arg0->unk48 = 0;
            arg0->unk40 = 0;
            arg0->unk42 = 1;
            setCallback(func_80043AB4_446B4);
        }
    }

    transformVector((s16 *)&arg0->unk2C, arg0->unk24->unk9F0, &arg0->unk8);

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, &arg0->unk4);
    }
}

extern s32 func_80059AC4_5A6C4(Func43CA4Unk28 *);
extern void func_80059A48_5A648(Func43CA4Unk28 *, s32);
void func_80043C00_44800(Func43CA4Arg *);

void func_80043AB4_446B4(Func4393CArg *arg0) {
    Func43CA4GameState *gameState;
    s32 sinVal;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 != 0) {
        goto transform_and_loop;
    }

    arg0->unk48 = arg0->unk48 + 0x80;
    sinVal = approximateSin(arg0->unk48);
    arg0->unk30 = 0x700000 - (sinVal * 3 << 8);

    if (arg0->unk48 < 0x800) {
        func_800437C4_443C4(arg0, &D_80090958_91558);
    } else if (arg0->unk48 == 0x800) {
        arg0->unk40 = 0;
        arg0->unk42 = 1;
        arg0->unk3C = func_80059AC4_5A6C4(arg0->unk28);
        func_80059A48_5A648(arg0->unk28, -arg0->unk3C);
        func_800437C4_443C4(arg0, &D_8009095C_9155C);
    } else {
        func_800437C4_443C4(arg0, &D_8009095C_9155C);
    }

    if (arg0->unk48 == 0xC00) {
        arg0->unk40 = 0;
        arg0->unk42 = 1;
        func_80059A48_5A648(arg0->unk24, arg0->unk3C);
        setCallback(func_80043C00_44800);
    }

transform_and_loop:
    transformVector((s16 *)&arg0->unk2C, arg0->unk28->unk9F0, &arg0->unk8);

    if (arg0->unk48 == 0x80) {
        func_80056B7C_5777C(&arg0->unk8, 0x1C);
    }

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, &arg0->unk4);
    }
}

void func_80043C00_44800(Func43CA4Arg *arg0) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        if (func_80043718_44318(arg0, &D_80090964_91564) != 0) {
            arg0->unk40 = 0;
            arg0->unk42 = 1;
            func_80056B7C_5777C(&arg0->unk8, 0x12);
            setCallback(func_80043CA4_448A4);
        }
    }

    transformVector(arg0->unk2C, arg0->unk28->unk9F0, &arg0->unk8);

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, &arg0->unk4);
    }
}

void func_80043CA4_448A4(Func43CA4Arg *arg0) {
    Func43CA4GameState *gameState;
    s32 i;

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 == 0) {
        if (func_80043718_44318(arg0, &D_80090974_91574) != 0) {
            func_80069CF8_6A8F8();
        }
    }

    transformVector(arg0->unk2C, arg0->unk28->unk9F0, &arg0->unk8);

    for (i = 0; i < 4; i++) {
        func_80066444_67044(i, &arg0->unk4);
    }
}

void func_80043D30_44930(void **arg0) {
    *arg0 = freeNodeMemory(*arg0);
}

void func_80043D5C_4495C(void *arg0, void *arg1, s16 arg2) {
    void *task;

    task = scheduleTask(func_80043860_44460, 0, 0, 0xDC);
    if (task != NULL) {
        *((void **)((u8 *)task + 0x24)) = arg0;
        *((void **)((u8 *)task + 0x28)) = arg1;
        *((s16 *)((u8 *)task + 0x42)) = arg2;
    }
}

void func_80043DC0_449C0(Func43DC0State *arg0) {
    getCurrentAllocation();
    arg0->unk20 = &D_8009A760_9B360;
    arg0->unk24 = loadAsset_B7E70();
    arg0->unk28 = loadAsset_216290();
    arg0->unk2C = 0;
    arg0->unk46 = 0x200;
    setCleanupCallback(func_80044018_44C18);
    setCallbackWithContinue(func_80043E24_44A24);
}

void func_80043E24_44A24(Func43DC0State *arg0) {
    Func43CA4GameState *allocation;
    Player *player;
    Player *temp_player;
    u16 count;
    u16 new_count;
    s32 i;

    allocation = (Func43CA4GameState *)getCurrentAllocation();
    createYRotationMatrix(&D_8009A8B0_9B4B0, arg0->unk44);
    func_8006B084_6BC84(&D_8009A8B0_9B4B0, (u8 *)arg0->unk3C + 0x3F8, arg0);

    if (arg0->unk46 == 0x200) {
        func_80056B7C_5777C(&arg0->unk14, 0x1D);
    }

    if (arg0->unk46 != 0x2000) {
        arg0->unk46 += 0x200;
    }

    scaleMatrix((Mat3x3Padded *)arg0, arg0->unk46, arg0->unk46, arg0->unk46);

    if (gFrameCounter & 4) {
        arg0->unk20 = &D_8009A760_9B360;
    } else {
        arg0->unk20 = &D_8009A770_9B370;
    }

    i = 0;
    do {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        i++;
    } while (i < 4);

    player = arg0->unk3C;
    if (player->unkB84 & 0x80000) {
        player->unkBA6 = 0;
    }

    temp_player = arg0->unk3C;
    count = temp_player->unkBA6;
    if (count != 0) {
        if (allocation->unk76 == 0) {
            temp_player->unkBA6 = count - 1;
            player = arg0->unk3C;
            new_count = player->unkBA6;
            if (new_count == 0) {
                if (player->unkBBB == 0x11) {
                    player->unkBA6 = new_count + 1;
                }
            }
        }
    } else {
        temp_player->unkBD1 = 0;
        arg0->unk40 = 0x40000;
        setCallback(func_80043F8C_44B8C);
    }
}

void func_80043F8C_44B8C(Func43DC0State *arg0) {
    GameState *state;
    s32 i;
    s32 pad[8];

    state = getCurrentAllocation();
    if (state->gamePaused == 0) {
        arg0->unk40 = arg0->unk40 - 0x8000;
        if ((s32)0xFFF80000 >= arg0->unk40) {
            func_80069CF8_6A8F8();
        }
        arg0->unk18 = arg0->unk18 + arg0->unk40;
    }

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
    }
}

void func_80044018_44C18(Func43DC0State *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void *func_80044050_44C50(Player *arg0) {
    Func43DC0State *task;

    task = scheduleTask(func_80043DC0_449C0, 0, 0, 0xC8);
    if (task != NULL) {
        task->unk3C = arg0;
        task->unk44 = 0;
        if (arg0->unkB84 & 2) {
            task->unk44 = 0x1000;
        }
    }
    return task;
}

extern s32 D_80090980_91580[];
extern s32 D_80090984_91584[];
extern s32 D_80090988_91588[];
extern s32 D_80090994_91594[];
extern s32 D_80090996_91596[];

typedef struct {
    u8 _pad0[0x5C];
    u8 unk5C;
} Func44CB4Allocation;

typedef struct {
    u8 pad0[0x14]; /* 0x00 */
    s32 unk14[3];  /* 0x14 */
    void *unk20;   /* 0x20 */
    void *unk24;   /* 0x24 */
    void *unk28;   /* 0x28 */
    s32 unk2C;     /* 0x2C */
    u8 pad30[0xC]; /* 0x30 */
    s16 unk3C;     /* 0x3C */
} Func44CB4State;

void func_80044500_45100(Func432D8Arg *);
void func_800441A4_44DA4(Func44CB4State *);

void func_800440B4_44CB4(Func44CB4State *arg0) {
    Func44CB4Allocation *allocation;
    s32 offset;

    allocation = getCurrentAllocation();
    offset = arg0->unk3C * 9;
    createCombinedRotationMatrix(arg0, *(u16 *)&D_80090994_91594[offset], *(u16 *)&D_80090996_91596[offset]);
    memcpy(&arg0->unk14, &D_80090988_91588[arg0->unk3C * 9], 0xC);
    arg0->unk20 = (void *)(D_80090980_91580[arg0->unk3C * 9] + (D_80090984_91584[arg0->unk3C * 9] << 4));
    arg0->unk24 = func_80055DC4_569C4(allocation->unk5C);
    arg0->unk28 = func_80055DF8_569F8(allocation->unk5C);
    arg0->unk2C = 0;
    setCleanupCallback(func_80044500_45100);
    setCallbackWithContinue(func_800441A4_44DA4);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_800441A4_44DA4);

void func_80044500_45100(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

void func_80044538_45138(s16 arg0) {
    Func44538TaskMem *task = (Func44538TaskMem *)scheduleTask(func_800440B4_44CB4, 0, 0, 0x32);
    if (task != NULL) {
        task->unk3C = arg0;
    }
}

void func_80044578_45178(Func44BBCArg *arg0) {
    Mat3x3Padded matrix;
    s32 sinVal;
    s32 i;

    arg0->unkCA = arg0->unkCA + 0x100;
    sinVal = approximateSin((s16)arg0->unkCA);
    createZRotationMatrix(&matrix, (sinVal >> 5) & 0xFFFF);

    matrix.unk14 = 0xFFF7490A;
    matrix.unk18 = 0xFFF98007;
    matrix.unk1C = 0xCB326;
    func_8006B084_6BC84(&matrix, arg0, &arg0->unk3C);

    sinVal = approximateSin(arg0->unkCA);
    createZRotationMatrix(&matrix, (-(sinVal >> 5)) & 0xFFFF);

    matrix.unk14 = 0x8B6F6;
    matrix.unk18 = 0xFFF98007;
    matrix.unk1C = 0xCB326;
    func_8006B084_6BC84(&matrix, arg0, &arg0->unk78);

    for (i = 0; i < 4; i++) {
        enqueueDisplayListWithFrustumCull(i, (DisplayListObject *)arg0);
        enqueueDisplayListWithFrustumCull(i, &arg0->unk3C);
        enqueueDisplayListWithFrustumCull(i, &arg0->unk78);
    }
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80044684_45284);

void func_800447D4_453D4(Func44BBCArg *arg0) {
    Func43CA4GameState *gameState;
    s32 output[3];

    gameState = getCurrentAllocation();
    if (gameState->unk76 == 0) {
        transformVector2(&D_80090AAC_916AC, arg0, output);
        arg0->unk14 = arg0->unk14 + output[0];
        arg0->unk1C = arg0->unk1C + output[2];
        memcpy(&arg0->unkB4->unkB44, &arg0->unk14, 0xC);
        arg0->unkB4->unkB74 = arg0->unkC6;
        if (arg0->unkC4 != 0) {
            arg0->unkC4 = arg0->unkC4 - 1;
        } else {
            arg0->unkC4 = 0xB4;
            setCallback(func_80044888_45488);
        }
    }

    func_80044578_45178(arg0);
}

void func_80044888_45488(Func44BBCArg *arg0) {
    Func43CA4GameState *gameState;
    s32 output[3];

    gameState = (Func43CA4GameState *)getCurrentAllocation();
    if (gameState->unk76 != 0) {
        goto end;
    }

    {
        s16 temp = arg0->unkC6;
        if (temp != -0x400) {
            arg0->unkC6 = temp - 0x10;
        }
    }

    createCombinedRotationMatrix(arg0, arg0->unkC6, arg0->unkC8);
    transformVector2(&D_80090AAC_916AC, arg0, output);

    arg0->unk14 = arg0->unk14 + output[0];
    arg0->unk18 = arg0->unk18 + output[1];
    arg0->unk1C = arg0->unk1C + output[2];

    memcpy(&arg0->unkB4->unkB44, &arg0->unk14, 0xC);
    arg0->unkB4->unkB74 = arg0->unkC6;

    {
        s16 temp = arg0->unkC4;
        if (temp != 0) {
            s16 temp2 = temp - 1;
            arg0->unkC4 = temp2;
            if (temp2 == 0x12) {
                arg0->unkB4->unkBCE |= 8;
            }
        } else {
            setCallback(func_80044990_45590);
        }
    }

end:
    func_80044578_45178(arg0);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80044990_45590);

void func_80044BBC_457BC(Func44BBCArg *);

void func_80044AB8_456B8(Func44BBCArg *arg0) {
    Func43CA4GameState *gameState;
    s32 output[3];

    gameState = getCurrentAllocation();
    if (gameState->unk76 != 0) {
        goto end;
    }

    if (arg0->unkC0 > 0x20000) {
        goto skip_rotation;
    }

    {
        s16 temp = arg0->unkC6;
        if (temp != 0) {
            arg0->unkC6 = temp + 0x40;
        }
    }

    createCombinedRotationMatrix(arg0, arg0->unkC6, arg0->unkC8);

skip_rotation:
    transformVector2(arg0->unkB8, arg0, output);
    arg0->unk14 = arg0->unk14 + output[0];
    arg0->unk18 = arg0->unk18 + output[1];
    arg0->unk1C = arg0->unk1C + output[2];
    memcpy(arg0->unkB4->unkB44, &arg0->unk14, 0xC);
    arg0->unkB4->unkB74 = arg0->unkC6;

    if (arg0->unkC0 != 0) {
        arg0->unkC0 = arg0->unkC0 - 0x2000;
    } else {
        arg0->unkC4 = 4;
        arg0->unkB4->unkBCE |= 4;
        setCallback(func_80044BBC_457BC);
    }

end:
    func_80044578_45178(arg0);
}

void func_80044BBC_457BC(Func44BBCArg *arg0) {
    Func43CA4GameState *gameState = (Func43CA4GameState *)getCurrentAllocation();
    s32 pad[4];

    if (gameState->unk76 == 0) {
        arg0->unkC4--;
        if (arg0->unkC4 == 0) {
            setCallback(func_80044C38_45838);
        }
        memcpy(&arg0->unkB4->unkB44, &arg0->unk14, 0xC);
        arg0->unkB4->unkB74 = arg0->unkC6;
    }

    func_80044578_45178(arg0);
}

void func_80044C38_45838(Func44BBCArg *arg0) {
    Func43CA4GameState *gameState = (Func43CA4GameState *)getCurrentAllocation();
    s32 pad[4];

    if (gameState->unk76 == 0) {
        arg0->unkBC -= 0x8000;
        if (arg0->unkBC < (s32)0xFFF00000) {
            func_80069CF8_6A8F8();
        }
        arg0->unk18 += arg0->unkBC;
    }

    func_80044578_45178(arg0);
}

void func_80044CA4_458A4(Func432D8Arg *arg0) {
    arg0->unk24 = freeNodeMemory(arg0->unk24);
    arg0->unk28 = freeNodeMemory(arg0->unk28);
}

typedef struct {
    u8 _pad0[0xB4];
    void *unkB4;
} Func44CDCTaskMem;

void func_80044CDC_458DC(void *arg0) {
    Func44CDCTaskMem *task;

    task = (Func44CDCTaskMem *)scheduleTask(func_80044684_45284, 0, 0, 0x32);
    if (task != NULL) {
        task->unkB4 = arg0;
    }
}

extern void *D_80090AB8_916B8[];
extern void *D_80090ABC_916BC[];
extern s32 D_80090AC0_916C0[];
extern void func_80044DB0_459B0(void);

typedef struct {
    void *unk0;    /* 0x00 */
    u8 _pad4[0x8]; /* 0x04 */
    void *unkC;    /* 0x0C */
    void *unk10;   /* 0x10 */
    s16 unk14;     /* 0x14 */
} Func44D1CArg;

typedef struct Func45010Arg Func45010Arg;
void func_80045010_45C10(Func45010Arg *);

void func_80044D1C_4591C(Func44D1CArg *arg0) {
    s16 idx;

    arg0->unkC = dmaRequestAndUpdateStateWithSize(&_3F3EF0_ROM_START, &_3F3EF0_ROM_END, 0x2608);
    idx = arg0->unk14;
    arg0->unk10 = dmaRequestAndUpdateStateWithSize(
        D_80090AB8_916B8[idx * 3],
        D_80090ABC_916BC[idx * 3],
        D_80090AC0_916C0[idx * 3]
    );
    arg0->unk0 = NULL;
    setCleanupCallback(func_80045010_45C10);
    setCallback(func_80044DB0_459B0);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80044DB0_459B0);

INCLUDE_ASM("asm/nonmatchings/42170", func_80044EC4_45AC4);

struct Func45010Arg {
    void *unk0;   /* 0x00 */
    u8 pad4[0x8]; /* 0x04 */
    void *unkC;   /* 0x0C */
    void *unk10;  /* 0x10 */
};

void func_80045010_45C10(Func45010Arg *arg0) {
    arg0->unk0 = freeNodeMemory(arg0->unk0);
    arg0->unkC = freeNodeMemory(arg0->unkC);
    arg0->unk10 = freeNodeMemory(arg0->unk10);
}

INCLUDE_ASM("asm/nonmatchings/42170", func_80045054_45C54);

typedef struct {
    u8 _pad0[0x14];
    s16 unk14;
} Func45434TaskMem;

void func_80045434_46034(s16 arg0) {
    Func45434TaskMem *task;

    task = (Func45434TaskMem *)scheduleTask(func_80044D1C_4591C, 0, 0, 0xD3);
    if (task != NULL) {
        task->unk14 = arg0;
    }
}
