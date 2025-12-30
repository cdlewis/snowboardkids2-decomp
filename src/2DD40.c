#include "2DD40.h"
#include "20F0.h"
#include "297B0.h"
#include "2C8F0.h"
#include "D_800AFE8C_A71FC_type.h"
#include "gamestate.h"
#include "graphics.h"
#include "rand.h"
#include "task_scheduler.h"

extern void spawnSpriteEffectEx(SceneModel *, s16, s16, s16, void *, s32, s8, u8, u8, s16);
extern u8 identityMatrix[];
extern u16 D_8008EF70_8FB70[];

typedef struct {
    /* 0x00 */ SceneModel *model;
    /* 0x04 */ Transform3D matrix;
    /* 0x24 */ u8 pad24[0x40];
} Func2E024Element; /* size = 0x64 */

void func_8002D668_2E268(Func2E024Arg *arg0);

void func_8002D140_2DD40(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 i;
    Func2E024Element *ptr;
    u16 temp;
    s32 pad[4];

    (void)pad;
    allocation = getCurrentAllocation();

    switch (arg0->unkD2) {
        case 0:
            temp = arg0->unk5A + 1;
            arg0->unk5A = temp;
            if ((s16)temp == 0x3C) {
                arg0->unkD2 = 1;
                arg0->unk5A = 0;
                arg0->unkB4 = 0x1C;
                setAnimationIndex(arg0->unk64, 3);
                spawnSpriteEffectEx(arg0->model, 0, 6, 0xF, &arg0->unk40, 0x10000, 0, 2, 0, 0);
                spawnSpriteEffectEx(arg0->unk64, 0, 6, 0xF, &arg0->unkA4, 0x10000, 0, 2, 0, 0);
            }
            break;

        case 1:
            if (arg0->unkC6 != 0) {
                arg0->unkC6 = -1;
                arg0->unkD2 = 2;
            }
            break;

        case 2:
            temp = arg0->unk5A + 1;
            arg0->unk5A = temp;
            if ((s16)temp == 0x14) {
                arg0->unkC6 = 0;
                arg0->unkB4 = 0x1E;
                setAnimationIndex(arg0->unk64, 2);
                arg0->unk50 = 0x17;
                setAnimationIndex(arg0->model, 2);
                arg0->unkD2 = 3;
                arg0->unk5A = 0;
            }
            break;

        case 3:
            if (arg0->unkC6 != 0) {
                arg0->unkB4 = 0x1F;
                arg0->unk50 = 0x18;
                arg0->unkD2 = 4;
            }
            break;

        case 4:
            arg0->unk62 = 0;
            arg0->unkC6 = 0;
            if (getFreeNodeCount(2) == 0x14) {
                arg0->unk40 = (s32)0xFFF90000;
                arg0->unk44 = 0x220000;
                spawnSpriteEffectEx(arg0->model, 0, 0x24, -1, &arg0->unk40, 0x10000, 1, 2, 1, 0);
                arg0->unkA4 = 0x90000;
                arg0->unkA8 = 0x260000;
                spawnSpriteEffectEx(arg0->unk64, 0, 0x24, -1, &arg0->unkA4, 0x10000, 0, 2, 0, 0);
            } else if (getFreeNodeCount(2) == 0x13) {
                arg0->unk40 = (s32)0xFFF90000;
                arg0->unk44 = 0x220000;
                spawnSpriteEffectEx(arg0->model, 0, 0x24, -1, &arg0->unk40, 0x10000, 1, 2, 1, 0);
            }
            break;
    }

    ptr = (Func2E024Element *)arg0;
    for (i = 0; i < 2; i++) {
        func_8002A2D0_2AED0((Func297D8Arg *)&ptr[i]);
        allocation->unk408[i] = ptr[i].matrix.translation.x;
        allocation->unk410[i] = ptr[i].matrix.translation.z;
    }

    if (allocation->unk42A == 0x11) {
        if (arg0->unkD2 == 4) {
            allocation->unk42E = 2;
        } else {
            allocation->unk42E = 1;
        }
        func_8002EBB0_2F7B0(arg0);
        arg0->unkC8 = func_8002D140_2DD40;
        setCallback(func_8002BEF4_2CAF4);
    }
}

