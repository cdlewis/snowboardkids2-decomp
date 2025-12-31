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

void func_8002D668_2E268(Func2E024Arg *arg0);

void func_8002D140_2DD40(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 i;
    Func297D8Arg *ptr;
    u16 temp;
    s32 pad[4];

    (void)pad;
    allocation = getCurrentAllocation();

    switch (arg0->unkD2) {
        case 0:
            temp = arg0->elements[0].unk5A + 1;
            arg0->elements[0].unk5A = temp;
            if ((s16)temp == 0x3C) {
                arg0->unkD2 = 1;
                arg0->elements[0].unk5A = 0;
                arg0->elements[1].unk50 = 0x1C;
                setAnimationIndex(arg0->elements[1].model, 3);
                spawnSpriteEffectEx(arg0->elements[0].model, 0, 6, 0xF, &arg0->elements[0].unk40, 0x10000, 0, 2, 0, 0);
                spawnSpriteEffectEx(arg0->elements[1].model, 0, 6, 0xF, &arg0->elements[1].unk40, 0x10000, 0, 2, 0, 0);
            }
            break;

        case 1:
            if (arg0->elements[1].unk62 != 0) {
                arg0->elements[1].unk62 = -1;
                arg0->unkD2 = 2;
            }
            break;

        case 2:
            temp = arg0->elements[0].unk5A + 1;
            arg0->elements[0].unk5A = temp;
            if ((s16)temp == 0x14) {
                arg0->elements[1].unk62 = 0;
                arg0->elements[1].unk50 = 0x1E;
                setAnimationIndex(arg0->elements[1].model, 2);
                arg0->elements[0].unk50 = 0x17;
                setAnimationIndex(arg0->elements[0].model, 2);
                arg0->unkD2 = 3;
                arg0->elements[0].unk5A = 0;
            }
            break;

        case 3:
            if (arg0->elements[1].unk62 != 0) {
                arg0->elements[1].unk50 = 0x1F;
                arg0->elements[0].unk50 = 0x18;
                arg0->unkD2 = 4;
            }
            break;

        case 4:
            arg0->elements[0].unk62 = 0;
            arg0->elements[1].unk62 = 0;
            if (getFreeNodeCount(2) == 0x14) {
                arg0->elements[0].unk40 = (s32)0xFFF90000;
                arg0->elements[0].unk44 = 0x220000;
                spawnSpriteEffectEx(
                    arg0->elements[0].model,
                    0,
                    0x24,
                    -1,
                    &arg0->elements[0].unk40,
                    0x10000,
                    1,
                    2,
                    1,
                    0
                );
                arg0->elements[1].unk40 = 0x90000;
                arg0->elements[1].unk44 = 0x260000;
                spawnSpriteEffectEx(
                    arg0->elements[1].model,
                    0,
                    0x24,
                    -1,
                    &arg0->elements[1].unk40,
                    0x10000,
                    0,
                    2,
                    0,
                    0
                );
            } else if (getFreeNodeCount(2) == 0x13) {
                arg0->elements[0].unk40 = (s32)0xFFF90000;
                arg0->elements[0].unk44 = 0x220000;
                spawnSpriteEffectEx(
                    arg0->elements[0].model,
                    0,
                    0x24,
                    -1,
                    &arg0->elements[0].unk40,
                    0x10000,
                    1,
                    2,
                    1,
                    0
                );
            }
            break;
    }

    ptr = arg0->elements;
    for (i = 0; i < 2; i++) {
        func_8002A2D0_2AED0(&ptr[i]);
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
        arg0->callback = func_8002D140_2DD40;
        setCallback(func_8002BEF4_2CAF4);
    }
}

