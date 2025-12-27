#include "2DD40.h"
#include "20F0.h"
#include "297B0.h"
#include "2C8F0.h"
#include "D_800AFE8C_A71FC_type.h"
#include "graphics.h"
#include "task_scheduler.h"

extern void func_8000A49C_B09C(SceneModel *, s16, s16, s16, void *, s32, s8, u8, u8, s16);
extern u8 identityMatrix[];
extern u16 D_8008EF70_8FB70[];

typedef struct Func2E024Arg Func2E024Arg;
struct Func2E024Arg {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ Mat3x3Padded unk4;
    /* 0x24 */ u8 pad24[0x6];
    /* 0x2A */ u16 unk2A;
    /* 0x2C */ u16 unk2C;
    /* 0x2E */ u8 pad2E[0x12];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 pad4C[0x4];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ u8 pad52[0x8];
    /* 0x5A */ u16 unk5A;
    /* 0x5C */ u8 pad5C[0x2];
    /* 0x5E */ u8 unk5E;
    /* 0x5F */ u8 pad5F[0x3];
    /* 0x62 */ s8 unk62;
    /* 0x63 */ u8 pad63[0x1];
    /* 0x64 */ SceneModel *unk64;
    /* 0x68 */ u8 pad68[0x14];
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ u8 pad80[0x4];
    /* 0x84 */ s32 unk84;
    /* 0x88 */ u8 pad88[0x1C];
    /* 0xA4 */ s32 unkA4;
    /* 0xA8 */ s32 unkA8;
    /* 0xAC */ s32 unkAC;
    /* 0xB0 */ u8 padB0[0x4];
    /* 0xB4 */ s16 unkB4;
    /* 0xB6 */ u8 padB6[0x8];
    /* 0xBE */ s16 unkBE;
    /* 0xC0 */ u8 padC0[0x2];
    /* 0xC2 */ u8 unkC2;
    /* 0xC3 */ u8 padC3[0x3];
    /* 0xC6 */ s8 unkC6;
    /* 0xC7 */ u8 padC7[0x1];
    /* 0xC8 */ void (*unkC8)(Func2E024Arg *);
    /* 0xCC */ s16 unkCC[2];
    /* 0xD0 */ u8 padD0[0x5];
    /* 0xD5 */ u8 unkD5;
};

typedef struct {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ Mat3x3Padded matrix;
    /* 0x24 */ u8 pad24[0x40];
} Func2E024Element; /* size = 0x64 */

void func_8002D668_2E268(Func2E024Arg *arg0);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D140_2DD40);

void func_8002D46C_2E06C(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 i;
    Func2E024Element *elements;
    u16 rotation;

    allocation = getCurrentAllocation();
    elements = (Func2E024Element *)arg0;

    for (i = 0; i < arg0->unkD5; i++) {
        memcpy((u8 *)&elements[i] + 4, identityMatrix, sizeof(Mat3x3Padded));
        ((Func297D8Arg *)&elements[i])->unk62 = 0;
        arg0->unkCC[i] = 0;

        if (i == 0) {
            arg0->unk4.unk14 = -0x80000;
            arg0->unk4.unk1C = -0x4E0000;
            setAnimationIndex(arg0->model, 1);
            arg0->unk50 = 0x11;
            arg0->unk44 = 0x260000;
            arg0->unk5E = 0;
            arg0->unk40 = 0;
            arg0->unk48 = 0;
            func_8000A49C_B09C(arg0->model, 0, 0x29, -1, &arg0->unk40, 0x10000, 0, 2, 0, 0);
        } else {
            arg0->unk7C = 0x1C0000;
            arg0->unk84 = -0x4E0000;
            setAnimationIndex(arg0->unk64, 2);
            arg0->unkB4 = 0x1E;
            arg0->unkC2 = 1;
            arg0->unkA4 = 0x40000;
            arg0->unkA8 = 0x280000;
            arg0->unkAC = 0;
            func_8000A49C_B09C(arg0->unk64, 0, 0x24, -1, &arg0->unkA4, 0x10000, 0, 2, 0, 0);
        }

        ((Func297D8Arg *)&elements[i])->rotation = 0x800 + i * 0x1000;
        ((Func297D8Arg *)&elements[i])->unk2E = 0x800 + i * 0x1000;
        ((Func297D8Arg *)&elements[i])->unk52 = ((Func297D8Arg *)&elements[i])->unk50;
        createYRotationMatrix((Mat3x3Padded *)((u8 *)&elements[i] + 4), ((Func297D8Arg *)&elements[i])->rotation);
        func_8002A290_2AE90((Func297D8Arg *)&elements[i]);
        allocation->unk408[i] = elements[i].matrix.unk14;
        allocation->unk410[i] = elements[i].matrix.unk1C;
        allocation->unk418[i] = D_8008EF70_8FB70[((Func297D8Arg *)&elements[i])->unk5C];
    }

    setCallback(func_8002D668_2E268);
}

