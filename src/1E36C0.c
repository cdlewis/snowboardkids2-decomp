#include "1E36C0.h"
#include "1E2BE0.h"
#include "geometry.h"

extern s8 gAnalogStickY;
extern s8 gAnalogStickX;
extern s32 gButtonsPressed;
extern s32 gControllerInputs;

s16 getSlotMoveDuration(cutsceneSys2Wait_exec_asset *arg0) {
    return arg0->unk86;
}

s16 calcAngleDiff(CutsceneSlotData *unused, s16 direction, s16 targetAngle, s16 currentAngle) {
    s16 target;
    s16 current;
    s16 rawDiff;
    s16 maskedDiff;
    s16 savedRaw;
    s16 result;
    s16 adjusted;

    target = targetAngle & 0x1FFF;
    current = currentAngle & 0x1FFF;
    rawDiff = target - current;
    maskedDiff = rawDiff & 0x1FFF;
    result = maskedDiff;
    savedRaw = rawDiff;

    if (direction == 0) {
        goto handle_zero;
    }
    if (direction > 0) {
        goto handle_positive;
    }
    if (direction == -1) {
        goto handle_negative;
    }
    goto end;

handle_positive:
    if (direction == 1) {
        goto handle_positive_one;
    }
    goto end;

handle_zero:
    if (maskedDiff < 0x1001) {
        goto end;
    }
    if (current < target) {
        adjusted = current + 0x2000;
        result = target - adjusted;
    } else {
        result = savedRaw;
    }
    goto end;

handle_positive_one:
    if (current >= target) {
        adjusted = current - 0x2000;
        result = target - adjusted;
    } else {
        result = savedRaw;
    }
    goto end;

handle_negative:
    if (target < current) {
        result = savedRaw;
    } else {
        adjusted = current + 0x2000;
        result = target - adjusted;
    }

end:
    return (s16)(result << 16 >> 16);
}

extern u8 identityMatrix[];

void initSlotData(CutsceneSlotData *slot) {
    memcpy(&slot->unk04, identityMatrix, 0x20);

    slot->unk0.bytes[0] = 0;
    slot->unk20_u.unk20_s32 = 0;
    slot->unk28 = 0;
    slot->unk2C = 0;
    slot->unk30 = 0;
    slot->unk34 = 0;
    slot->unk38 = 0;
    slot->unk3C = 0;
    slot->unk40 = 0;
    slot->unk44 = 0;

    slot->scaleCurrentX = 0x10000;
    slot->scaleCurrentY = 0x10000;
    slot->scaleCurrentZ = 0x10000;
    slot->scaleTargetX = 0x10000;
    slot->scaleTargetY = 0x10000;
    slot->scaleTargetZ = 0x10000;

    slot->scaleVelX = 0;
    slot->scaleVelY = 0;
    slot->scaleVelZ = 0;

    slot->unk78 = 0;
    slot->unk7A = 0;
    slot->unk7C = 0;
    slot->unk84 = 0;
    slot->unk86 = 0;
    slot->unk80 = 0;
    slot->unk82 = 0;

    slot->unk0.bytes[1] = 0;

    slot->finalRotY = 0;
    slot->angle = 0;
}

s32 setupSlotTransform(CutsceneSlotData *slot) {
    Transform3D rotY;
    Transform3D rotZ;
    Transform3D rotX;
    Transform3D tempYZ;
    Transform3D tempXYZ;
    Transform3D scaleMat;
    Transform3D *pScale;
    s16 tilt;
    s16 turnVel;
    s16 zRot;
    s32 scaleX;
    s32 scaleY;
    s32 scaleZ;
    s32 retval;
    s16 sx, sy, sz;

    memcpy(&rotY, identityMatrix, 0x20U);
    memcpy(&rotZ, identityMatrix, 0x20U);
    memcpy(&rotX, identityMatrix, 0x20U);
    memcpy(&tempYZ, identityMatrix, 0x20U);
    memcpy(&tempXYZ, identityMatrix, 0x20U);
    memcpy(&scaleMat, identityMatrix, 0x20U);

    tilt = slot->angle;
    zRot = 0;
    if (tilt != 0) {
        turnVel = slot->unk8A;
        if (turnVel < 0) {
            zRot = tilt;
        } else if (turnVel > 0) {
            zRot = -tilt;
        }
    } else {
        zRot = (s16)(u16)slot->unk82;
    }

    createZRotationMatrix(&rotZ, zRot & 0xFFFF);
    createYRotationMatrix(&rotY, (u16)slot->unk78);
    createXRotationMatrix(rotX.m, (u16)slot->unk80);
    func_8006B084_6BC84(&rotZ, &rotY, &tempXYZ);
    func_8006B084_6BC84(&rotX, &tempXYZ, &tempYZ);

    scaleX = slot->scaleCurrentX;
    pScale = &scaleMat;
    if (scaleX >= 0) {
        goto skip1;
    }
    scaleX += 7;
skip1:
    scaleY = slot->scaleCurrentY;
    sx = (s16)((scaleX << 0xD) >> 0x10);
    if (scaleY >= 0) {
        goto skip2;
    }
    scaleY += 7;
skip2:
    scaleZ = slot->scaleCurrentZ;
    sy = (s16)((scaleY << 0xD) >> 0x10);
    if (scaleZ >= 0) {
        goto skip3;
    }
    scaleZ += 7;
skip3:
    sz = (s16)((scaleZ << 0xD) >> 0x10);

    scaleMatrix(pScale, sx, sy, sz);
    func_8006B084_6BC84(pScale, &tempYZ, &slot->unk04);

    retval = slot->unk20_u.unk20_s32;
    slot->unk04.translation.x = retval;
    slot->unk04.translation.y = slot->unk28;
    slot->unk04.translation.z = slot->unk2C;

    return retval;
}

