#include "2DD40.h"
#include "20F0.h"
#include "297B0.h"
#include "2C8F0.h"
#include "D_800AFE8C_A71FC_type.h"
#include "task_scheduler.h"

extern void func_8000A49C_B09C(SceneModel *, s16, s16, s16, void *, s32, s8, u8, u8, s16);
extern u8 identityMatrix[];
extern u16 D_8008EF70_8FB70[];

typedef struct Func2E024Arg Func2E024Arg;
struct Func2E024Arg {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ u8 pad0[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1[0x4];
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8 pad2[0x1C];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 pad4C[0x4];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ u8 pad3b[0xC];
    /* 0x5E */ u8 unk5E;
    /* 0x5F */ u8 pad4[0x3];
    /* 0x62 */ s8 unk62;
    /* 0x63 */ u8 pad5[0x1];
    /* 0x64 */ SceneModel *unk64;
    /* 0x68 */ u8 pad6[0x14];
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ u8 pad80[0x4];
    /* 0x84 */ s32 unk84;
    /* 0x88 */ u8 pad88[0x1C];
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ s32 unkAC;
    /* 0xB0 */ u8 padB0[0x4];
    /* 0xB4 */ s16 unkB4;
    /* 0xB6 */ u8 pad8[0x8];
    /* 0xBE */ s16 unkBE;
    /* 0xC0 */ u8 pad9[0x2];
    /* 0xC2 */ u8 unkC2;
    /* 0xC3 */ u8 pad10[0x3];
    /* 0xC6 */ s8 unkC6;
    /* 0xC7 */ u8 pad11[0x1];
    /* 0xC8 */ void (*unkC8)(Func2E024Arg *);
    /* 0xCC */ s16 unkCC;
    /* 0xCE */ s16 unkCE;
    /* 0xD0 */ u8 pad12[0x5];
    /* 0xD5 */ u8 unkD5;
};

struct Func2E024Element {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ Mat3x3Padded matrix;
    /* 0x24 */ u8 pad24[0x40];
}; /* size = 0x64 */

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D140_2DD40);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D46C_2E06C);

void func_8002D668_2E268(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 i;
    struct Func2E024Element *elements;
    s32 val;

    allocation = getCurrentAllocation();
    elements = (struct Func2E024Element *)arg0;

    for (i = 0; i < arg0->unkD5; i++) {
        val = ((Func2E024Arg *)&elements[i])->unk5E;

        if (val < 0) {
            goto cont;
        }

        if (val >= 2) {
            if (val >= 4) {
                goto cont;
            }
            goto case23;
        }

        if (((Func2E024Arg *)&elements[i])->unk62 == 0) {
            goto cont;
        }
        if (i == 0) {
            arg0->unk50 = 0x12;
            arg0->unk5E = 2;
            goto cont;
        }
        arg0->unkB4 = 0x1F;
        arg0->unkC2 = 3;
        goto cont;

    case23:
        ((Func2E024Arg *)&elements[i])->unk62 = 0;
        if (getFreeNodeCount(2) == 0x14) {
            func_8000A49C_B09C(arg0->model, 0, 0x29, -1, &arg0->unk40, 0x10000, 0, 2, 0, 0);
            func_8000A49C_B09C(arg0->unk64, 0, 0x24, -1, &arg0->unkA4, 0x10000, 0, 2, 0, 0);
        }

    cont:
        func_8002A2D0_2AED0((Func297D8Arg *)&elements[i]);
        ((s32 *)allocation)[0x102 + i] = elements[i].matrix.unk14;
        ((s32 *)allocation)[0x104 + i] = elements[i].matrix.unk1C;
    }

    if (allocation->unk42A == 0x11) {
        allocation->unk42E = 1;
        arg0->unkCC = 1;
        arg0->unkCE = 1;
        func_8002EBB0_2F7B0(arg0);
        arg0->unkC8 = func_8002D668_2E268;
        setCallback(func_8002BEF4_2CAF4);
    }
}

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D814_2E414);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002DA54_2E654);

void func_8002E024_2EC24(Func2E024Arg *arg0);

void func_8002DE44_2EA44(Func2E024Arg *arg0) {
    GameState *gameState;
    s32 i;
    struct Func2E024Element *elements;

    gameState = getCurrentAllocation();
    elements = (struct Func2E024Element *)arg0;

    for (i = 0; i < arg0->unkD5; i++) {
        memcpy(&elements[i].matrix, identityMatrix, 0x20);
        ((Func2E024Arg *)&elements[i])->unk62 = 0;

        if (i == 0) {
            arg0->unk18 = 0xFFFC0000;
            arg0->unk20 = 0xFFBC0000;
            func_80001688_2288(arg0->model, 4);
            arg0->unk50 = 0x11;
            arg0->unk5E = 0x63;
            arg0->unk44 = 0x1D0000;
            arg0->unk40 = 0xFFFC0000;
            arg0->unk48 = 0;
            func_8000A49C_B09C(arg0->model, 0, 0x2F, -1, &arg0->unk40, 0x10000, 0, 2, 0, 0);
        } else {
            arg0->unk7C = 0x200000;
            arg0->unkC2 = 0x62;
            arg0->unkA8 = 0x260000;
            arg0->unk84 = 0xFFBC0000;
            arg0->unkB4 = 0;
            arg0->unkBE = 0;
            arg0->unkA4 = 0;
            arg0->unkAC = 0;
            func_8000A49C_B09C(arg0->unk64, 0, 7, 0x2D, &arg0->unkA4, 0x10000, 0, 2, 0, 0);
        }

        ((Func297D8Arg *)&elements[i])->rotation = 0x1800;
        ((Func297D8Arg *)&elements[i])->unk2E = 0x1800;
        ((Func297D8Arg *)&elements[i])->unk52 = ((Func297D8Arg *)&elements[i])->unk50;
        createYRotationMatrix(&elements[i].matrix, ((Func297D8Arg *)&elements[i])->rotation);
        func_8002A290_2AE90((Func297D8Arg *)&elements[i]);
        ((s32 *)gameState)[0x102 + i] = elements[i].matrix.unk14;
        ((s32 *)gameState)[0x104 + i] = elements[i].matrix.unk1C;
        ((s16 *)gameState)[0x20C + i] = D_8008EF70_8FB70[((Func297D8Arg *)&elements[i])->unk5C];
    }

    setCallback(func_8002E024_2EC24);
}