void func_8002D668_2E268(Func2E024Arg *arg0) {
    GameState *gameState;
    s32 i;
    Func2E024Element *elements;
    s32 state;

    gameState = getCurrentAllocation();
    elements = (Func2E024Element *)arg0;

    for (i = 0; i < arg0->unkD5; i++) {
        state = ((Func2E024Arg *)&elements[i])->unk5E;

        if (state < 0) {
            goto cont;
        }

        if (state >= 2) {
            if (state >= 4) {
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
        gameState->unk408[i] = elements[i].matrix.unk14;
        gameState->unk410[i] = elements[i].matrix.unk1C;
    }

    if (gameState->unk42A == 0x11) {
        gameState->unk42E = 1;
        arg0->unkCC[0] = 1;
        arg0->unkCC[1] = 1;
        func_8002EBB0_2F7B0(arg0);
        arg0->unkC8 = func_8002D668_2E268;
        setCallback(func_8002BEF4_2CAF4);
    }
}

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D814_2E414);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002DA54_2E654);

void func_8002E024_2EC24(Func2E024Arg *arg0);

void func_8002DE44_2EA44(Func2E024Arg *container) {
    GameState *gameState;
    s32 i;
    Func2E024Element *elements;

    gameState = getCurrentAllocation();
    elements = (Func2E024Element *)container;

    for (i = 0; i < container->unkD5; i++) {
        Func297D8Arg *element = (Func297D8Arg *)&elements[i];

        memcpy(&elements[i].matrix, identityMatrix, 0x20);
        element->unk62 = 0;

        if (i == 0) {
            container->unk4.unk14 = 0xFFFC0000;
            container->unk4.unk1C = 0xFFBC0000;
            setAnimationIndex(container->model, 4);
            container->unk50 = 0x11;
            container->unk5E = 0x63;
            container->unk44 = 0x1D0000;
            container->unk40 = 0xFFFC0000;
            container->unk48 = 0;
            func_8000A49C_B09C(container->model, 0, 0x2F, -1, &container->unk40, 0x10000, 0, 2, 0, 0);
        } else {
            container->unk7C = 0x200000;
            container->unkC2 = 0x62;
            container->unkA8 = 0x260000;
            container->unk84 = 0xFFBC0000;
            container->unkB4 = 0;
            container->unkBE = 0;
            container->unkA4 = 0;
            container->unkAC = 0;
            func_8000A49C_B09C(container->unk64, 0, 7, 0x2D, &container->unkA4, 0x10000, 0, 2, 0, 0);
        }

        element->rotation = 0x1800;
        element->unk2E = 0x1800;
        element->unk52 = element->unk50;
        createYRotationMatrix(&elements[i].matrix, element->rotation);
        func_8002A290_2AE90(element);
        gameState->unk408[i] = elements[i].matrix.unk14;
        gameState->unk410[i] = elements[i].matrix.unk1C;
        gameState->unk418[i] = D_8008EF70_8FB70[element->unk5C];
    }

    setCallback(func_8002E024_2EC24);
}

void func_8002E024_2EC24(Func2E024Arg *arg0) {
    GameState *gameState;
    s32 i;
    Func2E024Element *elements;

    gameState = getCurrentAllocation();

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

    elements = (Func2E024Element *)arg0;
    for (i = 0; i < arg0->unkD5; i++) {
        func_8002A2D0_2AED0((Func297D8Arg *)&elements[i]);
        gameState->unk408[i] = elements[i].matrix.unk14;
        gameState->unk410[i] = elements[i].matrix.unk1C;
    }

    if (gameState->unk42A == 0x11) {
        func_8002EBB0_2F7B0(arg0);
        arg0->unkC8 = func_8002E024_2EC24;
        setCallback(func_8002BEF4_2CAF4);
    }
}

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E170_2ED70);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E314_2EF14);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E680_2F280);

