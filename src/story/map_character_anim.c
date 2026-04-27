#include "story/map_character_anim.h"
#include "common.h"
#include "core/session_manager.h"
#include "gamestate.h"
#include "graphics/graphics.h"
#include "math/geometry.h"
#include "system/task_scheduler.h"
#include "triggers/town_collision.h"

typedef struct {
    s16 x;
    s16 z;
} CoordPair;

typedef struct {
    s16 viewMatrix[9];
    u8 pad12[0x2];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s16 orientMatrix[9];
    u8 pad32[0x2];
    s32 cameraX;
    s32 cameraY;
    s32 cameraZ;
    u8 pad40[0x4];
    s16 viewAngle;
    s16 targetAngle;
    s16 orbitAngle;
    u8 pad4A[0x2];
    s32 orbitRadius;
    s16 unk50;
    s32 travelDistance;
    u8 pad58[0x2];
    s8 speedH;
    s8 speedV;
} StoryMapCameraState;

extern u8 storyMapLocationIndex;
extern CoordPair storyMapLocationCoords[];
extern s8 gAnalogStickX[];
extern s8 gAnalogStickY[];
extern s32 gControllerInputs[];

void initStoryMapCamera(StoryMapCameraState *arg0);
void updateStoryMapCameraFreeRoam(StoryMapCameraState *camera);
void updateStoryMapCameraOrbit(StoryMapCameraState *arg0);
void initStoryMapCameraAtLocation(StoryMapCameraState *arg0);
void approachStoryMapOrigin(StoryMapCameraState *camera);
void startStoryMapCameraTravel(StoryMapCameraState *camera);
void updateStoryMapCameraTravel(StoryMapCameraState *camera);
void finalizeStoryMapCameraTravel(void);
void storyMapCameraTravelComplete(void);
void finalizeStoryMapExit(void);
void storyMapExitComplete(void);
s32 checkStoryMapLocationSelection(void *);
void updateStoryMapDialogueTurn(void *);

void storyMapCameraTask(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (state->unk429 == 0) {
        setCallback(&initStoryMapCamera);
    }
}

void initStoryMapCamera(StoryMapCameraState *camera) {
    GameState *state;
    s32 temp;
    s32 x, z;
    u8 mode;

    state = getCurrentAllocation();
    state->animState = 4;

    if (storyMapLocationIndex != 0) {
        state->discoveredLocationId = storyMapLocationIndex - 1;
        setCallbackWithContinue(&initStoryMapCameraAtLocation);
        return;
    }

    memcpy(&camera->orientMatrix, &identityMatrix, sizeof(Transform3D));
    memcpy(&camera->viewMatrix, &identityMatrix, sizeof(Transform3D));

    mode = getStoryMapCameraMode();

    if (mode == 3) {
        camera->cameraX = 0xFFE60000;
        camera->cameraZ = 0xFF9C0000;
        camera->orbitAngle = atan2Fixed(camera->cameraX, camera->cameraZ);

        x = camera->cameraX;
        z = camera->cameraZ;
        camera->orbitRadius = isqrt64((s64)x * x + (s64)z * z);

        camera->targetAngle = 0;
        camera->viewAngle = 0;
        state->animState = 0;
    } else {
        mode = getStoryMapCameraMode();
        if (mode == 1) {
            camera->orbitAngle = 0x14B0;
            camera->orbitRadius = 0x650000;
            camera->targetAngle = 0x1800;
            camera->viewAngle = 0x1800;
        } else {
            camera->orbitAngle = 0x0B50;
            camera->orbitRadius = 0x650000;
            camera->targetAngle = 0x0800;
            camera->viewAngle = 0x0800;
        }

        temp = approximateSin(camera->orbitAngle);
        temp = temp * -(camera->orbitRadius >> 8);
        if (temp < 0) {
            temp += 0x1FFF;
        }
        camera->cameraX = (temp >> 13) << 8;

        temp = approximateCos(camera->orbitAngle);
        temp = temp * -(camera->orbitRadius >> 8);
        if (temp < 0) {
            temp += 0x1FFF;
        }
        camera->cameraZ = (temp >> 13) << 8;
    }

    state->unk3EC = camera->cameraX;
    state->unk3F0 = camera->cameraZ;
    state->unk3F4 = camera->orbitAngle;
    state->unk3F8 = camera->orbitRadius;
    state->unk3FC = camera->viewAngle & 0x1FFF;

    camera->unk50 = 0;
    camera->speedV = 8;
    camera->speedH = 8;

    createYRotationMatrix((Transform3D *)&camera->viewMatrix, camera->viewAngle);
    createYRotationMatrix((Transform3D *)&camera->orientMatrix, camera->orbitAngle);

    mode = getStoryMapCameraMode();
    if (mode == 3) {
        setCallback(&updateStoryMapCameraFreeRoam);
    } else {
        setCallback(&updateStoryMapCameraOrbit);
    }
}