void setSlotScale(CutsceneSlotScaleData *slot, s32 scaleX, s32 scaleY, s32 scaleZ) {
    slot->scaleTargetX = scaleX;
    slot->scaleCurrentX = scaleX;
    slot->scaleTargetY = scaleY;
    slot->scaleCurrentY = scaleY;
    slot->scaleTargetZ = scaleZ;
    slot->scaleCurrentZ = scaleZ;
}

void handleSlotDebugInput(CutsceneSlotData *slot, func_800B5E64_1E2F14_arg0 *camera) {
    s32 sinVal;
    s32 cosVal;
    s32 inputX;
    s32 inputZ;
    u16 newAngle;
    s16 *pRotX;
    s16 *pRotZ;

    if (((gButtonsPressed & (L_TRIG + R_TRIG)) == (L_TRIG + R_TRIG)) && (gControllerInputs & Z_TRIG)) {
        newAngle = (slot->unk78 + 0x800) & 0x1FFF;
        slot->unk78 = newAngle;
        slot->unk7A = newAngle;
        return;
    }
    if ((gButtonsPressed & (Z_TRIG + R_TRIG)) == (Z_TRIG + R_TRIG)) {
        slot->unk28 = 0;
        slot->unk80 = 0;
        slot->unk82 = 0;
        return;
    }
    if (gButtonsPressed & Z_TRIG) {
        slot->unk78 = slot->unk78 - (gAnalogStickX * 4);
        return;
    }
    if (gButtonsPressed & R_TRIG) {
        slot->unk28 = slot->unk28 + (gAnalogStickY << 12);
        return;
    }
    if (gButtonsPressed & L_TRIG) {
        pRotX = &slot->unk80;
        *pRotX = (u16)(*pRotX + (gAnalogStickY * 4));
        pRotZ = &slot->unk82;
        *pRotZ = (u16)(*pRotZ + (gAnalogStickX * 4));
        return;
    }

    sinVal = approximateSin(camera->unk22) * 4;
    cosVal = (approximateCos(camera->unk22) * 4) >> 8;
    inputX = gAnalogStickX * 16;
    inputZ = -gAnalogStickY * 16;
    sinVal = sinVal >> 8;

    slot->unk20_u.unk20_s32 = slot->unk20_u.unk20_s32 + ((cosVal * inputX) + (sinVal * inputZ));
    slot->unk2C = slot->unk2C + ((-sinVal * inputX) + (cosVal * inputZ));
}

void updateSlotRotVelocity(CutsceneSlotData *slot, s16 speedMode) {
    s32 diff;
    s16 delta;

    diff = calcAngleDiff(slot, 0, slot->unk7A, slot->unk78);

    if (speedMode == 1)
        goto set_aa;

    if (speedMode < 2) {
        if (speedMode == 0) {
            delta = 0x100;
        }
    }
    goto check_diff;

set_aa:
    delta = 0xAA;

check_diff:
    if (diff > 0) {
        slot->unk7C = delta;
        slot->unk78 -= delta;
    } else if (diff < 0) {
        slot->unk7C = -delta;
        slot->unk78 += delta;
    } else {
        slot->unk7C = 0;
    }
}

void updateSlotRotVelocityFixed(CutsceneSlotData *slot) {
    s16 diff;

    diff = calcAngleDiff(slot, 0, slot->unk7A, slot->unk78);

    if (diff > 0) {
        slot->unk7C = 0xAA;
        slot->unk78 -= 0xAA;
    } else if (diff < 0) {
        slot->unk7C = -0xAA;
        slot->unk78 += 0xAA;
    } else {
        slot->unk7C = 0;
    }
}

void initSlotPosition(CutsceneSlotData *slot, s32 x, s32 y, s32 z, s16 rotY) {
    initSlotPositionEx(slot, x, y, z, rotY, 0, 0);
}

void initSlotPositionEx(CutsceneSlotData *slot, s32 x, s32 y, s32 z, s16 rotY, s16 rotX, s16 rotZ) {
    s16 maskedRotY;

    maskedRotY = rotY & 0x1FFF;

    slot->unk20_u.unk20_s32 = x;
    slot->unk30 = x;
    slot->unk0.bytes[0] = 0;
    slot->unk28 = y;
    slot->unk2C = z;
    slot->unk34 = y;
    slot->unk38 = z;
    slot->unk3C = 0;
    slot->unk40 = 0;
    slot->unk44 = 0;
    slot->scaleVelX = 0;
    slot->scaleVelY = 0;
    slot->scaleVelZ = 0;
    slot->unk7C = 0;
    slot->unk84 = 0;
    slot->unk86 = 0;
    slot->unk0.bytes[1] = 0;
    slot->finalRotY = 0;
    slot->angle = 0;
    slot->unk78 = maskedRotY;
    slot->unk7A = maskedRotY;
    slot->unk80 = rotX;
    slot->unk82 = rotZ;

    updateSlotRotVelocity(slot, 0);
}

s32 setupSlotMoveTo(
    CutsceneSlotData *slot,
    SceneModel *model,
    s32 targetX,
    s32 targetY,
    s32 targetZ,
    s16 duration,
    s16 fallbackRotY
) {
    return setupSlotMoveToEx(slot, model, targetX, targetY, targetZ, duration, fallbackRotY, 0, 1);
}

