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
    state->subtitleShadowX = 0;
    state->subtitleShadowY = 0;
    state->subtitleShadowStyle.value = 0xFF;
    state->subtitleShadowAlpha.value = 0xFF;
    state->subtitleShadowTileMode = 0;
    state->subtitleX = 0;
    state->subtitleY = 0;
    state->subtitleShadowAsset = temp;
    state->subtitleRenderAsset = state->textRenderAsset;
    state->subtitleText = getTable2DEntry(state->subtitleTextTable, state->subtitleIndex, 0);
    state->subtitleStyle.value = 0xFF;
    state->subtitleAlpha.value = 0xFF;
    state->subtitleTileMode = 2;
    state->subtitleFrameCounter = 0;
    state->subtitleScheduleIndex = 0;
    state->nextSubtitleFrame = 0;
    state->subtitleScrollPosition.value = 0;
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
        state->subtitleScrollPosition.value = 0;
        state->subtitleScrollStep = 0;
        state->subtitleDelay = 0;
        state->subtitleInitialDelay = 0;
        state->subtitleScheduleIndex++;

        if (getOffsetTableEntryData(state->subtitleSchedule, state->subtitleScheduleIndex) != 0) {
            state->subtitleIndex = state->subtitleIndex + 1;
        }

        temp_s0 = state->subtitleScheduleIndex;
        if (temp_s0 < (getOffsetTableEntryCount(state->subtitleSchedule) - 1)) {
            state->nextSubtitleFrame =
                getOffsetTableEntryValue0(state->subtitleSchedule, (s16)state->subtitleScheduleIndex + 1);
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

    if (getOffsetTableEntryData(state->subtitleSchedule, state->subtitleScheduleIndex) != 0) {
        temp_v1 = state->subtitleInitialDelay;
        if (temp_v1 == 0) {
            if (state->subtitleDelay == 0) {
                goto loop_start;
            }
            if (state->subtitleLineIndex != 0) {
                if (state->subtitleScrollStep != -1) {
                    state->subtitleScrollPosition.value += state->subtitleScrollStep;
                }
            }
            state->subtitleDelay--;
        } else {
            state->subtitleInitialDelay = temp_v1 - 1;
        }

        if (state->subtitleDelay == 0) {
        loop_start:
            while (state->subtitleCommandIndex <
                   getOffsetTableEntryValue2(state->subtitleSchedule, state->subtitleScheduleIndex)) {
                u8 *ptr;

                ptr = getOffsetTableEntryData(state->subtitleSchedule, state->subtitleScheduleIndex);
                state->subtitleDelay = ptr[state->subtitleCommandIndex];
                state->subtitleScrollPosition.value = state->subtitleLineIndex << 20;

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
            state->subtitleShadowText = getTable2DEntry(state->subtitleTextTable, state->subtitleIndex, 0);
            state->subtitleText = getTable2DEntry(state->subtitleTextTable, state->subtitleIndex, 0);
            state->subtitleTextWidth = getMaxLinePixelWidth(state->subtitleShadowText);
        }

        var_s3 = state->subtitleScrollPosition.halves.high;
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

        state->subtitleShadowAlpha.value = state->subtitleAlphaFixed >> 16;
        state->subtitleAlpha.value = state->subtitleAlphaFixed >> 16;

        enqueueTextLayout(
            state->subtitleShadowAsset,
            state->subtitleShadowText,
            state->subtitleShadowX,
            state->subtitleShadowY,
            state->subtitleShadowStyle.bytes.low,
            state->subtitleShadowAlpha.bytes.low,
            state->subtitleShadowTileMode,
            2,
            0
        );
        enqueueTextLayout(
            state->subtitleRenderAsset,
            state->subtitleText,
            state->subtitleX,
            state->subtitleY,
            state->subtitleStyle.bytes.low,
            state->subtitleAlpha.bytes.low,
            state->subtitleTileMode,
            3,
            1
        );
    }
    state->subtitleFrameCounter++;
}
