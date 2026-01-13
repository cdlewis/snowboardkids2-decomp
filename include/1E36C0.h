#pragma once

#include "common.h"
#include "cutscene/cutscene_sys2.h"
#include "1DFAA0.h"
#include "1E2BE0.h"

typedef struct {
    u8 pad[0x54];
    s32 scaleCurrentX;
    s32 scaleCurrentY;
    s32 scaleCurrentZ;
    s32 scaleTargetX;
    s32 scaleTargetY;
    s32 scaleTargetZ;
} CutsceneSlotScaleData;

s16 getSlotMoveDuration(cutsceneSys2Wait_exec_asset *slot);
s16 calcAngleDiff(CutsceneSlotData *unused, s16 direction, s16 targetAngle, s16 currentAngle);
void initSlotData(CutsceneSlotData *slot);
s32 setupSlotTransform(CutsceneSlotData *slot);
s32 syncModelFromSlot(void *, void *);
void setSlotScale(CutsceneSlotScaleData *slot, s32 scaleX, s32 scaleY, s32 scaleZ);
void handleSlotDebugInput(CutsceneSlotData *slot, func_800B5E64_1E2F14_arg0 *camera);
void updateSlotRotVelocity(CutsceneSlotData *slot, s16 speedMode);
void updateSlotRotVelocityFixed(CutsceneSlotData *slot);
void initSlotPosition(CutsceneSlotData *slot, s32 x, s32 y, s32 z, s16 rotY);
void initSlotPositionEx(CutsceneSlotData *slot, s32 x, s32 y, s32 z, s16 rotY, s16 rotX, s16 rotZ);
s32 setupSlotMoveTo(CutsceneSlotData *slot, SceneModel *model, s32 targetX, s32 targetY, s32 targetZ, s16 duration, s16 fallbackRotY);
s32 setupSlotMoveToEx(CutsceneSlotData *slot, SceneModel *model, s32 targetX, s32 targetY, s32 targetZ, s16 duration, s16 fallbackRotY, s32 moveMode, s32 decelRate);
s32 setupSlotMoveToWithRotation(CutsceneSlotData *slot, SceneModel *unused, s32 targetX, s32 targetY, s32 targetZ, s16 duration, s16 targetRotY);
void setupSlotWalkTo(CutsceneSlotData *slot, SceneModel *model, s32 targetX, s32 targetY, s32 targetZ, s16 duration, s16 finalRotY, s16 walkAnim, s8 turnAnimFlag, s8 decelMode);
s32 setupSlotRotateTo(CutsceneSlotData *slot, SceneModel *unused, s16 targetRotY);
s32 setupSlotRotateToWithDir(CutsceneSlotData *slot, SceneModel *unused, s16 targetRotY, s16 direction, s16 duration);
s32 setupSlotRotateWithSpeed(CutsceneSlotData *slot, SceneModel *model, s16 targetRotY, s32 speedMult, s32 duration, s32 direction);
void setupSlotOrbit(CutsceneSlotData *slot, s32 orbitDir, s16 duration, s16 orbitSpeed);
void interpolateSlotScaleX(CutsceneSlotData *slot, s32 targetScaleX, s16 duration);
void interpolateSlotScaleY(CutsceneSlotData *slot, s32 targetScaleY, s16 duration);
void interpolateSlotScaleZ(CutsceneSlotData *slot, s32 targetScaleZ, s16 duration);
void setupSlotProjectile(CutsceneSlotData *slot, s32 speed, s32 velY, s32 gravity);
void setupSlotMoveToFacing(CutsceneSlotData *slot, s32 targetX, s32 targetY, s32 targetZ, s32 duration);
void setupSlotMoveToNoRotation(CutsceneSlotData *slot, SceneModel *unused, s32 targetX, s32 targetY, s32 targetZ, s16 duration);
void setupSlotMoveToWithBounce(CutsceneSlotData *slot, s32 *targetPos, s16 duration, s32 bounceVelY, s32 gravity);
s16 updateSlotLinearMove(CutsceneSlotData *slot, SceneModel *model);
s32 updateSlotRotation(CutsceneSlotData *slot, StateEntry *state);
s16 updateSlotWalk(CutsceneSlotData *slot, SceneModel *model);
s32 updateSlotDecelMove(CutsceneSlotData *slot, SceneModel *model);
s32 updateSlotOrbit(CutsceneSlotData *slot, SceneModel *model);
void updateSlotScale(CutsceneSlotData *slot);
s32 updateSlotProjectile(CutsceneSlotData *slot);
s32 updateSlotProjectileTimed(CutsceneSlotData *slot);
