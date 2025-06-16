#include "69EF0.h"

#include "3A1F0.h"
#include "gamestate.h"

extern Node *gDMAOverlay;

typedef struct {
    struct D_800A32C0_A3EC0_type *unk0;
    struct D_800A32C0_A3EC0_type *unk4;
    struct D_800A32C0_A3EC0_type *unk8;
    struct D_800A32C0_A3EC0_type *unkC;
    void (*unk10)();
    void (*unk14)();
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    s32 unk1C;
    void *unk20;
    void *unk24;
    /* 0x28 */ GameState *GameState;
    s32 unk2C;
    /* 0x30 */ Node *activeList;
    /* 0x34 */ Node *freeList;
    /* 0x38 */ s16 counters[0x9];
    s16 unk4A;
    s16 unk4C;
} gTaskScheduler_type;
extern gTaskScheduler_type *gTaskScheduler;

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
extern D_800A32C0_A3EC0_type *D_800A3274_A3E74;
extern s32 D_800AB064_A23D4;
extern s32 D_800AB12C_A249C;
s32 func_80069D20_6A920();
void func_80069B04_6A704(void);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800692F0_69EF0);

void func_800693C4_69FC4(s32 arg0, s8 arg1) {
    D_800A32C0_A3EC0_type *temp_a2;
    D_800A32C0_A3EC0_type *temp_v1;
    D_800A32C0_A3EC0_type *var_a3;
    temp_a2 = D_800A32C0_A3EC0;
    var_a3 = &D_800A3270_A3E70;
    D_800A32C0_A3EC0 = temp_a2->unk8;

    if (D_800A3274_A3E74 != 0) {
        while (TRUE) {
            temp_v1 = var_a3->unk4;
            if ((u8)arg1 >= (u8)temp_v1->unk19) {
                var_a3 = temp_v1;
                if (temp_v1->unk4 != 0) {
                    continue;
                }
            }

            break;
        }
    }

    temp_a2->unk0 = var_a3;
    temp_a2->unk4 = var_a3->unk4;
    var_a3->unk4 = temp_a2;

    if (temp_a2->unk4 != 0) {
        temp_a2->unk4->unk0 = temp_a2;
    }

    temp_a2->unk1A = 0;
    temp_a2->unk10 = arg0;
    temp_a2->unk18 = 0;
    temp_a2->unk19 = arg1;
    temp_a2->unk28 = 0;
    temp_a2->unk30 = 0;
    temp_a2->unk34 = 0;
    temp_a2->unk2C = 0;
    temp_a2->unk48 = 0;
    temp_a2->unk1C = -1;
    temp_a2->unk20 = 0;
    temp_a2->unkC = 0;
    temp_a2->unk4C = 0;
    temp_a2->unk4A = 0;
}

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
    temp_a2->unkC = (void *)gTaskScheduler;
    gTaskScheduler->unk4C++;
}

void func_80069530_6A130(void) {
    D_800A32C0_A3EC0_type *temp_v0;
    D_800A32C0_A3EC0_type *temp_v0_2;
    D_800A32C0_A3EC0_type *temp_a0;

    gTaskScheduler = D_800A3274_A3E74;
    if (D_800A3274_A3E74 != NULL) {
        do {
            if (gTaskScheduler->unk18 == 0) {
                gTaskScheduler->unk18 = 1;
            }

            gTaskScheduler = gTaskScheduler->unk4;
        } while (gTaskScheduler != NULL);
    }

    gTaskScheduler = D_800A3274_A3E74;
    if (D_800A3274_A3E74 != NULL) {
        do {
            switch (gTaskScheduler->unk18) {
                case 0:
                    break;

                case 1:
                loop_9:
                    gTaskScheduler->unk1B = 0;

                    gTaskScheduler->unk10();

                    if (gTaskScheduler->unk1B != 0) {
                        if (gTaskScheduler->unk18 == 1) {
                            goto loop_9;
                        }
                    }

                    if (gTaskScheduler->unk18 == 1) {
                        func_80069B04_6A704();
                    }

                    break;

                case 2:
                    if (gTaskScheduler->unk4C == 0) {
                        func_80069D34_6A934();
                        func_80069B04_6A704();

                        if (func_80069D20_6A920() == 0) {
                            gTaskScheduler->unk2C = decrementNodeRefCount((s32 *)gTaskScheduler->unk2C);
                            gTaskScheduler->GameState = (GameState *)decrementNodeRefCount((s32 *)gTaskScheduler->GameState);
                            gTaskScheduler->unk1C = D_800AB064_A23D4;
                            gTaskScheduler->unk18 = 4;
                        }
                    }
                    break;

                case 4:
                    if (gTaskScheduler->unk1C > 0) {
                        if (((D_800AB12C_A249C - gTaskScheduler->unk1C) & 0x0FFFFFFF) <= 0x07FFFFFF) {
                            gTaskScheduler->unk1C = -1;
                        }
                        if (gTaskScheduler->unk1C <= 0) {
                            goto block_20;
                        }
                    } else {
                    block_20:
                        if (gTaskScheduler->unk14 != NULL) {
                            gTaskScheduler->unk14();
                        }

                        if (gTaskScheduler->unkC != 0) {
                            gTaskScheduler->unkC->unk4C--;
                        }

                        if (gTaskScheduler->unk4 != NULL) {
                            gTaskScheduler->unk4->unk0 = gTaskScheduler->unk0;
                        }

                        gTaskScheduler->unk0->unk4 = gTaskScheduler->unk4;
                        temp_a0 = D_800A32C0_A3EC0;
                        D_800A32C0_A3EC0 = gTaskScheduler;
                        gTaskScheduler->unk8 = temp_a0;
                    }
                    break;

                case 3:
                    if (getNodeOwner(gTaskScheduler->unk24) == 0) {
                        gTaskScheduler->unk18 = 1;
                    }
                    break;
            }

            gTaskScheduler = gTaskScheduler->unk4;
        } while (gTaskScheduler != NULL);
    }
}

