#include "1E64A0.h"
#include "36B80.h"
#include "90F0.h"
#include "B040.h"
#include "common.h"
#include "cutscene/cutscene_sys2.h"
#include "graphics.h"
#include "race_session.h"
#include "task_scheduler.h"

extern void func_800B99A0_1E6A50(void);
extern s16 gWipeOffsetX[];
extern s16 gWipeOffsetY[];

void func_800B9C20_1E6CD0(cutsceneSys2Wait_exec_asset *arg0);

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

s32 func_800B9708_1E67B8(cutsceneSys2Wait_exec_asset *arg0) {
    s32 temp_a1;
    s32 temp_v0;
    s32 result;

    temp_a1 = arg0->zoomLevelX;
    temp_v0 = ((0x10000 - temp_a1) >> 8) * (arg0->zoomDecelRate >> 8);
    result = 3;

    if (temp_v0 != 0) {
        arg0->zoomLevelX = temp_a1 + temp_v0;
    } else {
        arg0->zoomLevelX = 0x10000;
        result = 4;
    }

    updateWipeTransitionSlots(arg0);

    return result;
}

void *func_800B9774_1E6824(cutsceneSys2Wait_exec_asset *arg0) {
    void *result;

    result = getTable2DEntry(arg0->unkC, arg0->unk86, arg0->unk88);

    if (result == NULL) {
        arg0->unk84 = 6;
    }

    arg0->unk90 = result;
    arg0->unk84 = 5;

    return result;
}

extern s32 gControllerInputs;

void *func_800B97C8_1E6878(cutsceneSys2Wait_exec_asset *arg0) {
    void *var_s1;
    u8 temp_a0;
    u16 temp_v1_2;
    u16 *temp_a3;

    var_s1 = getTable2DEntry(arg0->unkC, arg0->unk86, arg0->unk88);
    temp_a0 = arg0->unk9D;
    arg0->unk90 = var_s1;

    if ((temp_a0 & 0xFF) != 0x64) {
        temp_a3 = (u16 *)((temp_a0 & 0xFF) * 2 + (s32)var_s1);
        temp_v1_2 = *temp_a3;

        if (temp_v1_2 != 0xFFFF) {
            switch (temp_v1_2) {
                case 0xFFF1:
                case 0xFFFD:
                    arg0->unk9D = arg0->unk9D + 1;
                    break;
                case 0xFFF0:
                    setModelAnimationLooped(
                        (SceneModel *)arg0->unk4,
                        *(s16 *)((u8 *)temp_a3 + 2),
                        *(s16 *)((u8 *)temp_a3 + 6),
                        *(s8 *)((u8 *)temp_a3 + 5)
                    );
                    arg0->unk9D = arg0->unk9D + 4;
                    break;
                case 0xFFFC:
                    arg0->unk9D = temp_a0 + 2;
                    break;
            }
            arg0->unk9D = arg0->unk9D + 1;
        } else {
            arg0->unk9D = 0x64;
        }
    }

    if (*(s8 *)((u8 *)arg0->unk0 + 0xFF7) != 0) {
        if (gControllerInputs & A_BUTTON) {
            if (arg0->unk9D != 0x64) {
                arg0->unk9D = 0x64;
            } else {
                s16 temp_v0 = arg0->unk88 + 1;
                arg0->unk88 = temp_v0;
                var_s1 = getTable2DEntry(arg0->unkC, arg0->unk86, temp_v0);
                if (var_s1 == NULL) {
                    *(s8 *)((u8 *)arg0->unk0 + 0xFF7) = 0;
                    arg0->unk84 = 6;
                } else {
                    playSoundEffect(0x2B);
                    arg0->unk9D = 0;
                    arg0->unk84 = 4;
                }
            }
        }
    } else {
        arg0->unk84 = 6;
    }

    return var_s1;
}

void func_800B993C_1E69EC(cutsceneSys2Wait_exec_asset *arg0) {
    arg0->unk84 = 0;
    arg0->unk88 = 0;
    arg0->unk8 = loadSpriteAssetData(0);
    arg0->unkA0 = loadTextRenderAsset(1);
    arg0->unkC = loadDmaAsset(0);
    setCleanupCallback(&func_800B9C20_1E6CD0);
    setCallback(&func_800B99A0_1E6A50);
}

INCLUDE_ASM("asm/nonmatchings/1E64A0", func_800B99A0_1E6A50);

void func_800B9C20_1E6CD0(cutsceneSys2Wait_exec_asset *arg0) {
    arg0->unkA0 = freeNodeMemory(arg0->unkA0);
    arg0->unk8 = freeNodeMemory(arg0->unk8);
    arg0->unkC = freeNodeMemory(arg0->unkC);
}