void func_8002D46C_2E06C(Func2E024Arg *container) {
    GameState *gameState;
    s32 i;
    Func297D8Arg *characters;

    gameState = getCurrentAllocation();
    characters = container->elements;

    for (i = 0; i < container->unkD5; i++) {
        memcpy(&characters[i].matrix, identityMatrix, sizeof(Transform3D));
        characters[i].unk62 = 0;
        container->unkCC[i] = 0;

        if (i == 0) {
            container->elements[0].matrix.translation.x = -0x80000;
            container->elements[0].matrix.translation.z = -0x4E0000;
            setAnimationIndex(container->elements[0].model, 1);
            container->elements[0].unk50 = 0x11;
            container->elements[0].unk44 = 0x260000;
            container->elements[0].unk5E = 0;
            container->elements[0].unk40 = 0;
            container->elements[0].unk48 = 0;
            spawnSpriteEffectEx(
                container->elements[0].model,
                0,
                0x29,
                -1,
                &container->elements[0].unk40,
                0x10000,
                0,
                2,
                0,
                0
            );
        } else {
            container->elements[1].matrix.translation.x = 0x1C0000;
            container->elements[1].matrix.translation.z = -0x4E0000;
            setAnimationIndex(container->elements[1].model, 2);
            container->elements[1].unk50 = 0x1E;
            container->elements[1].unk5E = 1;
            container->elements[1].unk40 = 0x40000;
            container->elements[1].unk44 = 0x280000;
            container->elements[1].unk48 = 0;
            spawnSpriteEffectEx(
                container->elements[1].model,
                0,
                0x24,
                -1,
                &container->elements[1].unk40,
                0x10000,
                0,
                2,
                0,
                0
            );
        }

        characters[i].rotation = 0x800 + i * 0x1000;
        characters[i].unk2E = 0x800 + i * 0x1000;
        characters[i].unk52 = characters[i].unk50;
        createYRotationMatrix(&characters[i].matrix, characters[i].rotation);
        func_8002A290_2AE90(&characters[i]);
        gameState->unk408[i] = characters[i].matrix.translation.x;
        gameState->unk410[i] = characters[i].matrix.translation.z;
        gameState->unk418[i] = D_8008EF70_8FB70[characters[i].unk5C];
    }

    setCallback(func_8002D668_2E268);
}

void func_8002D668_2E268(Func2E024Arg *arg0) {
    GameState *gameState;
    s32 i;
    Func297D8Arg *elements;
    s32 state;

    gameState = getCurrentAllocation();
    elements = arg0->elements;

    for (i = 0; i < arg0->unkD5; i++) {
        state = elements[i].unk5E;

        if (state < 0) {
            goto cont;
        }

        if (state >= 2) {
            if (state >= 4) {
                goto cont;
            }
            goto case23;
        }

        if (elements[i].unk62 == 0) {
            goto cont;
        }
        if (i == 0) {
            arg0->elements[0].unk50 = 0x12;
            arg0->elements[0].unk5E = 2;
            goto cont;
        }
        arg0->elements[1].unk50 = 0x1F;
        arg0->elements[1].unk5E = 3;
        goto cont;

    case23:
        elements[i].unk62 = 0;
        if (getFreeNodeCount(2) == 0x14) {
            spawnSpriteEffectEx(arg0->elements[0].model, 0, 0x29, -1, &arg0->elements[0].unk40, 0x10000, 0, 2, 0, 0);
            spawnSpriteEffectEx(arg0->elements[1].model, 0, 0x24, -1, &arg0->elements[1].unk40, 0x10000, 0, 2, 0, 0);
        }

    cont:
        func_8002A2D0_2AED0(&elements[i]);
        gameState->unk408[i] = elements[i].matrix.translation.x;
        gameState->unk410[i] = elements[i].matrix.translation.z;
    }

    if (gameState->unk42A == 0x11) {
        gameState->unk42E = 1;
        arg0->unkCC[0] = 1;
        arg0->unkCC[1] = 1;
        func_8002EBB0_2F7B0(arg0);
        arg0->callback = func_8002D668_2E268;
        setCallback(func_8002BEF4_2CAF4);
    }
}

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002D814_2E414);

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002DA54_2E654);

void func_8002E024_2EC24(Func2E024Arg *arg0);

