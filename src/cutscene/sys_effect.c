#include "cutscene/sys_effect.h"
#include "1E1BA0.h"
#include "1E2070.h"
#include "B040.h"
#include "C570.h"
#include "geometry.h"

extern u8 identityMatrix[];

typedef struct {
    union {
        Transform3D mat;
        struct {
            s16 padding[10];
            s32 x, y, z;
        } vec;
    } u;
} MatrixWithVec;

void cutsceneEffectDisp_init(void) {
}

s32 cutsceneEffectDisp_validate(void) {
    return 0;
}

void cutsceneEffectDisp_exec(cutsceneEffectDisp_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    Node_70B00 *uiResource;
    s16 slotIdx;
    s16 posX;
    s16 posY;
    CutsceneSlot *slots;

    uiResource = arg1->uiResource;
    posX = arg0->posX;
    posY = arg0->posY;
    slotIdx = uiResource->slot_index;

    slots = (CutsceneSlot *)arg1;

    spawnSpriteEffectInternal(
        slots[arg2].slotData.unkA4.ptr,
        slotIdx,
        posX,
        posY,
        arg0,
        (s32)(arg0->scale << 16) / 100,
        arg0->effectType,
        1,
        0,
        arg0->param1,
        arg0->param2
    );
}

s32 cutsceneEffectDisp_isDone(void) {
    return 0;
}

void cutsceneEffectFan_init(void) {
}

s32 cutsceneEffectFan_validate(void) {
    return 0;
}

void cutsceneEffectFan_exec(cutsceneEffectFan_exec_arg *arg0, CutsceneSlot *arg1, s8 arg2) {
    spawnFanEffect(arg1[arg2].slotData.unkA4.ptr, arg0->displayDuration);
}

s32 cutsceneEffectFan_isDone(void) {
    return 0;
}

void cutsceneEffectRocket_init(void) {
}

s32 cutsceneEffectRocket_validate(void) {
    return 0;
}

void cutsceneEffectRocket_exec(cutsceneEffectRocket_exec_arg *arg0, CutsceneSlot *arg1, s8 arg2) {
    spawnRocketEffect(arg1[arg2].slotData.unkA4.ptr, arg0->displayDuration);
}

s32 cutsceneEffectRocket_isDone(void) {
    return 0;
}

void cutsceneEffectMLight_init(void) {
}

s32 cutsceneEffectMLight_validate(void) {
    return 0;
}

void cutsceneEffectMLight_exec(cutsceneEffectMLight_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    Transform3D sp10;
    Transform3D sp30;
    MatrixWithVec sp50;
    s32 sp70[3];
    CutsceneSlot *slot;
    UIResource *resource;
    Transform3D *rotMatrix;

    slot = &arg1->slots[arg2];
    rotMatrix = &sp10;
    memcpy(rotMatrix, identityMatrix, 0x20);
    memcpy(&sp30, identityMatrix, 0x20);
    memcpy(&sp50, identityMatrix, 0x20);

    memcpy(&sp30, (u8 *)slot->model + 0x18, 0x20);

    transformVector((s16 *)&arg0->positionVec, (s16 *)&sp30, sp70);

    switch (arg0->effectMode) {
        case 0:
            memcpy(&sp50, &sp30, 0x20);
            sp50.u.vec.x = sp70[0];
            sp50.u.vec.y = sp70[1];
            sp50.u.vec.z = sp70[2];
            resource = *(UIResource **)((u8 *)slot->model + 0x10);
            spawnScrollingTextureEffect(resource->unk16, &sp50, arg0->colorIndex, arg0->effectMode);
            break;
        case 1:
            createZRotationMatrix(rotMatrix, 0x1000);
            func_8006B084_6BC84(rotMatrix, &sp30, &sp50);
            sp50.u.vec.x = sp70[0];
            sp50.u.vec.y = sp70[1];
            sp50.u.vec.z = sp70[2];
            resource = *(UIResource **)((u8 *)slot->model + 0x10);
            spawnTrickBurstEffect(resource->unk16, &sp50, arg0->colorIndex, 0x4000);
            break;
    }
}

s32 cutsceneEffectMLight_isDone(void) {
    return 0;
}

void cutsceneEffectTrick_init(void) {
}

s32 cutsceneEffectTrick_validate(void) {
    return 0;
}

void cutsceneEffectTrick_exec(cutsceneEffectTrick_exec_arg *arg0, CutsceneManager *arg1, s8 arg2) {
    CutsceneSlot *temp_v0 = getCutsceneSlot(arg1, arg2);
    temp_v0->unk0.TrickPayload.frameCount = arg0->frameCount;
    temp_v0->unk0.TrickPayload.effectParam = arg0->effectParam;
    enableSlotUpdate(arg1, arg2);
}

void cutsceneEffectTrick_update(CutsceneManager *arg0, s8 arg1) {
    CutsceneSlot *slot;
    CutsceneSlot *temp_s0;

    slot = &arg0->slots[arg1];
    temp_s0 = getCutsceneSlot(arg0, arg1);

    if (temp_s0->unk0.TrickPayload.frameCount > 0) {
        spawnTrickSpriteEffect(slot->model, temp_s0->unk0.TrickPayload.effectParam);
        temp_s0->unk0.TrickPayload.frameCount -= 1;
    } else {
        disableSlotUpdate(arg0, arg1);
    }
}

s32 cutsceneEffectTrick_isDone(void) {
    return 0;
}

void cutsceneInitTrack_init(void) {
}

s32 cutsceneInitTrack_validate(void) {
    return 0;
}

void cutsceneInitTrack_exec(void) {
}