void func_8002E8B4_2F4B4(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 incrementedCount;
    s32 i;
    Func2E024Arg *ptr;
    s32 sp10[3];
    s32 sp20[3];
    Mat3x3Padded sp30;
    Mat3x3Padded sp50;
    s16 var_v1;
    s32 var_a0;
    s32 tempAngle;
    s32 count;

    allocation = getCurrentAllocation();
    incrementedCount = 0;

    for (i = 0; i < arg0->unkD5; i++) {
        ptr = (Func2E024Arg *)((u8 *)arg0 + i * 0x64);

        memcpy(&sp30, identityMatrix, sizeof(Mat3x3Padded));
        memcpy(&sp50, &ptr->unk4, sizeof(Mat3x3Padded));

        switch (ptr->unk5E) {
            case 0:
                ptr->unk5A--;
                if ((ptr->unk5A << 16) == 0) {
                    ptr->unk5E = 1;
                }
                break;
            case 1:
                memcpy(sp20, &ptr->unk4.unk14, 0xC);
                memcpy(sp10, sp20, 0xC);

                if (i == 0) {
                    var_a0 = 0x5200;
                    var_v1 = -0x28;
                } else {
                    var_a0 = 0x4400;
                    var_v1 = -0x18;
                }

                sp10[0] += ((var_v1 * (sp20[2] >> 8)) / var_a0) << 12;
                sp10[2] += ((-var_v1 * (sp20[0] >> 8)) / var_a0) << 12;
                memcpy(&ptr->unk4.unk14, sp10, 0xC);
                break;
            case 2:
                incrementedCount++;
                break;
        }

        {
            u16 temp = arg0->unkCC[0];
            if (temp != 0) {
                temp--;
                arg0->unkCC[0] = temp;
                if ((temp & 0xFFFF) == 0) {
                    func_800585C8_591C8(0xF5);
                }
            }
        }

        {
            u16 temp = arg0->unkCC[1];
            if (temp != 0) {
                temp--;
                arg0->unkCC[1] = temp;
                if ((temp & 0xFFFF) == 0) {
                    func_800585C8_591C8(0x10E);
                }
            }
        }

        if (ptr->unk5E != 2) {
            createYRotationMatrix(&sp30, ptr->unk2C);
            tempAngle = atan2Fixed(ptr->unk4.unk14, ptr->unk4.unk1C);
            createYRotationMatrix(&sp50, tempAngle & 0xFFFF);
            memcpy(&sp50.unk14, &ptr->unk4.unk14, 0xC);
            func_8006B084_6BC84(&sp30, &sp50, &ptr->unk4);
            func_8002A2D0_2AED0((Func297D8Arg *)ptr);
            allocation->unk408[i] = ptr->unk4.unk14;
            allocation->unk410[i] = ptr->unk4.unk1C;
            if (((u32)((tempAngle - 0x1001) & 0xFFFF) < 0x468)) {
                ptr->unk5E = 2;
            }
        } else if ((incrementedCount & 0xFF) == ptr->unk5E) {
            func_80069CF8_6A8F8();
        }
    }
}

extern u8 D_8008F044_8FC44[];
extern s8 D_8008F054_8FC54[];

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
        arg0->elements[i].unk5F = getAnimationIndex(arg0->elements[i].unk0);

        if (((arg0->unkD4 == 1) & (i == 0)) && D_800AFE8C_A71FC->unk9[0] == 3) {
            setAnimationIndex(arg0->elements[i].unk0, -1);
            arg0->elements[i].unk48 = 0;
            arg0->elements[i].unk40 = 0;
            arg0->elements[i].unk44 = 0x240000;
        } else if (((arg0->unkD4 == 6) & (i == 1)) && D_800AFE8C_A71FC->unk9[0] == 3) {
            setAnimationIndex(arg0->elements[i].unk0, -1);
        } else {
            setAnimationIndex(arg0->elements[i].unk0, D_8008F054_8FC54[arg0->unkD4 * 2 + i]);
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