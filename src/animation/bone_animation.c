#include "animation/bone_animation.h"
#include "common.h"
#include "gamestate.h"
#include "math/geometry.h"

void *getAnimationFrameData(void *animData) {
    return (void *)((s8 *)animData + *(s32 *)((s8 *)animData + 4));
}

u16 *getAnimationDataByIndex(void *animData, s16 tableIndex, s16 boneIndex) {
    void *boneDataTable;

    boneDataTable = animData + *(s32 *)((s8 *)animData + tableIndex * 4 + 8);
    return boneDataTable + *(u16 *)((s8 *)boneDataTable + boneIndex * 2 + 4);
}

void initBoneAnimationState(void *animData, s32 tableIndex, s32 boneIndex, BoneAnimationState *state) {
    s16 *frameData;
    u16 *boneAnimEntry;

    frameData = getAnimationFrameData(animData);
    state->frame_data = frameData;

    boneAnimEntry = getAnimationDataByIndex(animData, (s16)tableIndex, (s16)boneIndex);
    state->animation_data = (s16 *)boneAnimEntry;

    state->flags = *boneAnimEntry + 0x8000;
}

void createBoneRotMatrix(s16 arg0, s16 arg1, s16 arg2, s16 *arg3) {
    s32 spVars[3];
    s16 temp_s0;
    s16 temp_s1;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_a3;
    s32 temp_s0_2;
    s32 temp_s1_2;
    s32 temp_s2;
    s32 temp_s4;
    s32 temp_s5;
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_t2;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 var_a0;
    s32 var_v0;
    s32 var_v1;

    temp_s0 = arg1;
    temp_s1 = -arg0;
    temp_s5 = approximateSin(temp_s1);
    temp_s0 = -temp_s0;
    temp_s1_2 = approximateSin(temp_s0);
    temp_s0_2 = approximateCos(temp_s1);
    temp_s4 = approximateCos(temp_s0);
    temp_s2 = approximateSin(arg2);
    temp_a3 = approximateCos(arg2);
    var_v1 = temp_s5 * temp_s1_2;
    if (var_v1 < 0) {
        var_v1 += 0x1FFF;
    }
    var_v0 = temp_s5 * temp_s4;
    temp_a2 = var_v1 >> 0xD;
    spVars[0] = temp_a2;
    spVars[1] = temp_s0_2;
    if (var_v0 < 0) {
        var_v0 += 0x1FFF;
    }
    var_v1 = temp_a2 * temp_s0_2;
    temp_a0 = var_v0 >> 0xD;
    spVars[2] = temp_a0;
    temp_a1 = 0x2000 - temp_a3;
    if (var_v1 < 0) {
        var_v1 += 0x1FFF;
    }
    var_v0 = (var_v1 >> 0xD) * temp_a1;
    if (var_v0 < 0) {
        var_v0 += 0x1FFF;
    }
    var_v1 = temp_s0_2 * temp_a0;
    temp_t2 = var_v0 >> 0xD;
    if (var_v1 < 0) {
        var_v1 += 0x1FFF;
    }
    var_v0 = (var_v1 >> 0xD) * temp_a1;
    if (var_v0 < 0) {
        var_v0 += 0x1FFF;
    }
    var_v1 = temp_a0 * temp_a2;
    temp_t0 = var_v0 >> 0xD;
    if (var_v1 < 0) {
        var_v1 += 0x1FFF;
    }
    var_v0 = (var_v1 >> 0xD) * temp_a1;
    if (var_v0 < 0) {
        var_v0 += 0x1FFF;
    }
    var_v1 = temp_a2 * temp_a2;
    temp_t1 = var_v0 >> 0xD;
    if (var_v1 < 0) {
        var_v1 += 0x1FFF;
    }
    var_a0 = temp_a2 * temp_s2;
    temp_a1 = var_v1 >> 0xD;
    if (var_a0 < 0) {
        var_a0 += 0x1FFF;
    }
    var_v0 = temp_a3 * (0x2000 - temp_a1);
    temp_v1 = var_a0 >> 0xD;
    if (var_v0 < 0) {
        var_v0 += 0x1FFF;
    }
    arg3[0] = temp_a1 + (var_v0 >> 0xD);
    arg3[7] = (s16)(temp_t0 - temp_v1);
    arg3[5] = (s16)(temp_t0 + temp_v1);
    var_v0 = spVars[1] * spVars[1];
    if (var_v0 < 0) {
        var_v0 += 0x1FFF;
    }
    var_v1 = spVars[1] * temp_s2;
    temp_a1 = var_v0 >> 0xD;
    if (var_v1 < 0) {
        var_v1 += 0x1FFF;
    }
    var_v0 = temp_a3 * (0x2000 - temp_a1);
    temp_v1_2 = var_v1 >> 0xD;
    if (var_v0 < 0) {
        var_v0 += 0x1FFF;
    }
    arg3[4] = (s16)(temp_a1 + (var_v0 >> 0xD));
    arg3[6] = (s16)(temp_t1 + temp_v1_2);
    arg3[2] = (s16)(temp_t1 - temp_v1_2);
    var_v0 = spVars[2] * spVars[2];
    if (var_v0 < 0) {
        var_v0 += 0x1FFF;
    }
    var_v1 = spVars[2] * temp_s2;
    temp_a1 = var_v0 >> 0xD;
    if (var_v1 < 0) {
        var_v1 += 0x1FFF;
    }
    var_v0 = temp_a3 * (0x2000 - temp_a1);
    temp_v1_3 = var_v1 >> 0xD;
    if (var_v0 < 0) {
        var_v0 += 0x1FFF;
    }
    arg3[8] = (s16)(temp_a1 + (var_v0 >> 0xD));
    arg3[3] = (s16)(temp_t2 - temp_v1_3);
    arg3[1] = (s16)(temp_t2 + temp_v1_3);
}