void updateStoryMapCameraFreeRoam(StoryMapCameraState *camera) {
    Vec3i pos;
    Vec3i savedPos;
    Transform3D combinedMatrix;
    GameState *state;
    s32 collisionResult;
    s32 i;
    s32 distance;
    s32 speedScale;
    s32 moveX, moveZ;
    s32 absMoveX;
    s32 absMoveZ;
    s16 turnDir;
    s16 absDiff;
    s16 hDir, vDir;
    s8 stickX, stickY;
    u8 maxSpeed;
    s32 walkSpeed;
    u8 selectedLocation;
    s32 temp_v1;
    s32 temp_a2;

    state = (GameState *)getCurrentAllocation();
    stickX = gAnalogStickX[0];
    stickY = gAnalogStickY[0];

    if ((ABS(stickX)) < 0x12 && (ABS(stickY)) < 0x12) {
        maxSpeed = 0xF;
        if (maxSpeed >= camera->speedH && maxSpeed >= camera->speedV) {
            walkSpeed = 2;
        } else {
            walkSpeed = 4;
        }
    } else {
        maxSpeed = 0x1F;
        walkSpeed = 4;
    }

    memcpy(&savedPos, &camera->cameraX, sizeof(Vec3i));
    memcpy(&pos, &savedPos, sizeof(Vec3i));
    state->unk426 = 1;

    if (stickX != 0 || stickY != 0) {
        moveX = stickX;
        moveZ = stickY;
        state->animState = walkSpeed;
        camera->targetAngle = (-atan2Fixed(moveX, moveZ)) & 0x1FFF;
        moveX = -moveX;

        if (moveX == 0) {
            hDir = 0;
        } else {
            hDir = (moveX > 0) ? 1 : -1;
        }

        if (moveZ == 0) {
            vDir = 0;
        } else {
            vDir = (moveZ > 0 ? 1 : -1);
        }

        moveX = (camera->speedH * hDir) / 8 + hDir;
        moveZ = (camera->speedV * vDir) / 8 + vDir;

        if (camera->speedH < maxSpeed) {
            camera->speedH += __abs(hDir);
        }
        if (camera->speedV < maxSpeed) {
            camera->speedV += __abs(vDir);
        }

        if (camera->speedH >= maxSpeed) {
            camera->speedH -= ABS(hDir);
        }
        if (camera->speedV >= maxSpeed) {
            camera->speedV -= ABS(vDir);
        }

        absMoveX = __abs(moveX);
        absMoveZ = __abs(moveZ);
        if (absMoveZ < absMoveX) {
            state->unk426 = absMoveX;
        } else {
            state->unk426 = absMoveZ;
        }

        camera->orbitRadius = isqrt64((s64)pos.x * pos.x + (s64)pos.z * pos.z);

        temp_v1 =
            ((moveX * (savedPos.z >> 8)) / (camera->orbitRadius >> 8) +
             (moveZ * (savedPos.x >> 8)) / (camera->orbitRadius >> 8));
        pos.x += (temp_v1 << 16);
        temp_v1 =
            (((-moveX * (savedPos.x >> 8)) / (camera->orbitRadius >> 8)) +
             (moveZ * (savedPos.z >> 8)) / (camera->orbitRadius >> 8));
        pos.z += (temp_v1 << 16);

        collisionResult = checkTownLamppostCollision(pos.x, pos.z, state->unk3FE);
        if (collisionResult != 0) {
            resolveTownLamppostCollision(&pos, state->unk3FE, collisionResult);
            camera->orbitRadius = distance_2d(pos.x, pos.z);
        }
    } else {
        camera->speedH = 8;
        camera->speedV = 8;
        state->animState = 0;
    }

    if ((isqrt64((s64)pos.x * pos.x + (s64)pos.z * pos.z) - 0x240000) > 0x640000U) {
        if (stickX) {
            memcpy(&savedPos, &camera->cameraX, sizeof(Vec3i));
            memcpy(&pos, &savedPos, sizeof(Vec3i));

            camera->orbitRadius = isqrt64((s64)pos.x * pos.x + (s64)pos.z * pos.z);

            temp_v1 = ((moveX * (savedPos.z >> 8)) / (camera->orbitRadius >> 8));
            pos.x += (temp_v1 << 16);
            temp_v1 = ((-moveX * (savedPos.x >> 8)) / (camera->orbitRadius >> 8));
            pos.z += (temp_v1 << 16);
        } else {
            memcpy(&pos, &camera->cameraX, sizeof(Vec3i));
        }

        camera->orbitAngle = atan2Fixed(pos.x, pos.z);
        if ((camera->orbitRadius - 0x240000) > 0x640000U) {
            if (camera->orbitRadius > 0x880000) {
                speedScale = -0x860000;
            } else {
                speedScale = -0x260000;
            }

            pos.x = (approximateSin(camera->orbitAngle) * (speedScale >> 8) / 8192) << 8;
            pos.z = (approximateCos(camera->orbitAngle) * (speedScale >> 8) / 8192) << 8;
        }
    }

    for (i = 0; i < state->unk41C; i++) {
        collisionResult = checkTownNPCCollision(pos.x, pos.z, i);
        if (collisionResult) {
            resolveTownNPCCollision((TownController *)camera, &pos, collisionResult);
            collisionResult = checkTownLamppostCollision(pos.x, pos.z, state->unk3FE);
            if (collisionResult) {
                resolveTownLamppostCollision(&pos, state->unk3FE, collisionResult);
                camera->orbitRadius = distance_2d(pos.x, pos.z);
            }
        }
    }

    memcpy(&camera->cameraX, &pos, sizeof(Vec3i));
    camera->orbitAngle = atan2Fixed(pos.x, pos.z);
    camera->orbitRadius = isqrt64((s64)pos.x * pos.x + (s64)pos.z * pos.z);

    if (camera->targetAngle != camera->viewAngle) {
        absDiff = __abs(camera->targetAngle - camera->viewAngle);
        if (absDiff > 0x1000) {
            if (camera->targetAngle > camera->viewAngle) {
                camera->viewAngle += 0x2000;
            } else {
                camera->targetAngle += 0x2000;
            }
        }

        turnDir = (camera->targetAngle > camera->viewAngle) ? 1 : -1;

        absDiff = __abs(camera->targetAngle - camera->viewAngle);
        if (absDiff > 0xAAA) {
            camera->viewAngle += 0x1000;
            state->animState = 1;
            state->unk404 = 1;
        } else if (absDiff > 0x1A0) {
            camera->viewAngle += turnDir * 0x1A0;
        } else {
            camera->viewAngle += absDiff * turnDir;
            camera->viewAngle &= 0x1FFF;
            camera->targetAngle = camera->viewAngle;
        }

        createYRotationMatrix((Transform3D *)camera, (u16)camera->viewAngle & 0x1FFF);
    }

    createYRotationMatrix((Transform3D *)&camera->orientMatrix, (u16)camera->orbitAngle);
    func_8006B084_6BC84((Transform3D *)camera, (Transform3D *)&camera->orientMatrix, &combinedMatrix);

    state->unk3EC = camera->cameraX;
    state->unk3F0 = camera->cameraZ;
    memcpy(&state->unk3B0, &combinedMatrix, sizeof(Transform3D));
    memcpy(&state->unk3D0, &combinedMatrix.translation, sizeof(Vec3i));
    state->unk3F8 = camera->orbitRadius;
    state->unk3F4 = camera->orbitAngle;
    state->unk3FC = camera->viewAngle & 0x1FFF;

    {
        s16 orbitAngle;
        s32 lowThresh, highThresh;

        orbitAngle = state->unk3F4;
        if (camera->orbitRadius > 0x580000) {
            lowThresh = 0xB00;
            highThresh = 0x1500;
        } else {
            lowThresh = 0xD50;
            highThresh = 0x12B0;
        }

        if ((lowThresh < orbitAngle && orbitAngle < 0x1000) || (!(orbitAngle < 0x1001) & (orbitAngle < highThresh))) {
            setViewportFadeValue(NULL, 0xFF, 0x10);
            if (camera->cameraX > 0) {
                setStoryMapCameraMode(1);
            } else {
                setStoryMapCameraMode(2);
            }
            setCallback(&finalizeStoryMapExit);
        }
    }

    if (state->locationDiscovered != 0 && (gControllerInputs[0] & A_BUTTON) && (state->discoveredLocationId < 0xA)) {
        if (state->discoveredLocationId < 6) {
            state->animState = 4;
            setCallbackWithContinue(&startStoryMapCameraTravel);
        } else {
            setViewportFadeValue(NULL, 0xFF, 0x10);
            setCallback(&finalizeStoryMapCameraTravel);
        }
    } else {
        state->locationDiscovered = 0;
        if (state->unk404 == 0) {
            selectedLocation = checkStoryMapLocationSelection(camera);
            if (selectedLocation) {
                state->unk42B = selectedLocation - 1;
                setCallback(&updateStoryMapDialogueTurn);
            }
        }
    }
}