void func_8002DE44_2EA44(Func2E024Arg *container) {
    GameState *gameState;
    s32 i;
    Func297D8Arg *elements;

    gameState = getCurrentAllocation();
    elements = container->elements;

    for (i = 0; i < container->unkD5; i++) {
        Func297D8Arg *element = &elements[i];

        memcpy(&elements[i].matrix, identityMatrix, 0x20);
        element->unk62 = 0;

        if (i == 0) {
            container->elements[0].matrix.translation.x = 0xFFFC0000;
            container->elements[0].matrix.translation.z = 0xFFBC0000;
            setAnimationIndex(container->elements[0].model, 4);
            container->elements[0].unk50 = 0x11;
            container->elements[0].unk5E = 0x63;
            container->elements[0].unk44 = 0x1D0000;
            container->elements[0].unk40 = 0xFFFC0000;
            container->elements[0].unk48 = 0;
            spawnSpriteEffectEx(
                container->elements[0].model,
                0,
                0x2F,
                -1,
                &container->elements[0].unk40,
                0x10000,
                0,
                2,
                0,
                0
            );
        } else {
            container->elements[1].matrix.translation.x = 0x200000;
            container->elements[1].unk5E = 0x62;
            container->elements[1].unk44 = 0x260000;
            container->elements[1].matrix.translation.z = 0xFFBC0000;
            container->elements[1].unk50 = 0;
            container->elements[1].unk5A = 0;
            container->elements[1].unk40 = 0;
            container->elements[1].unk48 = 0;
            spawnSpriteEffectEx(
                container->elements[1].model,
                0,
                7,
                0x2D,
                &container->elements[1].unk40,
                0x10000,
                0,
                2,
                0,
                0
            );
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
    Func297D8Arg *elements;

    gameState = getCurrentAllocation();

    switch (arg0->elements[1].unk5E) {
        case 0x62:
            arg0->elements[1].unk5A++;
            if (arg0->elements[1].unk5A == 0x2D) {
                arg0->elements[1].unk50 = 0x10;
                arg0->elements[1].unk5A = 0;
                arg0->elements[1].unk5E = 1;
            }
            break;
        case 1:
            if (arg0->elements[1].unk62 != 0) {
                arg0->elements[1].unk50 = 0x11;
                arg0->elements[1].unk5E = 0x63;
            }
            break;
        case 0x63:
            arg0->elements[0].unk62 = 0;
            arg0->elements[1].unk62 = 0;
            break;
    }

    elements = arg0->elements;
    for (i = 0; i < arg0->unkD5; i++) {
        func_8002A2D0_2AED0(&elements[i]);
        gameState->unk408[i] = elements[i].matrix.translation.x;
        gameState->unk410[i] = elements[i].matrix.translation.z;
    }

    if (gameState->unk42A == 0x11) {
        func_8002EBB0_2F7B0(arg0);
        arg0->callback = func_8002E024_2EC24;
        setCallback(func_8002BEF4_2CAF4);
    }
}

void func_8002E314_2EF14(Func2E024Arg *);

void func_8002E170_2ED70(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 i;

    allocation = getCurrentAllocation();
    i = 0;
    arg0->unkD2 = 0;

    for (; i < arg0->unkD5; i++) {
        memcpy(&arg0->elements[i].matrix, identityMatrix, sizeof(Transform3D));
        arg0->elements[i].unk62 = 0;
        arg0->elements[i].unk5A = 0;

        if (i == 0) {
            arg0->elements[0].matrix.translation.x = -0x60000;
            arg0->elements[0].matrix.translation.z = -0x4E0000;
            arg0->elements[0].unk50 = 0;
            arg0->elements[0].unk5E = 3;
        } else {
            arg0->elements[1].matrix.translation.x = 0x1E0000;
            arg0->elements[1].matrix.translation.z = -0x4E0000;
            arg0->elements[1].unk50 = 0x13;
            arg0->elements[1].unk5E = 0;
        }

        arg0->elements[i].unk48 = 0;
        arg0->elements[i].unk40 = 0;

        if (i == 0) {
            arg0->elements[0].unk44 = 0x290000;
            arg0->elements[0].unk40 += -0x90000;
        } else {
            arg0->elements[i].unk40 = 0x60000;
            arg0->elements[i].unk44 = 0x290000;
        }

        arg0->elements[i].rotation = 0x800 + i * 0x1000;
        arg0->elements[i].unk2E = 0x800 + i * 0x1000;
        arg0->elements[i].unk52 = arg0->elements[i].unk50;
        createYRotationMatrix(&arg0->elements[i].matrix, arg0->elements[i].rotation);
        func_8002A290_2AE90(&arg0->elements[i]);
        allocation->unk408[i] = arg0->elements[i].matrix.translation.x;
        allocation->unk410[i] = arg0->elements[i].matrix.translation.z;
        allocation->unk418[i] = D_8008EF70_8FB70[arg0->elements[i].unk5C];
    }

    arg0->unkCC[0] = (randB() & 0x1F) + 0x28;
    arg0->unkCC[1] = 0;
    setCallback(func_8002E314_2EF14);
}

void func_8002E314_2EF14(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 i;
    Func297D8Arg *ptr;
    int new_var;
    allocation = getCurrentAllocation();
    for (i = 0; i < arg0->unkD5; i++) {
        ptr = &arg0->elements[i];
        switch (ptr->unk5E) {
            case 0:
                if (ptr->unk62 != 0) {
                    ptr->unk62 = 0;
                    ptr->unk50 = 0x14;
                    ptr->unk5E = 1;
                }
                break;

            case 1:
                ptr->unk5A++;
                if ((ptr->unk5A) >= 0x3C) {
                    if (ptr->unk62 != 0) {
                        ptr->unk5A = 0;
                        ptr->unk62 = 0;
                        ptr->unk50 = 0x15;
                        ptr->unk5E = 2;
                        if (arg0->unkD2 == 2) {
                            arg0->elements[0].unk5E = 7;
                            arg0->elements[0].unk50 = 0xA;
                        }
                    }
                }
                break;

            case 2:
                if (ptr->unk62 != 0) {
                    u8 d2;
                    ptr->unk62 = 0;
                    d2 = arg0->unkD2;
                    if (d2 < 2) {
                        arg0->elements[d2].unk5E = 4;
                        ptr->unk50 = 0;
                        ptr->unk5E = 3;
                    } else {
                        ptr->unk50 = 0x13;
                        ptr->unk5E = 5;
                    }
                    arg0->unkD2++;
                }
                break;

            case 4:
                ptr->unk5E = 0;
                ptr->unk5A = 0;
                ptr->unk50 = 0x13;
                break;

            case 5:
                if (ptr->unk62 != 0) {
                    ptr->unk62 = 0;
                    ptr->unk50 = 0x14;
                    ptr->unk5E = 6;
                }
                break;

            case 7:
                if (arg0->elements[0].unk62 != 0) {
                    arg0->elements[0].unk50++;
                    if ((arg0->elements[0].unk50) >= 0xD) {
                        arg0->elements[0].unk50 = 0;
                        arg0->elements[0].unk5E = 8;
                    }
                }
                break;

            case 8:
                if (arg0->elements[1].unk5E == 6) {
                    arg0->elements[0].unk50 = 0x20;
                    arg0->elements[0].unk5E = 9;
                }
                break;

            case 6:

            case 9:
                ptr->unk62 = 0;
                break;
        }

        func_8002A2D0_2AED0(ptr);
        allocation->unk408[i] = ptr->matrix.translation.x;
        allocation->unk410[i] = ptr->matrix.translation.z;
    }

    if (arg0->unkCC[0] != 0) {
        arg0->unkCC[0]--;
        if (!(arg0->unkCC[0] & 0xFFFF)) {
            unsigned char temp_v0_5 = randB() & 0x1F;
            temp_v0_5 &= 0xFF;
            new_var = 0x10000;
            arg0->unkCC[1] = temp_v0_5 + 0x1E;
            spawnSpriteEffectEx(
                arg0->elements[0].model,
                0,
                0x3E,
                temp_v0_5 + 0x1A,
                &arg0->elements[0].unk40,
                new_var,
                0,
                2,
                0,
                0x400
            );
            spawnSpriteEffectEx(
                arg0->elements[1].model,
                0,
                0x3E,
                (s16)(arg0->unkCC[1] - 4),
                &arg0->elements[1].unk40,
                new_var,
                0,
                2,
                0,
                0x1F00
            );
        }
    } else {
        u16 temp2 = arg0->unkCC[1];
        s16 temp_v0_7;
        if (temp2 != 0) {
            temp_v0_7 = temp2 - 1;
            arg0->unkCC[1] = temp_v0_7;
            if (!(temp_v0_7 & 0xFFFF)) {
                arg0->unkCC[0] = (randB() & 0x1F) + 0x2D;
            }
        }
    }
    do {
        if (allocation->unk42A == 0x11) {
            func_8002EBB0_2F7B0(arg0);
            allocation->unk42E = 1;
            arg0->unkCC[0] = (randB() & 0x1F) + 0x28;
            arg0->unkCC[1] = 0;
            arg0->callback = func_8002E314_2EF14;
            setCallback(func_8002BEF4_2CAF4);
        }
    } while (0);
}

INCLUDE_ASM("asm/nonmatchings/2DD40", func_8002E680_2F280);

void func_8002E8B4_2F4B4(Func2E024Arg *arg0) {
    GameState *allocation;
    s32 incrementedCount;
    s32 i;
    Func297D8Arg *ptr;
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
        ptr = &arg0->elements[i];

        memcpy(&sp30, identityMatrix, sizeof(Transform3D));
        memcpy(&sp50, &ptr->matrix, sizeof(Transform3D));

        switch (ptr->unk5E) {
            case 0:
                ptr->unk5A--;
                if ((ptr->unk5A << 16) == 0) {
                    ptr->unk5E = 1;
                }
                break;
            case 1:
                memcpy(sp20, &ptr->matrix.translation, 0xC);
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
                memcpy(&ptr->matrix.translation, sp10, 0xC);
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
            createYRotationMatrix(&sp30, ptr->rotation);
            tempAngle = atan2Fixed(ptr->matrix.translation.x, ptr->matrix.translation.z);
            createYRotationMatrix(&sp50, tempAngle & 0xFFFF);
            memcpy(&sp50.translation, &ptr->matrix.translation, 0xC);
            func_8006B084_6BC84(&sp30, &sp50, &ptr->matrix);
            func_8002A2D0_2AED0(ptr);
            allocation->unk408[i] = ptr->matrix.translation.x;
            allocation->unk410[i] = ptr->matrix.translation.z;
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

void func_8002EBB0_2F7B0(void *ptr) {
    Func2E024Arg *arg0 = ptr;
    s32 i;

    for (i = 0; i < 2; i++) {
        arg0->elements[i].unk56 = arg0->elements[i].unk50;
        arg0->elements[i].unk5F = getAnimationIndex(arg0->elements[i].model);

        if (((arg0->unkD4 == 1) & (i == 0)) && D_800AFE8C_A71FC->unk9[0] == 3) {
            setAnimationIndex(arg0->elements[i].model, -1);
            arg0->elements[i].unk48 = 0;
            arg0->elements[i].unk40 = 0;
            arg0->elements[i].unk44 = 0x240000;
        } else if (((arg0->unkD4 == 6) & (i == 1)) && D_800AFE8C_A71FC->unk9[0] == 3) {
            setAnimationIndex(arg0->elements[i].model, -1);
        } else {
            setAnimationIndex(arg0->elements[i].model, D_8008F054_8FC54[arg0->unkD4 * 2 + i]);
        }

        if (D_8008F044_8FC44[arg0->unkD4 * 2 + i] != 0) {
            arg0->elements[i].unk50 = 0;
            arg0->ctrl[i] = 1;
        } else {
            arg0->elements[i].unk30 = arg0->elements[i].rotation;
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