s32 updateBoneAnimation(BoneAnimationState *state) {
    s16 stack_data[16];
    u16 flags;
    u16 frame_index;

    flags = state->flags;

    if (flags & 0x8000) {
        s16 *animation_data = state->animation_data;
        state->flags = flags & 0x7FFF;
        createBoneRotMatrix(animation_data[1], animation_data[2], animation_data[3], state->transform.current.m[0]);

        frame_index = state->animation_data[4];
        state->transform.current.translation.x = state->frame_data[frame_index * 3] << 10;

        frame_index = state->animation_data[4];
        state->transform.current.translation.y = state->frame_data[frame_index * 3 + 1] << 10;

        frame_index = state->animation_data[4];
        state->transform.current.translation.z = state->frame_data[frame_index * 3 + 2] << 10;

        memcpy(&state->transform.previous, &state->transform.current, sizeof(Transform3D));

        if (state->flags == 0) {
            goto ret0;
        }
        goto advance_animation;
    }

    if ((flags & 0xFFFF) == 0) {
        return 1;
    }

    state->counter = state->counter - (state->counter / flags);

    {
        s16 *animation_data = state->animation_data;
        if (animation_data[3] != (state->counter & 0xFFFF)) {
            createBoneRotMatrix(animation_data[1], animation_data[2], animation_data[3] - state->counter, stack_data);
            func_8006BDBC_6C9BC(&state->transform.current, (Transform3D *)stack_data, &state->transform.previous);
        }
    }

    frame_index = state->animation_data[4];
    state->transform.previous.translation.x +=
        ((state->frame_data[frame_index * 3] << 10) - state->transform.previous.translation.x) / (s32)state->flags;

    frame_index = state->animation_data[4];
    state->transform.previous.translation.y +=
        ((state->frame_data[frame_index * 3 + 1] << 10) - state->transform.previous.translation.y) / (s32)state->flags;

    frame_index = state->animation_data[4];
    state->transform.previous.translation.z +=
        ((state->frame_data[frame_index * 3 + 2] << 10) - state->transform.previous.translation.z) / (s32)state->flags;

    state->flags--;
    if ((state->flags & 0xFFFF) != 0) {
        goto ret0;
    }

    state->flags = state->animation_data[0];
    memcpy(&state->transform.current, &state->transform.previous, sizeof(Transform3D));

    if (state->flags == 0) {
        return 1;
    }

advance_animation: {
    s16 *animation_data = state->animation_data;
    state->animation_data = animation_data + 5;
    state->counter = animation_data[8];
}

ret0:
    return 0;
}

s32 updateBoneAnimationMirrored(BoneAnimationState *state) {
    s16 stack_data[16];
    u16 flags;
    u16 frameIndex;

    flags = state->flags;

    if (flags & 0x8000) {
        s16 *animation_data = state->animation_data;
        state->flags = flags & 0x7FFF;
        createBoneRotMatrix(animation_data[1], -animation_data[2], -animation_data[3], state->transform.current.m[0]);

        frameIndex = state->animation_data[4];
        state->transform.current.translation.x = -state->frame_data[frameIndex * 3] << 10;

        frameIndex = state->animation_data[4];
        state->transform.current.translation.y = state->frame_data[frameIndex * 3 + 1] << 10;

        frameIndex = state->animation_data[4];
        state->transform.current.translation.z = state->frame_data[frameIndex * 3 + 2] << 10;

        memcpy(&state->transform.previous, &state->transform.current, sizeof(Transform3D));

        if (state->flags == 0) {
            goto ret0;
        }
        goto advance_animation;
    }

    if ((flags & 0xFFFF) == 0) {
        return 1;
    }

    state->counter = state->counter - (state->counter / flags);

    {
        s16 *animation_data = state->animation_data;
        if (animation_data[3] != (state->counter & 0xFFFF)) {
            createBoneRotMatrix(animation_data[1], -animation_data[2], state->counter - animation_data[3], stack_data);
            func_8006BDBC_6C9BC(&state->transform.current, (Transform3D *)stack_data, &state->transform.previous);
        }
    }

    frameIndex = state->animation_data[4];
    state->transform.previous.translation.x +=
        ((-state->frame_data[frameIndex * 3] << 10) - state->transform.previous.translation.x) / (s32)state->flags;

    frameIndex = state->animation_data[4];
    state->transform.previous.translation.y +=
        ((state->frame_data[frameIndex * 3 + 1] << 10) - state->transform.previous.translation.y) / (s32)state->flags;

    frameIndex = state->animation_data[4];
    state->transform.previous.translation.z +=
        ((state->frame_data[frameIndex * 3 + 2] << 10) - state->transform.previous.translation.z) / (s32)state->flags;

    state->flags--;
    if ((state->flags & 0xFFFF) != 0) {
        goto ret0;
    }

    state->flags = state->animation_data[0];
    memcpy(&state->transform.current, &state->transform.previous, sizeof(Transform3D));

    if (state->flags == 0) {
        return 1;
    }

advance_animation: {
    s16 *animation_data = state->animation_data;
    state->animation_data = animation_data + 5;
    state->counter = animation_data[8];
}

ret0:
    return 0;
}