s32 setupSlotMoveToEx(
    CutsceneSlotData *slot,
    SceneModel *model,
    s32 targetX,
    s32 targetY,
    s32 targetZ,
    s16 duration,
    s16 fallbackRotY,
    s32 moveMode,
    s32 decelRate
) {
    s16 temp_v0;
    s16 frames;
    s32 deltaX;
    s32 deltaY;
    s32 deltaZ;
    s32 turnDir;
    s16 angleDiff;
    u16 savedRotVel;
    u16 rotYParam;
    s8 moveModeS8;
    u8 decelRateU8;

    rotYParam = (u16)fallbackRotY;
    moveModeS8 = (s8)moveMode;
    decelRateU8 = (u8)decelRate;
    turnDir = 0;
    temp_v0 = duration + 1;
    frames = temp_v0;
    if ((s32)(temp_v0 << 16) <= 0) {
        frames = 1;
    }
    slot->unk30 = targetX;
    slot->unk34 = targetY;
    slot->unk38 = targetZ;
    slot->unk84 = frames;
    slot->unk86 = frames;
    deltaX = slot->unk30 - slot->unk20_u.unk20_s32;
    deltaY = slot->unk34 - slot->unk28;
    deltaZ = slot->unk38 - slot->unk2C;
    slot->angle = 0;
    if (moveModeS8 == 0) {
        slot->unk0.Two = 1;
        slot->unk3C = deltaX / frames;
        slot->unk40 = deltaY / frames;
        slot->unk44 = deltaZ / frames;
    } else if (moveModeS8 == 1) {
        slot->unk0.Two = 5;
        slot->unk0.bytes[3] = decelRateU8;
        slot->unk48 = deltaX / frames;
        slot->unk4C = deltaY / frames;
        slot->unk50 = deltaZ / frames;
    }
    if ((deltaX == 0) && (deltaZ == 0)) {
        slot->unk7A = (s16)rotYParam;
    } else {
        slot->unk7A = (atan2Fixed(deltaX, deltaZ) + 0x1000) & 0x1FFF;
    }
    angleDiff = calcAngleDiff(slot, 0, slot->unk7A, slot->unk78);

    if ((angleDiff >= 0 ? angleDiff : -angleDiff) >= 0xAAB) {
        turnDir = 2;
        slot->unk78 = ((u16)slot->unk78 + 0x1000) & 0x1FFF;
        if (angleDiff > 0) {
            turnDir = 1;
        }
    }
    updateSlotRotVelocity(slot, 0);
    if (moveModeS8 == 1) {
        savedRotVel = (u16)slot->unk7C;
        slot->unk7C = 0;
        slot->unk7E = (s16)savedRotVel;
    }
    return turnDir;
}

s32 setupSlotMoveToWithRotation(
    CutsceneSlotData *slot,
    SceneModel *unused,
    s32 targetX,
    s32 targetY,
    s32 targetZ,
    s16 duration,
    s16 targetRotY
) {
    s16 temp_v0;
    s16 frames;
    s16 rotVel;

    temp_v0 = duration + 1;
    frames = temp_v0;
    if ((s32)(temp_v0 << 16) <= 0) {
        frames = 1;
    }

    slot->unk30 = targetX;
    slot->unk34 = targetY;
    slot->unk38 = targetZ;

    slot->unk84 = frames;
    slot->unk86 = frames;
    slot->angle = 0;
    slot->unk7A = targetRotY;
    slot->unk0.Two = 1;

    slot->unk3C = (slot->unk30 - slot->unk20_u.unk20_s32) / frames;
    slot->unk40 = (slot->unk34 - slot->unk28) / frames;
    slot->unk44 = (slot->unk38 - slot->unk2C) / frames;

    rotVel = calcAngleDiff(slot, 0, targetRotY, slot->unk78) / frames;
    slot->unk7C = rotVel;
    slot->unk7E = rotVel;

    return 0;
}

void setupSlotWalkTo(
    CutsceneSlotData *slot,
    SceneModel *model,
    s32 targetX,
    s32 targetY,
    s32 targetZ,
    s16 duration,
    s16 finalRotY,
    s16 walkAnim,
    s8 turnAnimFlag,
    s8 decelMode
) {
    volatile s32 *pTargetX;
    volatile s32 *pTargetY;
    volatile s32 *pTargetZ;
    s16 temp_v0;
    s16 frames;
    s32 deltaX;
    s32 deltaZ;
    s32 velX, velY, velZ;
    s32 finalAngleDiff;
    s32 absDiff;

    temp_v0 = duration + 1;
    frames = temp_v0;
    if ((s32)(temp_v0 << 16) <= 0) {
        frames = 1;
    }

    pTargetX = &slot->unk30;
    *pTargetX = targetX;
    deltaX = *pTargetX - slot->unk20_u.unk20_s32;
    velX = deltaX / frames;

    pTargetY = &slot->unk34;
    *pTargetY = targetY;
    velY = (*pTargetY - slot->unk28) / frames;

    pTargetZ = &slot->unk38;
    *pTargetZ = targetZ;
    deltaZ = *pTargetZ - slot->unk2C;
    velZ = deltaZ / frames;

    slot->unk0.bytes[1] = 0;
    slot->unk84 = frames;
    slot->unk86 = frames;
    slot->finalRotY = finalRotY;
    slot->angle = 0;
    slot->walkAnimIndex = walkAnim;
    slot->unk0.bytes[2] = turnAnimFlag;
    slot->unkA4.byte = decelMode;
    slot->unk0.Two = 3;

    slot->unk3C = velX;
    slot->unk40 = velY;
    slot->unk44 = velZ;

    if ((u32)deltaX < 1 && (u32)deltaZ < 1) {
    } else {
        slot->unk7A = (atan2Fixed(deltaX, deltaZ) + 0x1000) & 0x1FFF;
    }

    calcAngleDiff(slot, 0, slot->unk7A, slot->unk78);
    updateSlotRotVelocity(slot, 1);

    finalAngleDiff = calcAngleDiff(slot, 0, slot->finalRotY, slot->unk7A);
    absDiff = finalAngleDiff >= 0 ? finalAngleDiff : -finalAngleDiff;
    if (absDiff >= 0x1001) {
        finalAngleDiff = slot->unk78 - slot->unk7A;
    }

    if (finalAngleDiff > 0) {
        slot->unk8A = 0x55;
        slot->unk8C = (finalAngleDiff >= 0 ? finalAngleDiff : -finalAngleDiff) / 85;
    } else if (finalAngleDiff < 0) {
        slot->unk8A = -0x55;
        slot->unk8C = (finalAngleDiff >= 0 ? finalAngleDiff : -finalAngleDiff) / 85;
    } else {
        slot->unk8C = 0;
    }
}

