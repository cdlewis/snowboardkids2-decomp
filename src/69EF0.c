#include "69EF0.h"

#include "3A1F0.h"
#include "gamestate.h"

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
    s16 unk4C;
} D_8009A860_9B460_type;
extern D_8009A860_9B460_type *D_8009A860_9B460;

extern s32 D_800A3274_A3E74;

typedef struct D_800A32C0_A3EC0_type {
    struct D_800A32C0_A3EC0_type *unk0;
    struct D_800A32C0_A3EC0_type *unk4;
    struct D_800A32C0_A3EC0_type *unk8;
    void *unkC;
    s32 unk10;
    u8 pad14[4];
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 pad1B;
    s32 unk1C;
    s32 unk20;
    u8 pad24[4];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 pad38[0x10];
    s16 unk48;
    u16 unk4A;
    s16 unk4C;
} D_800A32C0_A3EC0_type;
extern D_800A32C0_A3EC0_type *D_800A3270_A3E70;
extern D_800A32C0_A3EC0_type *D_800A32C0_A3EC0;

extern void *queueDmaTransfer(void *, void *);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800692F0_69EF0);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800693C4_69FC4);

void func_80069470_6A070(u32 arg0, s8 arg1) {
    D_800A32C0_A3EC0_type *temp_a2;
    D_800A32C0_A3EC0_type *var_a3;
    D_800A32C0_A3EC0_type *temp_v1;
    D_800A32C0_A3EC0_type *temp_v0;

    temp_a2 = D_800A32C0_A3EC0;
    var_a3 = &D_800A3270_A3E70;
    D_800A32C0_A3EC0 = temp_a2->unk8;

    if (D_800A3274_A3E74 != 0) {
        while (TRUE) {
            temp_v1 = (D_800A32C0_A3EC0_type *)var_a3->unk4;
            if ((u8)arg1 < (u8)temp_v1->unk19) {
                break;
            }
            var_a3 = temp_v1;
            if (temp_v1->unk4 == 0) {
                break;
            }
        }
    }

    temp_a2->unk0 = var_a3;
    temp_a2->unk4 = var_a3->unk4;
    var_a3->unk4 = temp_a2;
    temp_v0 = temp_a2->unk4;

    if (temp_v0 != 0) {
        temp_v0->unk0 = temp_a2;
    }

    temp_a2->unk1A = 0;
    temp_a2->unk18 = 0;
    temp_a2->unk19 = arg1;
    temp_a2->unk10 = arg0;
    temp_a2->unk28 = 0;
    temp_a2->unk30 = 0;
    temp_a2->unk34 = 0;
    temp_a2->unk2C = 0;
    temp_a2->unk48 = 0;
    temp_a2->unk1C = -1;
    temp_a2->unk20 = 0;
    temp_a2->unk4C = 0;
    temp_a2->unk4A = 0;
    temp_a2->unkC = (void *)D_8009A860_9B460;
    D_8009A860_9B460->unk4C++;
}

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

void setGameStateHandler(void *arg0) {
    D_8009A860_9B460->unk10 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_8006983C_6A43C);