void interpolateBoneAnimation(BoneAnimationState *state, u16 progress) {
    s16 rotMatrix[0x10];

    if (state->flags & 0x8000) {
        state->flags &= 0x7FFF;
        createBoneRotMatrix(
            state->animation_data[1],
            state->animation_data[2],
            state->animation_data[3],
            state->transform.current.m[0]
        );
        {
            u16 frameIndex;

            frameIndex = state->animation_data[4];
            state->transform.current.translation.x = state->frame_data[frameIndex * 3] << 10;

            frameIndex = state->animation_data[4];
            state->transform.current.translation.y = state->frame_data[frameIndex * 3 + 1] << 10;

            frameIndex = state->animation_data[4];
            state->transform.current.translation.z = state->frame_data[frameIndex * 3 + 2] << 10;

            memcpy(&state->transform.previous, &state->transform.current, sizeof(Transform3D));
            state->animation_data += 5;
            state->counter = state->animation_data[3];
        }
    }

    {
        s32 interpCounter;

        interpCounter = state->animation_data[3] * progress;
        if (interpCounter < 0) {
            interpCounter += 0x1FF;
        }
        state->counter = interpCounter >> 9;

        if (state->counter & 0xFFFF) {
            createBoneRotMatrix(state->animation_data[1], state->animation_data[2], state->counter, rotMatrix);
            func_8006BDBC_6C9BC(&state->transform.current, (Transform3D *)rotMatrix, &state->transform.previous);
        }
    }

    {
        u16 frameIndex;
        s32 targetPos;
        s32 currentPos;
        s32 delta;

        /* Interpolate X position */
        frameIndex = state->animation_data[4];
        targetPos = state->frame_data[frameIndex * 3] << 10;
        currentPos = state->transform.current.translation.x;
        delta = (targetPos - currentPos) * progress;
        if (delta < 0) {
            delta += 0x1FF;
        }
        state->transform.previous.translation.x = (delta >> 9) + currentPos;

        /* Interpolate Y position */
        frameIndex = state->animation_data[4];
        targetPos = state->frame_data[frameIndex * 3 + 1] << 10;
        currentPos = state->transform.current.translation.y;
        delta = (targetPos - currentPos) * progress;
        if (delta < 0) {
            delta += 0x1FF;
        }
        state->transform.previous.translation.y = (delta >> 9) + currentPos;

        /* Interpolate Z position */
        frameIndex = state->animation_data[4];
        targetPos = state->frame_data[frameIndex * 3 + 2] << 10;
        delta = (targetPos - state->transform.current.translation.z) * progress;
        if (delta < 0) {
            delta += 0x1FF;
        }
        state->transform.previous.translation.z = (delta >> 9) + state->transform.current.translation.z;
    }
}

void interpolatedBoneAnimationMirrored(BoneAnimationState *state, u16 progress) {
    s16 rotMatrix[16];
    u16 frameIndex;
    s16 frameVal;
    s32 targetPos;
    s32 currentPos;
    s32 delta;

    if (state->flags & 0x8000) {
        state->flags &= 0x7FFF;
        createBoneRotMatrix(
            state->animation_data[1],
            -state->animation_data[2],
            -state->animation_data[3],
            state->transform.current.m[0]
        );

        frameIndex = state->animation_data[4];
        frameVal = state->frame_data[frameIndex * 3];
        state->transform.current.translation.x = -frameVal << 10;

        frameIndex = state->animation_data[4];
        frameVal = state->frame_data[frameIndex * 3 + 1];
        state->transform.current.translation.y = frameVal << 10;

        frameIndex = state->animation_data[4];
        frameVal = state->frame_data[frameIndex * 3 + 2];
        state->transform.current.translation.z = frameVal << 10;

        memcpy(&state->transform.previous, &state->transform.current, sizeof(Transform3D));
        state->animation_data += 5;
        state->counter = state->animation_data[3];
    }

    delta = state->animation_data[3] * (progress & 0xFFFF);
    if (delta < 0) {
        delta += 0x1FF;
    }
    state->counter = delta >> 9;

    if (state->counter & 0xFFFF) {
        createBoneRotMatrix(state->animation_data[1], -state->animation_data[2], -state->counter, rotMatrix);
        func_8006BDBC_6C9BC(&state->transform.current, (Transform3D *)rotMatrix, &state->transform.previous);
    }

    frameIndex = state->animation_data[4];
    targetPos = -state->frame_data[frameIndex * 3] << 10;
    currentPos = state->transform.current.translation.x;
    delta = (targetPos - currentPos) * progress;
    if (delta < 0) {
        delta += 0x1FF;
    }
    state->transform.previous.translation.x = (delta >> 9) + currentPos;

    frameIndex = state->animation_data[4];
    targetPos = state->frame_data[frameIndex * 3 + 1] << 10;
    currentPos = state->transform.current.translation.y;
    delta = (targetPos - currentPos) * progress;
    if (delta < 0) {
        delta += 0x1FF;
    }
    state->transform.previous.translation.y = (delta >> 9) + currentPos;

    frameIndex = state->animation_data[4];
    targetPos = state->frame_data[frameIndex * 3 + 2] << 10;
    delta = (targetPos - state->transform.current.translation.z) * progress;
    if (delta < 0) {
        delta += 0x1FF;
    }
    state->transform.previous.translation.z = (delta >> 9) + state->transform.current.translation.z;
}