s32 setupSlotRotateTo(CutsceneSlotData *slot, SceneModel *unused, s16 targetRotY) {
    s32 turnDir;
    s32 angleDiff;
    s16 rotVel;

    turnDir = 0;
    slot->unk7A = targetRotY;
    slot->unk0.Two = 4;
    slot->unk3C = 0;
    slot->unk40 = 0;
    slot->unk44 = 0;
    angleDiff = calcAngleDiff(slot, 0, targetRotY, slot->unk78);

    if ((angleDiff >= 0 ? angleDiff : -angleDiff) >= 0xAAB) {
        turnDir = 2;
        slot->unk78 = ((u16)slot->unk78 + 0x1000) & 0x1FFF;
        if (angleDiff > 0) {
            turnDir = 1;
        }
    }
    updateSlotRotVelocity(slot, 0);
    rotVel = slot->unk7C;
    if (rotVel != 0) {
        slot->unk84 = (angleDiff >= 0 ? angleDiff : -angleDiff) / (rotVel >= 0 ? rotVel : -rotVel);
    } else {
        slot->unk84 = 0;
    }
    slot->unk86 = slot->unk84;
    return turnDir;
}

s32 setupSlotRotateToWithDir(CutsceneSlotData *slot, SceneModel *unused, s16 targetRotY, s16 direction, s16 duration) {
    s32 initialDiff;
    s32 angleDiff;
    s32 turnDir;
    s32 rotVel;
    s32 absRotVel;
    s32 absDiff;
    s16 durationS16;

    turnDir = 0;
    slot->unk0.Two = 4;
    slot->unk3C = 0;
    slot->unk40 = 0;
    slot->unk44 = 0;
    slot->unk7A = targetRotY;
    initialDiff = calcAngleDiff(slot, direction, targetRotY, slot->unk78);

    if ((initialDiff >= 0 ? initialDiff : -initialDiff) >= 0xAAB) {
        turnDir = 2;
        slot->unk78 = ((u16)slot->unk78 + 0x1000) & 0x1FFF;
        if (initialDiff > 0) {
            turnDir = 1;
        }
    }
    angleDiff = calcAngleDiff(slot, 0, slot->unk7A, slot->unk78);
    durationS16 = duration;
    slot->unk84 = duration;
    slot->unk86 = duration;
    if (durationS16 != 0) {
        rotVel = angleDiff / durationS16;
        slot->unk7C = rotVel;
        absRotVel = (s16)rotVel >= 0 ? (s16)rotVel : -(s16)rotVel;
        if (absRotVel >= 0x101) {
            s16 result;
            if ((s16)rotVel != 0) {
                result = (((s32)(rotVel << 0x10) >> 0x1F) & ~0xFF) | 0x100;
            } else {
                result = 0;
            }
            slot->unk7C = result;
            absDiff = angleDiff >= 0 ? angleDiff : -angleDiff;
            if (absDiff < 0) {
                absDiff += 0xFF;
            }
            absDiff = absDiff >> 8;
            slot->unk84 = absDiff;
            slot->unk86 = absDiff;
        }
    } else {
        slot->unk7C = 0;
    }
    return turnDir;
}

s32 setupSlotRotateWithSpeed(
    CutsceneSlotData *slot,
    SceneModel *model,
    s16 targetRotY,
    s32 speedMult,
    s32 duration,
    s32 direction
) {
    s16 durationS16;

    durationS16 = duration;

    slot->unk0.Two = 0xB;
    slot->unk3C = 0;
    slot->unk40 = 0;
    slot->unk44 = 0;
    slot->unk7A = targetRotY;
    slot->unk84 = durationS16;
    slot->unk86 = durationS16;

    if (durationS16 != 0) {
        slot->unk7C = direction * speedMult;
    } else {
        slot->unk7C = 0;
    }

    return 0;
}

void setupSlotOrbit(CutsceneSlotData *slot, s32 orbitDir, s16 duration, s16 orbitSpeed) {
    s32 angle;
    s32 orbitDirLocal;
    s32 cosVal;
    s32 radius;
    s32 radiusAlt;

    slot->unk0.Two = 6;
    slot->unk94 = orbitDir;
    slot->unk92 = orbitSpeed;
    slot->unk84 = duration;
    slot->unk86 = duration;
    angle = ((s16)atan2Fixed(slot->unk20_u.unk20_s32, slot->unk2C) + 0x1000) & 0x1FFF;
    slot->unk9C_u.unk9C_s32 = angle;
    cosVal = approximateCos(angle) << 2;
    if (cosVal == 0) {
        radiusAlt = (slot->unk20_u.unk20_s32 << 8) / ((approximateSin(slot->unk9C_u.s.unk9E) << 2) >> 8);
        radiusAlt = (radiusAlt > 0) ? radiusAlt : -radiusAlt;
        slot->unk98 = radiusAlt;
    } else {
        radius = (slot->unk2C << 8) / (cosVal >> 8);
        radius = (radius > 0) ? radius : -radius;
        slot->unk98 = radius;
    }
    orbitDirLocal = slot->unk94;
    if (orbitDirLocal > 0) {
        slot->unk7A = (slot->unk9C_u.unk9C_s32 + 0x800) & 0x1FFF;
    } else if (orbitDirLocal < 0) {
        slot->unk7A = (slot->unk9C_u.unk9C_s32 - 0x800) & 0x1FFF;
    }
}

