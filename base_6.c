#include "common.h"

typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ void *spriteSheet;
    /* 0x08 */ s16 spriteIndex;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 alpha;
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 unk13;
} SaveSlotGoldIcon;

typedef struct {
    /* 0x00 */ s16 width;
    /* 0x02 */ s16 height;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 alpha;
    /* 0x08 */ void *textBuffer;
} SaveSlotGoldText;

typedef struct {
    /* 0x00 */ u8 data[0xA];
} SaveSlotGoldTextBuffer;

typedef struct {
    /* 0x00 */ SaveSlotGoldIcon icons[4];
    /* 0x50 */ SaveSlotGoldText text[4];
    /* 0x80 */ SaveSlotGoldTextBuffer textBuffers[4];
    /* 0xA8 */ u8 animFrames[4];
} SaveSlotGoldDisplayState;

typedef struct {
    /* 0x00 */ u8 pad0[0x5C];
} SaveSlotData;

typedef struct {
    /* 0x000 */ u8 pad0[0x944];
    /* 0x944 */ s32 gold;
    /* 0x948 */ SaveSlotData slots[4];
    /* 0xAB8 */ u8 padAB8[0xC];
    /* 0xAC4 */ u16 unkAC4;
    /* 0xAC6 */ u16 unkAC6;
    /* 0xAC8 */ u8 unkAC8;
    /* 0xAC9 */ u8 padAC9[5];
    /* 0xACE */ u8 slotEnabled[4];
} GoldDisplayAllocation;

extern char D_8009E48C_9F08C[];
extern void func_800112AC_11EAC(void);
extern void renderTextColored(void);

void *getCurrentAllocation(void);
void debugEnqueueCallback(u16, u8, void *, void *);
int sprintf(char *, const char *, ...);

void updateSaveSlotGoldDisplay(SaveSlotGoldDisplayState *arg0) {
    GoldDisplayAllocation *allocation;
    s32 textOffset;
    u8 *slotDataPtr;
    s32 i;
    s32 priority;

    allocation = getCurrentAllocation();
    i = 0;
    textOffset = 0x50;
    slotDataPtr = (u8 *)allocation;
    priority = 9;

    do {
        if (allocation->slotEnabled[i] != 0) {
            if (allocation->unkAC6 >= 0x32) {
                arg0->text[i].width = 0x1D;
                arg0->text[i].alpha = 0x60;
                arg0->icons[i].spriteIndex = 0;
                arg0->icons[i].alpha = 0x60;
            } else if (i == allocation->unkAC8) {
                arg0->text[i].width = 0x18;
                arg0->text[i].alpha = 0xFF;
                arg0->icons[i].alpha = 0xFF;
                if (allocation->unkAC6 < 2) {
                    arg0->animFrames[i] = arg0->animFrames[i] + 1;
                    if ((arg0->animFrames[i] & 0xFF) == 2) {
                        arg0->animFrames[i] = 0;
                        arg0->icons[i].spriteIndex = arg0->icons[i].spriteIndex + 1;
                        if ((u32)(arg0->icons[i].spriteIndex & 0xFFFF) >= 6) {
                            arg0->icons[i].spriteIndex = 0;
                        }
                    }
                } else {
                    arg0->icons[i].spriteIndex = 0;
                    arg0->icons[i].unk13 = 0;
                    arg0->text[i].unk4 = 0;
                    if (allocation->unkAC6 == 2) {
                        if (allocation->unkAC4 & 1) {
                            arg0->icons[i].unk13 = 0xFF;
                            arg0->text[i].unk4 = 0xFF;
                        }
                    }
                }
            } else {
                if (i < 3) {
                    arg0->text[i].width = 0x1D;
                    arg0->text[i].alpha = 0x60;
                    arg0->icons[i].spriteIndex = 0;
                    arg0->icons[i].alpha = 0x60;
                } else {
                    arg0->text[i].width = 0x17;
                    arg0->text[i].alpha = 0xFF;
                    arg0->icons[i].spriteIndex = 0;
                    arg0->icons[i].alpha = 0xFF;
                }
                arg0->animFrames[i] = 0;
            }
            sprintf((char *)arg0 + 0x80 + i * 0xA, D_8009E48C_9F08C, *(s32 *)(slotDataPtr + 0x944));
            if (allocation->unkAC6 != 0x18 || allocation->unkAC8 != i) {
                debugEnqueueCallback((u16)priority, 7, &renderTextColored, (char *)arg0 + textOffset);
            }
            debugEnqueueCallback((u16)priority, 0, &func_800112AC_11EAC, &arg0->icons[i]);
        }
        priority++;
        textOffset += 0xC;
        slotDataPtr += 0x5C;
        i++;
    } while (i < 4);
}