s32 advanceBoneAnimation(void *animData, s32 tableIndex, s32 boneIndex, BoneAnimationState *state) {
    s16 stack_data[16];
    u16 flags;
    u16 frame_idx;

    if ((state->flags < 2) && (*(u16 *)state->animation_data == 0)) {
        initBoneAnimationState(animData, (s16)tableIndex, (s16)boneIndex, state);
    }

    flags = state->flags;

    if (flags & 0x8000) {
        s16 *animation_data = state->animation_data;
        state->flags = flags & 0x7FFF;
        createBoneRotMatrix(animation_data[1], animation_data[2], animation_data[3], state->transform.current.m[0]);

        frame_idx = state->animation_data[4];
        state->transform.current.translation.x = state->frame_data[frame_idx * 3] << 10;

        frame_idx = state->animation_data[4];
        state->transform.current.translation.y = state->frame_data[frame_idx * 3 + 1] << 10;

        frame_idx = state->animation_data[4];
        state->transform.current.translation.z = state->frame_data[frame_idx * 3 + 2] << 10;

        memcpy(&state->transform.previous, &state->transform.current, sizeof(Transform3D));

        if (state->flags == 0) {
            goto ret0;
        }
        goto advance_animation;
    }

    if ((flags & 0xFFFF) == 0) {
        goto ret0;
    }

    state->counter = state->counter - (state->counter / flags);

    {
        s16 *animation_data = state->animation_data;
        if (animation_data[3] != (state->counter & 0xFFFF)) {
            createBoneRotMatrix(animation_data[1], animation_data[2], animation_data[3] - state->counter, stack_data);
            func_8006BDBC_6C9BC(&state->transform.current, (Transform3D *)stack_data, &state->transform.previous);
        }
    }

    frame_idx = state->animation_data[4];
    state->transform.previous.translation.x +=
        ((state->frame_data[frame_idx * 3] << 10) - state->transform.previous.translation.x) / (s32)state->flags;

    frame_idx = state->animation_data[4];
    state->transform.previous.translation.y +=
        ((state->frame_data[frame_idx * 3 + 1] << 10) - state->transform.previous.translation.y) / (s32)state->flags;

    frame_idx = state->animation_data[4];
    state->transform.previous.translation.z +=
        ((state->frame_data[frame_idx * 3 + 2] << 10) - state->transform.previous.translation.z) / (s32)state->flags;

    state->flags--;
    if ((state->flags & 0xFFFF) != 0) {
        goto ret0;
    }

    state->flags = state->animation_data[0];
    memcpy(&state->transform.current, &state->transform.previous, sizeof(Transform3D));

    if (state->flags == 0) {
        goto ret0;
    }

advance_animation: {
    s16 *animation_data = state->animation_data;
    state->animation_data = animation_data + 5;
    state->counter = animation_data[8];
}

ret0:
    if (state->flags == 1 && *(u16 *)state->animation_data == 0) {
        return 1;
    }
    return 0;
}

s32 advanceBoneAnimationMirrored(void *animData, s32 tableIndex, s32 boneIndex, void *state_void) {
    BoneAnimationState *state = (BoneAnimationState *)state_void;
    s16 rotMatrix[16];
    u16 flags;
    u16 frameIdx;

    if ((state->flags < 2) && (*(u16 *)state->animation_data == 0)) {
        initBoneAnimationState(animData, (s16)tableIndex, (s16)boneIndex, state);
    }

    flags = state->flags;

    if (flags & 0x8000) {
        s16 *animation_data = state->animation_data;
        state->flags = flags & 0x7FFF;
        createBoneRotMatrix(animation_data[1], -animation_data[2], -animation_data[3], state->transform.current.m[0]);

        frameIdx = state->animation_data[4];
        state->transform.current.translation.x = -state->frame_data[frameIdx * 3] << 10;

        frameIdx = state->animation_data[4];
        state->transform.current.translation.y = state->frame_data[frameIdx * 3 + 1] << 10;

        frameIdx = state->animation_data[4];
        state->transform.current.translation.z = state->frame_data[frameIdx * 3 + 2] << 10;

        memcpy(&state->transform.previous, &state->transform.current, sizeof(Transform3D));

        if (state->flags == 0) {
            goto ret0;
        }
        goto advance_animation;
    }

    if ((flags & 0xFFFF) == 0) {
        goto ret0;
    }

    state->counter = state->counter - (state->counter / flags);

    {
        s16 *animation_data = state->animation_data;
        if (animation_data[3] != (state->counter & 0xFFFF)) {
            createBoneRotMatrix(animation_data[1], -animation_data[2], state->counter - animation_data[3], rotMatrix);
            func_8006BDBC_6C9BC(&state->transform.current, (Transform3D *)rotMatrix, &state->transform.previous);
        }
    }

    frameIdx = state->animation_data[4];
    state->transform.previous.translation.x +=
        ((-state->frame_data[frameIdx * 3] << 10) - state->transform.previous.translation.x) / (s32)state->flags;

    frameIdx = state->animation_data[4];
    state->transform.previous.translation.y +=
        ((state->frame_data[frameIdx * 3 + 1] << 10) - state->transform.previous.translation.y) / (s32)state->flags;

    frameIdx = state->animation_data[4];
    state->transform.previous.translation.z +=
        ((state->frame_data[frameIdx * 3 + 2] << 10) - state->transform.previous.translation.z) / (s32)state->flags;

    state->flags--;
    if ((state->flags & 0xFFFF) != 0) {
        goto ret0;
    }

    state->flags = state->animation_data[0];
    memcpy(&state->transform.current, &state->transform.previous, sizeof(Transform3D));

    if (state->flags == 0) {
        goto ret0;
    }

advance_animation: {
    s16 *animation_data = state->animation_data;
    state->animation_data = animation_data + 5;
    state->counter = animation_data[8];
}

ret0:
    if (state->flags == 1 && *(u16 *)state->animation_data == 0) {
        return 1;
    }
    return 0;
}