void func_8002E024_2EC24(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 i;
    struct Func2E024Element *elements;

    allocation = (GameState *)getCurrentAllocation();

    switch (arg0->unkC2) {
        case 0x62:
            arg0->unkBE++;
            if (arg0->unkBE == 0x2D) {
                arg0->unkB4 = 0x10;
                arg0->unkBE = 0;
                arg0->unkC2 = 1;
            }
            break;
        case 1:
            if (arg0->unkC6 != 0) {
                arg0->unkB4 = 0x11;
                arg0->unkC2 = 0x63;
            }
            break;
        case 0x63:
            arg0->unk62 = 0;
            arg0->unkC6 = 0;
            break;
    }

    elements = (struct Func2E024Element *)arg0;
    for (i = 0; i < arg0->unkD5; i++) {
        func_8002A2D0_2AED0((Func297D8Arg *)&elements[i]);
        ((s32 *)allocation)[0x102 + i] = elements[i].matrix.unk14;
        ((s32 *)allocation)[0x104 + i] = elements[i].matrix.unk1C;
    }

    if (allocation->unk42A == 0x11) {
        func_8002EBB0_2F7B0(arg0);
        arg0->unkC8 = func_8002E024_2EC24;
        setCallback(func_8002BEF4_2CAF4);
    }
}

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E170_2ED70);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E314_2EF14);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E680_2F280);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E8B4_2F4B4);

extern u8 D_8008F044_8FC44[];
extern s8 D_8008F054_8FC54[];

s8 func_80001660_2260(void *arg0);

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ u8 pad0[0x28];
    /* 0x2C */ u16 unk2C;
    /* 0x2E */ u8 pad1[2];
    /* 0x30 */ u16 unk30;
    /* 0x32 */ u8 pad2[0xE];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 pad3[4];
    /* 0x50 */ u16 unk50;
    /* 0x52 */ u8 pad4[4];
    /* 0x56 */ u16 unk56;
    /* 0x58 */ u8 pad5[7];
    /* 0x5F */ u8 unk5F;
    /* 0x60 */ u8 pad6[4];
} Func2EBB0Element; /* size = 0x64 */

typedef struct {
    /* 0x00 */ Func2EBB0Element elements[2];
    /* 0xC8 */ u8 pad[8];
    /* 0xD0 */ u8 ctrl[2];
    /* 0xD2 */ u8 pad7;
    /* 0xD3 */ u8 unkD3;
    /* 0xD4 */ u8 unkD4;
} Func2EBB0Container;

void func_8002EBB0_2F7B0(void *ptr) {
    Func2EBB0Container *arg0 = ptr;
    s32 i;

    for (i = 0; i < 2; i++) {
        arg0->elements[i].unk56 = arg0->elements[i].unk50;
        arg0->elements[i].unk5F = func_80001660_2260(arg0->elements[i].unk0);

        if (((arg0->unkD4 == 1) & (i == 0)) && D_800AFE8C_A71FC->unk9[0] == 3) {
            func_80001688_2288(arg0->elements[i].unk0, -1);
            arg0->elements[i].unk48 = 0;
            arg0->elements[i].unk40 = 0;
            arg0->elements[i].unk44 = 0x240000;
        } else if (((arg0->unkD4 == 6) & (i == 1)) && D_800AFE8C_A71FC->unk9[0] == 3) {
            func_80001688_2288(arg0->elements[i].unk0, -1);
        } else {
            func_80001688_2288(arg0->elements[i].unk0, D_8008F054_8FC54[arg0->unkD4 * 2 + i]);
        }

        if (D_8008F044_8FC44[arg0->unkD4 * 2 + i] != 0) {
            arg0->elements[i].unk50 = 0;
            arg0->ctrl[i] = 1;
        } else {
            arg0->elements[i].unk30 = arg0->elements[i].unk2C;
            arg0->ctrl[i] = 3;
        }
    }

    arg0->unkD3 = 0;
}

s32 func_8002ED30_2F930(void) {
    return D_800AFE8C_A71FC->padding;
}

void func_8002ED40_2F940(s32 a0) {
    s32 new_var = 0x98967F;

    D_800AFE8C_A71FC->padding += a0;

    if (D_800AFE8C_A71FC->padding > new_var) {
        D_800AFE8C_A71FC->padding = new_var;
    }

    if (D_800AFE8C_A71FC->padding < 0) {
        D_800AFE8C_A71FC->padding = 0;
    }
}