GameState *func_80069854_6A454(s32 arg0) {
    u8 *node_exists;
    u32 temp_a0;
    GameState *temp_v0;
    s8 *var_v1;

    temp_v0 = allocateMemoryNode(0, arg0, &node_exists);
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

Node *scheduleTask(void *callback, u8 nodeType, u8 identifierFlag, u8 priority) {
    Node *newNode;
    Node *active;
    Node *freeNxt;
    D_8009A860_9B460_type *g = D_8009A860_9B460;
    s16 *countPtr;
    Node *temp;

    if (D_8009A860_9B460) {
        if (g->counters[nodeType]) {
            g->counters[nodeType]--;

            newNode = D_8009A860_9B460->freeList;
            active = D_8009A860_9B460->activeList;
            freeNxt = newNode->freeNext;
            D_8009A860_9B460->freeList = freeNxt;

            if (active == NULL) {
                newNode->next = NULL;
                newNode->prev = NULL;
                D_8009A860_9B460->activeList = newNode;
            } else if (priority < active->priority) {
                newNode->next = active;
                newNode->prev = NULL;
                temp = newNode->next;
                temp->prev = newNode;
                g->activeList = newNode;
            } else {
                Node *it = active;
                while (it->next != NULL) {
                    if (priority < it->next->priority) {
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

            newNode->field_C = nodeType;
            newNode->field_D = identifierFlag;
            newNode->priority = priority;
            newNode->field_20 = callback;
            newNode->field_24 = 0;
            newNode->field_E = 1;
            newNode->field_11 = 0;
            newNode->field_18 = 0;

            return (newNode + 0x1);
        }
    }

    return NULL;
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

MemoryAllocatorNode *dmaRequestAndUpdateState(void *start, void *end) {
    MemoryAllocatorNode *s0;

    if (D_8009A864_9B464 == NULL) {
        s0 = queueDmaTransfer(start, end);
        if (getNodeOwner(s0) != 0) {
            if (D_8009A860_9B460->unk18 != 3) {
                D_8009A860_9B460->unk20 = getNodeUserData(s0);
                D_8009A860_9B460->unk24 = s0;
                D_8009A860_9B460->unk18 = 3;
            } else if (D_8009A860_9B460->unk20 < getNodeUserData(s0)) {
                D_8009A860_9B460->unk20 = getNodeUserData(s0);
                D_8009A860_9B460->unk24 = s0;
            }
        }
    } else {
        s0 = queueDmaTransfer(start, end);
        if (getNodeOwner(s0) != 0) {
            if (((u8)(D_8009A864_9B464->unk0E - 3)) >= 2) {
                D_8009A864_9B464->unk18 = getNodeUserData(s0);
                D_8009A864_9B464->unk1C = s0;
                D_8009A864_9B464->unk0E = 3;
            } else {
                if (D_8009A864_9B464->unk18 < getNodeUserData(s0)) {
                    D_8009A864_9B464->unk18 = getNodeUserData(s0);
                    D_8009A864_9B464->unk1C = s0;
                }
            }
        }
        D_8009A864_9B464->unk11 = 1;
    }

    return s0;
}

MemoryAllocatorNode *dmaRequestAndUpdateStateWithSize(void *romStart, void *romEnd, s32 size) {
    MemoryAllocatorNode *s0;

    if (D_8009A864_9B464 == NULL) {
        s0 = dmaQueueRequest(romStart, romEnd, size);
        if (getNodeOwner(s0) != NULL) {
            if (D_8009A860_9B460->unk18 != 3) {
                D_8009A860_9B460->unk20 = getNodeUserData(s0);
                D_8009A860_9B460->unk24 = s0;
                D_8009A860_9B460->unk18 = 3;
            } else {
                if (D_8009A860_9B460->unk20 < getNodeUserData(s0)) {
                    D_8009A860_9B460->unk20 = getNodeUserData(s0);
                    D_8009A860_9B460->unk24 = s0;
                }
            }
        }
    } else {
        s0 = dmaQueueRequest(romStart, romEnd, size);
        if (getNodeOwner(s0) != NULL) {
            if (D_8009A864_9B464->unk0E < 3 || D_8009A864_9B464->unk0E >= 5) {
                D_8009A864_9B464->unk18 = getNodeUserData(s0);
                D_8009A864_9B464->unk1C = s0;
                D_8009A864_9B464->unk0E = 3;
            } else {
                if (D_8009A864_9B464->unk18 < getNodeUserData(s0)) {
                    D_8009A864_9B464->unk18 = getNodeUserData(s0);
                    D_8009A864_9B464->unk1C = s0;
                }
            }
        }

        D_8009A864_9B464->unk11 = 1;
    }

    return s0;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_8006A0EC_6ACEC);

GameState *allocateGameStateMemory(s32 arg0) {
    u8 *exists;
    GameState *temp_a0;

    temp_a0 = allocateMemoryNode(0, arg0, &exists);
    if (D_8009A864_9B464 != NULL) {
        D_8009A864_9B464->unk11 = 1;
    }
    return temp_a0;
}

s32 freeGameStateMemory(s32 *arg0) {
    return decrementNodeRefCount(arg0);
}