void resetBoneAnimation(void *animData, s16 tableIndex, s16 boneIndex, BoneAnimationStateIndexed *state) {
    u16 animFlags;

    state->animation_index = 0;
    animFlags = *getAnimationDataByIndex(animData, tableIndex, boneIndex);
    state->flags = (s16)(animFlags + 0x8000);
}

s32 advanceIndexedBoneAnimation(void *animData, s16 tableIndex, s16 boneIndex, BoneAnimationStateIndexed *state) {
    u16 *animation_data;
    s16 *frame_data;
    Transform3D stack_data;
    u16 idx;
    u16 flags;
    u16 frame_idx;
    u16 temp;

    animation_data = getAnimationDataByIndex(animData, tableIndex, boneIndex);
    frame_data = getAnimationFrameData(animData);
    flags = state->flags;

    if (flags & 0x8000) {
        state->flags = flags & 0x7FFF;

        idx = state->animation_index;
        createBoneRotMatrix(
            animation_data[idx * 5 + 1],
            animation_data[idx * 5 + 2],
            animation_data[idx * 5 + 3],
            state->transform.current.m[0]
        );

        idx = state->animation_index;
        frame_idx = animation_data[idx * 5 + 4];
        state->transform.current.translation.x = frame_data[frame_idx * 3] << 10;

        idx = state->animation_index;
        frame_idx = animation_data[idx * 5 + 4];
        state->transform.current.translation.y = frame_data[frame_idx * 3 + 1] << 10;

        idx = state->animation_index;
        frame_idx = animation_data[idx * 5 + 4];
        state->transform.current.translation.z = frame_data[frame_idx * 3 + 2] << 10;

        memcpy(&state->transform.previous, &state->transform.current, sizeof(Transform3D));

        if (state->flags == 0) {
            goto ret0;
        }
        goto advance_index;
    }

    if (flags == 0) {
        return 1;
    }

    state->counter = state->counter - (state->counter / flags);

    idx = state->animation_index;
    if ((s16)animation_data[idx * 5 + 3] != (state->counter & 0xFFFF)) {
        createBoneRotMatrix(
            animation_data[idx * 5 + 1],
            animation_data[idx * 5 + 2],
            (s16)animation_data[idx * 5 + 3] - state->counter,
            (s16 *)stack_data.m
        );
        func_8006BDBC_6C9BC(&state->transform.current, &stack_data, &state->transform.previous);
    }

    idx = state->animation_index;
    frame_idx = animation_data[idx * 5 + 4];
    state->transform.previous.translation.x +=
        ((frame_data[frame_idx * 3] << 10) - state->transform.previous.translation.x) / state->flags;

    idx = state->animation_index;
    frame_idx = animation_data[idx * 5 + 4];
    state->transform.previous.translation.y +=
        ((frame_data[frame_idx * 3 + 1] << 10) - state->transform.previous.translation.y) / state->flags;

    idx = state->animation_index;
    frame_idx = animation_data[idx * 5 + 4];
    state->transform.previous.translation.z +=
        ((frame_data[frame_idx * 3 + 2] << 10) - state->transform.previous.translation.z) / state->flags;

    state->flags--;
    if ((state->flags & 0xFFFF) != 0) {
        goto ret0;
    }

    idx = state->animation_index;
    state->flags = animation_data[idx * 5];
    memcpy(&state->transform.current, &state->transform.previous, sizeof(Transform3D));

    if (state->flags == 0) {
        return 1;
    }

advance_index:
    temp = state->animation_index + 1;
    state->animation_index = temp;
    state->counter = animation_data[temp * 5 + 3];
ret0:
    return 0;
}

