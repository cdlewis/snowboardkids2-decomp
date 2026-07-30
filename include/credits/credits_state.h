#pragma once

#include "animation/easing_state.h"
#include "credits/offset_table.h"
#include "data/data_table.h"
#include "graphics/graphics.h"
#include "graphics/palette.h"
#include "graphics/sprite_rdp.h"

typedef union {
    s16 value;
    struct {
        u8 high;
        u8 low;
    } bytes;
} CreditsHalfword;

typedef union {
    s32 value;
    struct {
        s16 high;
        s16 low;
    } halves;
} CreditsWord;

typedef struct {
    s16 x;
    s16 y;
    void *dataTable;
    s16 index;
    s16 width;
    s16 height;
    s16 rotation;
    s16 alpha;
    u8 shade;
    u8 flags;
    u8 opacity;
    u8 pad15[3];
} CreditsTextEntry;

typedef struct CreditsState {
    /* 0x000 */ s8 initialized;
    /* 0x001 */ u8 pad001;
    /* 0x002 */ s16 frameCounter;
    /* 0x004 */ s32 reserved004;
    /* 0x008 */ ViewportNode backgroundViewport;
    /* 0x1E0 */ ViewportNode textViewport;
    /* 0x3B8 */ ViewportNode subtitleShadowViewport;
    /* 0x590 */ ViewportNode subtitleViewport;
    /* 0x768 */ ViewportNode characterViewport;
    /* 0x940 */ s16 sceneOriginX;
    /* 0x942 */ s16 sceneOriginY;
    /* 0x944 */ s16 reserved944;
    /* 0x946 */ u8 pad946[2];
    /* 0x948 */ void *scratchBuffer;
    /* 0x94C */ s8 scratchBufferStorage;
    /* 0x94D */ u8 pad94D[0xF];
    /* 0x95C */ OffsetTable *subtitleSchedule;
    /* 0x960 */ void *textRenderAsset;
    /* 0x964 */ Table_B934 *subtitleTextTable;
    /* 0x968 */ Table_B934 *creditsTextTable;
    /* 0x96C */ s16 subtitleShadowX;
    /* 0x96E */ s16 subtitleShadowY;
    /* 0x970 */ u16 *subtitleShadowText;
    /* 0x974 */ void *subtitleShadowAsset;
    /* 0x978 */ CreditsHalfword subtitleShadowStyle;
    /* 0x97A */ CreditsHalfword subtitleShadowAlpha;
    /* 0x97C */ u8 subtitleShadowTileMode;
    /* 0x97D */ u8 pad97D[3];
    /* 0x980 */ s16 subtitleX;
    /* 0x982 */ s16 subtitleY;
    /* 0x984 */ u16 *subtitleText;
    /* 0x988 */ void *subtitleRenderAsset;
    /* 0x98C */ CreditsHalfword subtitleStyle;
    /* 0x98E */ CreditsHalfword subtitleAlpha;
    /* 0x990 */ u8 subtitleTileMode;
    /* 0x991 */ u8 pad991[3];
    /* 0x994 */ s32 subtitleAlphaFixed;
    /* 0x998 */ s32 subtitleScrollStep;
    /* 0x99C */ CreditsWord subtitleScrollPosition;
    /* 0x9A0 */ s16 subtitleIndex;
    /* 0x9A2 */ u8 pad9A2[2];
    /* 0x9A4 */ s16 subtitleFrameCounter;
    /* 0x9A6 */ s16 subtitleScheduleIndex;
    /* 0x9A8 */ u16 subtitleTextWidth;
    /* 0x9AA */ u8 pad9AA[2];
    /* 0x9AC */ s16 nextSubtitleFrame;
    /* 0x9AE */ s16 subtitleDelay;
    /* 0x9B0 */ s16 subtitleCommandIndex;
    /* 0x9B2 */ s16 subtitleLineIndex;
    /* 0x9B4 */ s16 subtitleInitialDelay;
    /* 0x9B6 */ u8 pad9B6[2];
    /* 0x9B8 */ DataTable_19E80 *paletteDataTables[6];
    /* 0x9D0 */ CreditsTextEntry textEntriesA[18];
    /* 0xB80 */ CreditsTextEntry textEntriesB[18];
    /* 0xD30 */ s32 currentPaletteAlpha;
    /* 0xD34 */ s32 paletteFadeSpeed;
    /* 0xD38 */ s16 nextCommandFrame;
    /* 0xD3A */ s16 currentPaletteIndex;
    /* 0xD3C */ PaletteContext paletteContexts[6];
    /* 0xD84 */ s32 overlayAlpha;
    /* 0xD88 */ s32 overlayAlphaSpeed;
    /* 0xD8C */ s16 commandIndex;
    /* 0xD8E */ s8 paletteChangePending;
    /* 0xD8F */ s8 textChangePending;
    /* 0xD90 */ void *creditsText[8];
    /* 0xDB0 */ s16 textXOffsets[8];
    /* 0xDC0 */ s16 textYOffsets[8];
    /* 0xDD0 */ s16 textScrollOffsets[8];
    /* 0xDE0 */ s16 textRowIndex;
    /* 0xDE2 */ u8 padDE2[2];
    /* 0xDE4 */ s32 textAlpha;
    /* 0xDE8 */ s32 textFadeSpeed;
    /* 0xDEC */ void *cornerDecorationAsset;
    /* 0xDF0 */ s32 leftCornerAlpha;
    /* 0xDF4 */ s32 leftCornerFadeSpeed;
    /* 0xDF8 */ s32 rightCornerAlpha;
    /* 0xDFC */ s32 rightCornerFadeSpeed;
    /* 0xE00 */ TextRenderArg leftTopCornerSprite;
    /* 0xE10 */ TextRenderArg leftBottomCornerSprite;
    /* 0xE20 */ TextRenderArg rightTopCornerSprite;
    /* 0xE30 */ TextRenderArg rightBottomCornerSprite;
    /* 0xE40 */ s16 nextCharacterConfigIndex;
    /* 0xE42 */ s16 characterLaneIndex;
    /* 0xE44 */ ColorData sceneLights[3];
    /* 0xE5C */ u8 ambientLightR;
    /* 0xE5D */ u8 ambientLightG;
    /* 0xE5E */ u8 ambientLightB;
    /* 0xE5F */ u8 padE5F;
    /* 0xE60 */ s16 nextCharacterSpawnFrame;
    /* 0xE62 */ u8 padE62[6];
} CreditsState;