void startStoryMapCameraTravel(StoryMapCameraState *camera) {
    s32 sp10[10];
    GameState *state;
    s32 targetX;
    s32 targetZ;
    s32 result;
    s32 masked;

    state = getCurrentAllocation();

    targetX = storyMapLocationCoords[state->discoveredLocationId].x;
    targetZ = storyMapLocationCoords[state->discoveredLocationId].z;
    targetX <<= 16;
    targetZ <<= 16;

    result = computeAngleToPosition(camera->cameraX, camera->cameraZ, targetX, targetZ);
    camera->targetAngle = (s16)result;
    camera->viewAngle = (s16)result;

    if ((s16)result < 0x1000) {
        camera->viewAngle = (s16)result + 0x1000;
    } else {
        camera->viewAngle = result - 0x1000;
    }

    targetX -= camera->cameraX;
    targetZ -= camera->cameraZ;

    masked = camera->viewAngle & 0x1FFF;
    camera->viewAngle = masked;
    camera->targetAngle = masked;

    camera->travelDistance = isqrt64((s64)targetX * targetX + (s64)targetZ * targetZ);

    setViewportFadeValue(NULL, 0xFF, 0x10);

    createYRotationMatrix((Transform3D *)camera, (camera->viewAngle - camera->orbitAngle) & 0xFFFF);

    func_8006B084_6BC84((Transform3D *)camera, (Transform3D *)&camera->orientMatrix, (Transform3D *)sp10);

    memcpy(&state->unk3B0, sp10, sizeof(Transform3D));

    setCallback(updateStoryMapCameraTravel);
}

