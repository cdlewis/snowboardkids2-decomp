#include "common.h"

extern u8 identityMatrix[];
extern u8 D_8008DA28_8E628[];
extern u16 D_8008DAA8_8E6A8[];
extern void renderLevelPreviewPortraits(void *);
extern void setCallback(void *);
extern void *getCurrentAllocation(void);
extern void getTableEntryByU16Index(void *, u16, void *);

typedef struct {
    u8 _pad0[0xB30];
    u8 toLevelId;
} Allocation_21308;

typedef struct {
    void *textureData;
    u8 matrix[0x14];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    void *data_ptr;
    void *index_ptr;
    u8 width;
    u8 height;
    u8 alpha;
    u8 _pad2F;
    u8 _pad30[4];
} MatrixSlot;

typedef struct {
    s16 posX;
    s16 posY;
    void *portraitAsset;
    s16 frameIndex;
    s16 _padA;
} PortraitEntry;

typedef struct {
    MatrixSlot slots[4];
    s16 unk_e8[4];
    s16 unk_f0[4];
    void *portraitAsset;
} PortraitStateBase;

typedef struct {
    void *data_ptr;
    void *index_ptr;
    u16 field1;
    u16 field2;
} TableOutput;

void func_80020708_21308(PortraitStateBase *arg0) {
    TableOutput sp30;
    Allocation_21308 *allocation;
    PortraitEntry *entryPtr;
    MatrixSlot *slot;
    s32 outerIdx;
    s32 innerIdx;
    s32 combinedIdx;
    u16 temp;
    s32 baseIdx;
    s16 posY;

    allocation = (Allocation_21308 *)getCurrentAllocation();

    outerIdx = 0;
    posY = -0x46;
    entryPtr = (PortraitEntry *)arg0;
    baseIdx = 0;

    do {
        innerIdx = 0;
        do {
            combinedIdx = baseIdx + innerIdx;
            slot = &arg0->slots[combinedIdx];

            memcpy(slot->matrix, identityMatrix, 0x20);

            arg0->unk_f0[combinedIdx] = (s16)(outerIdx << 12);

            slot->unk20 = 0xFF700000;
            slot->unk18 = 0;
            slot->unk1C = 0x3A0000;
            slot->textureData = &D_8008DA28_8E628[innerIdx * 64];

            temp = D_8008DAA8_8E6A8[allocation->toLevelId] + innerIdx;
            arg0->unk_e8[combinedIdx] = temp;

            getTableEntryByU16Index(arg0->portraitAsset, temp, &sp30);

            slot->data_ptr = sp30.data_ptr;
            slot->index_ptr = sp30.index_ptr;
            slot->width = (u8)sp30.field1;
            slot->alpha = 0xFF;
            slot->height = (u8)sp30.field2;

            innerIdx++;
        } while (innerIdx < 2);

        entryPtr->posX = -0x68;
        entryPtr->posY = posY;
        posY += 0x10;
        entryPtr->frameIndex = D_8008DAA8_8E6A8[allocation->toLevelId] + outerIdx;
        entryPtr->portraitAsset = arg0->portraitAsset;
        entryPtr++;
        baseIdx += 2;
        outerIdx++;
    } while (outerIdx < 2);

    setCallback(renderLevelPreviewPortraits);
}