void func_800697CC_6A3CC(void (*arg0)()) {
    if (gTaskScheduler != NULL) {
        gTaskScheduler->unk18 = 2;
        gTaskScheduler->unk14 = arg0;
    }
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800697F4_6A3F4);

s16 func_80069810_6A410(void) {
    s16 temp_v0;

    temp_v0 = gTaskScheduler->unk4A;
    gTaskScheduler->unk4A = 0;

    return temp_v0;
}

void setGameStateHandler(void *arg0) {
    gTaskScheduler->unk10 = arg0;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_8006983C_6A43C);

GameState *func_80069854_6A454(s32 arg0) {
    u8 *node_exists;
    u32 temp_a0;
    GameState *temp_v0;
    s8 *var_v1;

    temp_v0 = allocateMemoryNode(0, arg0, &node_exists);
    gTaskScheduler->GameState = temp_v0;
    if (arg0 != 0) {
        var_v1 = (s8 *)temp_v0;
        temp_a0 = ((s32)arg0 + (s32)var_v1);
        do {
            *var_v1 = 0;
            var_v1 += 1;
        } while ((u32)var_v1 < (u32)temp_a0);
    }
    return gTaskScheduler->GameState;
}

GameState *GameStateGet() {
    return gTaskScheduler->GameState;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800698CC_6A4CC);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800698DC_6A4DC);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_800698EC_6A4EC);

Node *scheduleTask(void *callback, u8 nodeType, u8 identifierFlag, u8 priority) {
    Node *newNode;
    Node *active;
    Node *freeNxt;
    gTaskScheduler_type *g = gTaskScheduler;
    s16 *countPtr;
    Node *temp;

    if (gTaskScheduler) {
        if (g->counters[nodeType]) {
            g->counters[nodeType]--;

            newNode = gTaskScheduler->freeList;
            active = gTaskScheduler->activeList;
            freeNxt = newNode->freeNext;
            gTaskScheduler->freeList = freeNxt;

            if (active == NULL) {
                newNode->next = NULL;
                newNode->prev = NULL;
                gTaskScheduler->activeList = newNode;
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

            newNode->unkC = nodeType;
            newNode->field_D = identifierFlag;
            newNode->priority = priority;
            newNode->callback = callback;
            newNode->unk24 = 0;
            newNode->unkE = 1;
            newNode->unk11 = 0;
            newNode->unk18 = 0;

            // This should just be newNode++. It's possible that
            // nodes are possbibly being embedded in larger
            // structs.
            return ((u8 *)newNode + 0x28);
        }
    }

    return NULL;
}

s16 func_80069AEC_6A6EC(s32 arg0) {
    return gTaskScheduler->counters[arg0];
}

void func_80069B04_6A704() {
    for (gDMAOverlay = gTaskScheduler->activeList; gDMAOverlay != 0; gDMAOverlay = gDMAOverlay->next) {
        switch (gDMAOverlay->unkE) {
            case 0:
                break;
            case 1:
                while (TRUE) {
                    gDMAOverlay->unk10 = 0;
                    gDMAOverlay->callback(&gDMAOverlay->payload);
                    if ((gDMAOverlay->unk10 != 0) && (gDMAOverlay->unkE == 1)) {
                        continue;
                    }
                    break;
                }
                break;

            case 2:
                if (gDMAOverlay->unk24 != NULL) {
                    gDMAOverlay->unk24(&gDMAOverlay->payload);
                }

                if (gDMAOverlay->prev == NULL) {
                    gTaskScheduler->activeList = gDMAOverlay->next;
                } else {
                    gDMAOverlay->prev->next = gDMAOverlay->next;
                }

                if (gDMAOverlay->next != NULL) {
                    gDMAOverlay->next->prev = gDMAOverlay->prev;
                }
                gDMAOverlay->freeNext = gTaskScheduler->freeList;
                gTaskScheduler->freeList = gDMAOverlay;
                gTaskScheduler->counters[(u8)gDMAOverlay->unkC]++;
                break;

            case 3:
                if (getNodeOwner(gDMAOverlay->unk1C) == 0) {
                    gDMAOverlay->unkE = 1;
                }
                break;

            case 4:
                if (getNodeOwner(gDMAOverlay->unk1C) == 0) {
                    gDMAOverlay->unkE = 2;
                }
                break;
        }
    }
}

void func_80069CC0_6A8C0(void *arg0) {
    gDMAOverlay->callback = arg0;
}

void func_80069CD0_6A8D0(void *arg0) {
    gDMAOverlay->callback = arg0;
    gDMAOverlay->unk10 = 1;
}

void func_80069CE8_6A8E8(void(arg0)(void *)) {
    gDMAOverlay->unk24 = arg0;
}

void func_80069CF8_6A8F8(void) {
    u8 temp = gDMAOverlay->unkE - 3;
    if (temp < 2) {
        gDMAOverlay->unkE = 4;
    } else {
        gDMAOverlay->unkE = 2;
    }
}

s32 func_80069D20_6A920() {
    return gTaskScheduler->activeList != NULL;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069D34_6A934);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069D7C_6A97C);