s32 advanceIndexedBoneAnimationMirrored(void *arg0, s16 arg1, s16 arg2, BoneAnimationStateIndexed *state) {
    u16 *animation_data;
    s16 *frame_data;
    s16 stack_data[16];
    u16 idx;
    u16 flags;
    u16 frame_idx;
    u16 temp;

    animation_data = getAnimationDataByIndex(arg0, arg1, arg2);
    frame_data = getAnimationFrameData(arg0);
    flags = state->flags;

    if (flags & 0x8000) {
        state->flags = flags & 0x7FFF;

        idx = state->animation_index;
        createBoneRotMatrix(
            animation_data[idx * 5 + 1],
            -animation_data[idx * 5 + 2],
            -animation_data[idx * 5 + 3],
            state->transform.current.m[0]
        );

        idx = state->animation_index;
        frame_idx = animation_data[idx * 5 + 4];
        state->transform.current.translation.x = -frame_data[frame_idx * 3] << 10;

        idx = state->animation_index;
        frame_idx = animation_data[idx * 5 + 4];
        state->transform.current.translation.y = frame_data[frame_idx * 3 + 1] << 10;

        idx = state->animation_index;
        frame_idx = animation_data[idx * 5 + 4];
        state->transform.current.translation.z = frame_data[frame_idx * 3 + 2] << 10;

        memcpy(&state->transform.previous, &state->transform.current, sizeof(Transform3D));

        if (state->flags == 0) {
            goto ret0;
        }
        goto advance_index;
    }

    if (flags == 0) {
        return 1;
    }

    state->counter = state->counter - (state->counter / flags);

    idx = state->animation_index;
    if ((s16)animation_data[idx * 5 + 3] != (state->counter & 0xFFFF)) {
        createBoneRotMatrix(
            animation_data[idx * 5 + 1],
            -animation_data[idx * 5 + 2],
            state->counter - (s16)animation_data[idx * 5 + 3],
            stack_data
        );
        func_8006BDBC_6C9BC(&state->transform.current, (Transform3D *)stack_data, &state->transform.previous);
    }

    idx = state->animation_index;
    frame_idx = animation_data[idx * 5 + 4];
    state->transform.previous.translation.x +=
        ((-frame_data[frame_idx * 3] << 10) - state->transform.previous.translation.x) / state->flags;

    idx = state->animation_index;
    frame_idx = animation_data[idx * 5 + 4];
    state->transform.previous.translation.y +=
        ((frame_data[frame_idx * 3 + 1] << 10) - state->transform.previous.translation.y) / state->flags;

    idx = state->animation_index;
    frame_idx = animation_data[idx * 5 + 4];
    state->transform.previous.translation.z +=
        ((frame_data[frame_idx * 3 + 2] << 10) - state->transform.previous.translation.z) / state->flags;

    state->flags--;
    if ((state->flags & 0xFFFF) != 0) {
        goto ret0;
    }

    idx = state->animation_index;
    state->flags = animation_data[idx * 5];
    memcpy(&state->transform.current, &state->transform.previous, sizeof(Transform3D));

    if (state->flags == 0) {
        return 1;
    }

advance_index:
    temp = state->animation_index + 1;
    state->animation_index = temp;
    state->counter = animation_data[temp * 5 + 3];
ret0:
    return 0;
}

void interpolateIndexedBoneAnimation(void *arg0, s16 arg1, s16 arg2, BoneAnimationStateIndexed *entity, s16 param_2) {
    s16 *frame_data;
    u16 *animation_data;
    s16 stack_data[16];
    u16 idx;
    s16 temp_val;
    s32 source_val;
    s32 dest_val;
    s32 result;

    animation_data = getAnimationDataByIndex(arg0, arg1, arg2);
    frame_data = getAnimationFrameData(arg0);

    if (entity->flags & 0x8000) {
        entity->flags &= 0x7FFF;

        idx = entity->animation_index;
        createBoneRotMatrix(
            animation_data[idx * 5 + 1],
            animation_data[idx * 5 + 2],
            animation_data[idx * 5 + 3],
            entity->transform.current.m[0]
        );

        idx = entity->animation_index;
        temp_val = frame_data[animation_data[idx * 5 + 4] * 3];
        entity->transform.current.translation.x = temp_val << 10;

        idx = entity->animation_index;
        temp_val = frame_data[animation_data[idx * 5 + 4] * 3 + 1];
        entity->transform.current.translation.y = temp_val << 10;

        idx = entity->animation_index;
        temp_val = frame_data[animation_data[idx * 5 + 4] * 3 + 2];
        entity->transform.current.translation.z = temp_val << 10;

        memcpy(&entity->transform.previous, &entity->transform.current, sizeof(Transform3D));
        entity->animation_index++;
        entity->counter = animation_data[entity->animation_index * 5 + 3];
    }

    idx = entity->animation_index;
    result = (s16)animation_data[idx * 5 + 3] * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->counter = result >> 9;

    if (entity->counter != 0) {
        idx = entity->animation_index;
        createBoneRotMatrix(animation_data[idx * 5 + 1], animation_data[idx * 5 + 2], entity->counter, stack_data);
        func_8006BDBC_6C9BC(&entity->transform.current, (Transform3D *)stack_data, &entity->transform.previous);
    }

    idx = entity->animation_index;
    source_val = frame_data[animation_data[idx * 5 + 4] * 3] << 10;
    dest_val = entity->transform.current.translation.x;
    result = (source_val - dest_val) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->transform.previous.translation.x = (result >> 9) + dest_val;

    idx = entity->animation_index;
    source_val = frame_data[animation_data[idx * 5 + 4] * 3 + 1] << 10;
    dest_val = entity->transform.current.translation.y;
    result = (source_val - dest_val) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->transform.previous.translation.y = (result >> 9) + dest_val;

    idx = entity->animation_index;
    source_val = frame_data[animation_data[idx * 5 + 4] * 3 + 2] << 10;
    result = (source_val - entity->transform.current.translation.z) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->transform.previous.translation.z = (result >> 9) + entity->transform.current.translation.z;
}