void func_8002D46C_2E06C(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 i;
    Func2E024Element *elements;
    u16 rotation;

    allocation = getCurrentAllocation();
    elements = (Func2E024Element *)arg0;

    for (i = 0; i < arg0->unkD5; i++) {
        memcpy((u8 *)&elements[i] + 4, identityMatrix, sizeof(Transform3D));
        ((Func297D8Arg *)&elements[i])->unk62 = 0;
        arg0->unkCC[i] = 0;

        if (i == 0) {
            arg0->unk4.translation.x = -0x80000;
            arg0->unk4.translation.z = -0x4E0000;
            setAnimationIndex(arg0->model, 1);
            arg0->unk50 = 0x11;
            arg0->unk44 = 0x260000;
            arg0->unk5E = 0;
            arg0->unk40 = 0;
            arg0->unk48 = 0;
            spawnSpriteEffectEx(arg0->model, 0, 0x29, -1, &arg0->unk40, 0x10000, 0, 2, 0, 0);
        } else {
            arg0->unk7C = 0x1C0000;
            arg0->unk84 = -0x4E0000;
            setAnimationIndex(arg0->unk64, 2);
            arg0->unkB4 = 0x1E;
            arg0->unkC2 = 1;
            arg0->unkA4 = 0x40000;
            arg0->unkA8 = 0x280000;
            arg0->unkAC = 0;
            spawnSpriteEffectEx(arg0->unk64, 0, 0x24, -1, &arg0->unkA4, 0x10000, 0, 2, 0, 0);
        }

        ((Func297D8Arg *)&elements[i])->rotation = 0x800 + i * 0x1000;
        ((Func297D8Arg *)&elements[i])->unk2E = 0x800 + i * 0x1000;
        ((Func297D8Arg *)&elements[i])->unk52 = ((Func297D8Arg *)&elements[i])->unk50;
        createYRotationMatrix((Transform3D *)((u8 *)&elements[i] + 4), ((Func297D8Arg *)&elements[i])->rotation);
        func_8002A290_2AE90((Func297D8Arg *)&elements[i]);
        allocation->unk408[i] = elements[i].matrix.translation.x;
        allocation->unk410[i] = elements[i].matrix.translation.z;
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
            spawnSpriteEffectEx(arg0->model, 0, 0x29, -1, &arg0->unk40, 0x10000, 0, 2, 0, 0);
            spawnSpriteEffectEx(arg0->unk64, 0, 0x24, -1, &arg0->unkA4, 0x10000, 0, 2, 0, 0);
        }

    cont:
        func_8002A2D0_2AED0((Func297D8Arg *)&elements[i]);
        gameState->unk408[i] = elements[i].matrix.translation.x;
        gameState->unk410[i] = elements[i].matrix.translation.z;
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
            container->unk4.translation.x = 0xFFFC0000;
            container->unk4.translation.z = 0xFFBC0000;
            setAnimationIndex(container->model, 4);
            container->unk50 = 0x11;
            container->unk5E = 0x63;
            container->unk44 = 0x1D0000;
            container->unk40 = 0xFFFC0000;
            container->unk48 = 0;
            spawnSpriteEffectEx(container->model, 0, 0x2F, -1, &container->unk40, 0x10000, 0, 2, 0, 0);
        } else {
            container->unk7C = 0x200000;
            container->unkC2 = 0x62;
            container->unkA8 = 0x260000;
            container->unk84 = 0xFFBC0000;
            container->unkB4 = 0;
            container->unkBE = 0;
            container->unkA4 = 0;
            container->unkAC = 0;
            spawnSpriteEffectEx(container->unk64, 0, 7, 0x2D, &container->unkA4, 0x10000, 0, 2, 0, 0);
        }

        element->rotation = 0x1800;
        element->unk2E = 0x1800;
        element->unk52 = element->unk50;
        createYRotationMatrix(&elements[i].matrix, element->rotation);
        func_8002A290_2AE90(element);
        gameState->unk408[i] = elements[i].matrix.translation.x;
        gameState->unk410[i] = elements[i].matrix.translation.z;
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
        gameState->unk408[i] = elements[i].matrix.translation.x;
        gameState->unk410[i] = elements[i].matrix.translation.z;
    }

    if (gameState->unk42A == 0x11) {
        func_8002EBB0_2F7B0(arg0);
        arg0->unkC8 = func_8002E024_2EC24;
        setCallback(func_8002BEF4_2CAF4);
    }
}

void func_8002E314_2EF14(Func2E024Arg *);

typedef struct {
    s32 unk0;
    Transform3D unk4;
    /* 0x24 */ u8 pad2[0x8];
    /* 0x2C */ u16 unk2C;
    /* 0x2E */ u16 unk2E;
    /* 0x30 */ u8 pad3[0x10];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 pad3b[0x4];
    /* 0x50 */ s16 unk50;
    /* 0x52 */ s16 unk52;
    /* 0x54 */ u8 pad4[0x6];
    /* 0x5A */ s16 unk5A;
    /* 0x5C */ u8 unk5C;
    /* 0x5D */ u8 pad5[0x1];
    /* 0x5E */ u8 unk5E;
    /* 0x5F */ u8 pad5b[0x3];
    /* 0x62 */ s8 unk62;
    /* 0x63 */ s8 unk63;
} FuncE170Element;

