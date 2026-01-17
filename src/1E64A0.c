#include "1E64A0.h"
#include "36B80.h"
#include "90F0.h"
#include "B040.h"
#include "common.h"
#include "cutscene/cutscene_sys2.h"
#include "graphics.h"
#include "race_session.h"
#include "task_scheduler.h"

extern void updateCutsceneWaitSystem(void);
extern s16 gWipeOffsetX[];
extern s16 gWipeOffsetY[];

void cleanupCutsceneWaitSystem(cutsceneSys2Wait_exec_asset *arg0);

s32 calculateZoomScaleFactor(s32 arg0) {
    s32 result;

    if (arg0 == 0) {
        arg0 = 1;
    }

    result = 0x4000000 / arg0;

    if (result >= 0x4000) {
        result = 0x4000;
    }

    return result;
}

void updateWipeTransitionSlots(cutsceneSys2Wait_exec_asset *arg0) {
    s32 i;
    s32 offset;
    s32 one;

    i = 0;
    one = 1;
    offset = 0;

    do {
        arg0->slots[i].zoomScaleX = calculateZoomScaleFactor(arg0->zoomLevelX);
        arg0->slots[i].zoomScaleY = calculateZoomScaleFactor(arg0->zoomLevelY);

        if (arg0->slots[i].zoomScaleX == 0) {
            arg0->slots[i].zoomScaleX = one;
        }
        if (arg0->slots[i].zoomScaleY == 0) {
            arg0->slots[i].zoomScaleY = one;
        }

        arg0->slots[i].posX =
            arg0->basePosX + ((((s16 *)((u8 *)gWipeOffsetX + offset))[0]) << 10) / arg0->slots[i].zoomScaleX;
        arg0->slots[i].posY =
            arg0->basePosY + ((((s16 *)((u8 *)gWipeOffsetY + offset))[0]) << 10) / arg0->slots[i].zoomScaleY;

        i++;
        offset += 4;
    } while (i < 4);
}

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B956C_1E661C);

s32 initWipeZoomAccel(cutsceneSys2Wait_exec_asset *arg0) {
    arg0->zoomSpeed = 0x6600;
    arg0->zoomDecelRate = 0x8000;
    updateWipeTransitionSlots(arg0);
    playSoundEffect(0x2C);
    return 2;
}

s32 accelerateWipeZoomIn(cutsceneSys2Wait_exec_asset *arg0) {
    s32 result = 2;

    arg0->zoomLevelX += arg0->zoomSpeed;

    if (arg0->zoomLevelX > 0x18000) {
        arg0->zoomLevelX = 0x18000;
        result = 3;
    }

    updateWipeTransitionSlots(arg0);

    return result;
}

s32 decelerateWipeZoomOut(cutsceneSys2Wait_exec_asset *arg0) {
    s32 currentZoom;
    s32 zoomDelta;
    s32 result;

    currentZoom = arg0->zoomLevelX;
    zoomDelta = ((0x10000 - currentZoom) >> 8) * (arg0->zoomDecelRate >> 8);
    result = 3;

    if (zoomDelta != 0) {
        arg0->zoomLevelX = currentZoom + zoomDelta;
    } else {
        arg0->zoomLevelX = 0x10000;
        result = 4;
    }

    updateWipeTransitionSlots(arg0);

    return result;
}

void *getCutsceneCommandEntry(cutsceneSys2Wait_exec_asset *ctx) {
    void *tableEntry;

    tableEntry = getTable2DEntry(ctx->commandTable, ctx->tableRowIndex, ctx->tableColumnIndex);

    if (tableEntry == NULL) {
        ctx->state = 6;
    }

    ctx->currentCommandEntry = tableEntry;
    ctx->state = 5;

    return tableEntry;
}

extern s32 gControllerInputs;

void *processCutsceneCommandSequence(cutsceneSys2Wait_exec_asset *ctx) {
    void *commandEntry;
    u8 commandOffset;
    u16 commandCode;
    u16 *commandPtr;

    commandEntry = getTable2DEntry(ctx->commandTable, ctx->tableRowIndex, ctx->tableColumnIndex);
    commandOffset = ctx->commandOffset;
    ctx->currentCommandEntry = commandEntry;

    if (commandOffset != 0x64) {
        commandPtr = (u16 *)(commandOffset * 2 + (s32)commandEntry);
        commandCode = *commandPtr;

        if (commandCode != 0xFFFF) {
            switch (commandCode) {
                case 0xFFF1:
                case 0xFFFD:
                    ctx->commandOffset = ctx->commandOffset + 1;
                    break;
                case 0xFFF0:
                    setModelAnimationLooped(
                        (SceneModel *)ctx->model,
                        *(s16 *)((u8 *)commandPtr + 2),
                        *(s16 *)((u8 *)commandPtr + 6),
                        *(s8 *)((u8 *)commandPtr + 5)
                    );
                    ctx->commandOffset = ctx->commandOffset + 4;
                    break;
                case 0xFFFC:
                    ctx->commandOffset = commandOffset + 2;
                    break;
            }
            ctx->commandOffset = ctx->commandOffset + 1;
        } else {
            ctx->commandOffset = 0x64;
        }
    }

    if (*(s8 *)((u8 *)ctx->cutsceneManager + 0xFF7) != 0) {
        if (gControllerInputs & A_BUTTON) {
            if (ctx->commandOffset != 0x64) {
                ctx->commandOffset = 0x64;
            } else {
                s16 nextColumn = ctx->tableColumnIndex + 1;
                ctx->tableColumnIndex = nextColumn;
                commandEntry = getTable2DEntry(ctx->commandTable, ctx->tableRowIndex, nextColumn);
                if (commandEntry == NULL) {
                    *(s8 *)((u8 *)ctx->cutsceneManager + 0xFF7) = 0;
                    ctx->state = 6;
                } else {
                    playSoundEffect(0x2B);
                    ctx->commandOffset = 0;
                    ctx->state = 4;
                }
            }
        }
    } else {
        ctx->state = 6;
    }

    return commandEntry;
}

void initCutsceneWaitSystem(cutsceneSys2Wait_exec_asset *arg0) {
    arg0->state = 0;
    arg0->tableColumnIndex = 0;
    arg0->sprites = loadSpriteAssetData(0);
    arg0->textRenderAsset = loadTextRenderAsset(1);
    arg0->commandTable = loadDmaAsset(0);
    setCleanupCallback(&cleanupCutsceneWaitSystem);
    setCallback(&updateCutsceneWaitSystem);
}

INCLUDE_ASM("asm/nonmatchings/1E64A0", updateCutsceneWaitSystem);

void cleanupCutsceneWaitSystem(cutsceneSys2Wait_exec_asset *arg0) {
    arg0->textRenderAsset = freeNodeMemory(arg0->textRenderAsset);
    arg0->sprites = freeNodeMemory(arg0->sprites);
    arg0->commandTable = freeNodeMemory(arg0->commandTable);
}