void updateStoryMapCameraTravel(StoryMapCameraState *camera) {
    GameState *state;
    Transform3D localMatrix;
    s32 velocity;
    s32 temp;
    s16 angle;

    state = getCurrentAllocation();

    velocity = camera->travelDistance;
    if (velocity < 0) {
        velocity += 0x1F;
    }
    velocity >>= 9;

    temp = approximateSin(camera->viewAngle) * velocity;
    if (temp < 0) {
        temp += 0x1FFF;
    }
    camera->cameraX = camera->cameraX + ((temp >> 13) << 4);

    temp = approximateCos(camera->viewAngle) * velocity;
    if (temp < 0) {
        temp += 0x1FFF;
    }
    camera->cameraZ = camera->cameraZ + ((temp >> 13) << 4);

    angle = atan2Fixed(camera->cameraX, camera->cameraZ);

    createYRotationMatrix((Transform3D *)&camera->orientMatrix, (u16)angle);
    createYRotationMatrix((Transform3D *)camera, (u16)(camera->viewAngle - angle));

    func_8006B084_6BC84((Transform3D *)camera, (Transform3D *)&camera->orientMatrix, &localMatrix);

    memcpy(&state->unk3B0, &localMatrix, sizeof(Transform3D));

    state->unk3EC = camera->cameraX;
    state->unk3F0 = camera->cameraZ;

    if (getViewportFadeMode(0) == 0) {
        storyMapLocationIndex = state->discoveredLocationId;
        state->unk427 = state->discoveredLocationId + 1;
        setCallback(&storyMapCameraTravelComplete);
    }
}

void finalizeStoryMapCameraTravel(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (getViewportFadeMode(0) == 0) {
        storyMapLocationIndex = state->discoveredLocationId;
        state->unk427 = state->discoveredLocationId + 1;
        setCallback(&storyMapCameraTravelComplete);
    }
}

void storyMapCameraTravelComplete(void) {
}

void initStoryMapCameraAtLocation(StoryMapCameraState *camera) {
    s32 pad[2];
    GameState *state;
    s32 x, z;

    state = getCurrentAllocation();

    memcpy(&camera->orientMatrix, &identityMatrix, sizeof(Transform3D));
    memcpy(camera, &identityMatrix, sizeof(Transform3D));

    camera->cameraX = storyMapLocationCoords[(u8)state->discoveredLocationId].x;
    camera->cameraZ = storyMapLocationCoords[(u8)state->discoveredLocationId].z;

    camera->cameraX <<= 16;
    camera->cameraZ <<= 16;

    camera->orbitAngle = atan2Fixed(camera->cameraX, camera->cameraZ);

    x = camera->cameraX;
    z = camera->cameraZ;
    camera->orbitRadius = isqrt64((s64)x * x + (s64)z * z);

    camera->targetAngle = 0;
    camera->viewAngle = 0;

    state->unk3EC = camera->cameraX;
    state->unk3F0 = camera->cameraZ;
    state->unk3F4 = camera->orbitAngle;
    state->unk3F8 = camera->orbitRadius;
    state->unk3FC = camera->viewAngle & 0x1FFF;

    camera->unk50 = 0;
    camera->speedV = 8;
    camera->speedH = 8;

    createYRotationMatrix((Transform3D *)&camera->orientMatrix, camera->orbitAngle);
    createYRotationMatrix((Transform3D *)camera, camera->viewAngle);

    setCallback(approachStoryMapOrigin);
}

