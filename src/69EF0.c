#include "69EF0.h"

#include "3A1F0.h"
#include "gamestate.h"

GameState *func_8006A258_6AE58(s32, s32, void *);

typedef struct {
    u8 padding[0xE];
    u8 unk0E;
    u8 padding2[0x1];
    u8 unk10;
    u8 unk11;
    u8 padding3[0x4];
    void *unk18;
    void *unk1C;
    void *unk20;
    s32 unk24;
} D_8009A864_9B464_type;
extern D_8009A864_9B464_type *D_8009A864_9B464;

typedef struct Node {
    /* 0x00 */ struct Node *prev;
    /* 0x04 */ struct Node *next;
    /* 0x08 */ struct Node *freeNext;
    /* 0x0C */ u8 field_C;
    /* 0x0D */ u8 field_D;
    /* 0x0E */ u8 field_E;
    /* 0x0F */ u8 priority;
    /* 0x10 */ u8 pad10;
    /* 0x11 */ u8 field_11;
    /* 0x12 */ u8 pad12[6];
    /* 0x18 */ u32 field_18;
    /* 0x1C */ u32 pad1C;
    /* 0x20 */ void *field_20;
    /* 0x24 */ u32 field_24;
} Node;

typedef struct {
    char padding[0x10];
    void *unk10;
    s32 unk14;
    u8 unk18;
    char padding5[0x4];
    void *unk20;
    void *unk24;
    /* 0x28 */ GameState *GameState;
    char padding3[0x2];
    /* 0x30 */ Node *activeList;
    /* 0x34 */ Node *freeList;
    /* 0x38 */ s16 counters[0x9];
    s16 unk4A;
} D_8009A860_9B460_type;
extern D_8009A860_9B460_type *D_8009A860_9B460;

extern void *func_8003B8F0_3C4F0(void);
extern s32 func_8006A51C_6B11C(void *);
extern void *func_8006A52C_6B12C(void *);
extern void func_8006A3FC_6AFFC();

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800692F0_69EF0);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800693C4_69FC4);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069470_6A070);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069530_6A130);

void func_800697CC_6A3CC(s32 arg0) {
    if (D_8009A860_9B460 != NULL) {
        D_8009A860_9B460->unk18 = 2;
        D_8009A860_9B460->unk14 = arg0;
    }
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800697F4_6A3F4);

s16 func_80069810_6A410(void) {
    s16 temp_v0;

    temp_v0 = D_8009A860_9B460->unk4A;
    D_8009A860_9B460->unk4A = 0;

    return temp_v0;
}

void func_8006982C_6A42C(void *arg0) {
    D_8009A860_9B460->unk10 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_8006983C_6A43C);

GameState *func_80069854_6A454(s32 arg0) {
    s32 sp10;
    u32 temp_a0;
    GameState *temp_v0;
    s8 *var_v1;

    temp_v0 = func_8006A258_6AE58(0, arg0, &sp10);
    D_8009A860_9B460->GameState = temp_v0;
    if (arg0 != 0) {
        var_v1 = (s8 *)temp_v0;
        temp_a0 = ((s32)arg0 + (s32)var_v1);
        do {
            *var_v1 = 0;
            var_v1 += 1;
        } while ((u32)var_v1 < (u32)temp_a0);
    }
    return D_8009A860_9B460->GameState;
}

GameState *GameStateGet() {
    return D_8009A860_9B460->GameState;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800698CC_6A4CC);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800698DC_6A4DC);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800698EC_6A4EC);

void *func_800699F4_6A5F4(void *a0, u8 a1, u8 a2, u8 a3) {
    Node *newNode;
    Node *active;
    Node *freeNxt;
    D_8009A860_9B460_type *g = D_8009A860_9B460;
    s16 *countPtr;
    Node *temp;

    if (D_8009A860_9B460) {
        if (g->counters[a1]) {
            g->counters[a1]--;

            newNode = D_8009A860_9B460->freeList;
            active = D_8009A860_9B460->activeList;
            freeNxt = newNode->freeNext;
            D_8009A860_9B460->freeList = freeNxt;

            if (active == NULL) {
                newNode->next = NULL;
                newNode->prev = NULL;
                D_8009A860_9B460->activeList = newNode;
            } else if (a3 < active->priority) {
                newNode->next = active;
                newNode->prev = NULL;
                temp = newNode->next;
                temp->prev = newNode;
                g->activeList = newNode;
            } else {
                Node *it = active;
                while (it->next != NULL) {
                    if (a3 < it->next->priority) {
                        break;
                    }
                    it = it->next;
                }

                newNode->next = it->next;
                newNode->prev = it;
                it->next = newNode;
                if (newNode->next) {
                    newNode->next->prev = newNode;
                }
            }

            newNode->field_C = a1;
            newNode->field_D = a2;
            newNode->priority = a3;
            newNode->field_20 = a0;
            newNode->field_24 = 0;
            newNode->field_E = 1;
            newNode->field_11 = 0;
            newNode->field_18 = 0;

            return (void *)((s32)newNode + 0x28);
        }
    }

    return 0;
}

