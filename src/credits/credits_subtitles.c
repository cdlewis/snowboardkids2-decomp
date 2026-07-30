#include "credits/credits_subtitles.h"
#include "animation/easing_state.h"
#include "common.h"
#include "credits/offset_table.h"
#include "graphics/graphics.h"
#include "race/race_session.h"

void initCreditsSubtitles(CreditsState *state) {
    void *temp;

    state->subtitleIndex = -1;
    temp = state->textRenderAsset;
    state->subtitleShadowLayout.startX = 0;
    state->subtitleShadowLayout.startY = 0;
    state->subtitleShadowLayout.shade.value = 0xFF;
    state->subtitleShadowLayout.textAlpha.value = 0xFF;
    state->subtitleShadowLayout.paletteIndex = 0;
    state->subtitleLayout.startX = 0;
    state->subtitleLayout.startY = 0;
    state->subtitleShadowLayout.fontAsset = temp;
    state->subtitleLayout.fontAsset = state->textRenderAsset;
    state->subtitleLayout.textData = getTable2DEntry(state->subtitleTextTable, state->subtitleIndex, 0);
    state->subtitleLayout.shade.value = 0xFF;
    state->subtitleLayout.textAlpha.value = 0xFF;
    state->subtitleLayout.paletteIndex = 2;
    state->subtitleFrameCounter = 0;
    state->subtitleScheduleIndex = 0;
    state->nextSubtitleFrame = 0;
    state->subtitleScrollPosition.fixedPoint = 0;
    state->subtitleScrollStep = 0;
    state->subtitleCommandIndex = 0;
    state->subtitleLineIndex = 0;
    state->subtitleDelay = 0;
    state->subtitleAlphaFixed = 0;
}

void updateCreditsSubtitles(CreditsState *state) {
    s16 temp_s0;
    s32 var_s3;
    s16 temp_v1;
    s16 temp_a8;

    if (state->frameCounter >= state->nextSubtitleFrame) {
        state->subtitleCommandIndex = 0;
        state->subtitleLineIndex = 0;
        state->subtitleFrameCounter = 0;
        state->subtitleScrollPosition.fixedPoint = 0;
        state->subtitleScrollStep = 0;
        state->subtitleDelay = 0;
        state->subtitleInitialDelay = 0;
        state->subtitleScheduleIndex++;

        if (getSubtitleScheduleEntryCommands(state->subtitleSchedule, state->subtitleScheduleIndex) != 0) {
            state->subtitleIndex = state->subtitleIndex + 1;
        }

        temp_s0 = state->subtitleScheduleIndex;
        if (temp_s0 < (getSubtitleScheduleEntryCount(state->subtitleSchedule) - 1)) {
            state->nextSubtitleFrame =
                getSubtitleScheduleEntryStartFrame(state->subtitleSchedule, (s16)state->subtitleScheduleIndex + 1);
        } else {
            state->nextSubtitleFrame = 0x7530;
        }
    } else if (state->frameCounter >= (state->nextSubtitleFrame - 2)) {
        state->subtitleAlphaFixed = state->subtitleAlphaFixed + 0xFF800000;
        if (state->subtitleAlphaFixed < 0) {
            state->subtitleAlphaFixed = 0;
        }
    } else {
        state->subtitleAlphaFixed += 0x800000;
        if (state->subtitleAlphaFixed > 0xFF0000) {
            state->subtitleAlphaFixed = 0xFF0000;
        }
    }

    if (getSubtitleScheduleEntryCommands(state->subtitleSchedule, state->subtitleScheduleIndex) != 0) {
        temp_v1 = state->subtitleInitialDelay;
        if (temp_v1 == 0) {
            if (state->subtitleDelay == 0) {
                goto loop_start;
            }
            if (state->subtitleLineIndex != 0) {
                if (state->subtitleScrollStep != -1) {
                    state->subtitleScrollPosition.fixedPoint += state->subtitleScrollStep;
                }
            }
            state->subtitleDelay--;
        } else {
            state->subtitleInitialDelay = temp_v1 - 1;
        }

        if (state->subtitleDelay == 0) {
        loop_start:
            while (state->subtitleCommandIndex <
                   getSubtitleScheduleEntryCommandCount(state->subtitleSchedule, state->subtitleScheduleIndex)) {
                u8 *commands;

                commands = getSubtitleScheduleEntryCommands(state->subtitleSchedule, state->subtitleScheduleIndex);
                state->subtitleDelay = commands[state->subtitleCommandIndex];
                state->subtitleScrollPosition.fixedPoint = state->subtitleLineIndex << 20;

                if (state->subtitleDelay & 0x80) {
                    state->subtitleDelay &= 0x7F;
                    state->subtitleScrollStep = -1;
                } else {
                    if (state->subtitleDelay != 0) {
                        state->subtitleScrollStep = 0x100000 / state->subtitleDelay;
                    } else {
                        state->subtitleScrollStep = -1;
                    }

                    state->subtitleLineIndex++;
                }

                state->subtitleCommandIndex++;

                if (state->subtitleDelay != 0) {
                    break;
                }
            }
        }

        if (state->subtitleFrameCounter == 0) {
            state->subtitleShadowLayout.textData = getTable2DEntry(state->subtitleTextTable, state->subtitleIndex, 0);
            state->subtitleLayout.textData = getTable2DEntry(state->subtitleTextTable, state->subtitleIndex, 0);
            state->subtitleTextWidth = getMaxLinePixelWidth(state->subtitleShadowLayout.textData);
        }

        var_s3 = state->subtitleScrollPosition.parts.pixels;
        temp_a8 = state->subtitleTextWidth;
        temp_s0 = -(temp_a8 / 2);
        if (temp_a8 < var_s3) {
            var_s3 = temp_a8;
        }

        if (state->subtitleScrollStep == 0) {
            var_s3 = 0;
        }

        setModelCameraTransform(&state->subtitleShadowViewport, temp_s0, 0x58, 0, 0, state->subtitleTextWidth, 0x10);
        setModelCameraTransform(&state->subtitleViewport, temp_s0, 0x58, 0, 0, var_s3, 0x10);

        state->subtitleShadowLayout.textAlpha.value = state->subtitleAlphaFixed >> 16;
        state->subtitleLayout.textAlpha.value = state->subtitleAlphaFixed >> 16;

        enqueueTextLayout(
            state->subtitleShadowLayout.fontAsset,
            state->subtitleShadowLayout.textData,
            state->subtitleShadowLayout.startX,
            state->subtitleShadowLayout.startY,
            state->subtitleShadowLayout.shade.bytes.low,
            state->subtitleShadowLayout.textAlpha.bytes.low,
            state->subtitleShadowLayout.paletteIndex,
            2,
            0
        );
        enqueueTextLayout(
            state->subtitleLayout.fontAsset,
            state->subtitleLayout.textData,
            state->subtitleLayout.startX,
            state->subtitleLayout.startY,
            state->subtitleLayout.shade.bytes.low,
            state->subtitleLayout.textAlpha.bytes.low,
            state->subtitleLayout.paletteIndex,
            3,
            1
        );
    }
    state->subtitleFrameCounter++;
}