void interpolateIndexedBoneAnimationMirrored(
    void *arg0,
    s16 arg1,
    s16 arg2,
    BoneAnimationStateIndexed *entity,
    u16 param_2
) {
    u16 *animation_data;
    s16 *frame_data;
    s16 stack_data[16];
    u16 idx;
    s16 temp_val;
    s32 source_val;
    s32 dest_val;
    s32 result;

    animation_data = getAnimationDataByIndex(arg0, arg1, arg2);
    frame_data = getAnimationFrameData(arg0);

    if (entity->flags & 0x8000) {
        entity->flags &= 0x7FFF;

        idx = entity->animation_index;
        createBoneRotMatrix(
            animation_data[idx * 5 + 1],
            -animation_data[idx * 5 + 2],
            -animation_data[idx * 5 + 3],
            entity->transform.current.m[0]
        );

        idx = entity->animation_index;
        temp_val = frame_data[animation_data[idx * 5 + 4] * 3];
        entity->transform.current.translation.x = -temp_val << 10;

        idx = entity->animation_index;
        temp_val = frame_data[animation_data[idx * 5 + 4] * 3 + 1];
        entity->transform.current.translation.y = temp_val << 10;

        idx = entity->animation_index;
        temp_val = frame_data[animation_data[idx * 5 + 4] * 3 + 2];
        entity->transform.current.translation.z = temp_val << 10;

        memcpy(&entity->transform.previous, &entity->transform.current, sizeof(Transform3D));
        entity->animation_index++;
        entity->counter = animation_data[entity->animation_index * 5 + 3];
    }

    idx = entity->animation_index;
    result = (s16)animation_data[idx * 5 + 3] * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->counter = result >> 9;

    if (entity->counter != 0) {
        idx = entity->animation_index;
        createBoneRotMatrix(animation_data[idx * 5 + 1], -animation_data[idx * 5 + 2], -entity->counter, stack_data);
        func_8006BDBC_6C9BC(&entity->transform.current, (Transform3D *)stack_data, &entity->transform.previous);
    }

    idx = entity->animation_index;
    source_val = -frame_data[animation_data[idx * 5 + 4] * 3] << 10;
    dest_val = entity->transform.current.translation.x;
    result = (source_val - dest_val) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->transform.previous.translation.x = (result >> 9) + dest_val;

    idx = entity->animation_index;
    source_val = frame_data[animation_data[idx * 5 + 4] * 3 + 1] << 10;
    dest_val = entity->transform.current.translation.y;
    result = (source_val - dest_val) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->transform.previous.translation.y = (result >> 9) + dest_val;

    idx = entity->animation_index;
    source_val = frame_data[animation_data[idx * 5 + 4] * 3 + 2] << 10;
    result = (source_val - entity->transform.current.translation.z) * param_2;
    if (result < 0) {
        result += 0x1FF;
    }
    entity->transform.previous.translation.z = (result >> 9) + entity->transform.current.translation.z;
}

s32 advanceIndexedBoneAnimationAuto(void *arg0, s16 arg1, s16 arg2, BoneAnimationStateIndexed *state) {
    u16 *animation_data;
    s16 *frame_data;
    s16 stack_data[16];
    u16 idx;
    u16 frame_idx;
    u16 flags;
    s16 diff;

    animation_data = getAnimationDataByIndex(arg0, arg1, arg2);
    frame_data = getAnimationFrameData(arg0);

    if ((state->flags < 2) && (animation_data[state->animation_index * 5] == 0)) {
        resetBoneAnimation(arg0, arg1, arg2, state);
        animation_data = getAnimationDataByIndex(arg0, arg1, arg2);
    }

    flags = state->flags;
    if (flags & 0x8000) {
        state->flags = flags & 0x7FFF;

        idx = state->animation_index;
        createBoneRotMatrix(
            animation_data[idx * 5 + 1],
            animation_data[idx * 5 + 2],
            animation_data[idx * 5 + 3],
            state->transform.current.m[0]
        );

        idx = state->animation_index;
        frame_idx = animation_data[idx * 5 + 4];
        state->transform.current.translation.x = frame_data[frame_idx * 3] << 10;

        idx = state->animation_index;
        frame_idx = animation_data[idx * 5 + 4];
        state->transform.current.translation.y = frame_data[frame_idx * 3 + 1] << 10;

        idx = state->animation_index;
        frame_idx = animation_data[idx * 5 + 4];
        state->transform.current.translation.z = frame_data[frame_idx * 3 + 2] << 10;

        memcpy(&state->transform.previous, &state->transform.current, sizeof(Transform3D));

        if (state->flags == 0) {
            goto ret0;
        }
        goto advance_index;
    }

    if (flags == 0) {
        goto ret0;
    }

    state->counter = state->counter - (state->counter / flags);

    idx = state->animation_index;
    diff = (s16)animation_data[idx * 5 + 3];
    if (diff != (state->counter & 0xFFFF)) {
        createBoneRotMatrix(
            animation_data[idx * 5 + 1],
            animation_data[idx * 5 + 2],
            diff - state->counter,
            stack_data
        );
        func_8006BDBC_6C9BC(&state->transform.current, (Transform3D *)stack_data, &state->transform.previous);
    }

    idx = state->animation_index;
    frame_idx = animation_data[idx * 5 + 4];
    state->transform.previous.translation.x +=
        ((frame_data[frame_idx * 3] << 10) - state->transform.previous.translation.x) / state->flags;

    idx = state->animation_index;
    frame_idx = animation_data[idx * 5 + 4];
    state->transform.previous.translation.y +=
        ((frame_data[frame_idx * 3 + 1] << 10) - state->transform.previous.translation.y) / state->flags;

    idx = state->animation_index;
    frame_idx = animation_data[idx * 5 + 4];
    state->transform.previous.translation.z +=
        ((frame_data[frame_idx * 3 + 2] << 10) - state->transform.previous.translation.z) / state->flags;

    state->flags--;
    if ((state->flags & 0xFFFF) != 0) {
        goto ret0;
    }

    idx = state->animation_index;
    state->flags = animation_data[idx * 5];
    memcpy(&state->transform.current, &state->transform.previous, sizeof(Transform3D));

    if (state->flags == 0) {
        goto ret0;
    }

advance_index:
    state->animation_index = state->animation_index + 1;
    state->counter = animation_data[state->animation_index * 5 + 3];

ret0:
    if (state->flags == 1 && animation_data[state->animation_index * 5] == 0) {
        return 1;
    }
    return 0;
}