void interpolateSlotScaleX(CutsceneSlotData *slot, s32 targetScaleX, s16 duration) {
    s32 diff;
    s32 vel;

    if (duration > 0) {
        diff = targetScaleX - slot->scaleCurrentX;
        vel = diff / duration;
        slot->scaleTargetX = targetScaleX;
        slot->scaleVelX = vel;
    } else {
        slot->scaleCurrentX = targetScaleX;
        slot->scaleTargetX = targetScaleX;
        slot->scaleVelX = 0;
    }
}

void interpolateSlotScaleY(CutsceneSlotData *slot, s32 targetScaleY, s16 duration) {
    if (duration > 0) {
        slot->scaleTargetY = targetScaleY;
        slot->scaleVelY = (targetScaleY - slot->scaleCurrentY) / duration;
    } else {
        slot->scaleCurrentY = targetScaleY;
        slot->scaleTargetY = targetScaleY;
        slot->scaleVelY = 0;
    }
}

void interpolateSlotScaleZ(CutsceneSlotData *slot, s32 targetScaleZ, s16 duration) {
    s32 vel;

    if (duration > 0) {
        vel = targetScaleZ - slot->scaleCurrentZ;
        slot->scaleTargetZ = targetScaleZ;
        slot->scaleVelZ = vel / duration;
    } else {
        slot->scaleCurrentZ = targetScaleZ;
        slot->scaleTargetZ = targetScaleZ;
        slot->scaleVelZ = 0;
    }
}

void setupSlotProjectile(CutsceneSlotData *slot, s32 speed, s32 velY, s32 gravity) {
    s32 sinResult;
    s32 cosResult;
    s32 scaledSpeed;

    slot->unk0.bytes[0] = 7;

    sinResult = approximateSin(slot->unk78);
    scaledSpeed = speed >> 8;
    slot->unk3C = (scaledSpeed * (sinResult >> 4));

    slot->unk40 = velY;

    cosResult = approximateCos(slot->unk78);
    slot->unk44 = (scaledSpeed * (cosResult >> 4));

    slot->unkA0 = gravity;
}

void setupSlotMoveToFacing(CutsceneSlotData *slot, s32 targetX, s32 targetY, s32 targetZ, s32 duration) {
    volatile s32 *pTargetX;
    volatile s32 *pTargetY;
    volatile s32 *pTargetZ;
    s16 temp_v0;
    s16 frames;
    s32 deltaZ;
    s32 deltaX;
    s32 deltaY;
    s32 velX, velY, velZ;

    temp_v0 = duration + 1;
    frames = temp_v0;
    if ((s32)(temp_v0 << 16) <= 0) {
        frames = 1;
    }
    pTargetX = &slot->unk30;
    *pTargetX = targetX;
    deltaX = *pTargetX - slot->unk20_u.unk20_s32;
    velX = deltaX / frames;
    pTargetY = &slot->unk34;
    *pTargetY = targetY;
    deltaY = *pTargetY - slot->unk28;
    velY = deltaY / frames;
    pTargetZ = &slot->unk38;
    *pTargetZ = targetZ;
    deltaZ = *pTargetZ - slot->unk2C;
    velZ = deltaZ / frames;
    slot->unk84 = frames;
    slot->unk86 = frames;
    slot->angle = 0;
    slot->unk0.Two = 8;
    slot->unk3C = velX;
    slot->unk40 = velY;
    slot->unk44 = velZ;
    if ((u32)deltaX < 1 && (u32)deltaZ < 1) {
    } else {
        slot->unk7A = atan2Fixed(deltaX, deltaZ) & 0x1FFF;
    }
    updateSlotRotVelocity(slot, 0);
}

void setupSlotMoveToNoRotation(
    CutsceneSlotData *slot,
    SceneModel *unused,
    s32 targetX,
    s32 targetY,
    s32 targetZ,
    s16 duration
) {
    s16 temp_v0;
    s16 frames;

    temp_v0 = duration + 1;
    frames = temp_v0;
    if ((temp_v0 << 16) <= 0) {
        frames = 1;
    }

    slot->unk30 = targetX;
    slot->unk34 = targetY;
    slot->unk38 = targetZ;

    slot->unk3C = (slot->unk30 - slot->unk20_u.unk20_s32) / frames;
    slot->unk40 = (slot->unk34 - slot->unk28) / frames;
    slot->unk44 = (slot->unk38 - slot->unk2C) / frames;

    slot->unk84 = frames;
    slot->unk86 = frames;
    slot->angle = 0;
    slot->unk7C = 0;
    slot->unk7E = 0;
    slot->unk0.bytes[0] = 1;
    slot->unk7A = slot->unk78;
}

