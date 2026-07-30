#include "credits/credits_text.h"
#include "animation/easing_state.h"
#include "graphics/graphics.h"
#include "graphics/palette.h"
#include "graphics/sprite_rdp.h"
#include "text/text_layout.h"

CreditsTextCommand creditsCommands[] = {
    { 0x0000, 0x02, 0x00 },
    { 0x001E, 0x03, 0x00 },
    { 0x00F0, 0x04, 0x00 },
    { 0x010E, 0x00, 0x04 },
    { 0x01E0, 0x01, 0x01 },
    { 0x023A, 0x03, 0x01 },
    { 0x02D0, 0x03, 0x02 },
    { 0x03C0, 0x03, 0x03 },
    { 0x0456, 0x03, 0x04 },
    { 0x050A, 0x04, 0x00 },
    { 0x0564, 0x00, 0x03 },
    { 0x0636, 0x01, 0x01 },
    { 0x0672, 0x03, 0x05 },
    { 0x0762, 0x03, 0x06 },
    { 0x0870, 0x03, 0x07 },
    { 0x0960, 0x03, 0x08 },
    { 0x0A14, 0x04, 0x00 },
    { 0x0A6E, 0x00, 0x02 },
    { 0x0B40, 0x01, 0x01 },
    { 0x0B7C, 0x03, 0x09 },
    { 0x0C4E, 0x03, 0x0A },
    { 0x0D20, 0x03, 0x0B },
    { 0x0DF2, 0x03, 0x0C },
    { 0x0EC4, 0x04, 0x00 },
    { 0x0F3C, 0x00, 0x01 },
    { 0x1068, 0x01, 0x01 },
    { 0x10A4, 0x03, 0x0D },
    { 0x1176, 0x03, 0x0E },
    { 0x1248, 0x03, 0x0F },
    { 0x12FC, 0x04, 0x00 },
    { 0x1338, 0x00, 0x05 },
    { 0x13EC, 0x01, 0x01 },
    { 0x1428, 0x03, 0x10 },
    { 0x14DC, 0x03, 0x11 },
    { 0x1590, 0x03, 0x12 },
    { 0x1644, 0x03, 0x13 },
    { 0x16F8, 0x04, 0x00 },
    { 0x1752, 0x00, 0x00 },
    { 0x1842, 0x01, 0x01 },
    { 0x187E, 0x03, 0x14 },
    { 0x1932, 0x03, 0x15 },
    { 0x19E6, 0x04, 0x00 },
    { 0x19E6, 0x02, 0x00 },
};

s16 creditsCommandCount = 0x002B;

s32 paletteFadeSpeeds[] = { 0xFFF77778, 0x00088888 };

s32 overlayFadeSpeeds[] = { 0xFFFA4FA5, 0x0005B05B };

s32 textFadeSpeeds[] = { 0xFFEEEEEF, 0x00111111, 0x00000000, 0x00000000 };

void initCreditsScrollingTextEffects(CreditsState *s) {
    s32 i;
    void *creditsText;
    s16 yOffset;

    s->currentPaletteIndex = 0;
    s->commandIndex = 0;
    s->currentPaletteAlpha = 0;
    s->nextCommandFrame = 0;
    s->paletteChangePending = 0;

    for (i = 0; i < 0x12; i++) {
        s->paletteSprites[i].x = 0;
        s->paletteSprites[i].y = -0x60 + i * 8;
        s->paletteSprites[i].spriteData = (SpriteSheetData *)s->paletteDataTables[s->currentPaletteIndex];
        s->paletteSprites[i].frameIndex = i;
        s->paletteSprites[i].scaleY = 0x400;
        s->paletteSprites[i].scaleX = 0x400;
        s->paletteSprites[i].rotation = 0;
        s->paletteSprites[i].shade.shadeWithPadding = 0xFF;
        s->paletteSprites[i].tileMode = 0;
        s->paletteSprites[i].overridePaletteCount = 0;
        s->paletteSprites[i].alpha = 0;
        s->paletteOverlaySprites[i].x = 0;
        s->paletteOverlaySprites[i].y = -0x60 + i * 8;
        s->paletteOverlaySprites[i].spriteData = (SpriteSheetData *)s->paletteDataTables[s->currentPaletteIndex];
        s->paletteOverlaySprites[i].frameIndex = i;
        s->paletteOverlaySprites[i].scaleY = 0x400;
        s->paletteOverlaySprites[i].scaleX = 0x400;
        s->paletteOverlaySprites[i].rotation = 0;
        s->paletteOverlaySprites[i].shade.shadeWithPadding = 0x64;
        s->paletteOverlaySprites[i].tileMode = 0;
        s->paletteOverlaySprites[i].overridePaletteCount = 0x11;
        s->paletteOverlaySprites[i].alpha = 0;
    }

    for (i = 0; i < 6; i++) {
        initPaletteContext(&s->paletteContexts[i], s->paletteDataTables[i]);
        applyPaletteShift(&s->paletteContexts[i], 0xF00000, 0xB80000, 0x800000, 0x640000);
    }

    s->overlayAlpha = 0;
    s->overlayAlphaSpeed = 0;
    s->textRowIndex = 0;
    s->textAlpha = 0;
    s->textFadeSpeed = 0;
    s->textChangePending = 0;

    for (i = 0; i < 8; i++) {
        creditsText = getTable2DEntry(s->creditsTextTable, s->textRowIndex, 0);
        s->creditsText[i] = creditsText;
        s->textXOffsets[i] = -(getMaxLinePixelWidth(creditsText) / 2);
        s->textYOffsets[i] = -0x18;
        s->textScrollOffsets[i] = 0;
    }
}