typedef struct FuncE170Container FuncE170Container;
struct FuncE170Container {
    /* 0x00 */ FuncE170Element unk0[2];
    /* 0xC8 */ void (*unkC8)(FuncE170Container *);
    /* 0xCC */ s16 unkCC;
    /* 0xCE */ s16 unkCE;
    /* 0xD0 */ u8 pad9[0x2];
    /* 0xD2 */ u8 unkD2;
    /* 0xD3 */ u8 pad9b[0x2];
    /* 0xD5 */ u8 unkD5;
};

void func_8002E170_2ED70(Func2E024Arg *arg0) {
    FuncE170Container *container = (FuncE170Container *)arg0;
    GameState *allocation;
    s32 i;

    allocation = getCurrentAllocation();
    i = 0;
    container->unkD2 = 0;

    for (; i < container->unkD5; i++) {
        memcpy(&container->unk0[i].unk4, identityMatrix, sizeof(Transform3D));
        container->unk0[i].unk62 = 0;
        container->unk0[i].unk5A = 0;

        if (i == 0) {
            container->unk0[0].unk4.translation.x = -0x60000;
            container->unk0[0].unk4.translation.z = -0x4E0000;
            container->unk0[0].unk50 = 0;
            container->unk0[0].unk5E = 3;
        } else {
            container->unk0[1].unk4.translation.x = 0x1E0000;
            container->unk0[1].unk4.translation.z = -0x4E0000;
            container->unk0[1].unk50 = 0x13;
            container->unk0[1].unk5E = 0;
        }

        container->unk0[i].unk48 = 0;
        container->unk0[i].unk40 = 0;

        if (i == 0) {
            container->unk0[0].unk44 = 0x290000;
            container->unk0[0].unk40 += -0x90000;
        } else {
            container->unk0[i].unk40 = 0x60000;
            container->unk0[i].unk44 = 0x290000;
        }

        container->unk0[i].unk2C = 0x800 + i * 0x1000;
        container->unk0[i].unk2E = 0x800 + i * 0x1000;
        container->unk0[i].unk52 = container->unk0[i].unk50;
        createYRotationMatrix(&container->unk0[i].unk4, container->unk0[i].unk2C);
        func_8002A290_2AE90((Func297D8Arg *)&container->unk0[i]);
        allocation->unk408[i] = container->unk0[i].unk4.translation.x;
        allocation->unk410[i] = container->unk0[i].unk4.translation.z;
        allocation->unk418[i] = D_8008EF70_8FB70[container->unk0[i].unk5C];
    }

    container->unkCC = (randB() & 0x1F) + 0x28;
    container->unkCE = 0;
    setCallback(func_8002E314_2EF14);
}

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E314_2EF14);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E680_2F280);

void func_8002E8B4_2F4B4(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 incrementedCount;
    s32 i;
    Func2E024Arg *ptr;
    s32 sp10[3];
    s32 sp20[3];
    Transform3D sp30;
    Transform3D sp50;
    s16 var_v1;
    s32 var_a0;
    s32 tempAngle;
    s32 count;

    allocation = getCurrentAllocation();
    incrementedCount = 0;

    for (i = 0; i < arg0->unkD5; i++) {
        ptr = (Func2E024Arg *)((u8 *)arg0 + i * 0x64);

        memcpy(&sp30, identityMatrix, sizeof(Transform3D));
        memcpy(&sp50, &ptr->unk4, sizeof(Transform3D));

        switch (ptr->unk5E) {
            case 0:
                ptr->unk5A--;
                if ((ptr->unk5A << 16) == 0) {
                    ptr->unk5E = 1;
                }
                break;
            case 1:
                memcpy(sp20, &ptr->unk4.translation, 0xC);
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
                memcpy(&ptr->unk4.translation, sp10, 0xC);
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
            tempAngle = atan2Fixed(ptr->unk4.translation.x, ptr->unk4.translation.z);
            createYRotationMatrix(&sp50, tempAngle & 0xFFFF);
            memcpy(&sp50.translation, &ptr->unk4.translation, 0xC);
            func_8006B084_6BC84(&sp30, &sp50, &ptr->unk4);
            func_8002A2D0_2AED0((Func297D8Arg *)ptr);
            allocation->unk408[i] = ptr->unk4.translation.x;
            allocation->unk410[i] = ptr->unk4.translation.z;
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