void setupSlotMoveToWithBounce(CutsceneSlotData *slot, s32 *targetPos, s16 duration, s32 bounceVelY, s32 gravity) {
    s16 frames;
    s32 deltaX, deltaZ;

    if (duration == 0) {
        duration = 1;
    }

    frames = duration;

    slot->unk86 = frames;
    slot->unk84 = frames;
    slot->unk0.Two = 10;

    slot->unk30 = targetPos[0];
    slot->unk34 = targetPos[1];
    slot->unk38 = targetPos[2];

    deltaX = targetPos[0] - slot->unk20_u.unk20_s32;
    slot->unk3C = deltaX / frames;
    slot->unk48 = deltaX / frames;

    slot->unk40 = bounceVelY;
    slot->unk4C = bounceVelY;

    deltaZ = targetPos[2] - slot->unk2C;
    slot->unk44 = deltaZ / frames;
    slot->unk50 = deltaZ / frames;

    slot->unkA0 = gravity;
}

INCLUDE_ASM("asm/nonmatchings/1E36C0", func_800B7C48_1E4CF8);

extern s32 func_800B7C48_1E4CF8(CutsceneSlotData *, SceneModel *);

s16 updateSlotLinearMove(CutsceneSlotData *slot, SceneModel *model) {
    s16 angleDiff;
    s16 rotVel;
    s32 moving;

    moving = 0;
    if (slot->unk84 > 0) {
        slot->unk20_u.unk20_s32 += slot->unk3C;
        slot->unk28 += slot->unk40;
        slot->unk2C += slot->unk44;
        slot->unk78 += slot->unk7C;

        angleDiff = calcAngleDiff(slot, 0, slot->unk7A, slot->unk78);
        rotVel = slot->unk7C;

        if (((rotVel > 0) & (angleDiff < 0)) || ((rotVel < 0) & (angleDiff > 0))) {
            slot->unk7C = 0;
            slot->unk78 = slot->unk7A;
        } else if (rotVel == 0) {
            slot->unk78 = slot->unk7A;
        }

        moving = 1;
        slot->unk84--;
    } else {
        slot->unk3C = 0;
        slot->unk40 = 0;
        slot->unk44 = 0;
        slot->unk7C = 0;
        slot->unk78 = slot->unk7A;

        if ((model->unk38 != -1) && (model->unk3E <= 0)) {
            setModelAnimation(model, model->unk38);
        }
        slot->unk0.Two = 0;
    }
    return moving | func_800B7C48_1E4CF8(slot, model);
}

s32 updateSlotRotation(CutsceneSlotData *slot, StateEntry *state) {
    s32 result;
    s16 nextIndex;

    result = 0;
    if (slot->unk84 > 0) {
        slot->unk78 += slot->unk7C;
        slot->unk84--;
        result = 1;
    } else {
        slot->unk7C = 0;
        nextIndex = state->next_index;
        if ((nextIndex == -1) || ((s8)state->commandCategory > 0)) {
            slot->unk0.bytes[0] = 0;
        } else {
            setModelAnimation((SceneModel *)state, nextIndex);
            slot->unk0.bytes[0] = 0;
        }
    }
    slot->unk7A = slot->unk78;
    return result;
}