INCLUDE_ASM("asm/nonmatchings/69EF0", func_80069DD4_6A9D4);

MemoryAllocatorNode *dmaRequestAndUpdateState(void *start, void *end) {
    MemoryAllocatorNode *s0;

    if (gDMAOverlay == NULL) {
        s0 = queueDmaTransfer(start, end);
        if (getNodeOwner(s0) != 0) {
            if (gTaskScheduler->unk18 != 3) {
                gTaskScheduler->unk20 = getNodeUserData(s0);
                gTaskScheduler->unk24 = s0;
                gTaskScheduler->unk18 = 3;
            } else if (gTaskScheduler->unk20 < getNodeUserData(s0)) {
                gTaskScheduler->unk20 = getNodeUserData(s0);
                gTaskScheduler->unk24 = s0;
            }
        }
    } else {
        s0 = queueDmaTransfer(start, end);
        if (getNodeOwner(s0) != 0) {
            if (((u8)(gDMAOverlay->unkE - 3)) >= 2) {
                gDMAOverlay->unk18 = getNodeUserData(s0);
                gDMAOverlay->unk1C = s0;
                gDMAOverlay->unkE = 3;
            } else {
                if (gDMAOverlay->unk18 < getNodeUserData(s0)) {
                    gDMAOverlay->unk18 = getNodeUserData(s0);
                    gDMAOverlay->unk1C = s0;
                }
            }
        }
        gDMAOverlay->unk11 = 1;
    }

    return s0;
}

MemoryAllocatorNode *dmaRequestAndUpdateStateWithSize(void *romStart, void *romEnd, s32 size) {
    MemoryAllocatorNode *s0;

    if (gDMAOverlay == NULL) {
        s0 = dmaQueueRequest(romStart, romEnd, size);
        if (getNodeOwner(s0) != NULL) {
            if (gTaskScheduler->unk18 != 3) {
                gTaskScheduler->unk20 = getNodeUserData(s0);
                gTaskScheduler->unk24 = s0;
                gTaskScheduler->unk18 = 3;
            } else {
                if (gTaskScheduler->unk20 < getNodeUserData(s0)) {
                    gTaskScheduler->unk20 = getNodeUserData(s0);
                    gTaskScheduler->unk24 = s0;
                }
            }
        }
    } else {
        s0 = dmaQueueRequest(romStart, romEnd, size);
        if (getNodeOwner(s0) != NULL) {
            if (gDMAOverlay->unkE < 3 || gDMAOverlay->unkE >= 5) {
                gDMAOverlay->unk18 = getNodeUserData(s0);
                gDMAOverlay->unk1C = s0;
                gDMAOverlay->unkE = 3;
            } else {
                if (gDMAOverlay->unk18 < getNodeUserData(s0)) {
                    gDMAOverlay->unk18 = getNodeUserData(s0);
                    gDMAOverlay->unk1C = s0;
                }
            }
        }

        gDMAOverlay->unk11 = 1;
    }

    return s0;
}

INCLUDE_ASM("asm/nonmatchings/69EF0", func_8006A0EC_6ACEC);

GameState *allocateGameStateMemory(s32 arg0) {
    u8 *exists;
    GameState *temp_a0;

    temp_a0 = allocateMemoryNode(0, arg0, &exists);
    if (gDMAOverlay != NULL) {
        gDMAOverlay->unk11 = 1;
    }
    return temp_a0;
}

s32 freeGameStateMemory(s32 *arg0) {
    return decrementNodeRefCount(arg0);
}

static const u32 D_8009EA54_padding[3] = {0, 0, 0};