void updateCreditsScrollingTextEffects(CreditsState *s) {
    s32 showPalette;
    s32 i;
    s32 scaledAlpha;
    CreditsTextCommand *cmd;
    s32 yAccum;
    s32 alphaSum;
    s16 initialFrame;
    s32 height;

    showPalette = 1;
    while (s->frameCounter == s->nextCommandFrame) {
        if (s->commandIndex >= creditsCommandCount) {
            break;
        }
        cmd = &creditsCommands[s->commandIndex];
        switch (cmd->commandType) {
            case CREDITS_TEXT_COMMAND_SELECT_PALETTE:
                s->currentPaletteIndex = (s8)cmd->parameter;
                s->paletteChangePending = 1;
                s->paletteFadeSpeed = paletteFadeSpeeds[0];
                break;
            case CREDITS_TEXT_COMMAND_SET_OVERLAY_FADE:
                s->overlayAlphaSpeed = overlayFadeSpeeds[(s8)cmd->parameter];
                break;
            case CREDITS_TEXT_COMMAND_SET_PALETTE_FADE:
                s->paletteFadeSpeed = paletteFadeSpeeds[(s8)cmd->parameter];
                s->paletteChangePending = 0;
                break;
            case CREDITS_TEXT_COMMAND_SELECT_TEXT_ROW:
                s->textRowIndex = (s8)cmd->parameter;
                s->textFadeSpeed = textFadeSpeeds[0];
                s->textChangePending = 1;
                break;
            case CREDITS_TEXT_COMMAND_SET_TEXT_FADE:
                s->textFadeSpeed = textFadeSpeeds[(s8)cmd->parameter];
                break;
        }

        s->commandIndex++;
        if (s->commandIndex >= creditsCommandCount) {
            break;
        }

        cmd = &creditsCommands[s->commandIndex];
        s->nextCommandFrame = cmd->triggerFrame;
    }

    do {
        s32 alphaSum = s->currentPaletteAlpha + s->paletteFadeSpeed;
        s->currentPaletteAlpha = alphaSum;
        if (alphaSum > 0xFF0000) {
            s->currentPaletteAlpha = 0xFF0000;
        } else if (alphaSum < 0) {
            s->currentPaletteAlpha = 0;
            if (s->paletteChangePending != 0) {
                for (i = 0; i < 0x12; i++) {
                    s->paletteSprites[i].spriteData = (SpriteSheetData *)s->paletteDataTables[s->currentPaletteIndex];
                    s->paletteOverlaySprites[i].spriteData =
                        (SpriteSheetData *)s->paletteDataTables[s->currentPaletteIndex];
                }
                s->paletteFadeSpeed = paletteFadeSpeeds[1];
                s->paletteChangePending = 0;
                s->overlayAlpha = 0;
                s->overlayAlphaSpeed = 0;
            }
        }
    } while (0);

    if (s->currentPaletteAlpha >> 16) {
        s->overlayAlpha += s->overlayAlphaSpeed;
        if (s->overlayAlpha > 0xFF0000) {
            s->overlayAlpha = 0xFF0000;
        }
        if (s->overlayAlpha < 0) {
            s->overlayAlpha = 0;
        }

        if (s->overlayAlpha != 0) {
            scaledAlpha = (((s->currentPaletteAlpha >> 16) * (s->overlayAlpha >> 8)) / 255) << 8;
            if (scaledAlpha > 0xFF0000) {
                scaledAlpha = 0xFF0000;
            }
            scaledAlpha &= (~scaledAlpha >> 31);
        }

        for (i = 0; i < 0x12; i++) {
            if (scaledAlpha != 0) {
                s->paletteOverlaySprites[i].alpha = scaledAlpha >> 16;
                enqueueCallbackBySlotIndex(0, 2, renderScaledAlphaSpriteFrame, &s->paletteOverlaySprites[i]);
            }
            if (s->paletteFadeSpeed < 0) {
                if (s->overlayAlpha == 0xFF0000) {
                    showPalette = 0;
                }
            }
            if (showPalette != 0) {
                s->paletteSprites[i].alpha = s->currentPaletteAlpha >> 16;
                enqueueCallbackBySlotIndex(0, 2, renderScaledAlphaSpriteFrame, &s->paletteSprites[i]);
            }
        }
    }

    do {
        alphaSum = s->textAlpha + s->textFadeSpeed;
        s->textAlpha = alphaSum;
        if (alphaSum > 0xFF0000) {
            s->textAlpha = 0xFF0000;
        } else if (alphaSum < 0) {
            s->textAlpha = 0;
            if (s->textChangePending != 0) {
                for (i = 0, yAccum = -0x18; i < 8; i++) {
                    s->creditsText[i] = getTable2DEntry(s->creditsTextTable, s->textRowIndex, i);
                    if (!s->creditsText[i]) {
                        break;
                    }
                    s->textXOffsets[i] = -(getMaxLinePixelWidth(s->creditsText[i]) / 2);
                    height = getTable2DRowCount(s->creditsTextTable, s->textRowIndex, i) * 8 + 8;
                    s->textYOffsets[i] = (yAccum + (i * 0x10)) - height;
                    s->textFadeSpeed = paletteFadeSpeeds[1];
                }
                s->textChangePending = 0;
            }
        }
    } while (0);

    if (s->textAlpha >> 16) {
        for (i = 0; i < 8; i++) {
            if (s->creditsText[i] == NULL) {
                break;
            }
            enqueueTextLayout(
                s->textRenderAsset,
                s->creditsText[i],
                s->textXOffsets[i],
                s->textYOffsets[i],
                0xFF,
                s->textAlpha >> 16,
                0,
                1,
                4
            );
        }
    }
}