s16 func_80069AEC_6A6EC(s32 arg0) {
    return D_8009A860_9B460->counters[arg0];
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069B04_6A704);

void func_80069CC0_6A8C0(void *arg0) {
    D_8009A864_9B464->unk20 = arg0;
}

void func_80069CD0_6A8D0(void *arg0) {
    D_8009A864_9B464->unk20 = arg0;
    D_8009A864_9B464->unk10 = 1;
}

void func_80069CE8_6A8E8(s32 arg0) {
    D_8009A864_9B464->unk24 = arg0;
}

void func_80069CF8_6A8F8(void) {
    u8 temp = D_8009A864_9B464->unk0E - 3;
    if (temp < 2) {
        D_8009A864_9B464->unk0E = 4;
    } else {
        D_8009A864_9B464->unk0E = 2;
    }
}

s32 func_80069D20_6A920() {
    return D_8009A860_9B460->activeList != NULL;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069D34_6A934);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069D7C_6A97C);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069DD4_6A9D4);

void *func_80069E3C_6AA3C(void) {
    void *s0;

    if (D_8009A864_9B464 == 0) {
        s0 = func_8003B8F0_3C4F0();
        if (func_8006A51C_6B11C(s0) != 0) {
            if (D_8009A860_9B460->unk18 != 3) {
                D_8009A860_9B460->unk20 = func_8006A52C_6B12C(s0);
                D_8009A860_9B460->unk24 = s0;
                D_8009A860_9B460->unk18 = 3;
            } else if (D_8009A860_9B460->unk20 < func_8006A52C_6B12C(s0)) {
                D_8009A860_9B460->unk20 = func_8006A52C_6B12C(s0);
                D_8009A860_9B460->unk24 = s0;
            }
        }
    } else {
        s0 = func_8003B8F0_3C4F0();
        if (func_8006A51C_6B11C(s0) != 0) {
            if ((((u8)(D_8009A864_9B464->unk0E - 3)) >= 2)) {
                D_8009A864_9B464->unk18 = func_8006A52C_6B12C(s0);
                D_8009A864_9B464->unk1C = s0;
                D_8009A864_9B464->unk0E = 3;
            } else {
                if (D_8009A864_9B464->unk18 < func_8006A52C_6B12C(s0)) {
                    D_8009A864_9B464->unk18 = func_8006A52C_6B12C(s0);
                    D_8009A864_9B464->unk1C = s0;
                }
            }
        }
        D_8009A864_9B464->unk11 = 1;
    }

    return s0;
}

void *dmaRequestAndUpdateState(void *arg0, void *arg1, int arg2) {
    void *s0;

    if (D_8009A864_9B464 == NULL) {
        s0 = dmaQueueRequest(arg0, arg1, arg2);
        if (func_8006A51C_6B11C(s0) != NULL) {
            if (D_8009A860_9B460->unk18 != 3) {
                D_8009A860_9B460->unk20 = func_8006A52C_6B12C(s0);
                D_8009A860_9B460->unk24 = s0;
                D_8009A860_9B460->unk18 = 3;
            } else {
                if (D_8009A860_9B460->unk20 < func_8006A52C_6B12C(s0)) {
                    D_8009A860_9B460->unk20 = func_8006A52C_6B12C(s0);
                    D_8009A860_9B460->unk24 = s0;
                }
            }
        }
    } else {
        s0 = dmaQueueRequest(arg0, arg1, arg2);
        if (func_8006A51C_6B11C(s0) != 0) {
            if (D_8009A864_9B464->unk0E < 3 || D_8009A864_9B464->unk0E >= 5) {
                D_8009A864_9B464->unk18 = func_8006A52C_6B12C(s0);
                D_8009A864_9B464->unk1C = s0;
                D_8009A864_9B464->unk0E = 3;
            } else {
                if (D_8009A864_9B464->unk18 < func_8006A52C_6B12C(s0)) {
                    D_8009A864_9B464->unk18 = func_8006A52C_6B12C(s0);
                    D_8009A864_9B464->unk1C = s0;
                }
            }
        }

        D_8009A864_9B464->unk11 = 1;
    }

    return s0;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_8006A0EC_6ACEC);

GameState *func_8006A1C0_6ADC0(s32 arg0) {
    s32 sp10;
    GameState *temp_a0;

    temp_a0 = func_8006A258_6AE58(0, arg0, &sp10);
    if (D_8009A864_9B464 != NULL) {
        D_8009A864_9B464->unk11 = 1;
    }
    return temp_a0;
}

void func_8006A200_6AE00(void) {
    func_8006A3FC_6AFFC();
}
