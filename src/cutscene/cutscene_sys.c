#include "cutscene/cutscene_sys.h"
#include "assets.h"
#include "cutscene/1DFAA0.h"
#include "graphics/graphics.h"
#include "graphics/model_loader.h"
#include "os_cont.h"
#include "os_vi.h"
#include "system/task_scheduler.h"
#include "ui/cutscene_wait_menu.h"

extern s32 gButtonsPressed[];

typedef struct {
    CutsceneManager *cutsceneManager;
    u8 padding[0xA];
    s16 waitFrameCount;
} cutsceneSysWait_exec_task;

typedef struct {
    u8 padding[0xFF7];
    /* 0xFF7 */ s8 skipAnimation;
} skipCutsceneCallback_arg;

u8 gCutsceneFadeBrightnessTable[16] = {
    0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

CutsceneFrameInfo D_800BA5D0_1E7680[1] = {
    { &_5C4040_ROM_START, &_5C4040_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA5DC_1E768C[1] = {
    { &_5C6450_ROM_START, &_5C6450_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA5E8_1E7698[1] = {
    { &_5C8110_ROM_START, &_5C8110_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA5F4_1E76A4[1] = {
    { &_5CA830_ROM_START, &_5CA830_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA600_1E76B0[1] = {
    { &_5CCA40_ROM_START, &_5CCA40_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA60C_1E76BC[1] = {
    { &_5CEEC0_ROM_START, &_5CEEC0_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA618_1E76C8[1] = {
    { &_5D1350_ROM_START, &_5D1350_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA624_1E76D4[4] = {
    { &_5DCA00_ROM_START, &_5DCA00_ROM_END, 0x78E0 },
    { &_5DF040_ROM_START, &_5DF040_ROM_END, 0x78E0 },
    { &_5E0A70_ROM_START, &_5E0A70_ROM_END, 0x78E0 },
    { &_5E2520_ROM_START, &_5E2520_ROM_END, 0x78E0 },
};
CutsceneFrameInfo D_800BA654_1E7704[1] = {
    { &_5E4A50_ROM_START, &_5E4A50_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA660_1E7710[1] = {
    { &_5E6D10_ROM_START, &_5E6D10_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA66C_1E771C[1] = {
    { &_5D3500_ROM_START, &_5D3500_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA678_1E7728[1] = {
    { &_5D5960_ROM_START, &_5D5960_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA684_1E7734[1] = {
    { &_5D7DD0_ROM_START, &_5D7DD0_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA690_1E7740[1] = {
    { &_5DA5C0_ROM_START, &_5DA5C0_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA69C_1E774C[1] = {
    { &_5EEE90_ROM_START, &_5EEE90_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA6A8_1E7758[1] = {
    { &_5F15F0_ROM_START, &_5F15F0_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA6B4_1E7764[2] = {
    { &_5E9130_ROM_START, &_5E9130_ROM_END, 0x78E0 },
    { &_5EB590_ROM_START, &_5EB590_ROM_END, 0x78E0 },
};
CutsceneFrameInfo D_800BA6CC_1E777C[1] = {
    { &_5ED030_ROM_START, &_5ED030_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA6D8_1E7788[1] = {
    { &_5F3D30_ROM_START, &_5F3D30_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA6E4_1E7794[1] = {
    { &_5F62C0_ROM_START, &_5F62C0_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA6F0_1E77A0[1] = {
    { &_5F8140_ROM_START, &_5F8140_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA6FC_1E77AC[1] = {
    { &_5FAB50_ROM_START, &_5FAB50_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA708_1E77B8[1] = {
    { &_5FCFB0_ROM_START, &_5FCFB0_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA714_1E77C4[1] = {
    { &_5FF220_ROM_START, &_5FF220_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA720_1E77D0[3] = {
    { &_6013B0_ROM_START, &_6013B0_ROM_END, 0x78E0 },
    { &_6034D0_ROM_START, &_6034D0_ROM_END, 0x78E0 },
    { &_605840_ROM_START, &_605840_ROM_END, 0x78E0 },
};
CutsceneFrameInfo D_800BA744_1E77F4[1] = {
    { &_6099A0_ROM_START, &_6099A0_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA750_1E7800[1] = {
    { &_60B690_ROM_START, &_60B690_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA75C_1E780C[1] = {
    { &_60D380_ROM_START, &_60D380_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA768_1E7818[1] = {
    { &_60F070_ROM_START, &_60F070_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA774_1E7824[1] = {
    { &_610D60_ROM_START, &_610D60_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA780_1E7830[1] = {
    { &_612A50_ROM_START, &_612A50_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA78C_1E783C[1] = {
    { &_614740_ROM_START, &_614740_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA798_1E7848[1] = {
    { &_616430_ROM_START, &_616430_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA7A4_1E7854[1] = {
    { &_618120_ROM_START, &_618120_ROM_END, 0x78E0 }
};
CutsceneFrameInfo D_800BA7B0_1E7860[1] = {
    { &_607BF0_ROM_START, &_607BF0_ROM_END, 0x78E0 }
};

CutsceneAssetTable gCutsceneAssetTable[16] = {
    { D_800BA5D0_1E7680, 1, 0, D_800BA5DC_1E768C, 1, 0, D_800BA720_1E77D0, 3, 0 },
    { D_800BA5E8_1E7698, 1, 0, D_800BA5F4_1E76A4, 1, 0, D_800BA744_1E77F4, 1, 0 },
    { D_800BA600_1E76B0, 1, 0, D_800BA60C_1E76BC, 1, 0, D_800BA750_1E7800, 1, 0 },
    { D_800BA60C_1E76BC, 1, 0, D_800BA618_1E76C8, 1, 0, D_800BA75C_1E780C, 1, 0 },
    { D_800BA66C_1E771C, 1, 0, D_800BA678_1E7728, 1, 0, D_800BA768_1E7818, 1, 0 },
    { D_800BA684_1E7734, 1, 0, D_800BA690_1E7740, 1, 0, D_800BA774_1E7824, 1, 0 },
    { D_800BA624_1E76D4, 4, 0, D_800BA654_1E7704, 1, 0, D_800BA780_1E7830, 1, 0 },
    { D_800BA654_1E7704, 1, 0, D_800BA660_1E7710, 1, 0, D_800BA78C_1E783C, 1, 0 },
    { D_800BA6B4_1E7764, 2, 0, D_800BA6CC_1E777C, 1, 0, D_800BA798_1E7848, 1, 0 },
    { D_800BA69C_1E774C, 1, 0, D_800BA6A8_1E7758, 1, 0, D_800BA7A4_1E7854, 1, 0 },
    { D_800BA6D8_1E7788, 1, 0, D_800BA6E4_1E7794, 1, 0, D_800BA7B0_1E7860, 1, 0 },
    { D_800BA6E4_1E7794, 1, 0, D_800BA6F0_1E77A0, 1, 0, NULL,              0, 0 },
    { D_800BA708_1E77B8, 1, 0, NULL,              0, 0, NULL,              0, 0 },
    { D_800BA6FC_1E77AC, 1, 0, NULL,              0, 0, NULL,              0, 0 },
    { D_800BA714_1E77C4, 1, 0, NULL,              0, 0, NULL,              0, 0 },
    { NULL,              0, 0, NULL,              0, 0, NULL,              0, 0 },
};

StateEntry *gControllerPakTransferPointer = NULL;
u32 D_800BA940_1E79F0[7] = { 0x10101010, 0x10101010, 0x10101010, 0x10101010, 0x10101010, 0x10100000, 0x00000000 };
StateEntry *gControllerPakStateTablePointer = NULL;

CutsceneFadeAssetNode gCutsceneFadeAssetTable[16] = {
    { &_64F190_ROM_START, &_64F190_ROM_END, 0x36B8, 0, { 0, 0, 0 } },
    { &_650AD0_ROM_START, &_650AD0_ROM_END, 0x3CC8, 1, { 0, 0, 0 } },
    { &_652620_ROM_START, &_652620_ROM_END, 0x36B8, 0, { 0, 0, 0 } },
    { NULL,               NULL,             0,      0, { 0, 0, 0 } },
    { &_654050_ROM_START, &_654050_ROM_END, 0x3CC8, 1, { 0, 0, 0 } },
    { &_655BF0_ROM_START, &_655BF0_ROM_END, 0x36B8, 0, { 0, 0, 0 } },
    { &_657610_ROM_START, &_657610_ROM_END, 0x3CC8, 1, { 0, 0, 0 } },
    { NULL,               NULL,             0,      0, { 0, 0, 0 } },
    { &_6592F0_ROM_START, &_6592F0_ROM_END, 0x36B8, 0, { 0, 0, 0 } },
    { &_65AC00_ROM_START, &_65AC00_ROM_END, 0x36B8, 0, { 0, 0, 0 } },
    { &_65C5C0_ROM_START, &_65C5C0_ROM_END, 0x36B8, 0, { 0, 0, 0 } },
    { NULL,               NULL,             0,      0, { 0, 0, 0 } },
    { &_65DFD0_ROM_START, &_65DFD0_ROM_END, 0x36B8, 0, { 0, 0, 0 } },
    { &_65F910_ROM_START, &_65F910_ROM_END, 0x36B8, 0, { 0, 0, 0 } },
    { &_6613B0_ROM_START, &_6613B0_ROM_END, 0x3CC8, 1, { 0, 0, 0 } },
    { NULL,               NULL,             0,      0, { 0, 0, 0 } },
};

s16 gFanSoundIds[343] = {
    0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x0009, 0x000D,
    0x000E, 0x000F, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x001B,
    0x001C, 0x001D, 0x001E, 0x001F, 0x0009, 0x0021, 0x0009, 0x0023, 0x0024, 0x0009, 0x0009, 0x0009, 0x0027, 0x0028,
    0x0029, 0x0009, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035,
    0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F, 0x0040, 0x0041, 0x0042, 0x0043,
    0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x0009, 0x004D, 0x004F, 0x0050, 0x0051,
    0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
    0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D,
    0x006E, 0x006F, 0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x007B,
    0x007C, 0x007D, 0x007E, 0x007F, 0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087, 0x0088, 0x0089,
    0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x008F, 0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,
    0x0098, 0x0099, 0x009A, 0x009B, 0x009C, 0x009D, 0x009E, 0x009F, 0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5,
    0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF, 0x00B0, 0x00B1, 0x00B2, 0x00B3,
    0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF, 0x00C0, 0x00C1,
    0x00C2, 0x00C3, 0x0009, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE,
    0x00CF, 0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC,
    0x00DD, 0x00DE, 0x00DF, 0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA,
    0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF, 0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7, 0x00F8,
    0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x0009, 0x0009, 0x00FE, 0x00FF, 0x0100, 0x0101, 0x0102, 0x0103, 0x0104,
    0x0105, 0x0106, 0x0107, 0x0108, 0x0109, 0x010A, 0x010B, 0x010C, 0x010D, 0x010E, 0x010F, 0x0110, 0x0111, 0x0112,
    0x0113, 0x0114, 0x0115, 0x0116, 0x0117, 0x0118, 0x0119, 0x011A, 0x011B, 0x011C, 0x011D, 0x011E, 0x011F, 0x0120,
    0x0121, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0122, 0x0123, 0x0124, 0x0125, 0x0126, 0x0127,
    0x0128, 0x0129, 0x012A, 0x012B, 0x012C, 0x012D, 0x012E, 0x0009, 0x0009, 0x0009, 0x0009, 0x012F, 0x0130, 0x0131,
    0x0132, 0x0009, 0x0009, 0x0133, 0x0009, 0x0134, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009, 0x0009,
    0x0009, 0x0009, 0x0009, 0x0009, 0x0135, 0x0009, 0x0009,
};

s16 gFanSoundCount = 0x0157;

void cutsceneSysDisp_init(void) {
}

s32 cutsceneSysDisp_validate(void) {
    return 0;
}

void cutsceneSysDisp_exec(u8 *enableTransparencyValue, cutsceneSysDisp_exec_arg *displayArgs) {
    displayArgs->enableTransparency = *enableTransparencyValue;
}

void cutsceneSysFadeIn_init(void) {
}

s32 cutsceneSysFadeIn_validate(void) {
    return 0;
}

void cutsceneSysFadeIn_exec(cutsceneSysFadeIn_exec_arg *fadeParams, ViewportNode *viewport) {
    u8 brightness = gCutsceneFadeBrightnessTable[fadeParams->brightnessIndex];
    setViewportEnvColor(viewport->unk0.next, brightness, brightness, brightness);
    setViewportFadeValue(viewport->unk0.next, 0, fadeParams->fadeMode);
}

s32 cutsceneSysFadeIn_isDone(void) {
    return 0;
}

void cutsceneSysFadeOut_init(void) {
}

s32 cutsceneSysFadeOut_validate(void) {
    return 0;
}

void cutsceneSysFadeOut_exec(cutsceneSysFadeOut_exec_arg *fadeParams, ViewportNode *viewport) {
    u8 brightness = gCutsceneFadeBrightnessTable[fadeParams->brightnessIndex];
    setViewportEnvColor(viewport->unk0.next, brightness, brightness, brightness);
    setViewportFadeValue(viewport->unk0.next, 0xFF, fadeParams->fadeMode);
}

s32 cutsceneSysFadeOut_isDone(void) {
    return 0;
}

void cutsceneSysFlash_init(void) {
}

s32 cutsceneSysFlash_validate(void) {
    return 0;
}

void cutsceneSysFlash_setup(CutsceneCmdItem *cmdItem, CutsceneManager *cutsceneManager, s8 slotIndex) {
    CutsceneSlot *slot;

    slot = getCutsceneSlot(cutsceneManager, slotIndex);
    slot->unk0.FlashPayload.frameCounter = cmdItem->unk0.FlashPayload.frameCounter;
    slot->unk0.FlashPayload.colorToggle = 0xFF;
    memcpy(&slot->unk0.FlashPayload.colors[0], &cmdItem->unk0.FlashPayload.colors[0], 4);
    memcpy(&slot->unk0.FlashPayload.colors[1], &cmdItem->unk0.FlashPayload.colors[1], 4);
    enableSlotUpdate(cutsceneManager, slotIndex);
}

void cutsceneSysFlash_update(CutsceneManager *cutsceneManager, s8 slotIndex) {
    CutsceneSlot *slot;
    s32 fadeAlpha = 0xFF;
    s32 colorIndex;

    slot = getCutsceneSlot(cutsceneManager, slotIndex);

    if (slot->unk0.FlashPayload.frameCounter > 0) {
        // Toggle between the two colors each frame
        colorIndex = slot->unk0.FlashPayload.colorToggle & 1;
        setViewportEnvColor(
            cutsceneManager->uiResource,
            slot->unk0.FlashPayload.colors[colorIndex].r,
            slot->unk0.FlashPayload.colors[colorIndex].g,
            slot->unk0.FlashPayload.colors[colorIndex].b
        );

        // Alternate colorToggle between 0xFF and 0
        if (slot->unk0.FlashPayload.colorToggle != 0) {
            slot->unk0.FlashPayload.colorToggle = 0;
            // Check if the second color is null (all zeros)
            if (*(u16 *)&slot->unk0.FlashPayload.colors[1] == 0) {
                // When second color is null, set alpha based on Two.unk8 (overlaps with colors[1].b)
                fadeAlpha = -(slot->unk0.Two.unk8 != 0);
            }
        } else {
            slot->unk0.FlashPayload.colorToggle = fadeAlpha;
        }

        setViewportFadeValue(cutsceneManager->uiResource, fadeAlpha & 0xFF, 0);
        slot->unk0.FlashPayload.frameCounter--;
    } else {
        // Flash effect complete - clear the screen overlay
        setViewportFadeValue(cutsceneManager->uiResource, 0, 0);
        disableSlotUpdate(cutsceneManager, slotIndex);
    }
}

s32 cutsceneSysFlash_isDone(void) {
    return 0;
}

void cutsceneSysIntMode_init(void) {
}

s32 cutsceneSysIntMode_validate(void) {
    return 0;
}

void cutsceneSysIntMode_exec(cutsceneSysIntMode_exec_arg *vStartParams) {
    osViExtendVStart(vStartParams->vStartExtension);
}

void cutsceneSysWipeColor_init(void) {
}

s32 cutsceneSysWipeColor_validate(void) {
    return 0;
}

void cutsceneSysWipeColor_exec(WipeColorParams *colorParams, WipeColorNodeParams *wipeParams) {
    setNodeWipeColor((NodeWipeColorArg *)&wipeParams->wipeColorNode, colorParams->r, colorParams->g, colorParams->b);
}

void skipCutsceneOnInputCallback(skipCutsceneCallback_arg **arg0) {
    skipCutsceneCallback_arg *taskPayload = *arg0;
    if ((taskPayload->skipAnimation != 0) && (gButtonsPressed[0] & A_BUTTON)) {
        taskPayload->skipAnimation = 0;
        terminateCurrentTask();
    }
}

void cutsceneSysWait_init(void) {
}

s32 cutsceneSysWait_validate(void) {
    return 0;
}

void cutsceneSysWait_exec(u16 *waitFrames, CutsceneManager *cutsceneManager) {
    cutsceneSysWait_exec_task *task = scheduleTask(&initCutsceneWaitMenu, 1, 0, 0x64);
    if (task != NULL) {
        enableCutsceneSkip(cutsceneManager);
        task->cutsceneManager = cutsceneManager;
        task->waitFrameCount = *waitFrames;
    }
}

void cutsceneSysCurtain_init(void) {
}

s32 cutsceneSysCurtain_validate(void) {
    return 0;
}

void cutsceneSysCurtain_exec(CurtainParams *params, CutsceneManager *cutsceneManager, s8 idx) {
    CutsceneSlot *slot;

    slot = getCutsceneSlot(cutsceneManager, idx);

    slot->unk0.CurtainPayload.targetPosition = (params->targetPercent << 16) / 100;

    if (params->duration) {
        slot->unk0.CurtainPayload.stepDelta =
            (slot->unk0.CurtainPayload.targetPosition - cutsceneManager->curtainPosition) / params->duration;
        slot->unk0.CurtainPayload.framesRemaining = params->duration;

        if (slot->unk0.CurtainPayload.stepDelta != 0) {
            enableSlotUpdate(cutsceneManager, idx);
        } else {
            cutsceneManager->curtainPosition = slot->unk0.CurtainPayload.targetPosition;
        }
    } else {
        cutsceneManager->curtainPosition = slot->unk0.CurtainPayload.targetPosition;
    }
}

void cutsceneSysCurtain_update(CutsceneManager *cutsceneManager, s8 slotIndex) {
    CutsceneSlot *slot;

    slot = getCutsceneSlot(cutsceneManager, slotIndex);
    cutsceneManager->curtainPosition += slot->unk0.CurtainPayload.stepDelta;

    if (slot->unk0.CurtainPayload.stepDelta > 0) {
        if (slot->unk0.CurtainPayload.targetPosition < cutsceneManager->curtainPosition) {
            cutsceneManager->curtainPosition = slot->unk0.CurtainPayload.targetPosition;
            disableSlotUpdate(cutsceneManager, slotIndex);
        }
    } else {
        if (cutsceneManager->curtainPosition < slot->unk0.CurtainPayload.targetPosition) {
            cutsceneManager->curtainPosition = slot->unk0.CurtainPayload.targetPosition;
            disableSlotUpdate(cutsceneManager, slotIndex);
        }
    }
}

s32 cutsceneSysCurtain_isDone(void) {
    return 0;
}

void cutsceneSysFilter_init(void) {
}

s32 cutsceneSysFilter_validate(void) {
    return 0;
}

void cutsceneSysFilter_exec(cutsceneSysFilter_exec_arg *filterArgs, ViewportNode *viewport) {
    setViewportEnvColor(
        viewport->unk0.next,
        filterArgs->envColorRed,
        filterArgs->envColorGreen,
        filterArgs->envColorBlue
    );
    setViewportFadeValue(viewport->unk0.next, filterArgs->cameraFilterParam, filterArgs->cameraFilterEnable);
}

s32 cutsceneSysFilter_isDone(void) {
    return 0;
}