s16 updateSlotWalk(CutsceneSlotData *arg0, SceneModel *arg1) {
    s16 temp_v0;
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v0_4;
    s16 temp_v0_5;
    s16 temp_v0_6;
    s16 temp_v0_7;
    s16 temp_v0_8;
    s16 temp_v0_9;
    s16 temp_a0;
    s16 temp_a1;
    s16 var_a1;
    s32 var_s2;
    s32 var_v0_angle;
    s32 temp_v1_3;
    s32 var_v0;
    s32 temp_a0_2;
    s32 var_v0_2;
    s32 temp_v1_4;
    s32 var_v0_3;
    s32 temp_a3;
    s32 temp_t0_2;
    s32 temp_t1;
    s8 temp_t0;
    s8 switch_v1;
    s8 byte1_check;
    u16 temp_v1;
    s16 unk84_val;
    s16 unk8C_val;

    var_s2 = 0;
    arg0->unk20_u.unk20_s32 += arg0->unk3C;
    arg0->unk28 += arg0->unk40;
    arg0->unk2C += arg0->unk44;
    temp_t0 = arg0->unk0.bytes[1];

    switch (temp_t0) {
        case 0:
            unk84_val = arg0->unk84;
            unk8C_val = arg0->unk8C;
            if (unk84_val > unk8C_val) {
                temp_v0 = (u16)arg0->unk78 + (u16)arg0->unk7C;
                arg0->unk78 = temp_v0;
                temp_v0_2 = calcAngleDiff(arg0, 0, arg0->unk7A, temp_v0);
                temp_a0 = arg0->unk7C;
                if (((temp_a0 > 0) & (temp_v0_2 < 0)) || ((temp_a0 < 0) & (temp_v0_2 > 0))) {
                    arg0->unk7C = 0;
                    arg0->unk78 = arg0->unk7A;
                } else if (temp_a0 == 0) {
                    temp_v0_3 = (u16)arg0->angle - 0x40;
                    arg0->angle = temp_v0_3;
                    arg0->unk78 = arg0->unk7A;
                    if (temp_v0_3 < 0) {
                        arg0->angle = 0;
                    }
                }
                arg0->unk84 = (u16)arg0->unk84 - 1;
            } else {
                arg0->unk0.bytes[1] = 1;
                arg0->unk78 = arg0->unk7A;
            }
            var_s2 = 1;
            break;
        case 1:
            if (arg0->unk84 > 0) {
                temp_v0_4 = arg0->unk78 + arg0->unk8A;
                arg0->unk78 = (u16)temp_v0_4;
                temp_v0_5 = calcAngleDiff(arg0, 0, arg0->finalRotY, temp_v0_4);
                temp_a1 = arg0->unk8A;
                if (((temp_a1 > 0) & (temp_v0_5 < 0)) || ((temp_a1 < 0) & (temp_v0_5 > 0))) {
                    arg0->unk8A = 0;
                    arg0->unk78 = arg0->finalRotY;
                } else if (arg0->unk84 < 6) {
                    temp_v0_6 = arg0->angle;
                    var_v0_angle = -temp_v0_6;
                    if (var_v0_angle < 0) {
                        var_v0_angle += 0x1F;
                    }
                    temp_v0_7 = temp_v0_6 + (var_v0_angle >> 5);
                    arg0->angle = temp_v0_7;
                    if (temp_v0_7 < 0) {
                        arg0->angle = 0;
                    }
                } else {
                    temp_v0_8 = (u16)arg0->angle + 0x20;
                    arg0->angle = temp_v0_8;
                    if (temp_v0_8 >= 0x201) {
                        arg0->angle = 0x200;
                    }
                }
                var_s2 = 1;
                arg0->unk84 = (u16)arg0->unk84 - 1;
            } else {
                temp_v1 = (u16)arg0->finalRotY;
                arg0->unk0.bytes[0] = 9;
                arg0->unk78 = (s16)temp_v1;
                arg0->unk7A = (s16)temp_v1;
            }
            break;
    }

    switch_v1 = arg0->unkA4.byte;
    switch (switch_v1) {
        case 0:
            if (arg0->unk84 < 6) {
                temp_v1_3 = arg0->unk3C;
                var_v0 = -temp_v1_3;
                if (var_v0 < 0) {
                    var_v0 += 0x1F;
                }
                arg0->unk3C = temp_v1_3 + (var_v0 >> 5);
                temp_a0_2 = arg0->unk40;
                var_v0_2 = -temp_a0_2;
                if (var_v0_2 < 0) {
                    var_v0_2 += 0x1F;
                }
                arg0->unk40 = temp_a0_2 + (var_v0_2 >> 5);
                temp_v1_4 = arg0->unk44;
                var_v0_3 = -temp_v1_4;
                if (var_v0_3 < 0) {
                    var_v0_3 += 0x1F;
                }
                arg0->unk44 = temp_v1_4 + (var_v0_3 >> 5);
            }
            break;
        case 1:
            if (arg0->unk84 < 0x14) {
                temp_a3 = arg0->unk3C;
                temp_t0_2 = arg0->unk40;
                temp_t1 = arg0->unk44;
                arg0->unk3C = temp_a3 + (-temp_a3 / 12);
                arg0->unk40 = temp_t0_2 + (-temp_t0_2 / 12);
                arg0->unk44 = temp_t1 + (-temp_t1 / 12);
            }
            break;
    }

    if (arg0->angle != 0) {
        byte1_check = arg0->unk0.bytes[1];
        if (byte1_check != 0) {
            if (byte1_check == 1) {
                temp_v0_9 = arg0->unk8A;
                if (temp_v0_9 < 0) {
                    var_a1 = 0x77;
                    setModelAnimationQueued(arg1, var_a1, arg1->unk38, arg1->unk3E, arg1->unk3A);
                } else if (temp_v0_9 > 0) {
                    var_a1 = 0x78;
                    setModelAnimationQueued(arg1, var_a1, arg1->unk38, arg1->unk3E, arg1->unk3A);
                }
            }
        } else {
            goto block_51;
        }
    } else if (arg0->unk7C != 0) {
        if (arg0->unk0.bytes[2] != 0) {
            var_a1 = 0x71;
            setModelAnimationQueued(arg1, var_a1, arg1->unk38, arg1->unk3E, arg1->unk3A);
        }
    } else {
    block_51:
        if ((arg1->unk16 != 0x71) || (arg1->unk95 != 0)) {
            var_a1 = arg0->walkAnimIndex;
            setModelAnimationQueued(arg1, var_a1, arg1->unk38, arg1->unk3E, arg1->unk3A);
        }
    }

    return (s16)(var_s2 | func_800B7C48_1E4CF8(arg0, arg1));
}

s32 updateSlotDecelMove(CutsceneSlotData *slot, SceneModel *model) {
    s32 absX;
    s32 absY;
    s32 absZ;
    s16 animIndex;
    s32 delta;
    s16 angleDiff;
    s16 temp7C;

    if (slot->unk84 != 0) {
        goto update_position;
    }

    slot->unk50 = 0;
    slot->unk4C = 0;

    absX = ABS(slot->unk3C);

    slot->unk48 = 0;

    if (absX >= 0x2000) {
        goto update_position;
    }

    absY = slot->unk40;
    if (absY < 0) {
        absY = -absY;
    }
    if (absY >= 0x2000) {
        goto update_position;
    }

    absZ = slot->unk44;
    if (absZ < 0) {
        absZ = -absZ;
    }
    if (absZ >= 0x2000) {
        goto update_position;
    }

    animIndex = model->unk38;
    if (animIndex != -1 && model->unk3E <= 0) {
        setModelAnimation(model, animIndex);
    }

    slot->unk0.Two = 0;
    return 0;

update_position:
    slot->unk3C += (slot->unk48 - slot->unk3C) / slot->unk0.bytes[3];
    slot->unk40 += (slot->unk4C - slot->unk40) / slot->unk0.bytes[3];
    slot->unk44 += (slot->unk50 - slot->unk44) / slot->unk0.bytes[3];

    slot->unk7C += (slot->unk7E - slot->unk7C) / 32;

    slot->unk20_u.unk20_s32 += slot->unk3C;
    slot->unk28 += slot->unk40;
    slot->unk2C += slot->unk44;
    slot->unk78 += slot->unk7C;

    angleDiff = calcAngleDiff(slot, 0, slot->unk7A, slot->unk78);

    if ((slot->unk7C > 0 && angleDiff < 0) || (slot->unk7C < 0 && angleDiff > 0)) {
        slot->unk7C = 0;
        slot->unk78 = slot->unk7A;
    } else if (slot->unk7C == 0) {
        slot->unk78 = slot->unk7A;
    }

    if (slot->unk84 > 0) {
        slot->unk84--;
    }

    return 1;
}