s32 advanceIndexedBoneAnimationAutoMirrored(void *arg0, s16 arg1, s16 arg2, BoneAnimationStateIndexed *state) {
    u16 *animation_data;
    s16 *frame_data;
    s16 stack_data[16];
    u16 idx;
    u16 frame_idx;
    u16 flags;
    s16 diff;

    animation_data = getAnimationDataByIndex(arg0, arg1, arg2);
    frame_data = getAnimationFrameData(arg0);

    if ((state->flags < 2) && (animation_data[state->animation_index * 5] == 0)) {
        resetBoneAnimation(arg0, arg1, arg2, state);
        animation_data = getAnimationDataByIndex(arg0, arg1, arg2);
    }

    flags = state->flags;
    if (flags & 0x8000) {
        state->flags = flags & 0x7FFF;

        idx = state->animation_index;
        createBoneRotMatrix(
            animation_data[idx * 5 + 1],
            -(s16)animation_data[idx * 5 + 2],
            -(s16)animation_data[idx * 5 + 3],
            state->transform.current.m[0]
        );

        idx = state->animation_index;
        frame_idx = animation_data[idx * 5 + 4];
        state->transform.current.translation.x = -frame_data[frame_idx * 3] << 10;

        idx = state->animation_index;
        frame_idx = animation_data[idx * 5 + 4];
        state->transform.current.translation.y = frame_data[frame_idx * 3 + 1] << 10;

        idx = state->animation_index;
        frame_idx = animation_data[idx * 5 + 4];
        state->transform.current.translation.z = frame_data[frame_idx * 3 + 2] << 10;

        memcpy(&state->transform.previous, &state->transform.current, sizeof(Transform3D));

        if (state->flags == 0) {
            goto ret0;
        }
        goto advance_index;
    }

    if (flags == 0) {
        goto ret0;
    }

    state->counter = state->counter - (state->counter / flags);

    idx = state->animation_index;
    diff = (s16)animation_data[idx * 5 + 3];
    if (diff != (state->counter & 0xFFFF)) {
        createBoneRotMatrix(
            animation_data[idx * 5 + 1],
            -(s16)animation_data[idx * 5 + 2],
            state->counter - diff,
            stack_data
        );
        func_8006BDBC_6C9BC(&state->transform.current, (Transform3D *)stack_data, &state->transform.previous);
    }

    idx = state->animation_index;
    frame_idx = animation_data[idx * 5 + 4];
    state->transform.previous.translation.x +=
        ((-frame_data[frame_idx * 3] << 10) - state->transform.previous.translation.x) / state->flags;

    idx = state->animation_index;
    frame_idx = animation_data[idx * 5 + 4];
    state->transform.previous.translation.y +=
        ((frame_data[frame_idx * 3 + 1] << 10) - state->transform.previous.translation.y) / state->flags;

    idx = state->animation_index;
    frame_idx = animation_data[idx * 5 + 4];
    state->transform.previous.translation.z +=
        ((frame_data[frame_idx * 3 + 2] << 10) - state->transform.previous.translation.z) / state->flags;

    state->flags--;
    if ((state->flags & 0xFFFF) != 0) {
        goto ret0;
    }

    idx = state->animation_index;
    state->flags = animation_data[idx * 5];
    memcpy(&state->transform.current, &state->transform.previous, sizeof(Transform3D));

    if (state->flags == 0) {
        goto ret0;
    }

advance_index:
    state->animation_index = state->animation_index + 1;
    state->counter = animation_data[state->animation_index * 5 + 3];

ret0:
    if (state->flags == 1 && animation_data[state->animation_index * 5] == 0) {
        return 1;
    }
    return 0;
}

typedef struct {
    s32 baseOffset;
    s32 offsetTable[0];
} IndexedAnimationDataLayout;

typedef struct {
    u16 padding;
    u16 index;
} IndexedAnimationDataEntry;

s32 getIndexedAnimationDataPtr(IndexedAnimationDataLayout *data, s16 index) {
    s32 baseOffset;
    s32 targetOffset;
    IndexedAnimationDataEntry *target;
    s32 indexValue;
    IndexedAnimationDataLayout *baseData;

    targetOffset = data->offsetTable[index + 1];
    target = (IndexedAnimationDataEntry *)(((s8 *)data) + targetOffset);
    baseOffset = data->baseOffset;
    indexValue = target->index;
    baseData = (IndexedAnimationDataLayout *)(((s8 *)data) + baseOffset);
    return (s32)(((s8 *)baseData) + (indexValue << 1));
}

s32 getAnimationBoneCount(AnimationBoneCountTable *table, s32 index) {
    u8 *base = (u8 *)table;
    u32 offset = *(u32 *)(base + ((index << 16) >> 14) + sizeof(table->header));
    return *(u16 *)(base + offset);
}