void approachStoryMapOrigin(StoryMapCameraState *camera) {
    GameState *state;
    Transform3D localMatrix;
    s32 sinVal;
    s32 cosVal;
    s32 x, z;
    s64 sumSquares;
    s32 distance;

    state = getCurrentAllocation();

    sinVal = approximateSin(camera->orbitAngle);
    camera->cameraX = camera->cameraX + (sinVal << 4);

    cosVal = approximateCos(camera->orbitAngle);
    camera->cameraZ = camera->cameraZ + (cosVal << 4);

    func_8006B084_6BC84((Transform3D *)camera, (Transform3D *)&camera->orientMatrix, &localMatrix);

    state->unk3EC = camera->cameraX;
    state->unk3F0 = camera->cameraZ;

    memcpy(&state->unk3B0, &localMatrix, sizeof(Transform3D));

    state->unk3F4 = camera->orbitAngle;
    state->unk3F8 = camera->orbitRadius;
    state->unk3FC = camera->viewAngle & 0x1FFF;

    x = camera->cameraX;
    z = camera->cameraZ;
    sumSquares = (s64)x * x + (s64)z * z;
    distance = isqrt64(sumSquares);

    if (distance <= 0x880000) {
        setCallback(&updateStoryMapCameraFreeRoam);
    }
}

void updateStoryMapCameraOrbit(StoryMapCameraState *camera) {
    s32 combinedMatrix[8];
    s32 cameraPosition[3];
    s32 pad3C[5];
    s32 sp58;
    u32 sp5C;
    GameState *state;
    Transform3D *orientMatrixPtr;
    s32 zDelta;
    s32 x, z;
    u8 mode;
    s8 orbitSpeed;

    state = getCurrentAllocation();
    memcpy(cameraPosition, &camera->cameraX, sizeof(Vec3i));

    mode = getStoryMapCameraMode();
    orbitSpeed = -3;
    if (mode == 1) {
        orbitSpeed = 3;
    }

    camera->cameraX = camera->cameraX + (((orbitSpeed * (cameraPosition[2] >> 8)) / (camera->orbitRadius >> 8)) << 16);

    zDelta = (-orbitSpeed * (cameraPosition[0] >> 8)) / (camera->orbitRadius >> 8);
    camera->cameraZ = camera->cameraZ + (zDelta << 16);

    camera->orbitAngle = atan2Fixed(camera->cameraX, camera->cameraZ);

    x = camera->cameraX;
    z = camera->cameraZ;
    camera->orbitRadius = isqrt64((s64)x * x + (s64)z * z);

    orientMatrixPtr = (Transform3D *)&camera->orientMatrix;
    createYRotationMatrix(orientMatrixPtr, camera->orbitAngle);
    func_8006B084_6BC84((Transform3D *)camera, orientMatrixPtr, (Transform3D *)combinedMatrix);

    state->unk3EC = camera->cameraX;
    state->unk3F0 = camera->cameraZ;
    memcpy(&state->unk3B0, combinedMatrix, sizeof(Transform3D));
    state->unk3F4 = camera->orbitAngle;
    state->unk3F8 = camera->orbitRadius;
    state->unk3FC = camera->viewAngle & 0x1FFF;

    mode = getStoryMapCameraMode();
    if (mode == 2) {
        if (camera->orbitAngle < 0x8D1) {
            setCallback(updateStoryMapCameraFreeRoam);
        }
    } else if (camera->orbitAngle >= 0x1730) {
        setCallback(updateStoryMapCameraFreeRoam);
    }
}

void finalizeStoryMapExit(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (getViewportFadeMode(0) == 0) {
        state->unk427 = 0xFF;
        setCallbackWithContinue(&storyMapExitComplete);
    }
}

void storyMapExitComplete(void) {
}

s32 __abs(s32 n) {
    if (n < 0) {
        n = -n;
    }
    return n;
}