s32 updateSlotOrbit(CutsceneSlotData *slot, SceneModel *model) {
    s32 newAngle;
    s32 cosVal;
    s32 sinVal;
    s32 scaledCos;
    s32 scaledSin;
    s32 angleDiff;
    s32 *anglePtr = &slot->unk9C_u.unk9C_s32;
    s16 animIndex;

    if (slot->unk84 > 0 || slot->unk86 == -1) {
        sinVal = *anglePtr;
        *anglePtr = (newAngle = sinVal + slot->unk94);
        cosVal = approximateCos((s16)newAngle);
        scaledCos = cosVal << 2;
        scaledCos = scaledCos >> 8;
        sinVal = approximateSin(slot->unk9C_u.s.unk9E);
        scaledSin = (sinVal << 2) >> 8;

        slot->unk2C = scaledCos * (slot->unk98 >> 8);
        slot->unk20_u.unk20_s32 = scaledSin * (slot->unk98 >> 8);

        if (slot->unk94 > 0) {
            newAngle = *anglePtr + 0x800;
            slot->unk7A = newAngle & 0x1FFF;
        } else if (slot->unk94 < 0) {
            newAngle = *anglePtr - 0x800;
            slot->unk7A = newAngle & 0x1FFF;
        }

        slot->unk78 += slot->unk7C;
        angleDiff = calcAngleDiff(slot, 0, slot->unk7A, slot->unk78);

        if ((slot->unk7C > 0 && angleDiff < 0) || (slot->unk7C < 0 && angleDiff > 0)) {
            slot->unk7C = 0;
            slot->unk78 = slot->unk7A;
        } else if (slot->unk7C == 0) {
            slot->unk78 = slot->unk7A;
        }
    } else {
        animIndex = model->unk38;
        if (animIndex != -1) {
            setModelAnimation(model, animIndex);
        }
        slot->unk0.bytes[0] = 0;
    }

    if (slot->unk84 > 0) {
        slot->unk84--;
    }

    return 1;
}

void updateSlotScale(CutsceneSlotData *slot) {
    s32 vel;

    vel = slot->scaleVelX;
    if (vel > 0) {
        slot->scaleCurrentX += vel;
        if (slot->scaleCurrentX > slot->scaleTargetX) {
            slot->scaleCurrentX = slot->scaleTargetX;
            slot->scaleVelX = 0;
        }
    } else if (vel < 0) {
        slot->scaleCurrentX += vel;
        if (slot->scaleCurrentX < slot->scaleTargetX) {
            slot->scaleCurrentX = slot->scaleTargetX;
            slot->scaleVelX = 0;
        }
    }

    vel = slot->scaleVelY;
    if (vel > 0) {
        slot->scaleCurrentY += vel;
        if (slot->scaleCurrentY > slot->scaleTargetY) {
            slot->scaleCurrentY = slot->scaleTargetY;
            slot->scaleVelY = 0;
        }
    } else if (vel < 0) {
        slot->scaleCurrentY += vel;
        if (slot->scaleCurrentY < slot->scaleTargetY) {
            slot->scaleCurrentY = slot->scaleTargetY;
            slot->scaleVelY = 0;
        }
    }

    vel = slot->scaleVelZ;
    if (vel > 0) {
        slot->scaleCurrentZ += vel;
        if (slot->scaleCurrentZ > slot->scaleTargetZ) {
            slot->scaleCurrentZ = slot->scaleTargetZ;
            slot->scaleVelZ = 0;
        }
    } else if (vel < 0) {
        slot->scaleCurrentZ += vel;
        if (slot->scaleCurrentZ < slot->scaleTargetZ) {
            slot->scaleCurrentZ = slot->scaleTargetZ;
            slot->scaleVelZ = 0;
        }
    }
}

s32 updateSlotProjectile(CutsceneSlotData *slot) {
    slot->unk20_u.unk20_s32 += slot->unk3C;
    slot->unk28 += slot->unk40;
    slot->unk2C += slot->unk44;

    if (slot->unk40 < 0) {
        if (slot->unk28 < 0) {
            slot->unk3C = 0;
            slot->unk40 = 0;
            slot->unk44 = 0;
            slot->unk28 = 0;
            slot->unkA0 = 0;
        }
    }

    slot->unk40 += slot->unkA0;
    return 1;
}

s32 updateSlotProjectileTimed(CutsceneSlotData *slot) {
    slot->unk20_u.unk20_s32 += slot->unk3C;
    slot->unk28 += slot->unk40;
    slot->unk2C += slot->unk44;

    if (slot->unk40 < 0) {
        if (slot->unk28 < 0) {
            slot->unk40 = 0;
            slot->unk28 = 0;
            slot->unkA0 = 0;
        }
    }

    slot->unk40 += slot->unkA0;

    if (slot->unk84 == 0) {
        slot->unk0.bytes[0] = 0;
    } else {
        slot->unk84--;
    }

    return 1;
}

INCLUDE_ASM("asm/nonmatchings/1E36C0", syncModelFromSlot);
