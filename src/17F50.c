#include "15690.h"
#include "3E160.h"
#include "6E840.h"
#include "common.h"
#include "gamestate.h"
#include "geometry.h"
#include "task_scheduler.h"

extern u8 storyMapLocationIndex;
extern s16 identityMatrix[9];

typedef struct {
    s16 x;
    s16 z;
} CoordPair;

extern CoordPair D_8008D6EC_8E2EC[];

typedef struct {
    s16 viewMatrix[9];
    u8 pad12[0x2];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s16 orientMatrix[9];
    u8 pad32[0x2];
    s32 cameraX;
    s32 unk38;
    s32 cameraZ;
    u8 pad40[0x4];
    u16 viewAngle;
    s16 targetAngle;
    s16 orbitAngle;
    u8 pad4A[0x2];
    s32 orbitRadius;
    s16 unk50;
    s32 travelDistance;
    u8 pad58[0x2];
    u8 unk5A;
    u8 unk5B;
} StoryMapCameraState;

void initStoryMapCamera(StoryMapCameraState *arg0);
void func_800175E0_181E0(void);
void func_80018580_19180(StoryMapCameraState *arg0);
void func_800182FC_18EFC(StoryMapCameraState *arg0);

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
    state->unk400 = 4;

    if (storyMapLocationIndex != 0) {
        state->unk425 = storyMapLocationIndex - 1;
        setCallbackWithContinue(&func_800182FC_18EFC);
        return;
    }

    memcpy(&camera->orientMatrix, identityMatrix, 0x20);
    memcpy(&camera->viewMatrix, identityMatrix, 0x20);

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
        state->unk400 = 0;
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
    camera->unk5B = 8;
    camera->unk5A = 8;

    createYRotationMatrix((Transform3D *)&camera->viewMatrix, camera->viewAngle);
    createYRotationMatrix((Transform3D *)&camera->orientMatrix, camera->orbitAngle);

    mode = getStoryMapCameraMode();
    if (mode == 3) {
        setCallback(&func_800175E0_181E0);
    } else {
        setCallback(&func_80018580_19180);
    }
}

INCLUDE_ASM("asm/nonmatchings/17F50", func_800175E0_181E0);

typedef struct {
    u8 pad[0x3B0];
    u8 unk3B0[0x20];
    u8 pad3D0[0x55];
    u8 unk425;
} TempState17F50;

void updateStoryMapCameraTravel(StoryMapCameraState *camera);

void startStoryMapCameraTravel(StoryMapCameraState *camera) {
    s32 sp10[10];
    TempState17F50 *state;
    s32 targetX;
    s32 targetZ;
    s16 result, masked;

    state = getCurrentAllocation();

    targetX = D_8008D6EC_8E2EC[state->unk425].x;
    targetZ = D_8008D6EC_8E2EC[state->unk425].z;
    targetX <<= 16;
    targetZ <<= 16;

    result = func_8006D21C_6DE1C(camera->cameraX, camera->cameraZ, targetX, targetZ);
    camera->targetAngle = result;
    camera->viewAngle = result;

    if (result < 0x1000) {
        camera->viewAngle = result + 0x1000;
    } else {
        camera->viewAngle = result - 0x1000;
    }

    targetX -= camera->cameraX;
    targetZ -= camera->cameraZ;

    masked = camera->viewAngle & 0x1FFF;
    camera->viewAngle = masked;
    camera->targetAngle = masked;

    camera->travelDistance = isqrt64((s64)targetX * targetX + (s64)targetZ * targetZ);

    func_8006FDA0_709A0(NULL, 0xFF, 0x10);

    createYRotationMatrix((Transform3D *)camera, (camera->viewAngle - camera->orbitAngle) & 0xFFFF);

    func_8006B084_6BC84(camera, &camera->orientMatrix, sp10);

    memcpy(state->unk3B0, sp10, 0x20);

    setCallback(updateStoryMapCameraTravel);
}

void func_800182F4_18EF4(void);

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

    func_8006B084_6BC84(camera, &camera->orientMatrix, &localMatrix);

    memcpy(&state->unk3B0, &localMatrix, 0x20);

    state->unk3EC = camera->cameraX;
    state->unk3F0 = camera->cameraZ;

    if (func_8006FE10_70A10(0) == 0) {
        storyMapLocationIndex = state->unk425;
        state->unk427 = state->unk425 + 1;
        setCallback(&func_800182F4_18EF4);
    }
}

void finalizeStoryMapCameraTravel(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (func_8006FE10_70A10(0) == 0) {
        storyMapLocationIndex = state->unk425;
        state->unk427 = state->unk425 + 1;
        setCallback(&func_800182F4_18EF4);
    }
}

void func_800182F4_18EF4(void) {
}

void func_80018474_19074(StoryMapCameraState *arg0);

void func_800182FC_18EFC(StoryMapCameraState *camera) {
    s32 pad[2];
    GameState *state;
    s32 x, z;

    state = getCurrentAllocation();

    memcpy(&camera->orientMatrix, identityMatrix, 0x20);
    memcpy(camera, identityMatrix, 0x20);

    camera->cameraX = D_8008D6EC_8E2EC[(u8)state->unk425].x;
    camera->cameraZ = D_8008D6EC_8E2EC[(u8)state->unk425].z;

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
    camera->unk5B = 8;
    camera->unk5A = 8;

    createYRotationMatrix((Transform3D *)&camera->orientMatrix, camera->orbitAngle);
    createYRotationMatrix((Transform3D *)camera, camera->viewAngle);

    setCallback(func_80018474_19074);
}

void func_80018474_19074(StoryMapCameraState *camera) {
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

    func_8006B084_6BC84(camera, &camera->orientMatrix, &localMatrix);

    state->unk3EC = camera->cameraX;
    state->unk3F0 = camera->cameraZ;

    memcpy(&state->unk3B0, &localMatrix, 0x20);

    state->unk3F4 = camera->orbitAngle;
    state->unk3F8 = camera->orbitRadius;
    state->unk3FC = camera->viewAngle & 0x1FFF;

    x = camera->cameraX;
    z = camera->cameraZ;
    sumSquares = (s64)x * x + (s64)z * z;
    distance = isqrt64(sumSquares);

    if (distance <= 0x880000) {
        setCallback(&func_800175E0_181E0);
    }
}

void func_80018580_19180(StoryMapCameraState *camera) {
    s32 sp10[8];
    s32 sp30[3];
    s32 pad3C[5];
    s32 sp58;
    u32 sp5C;
    GameState *state;
    Transform3D *matrixPtr;
    s32 divResult;
    s32 x, z;
    u8 mode;
    s8 orbitSpeed;

    state = getCurrentAllocation();
    memcpy(sp30, &camera->cameraX, 0xC);

    mode = getStoryMapCameraMode();
    orbitSpeed = -3;
    if (mode == 1) {
        orbitSpeed = 3;
    }

    camera->cameraX = camera->cameraX + (((orbitSpeed * (sp30[2] >> 8)) / (camera->orbitRadius >> 8)) << 16);

    divResult = (-orbitSpeed * (sp30[0] >> 8)) / (camera->orbitRadius >> 8);
    camera->cameraZ = camera->cameraZ + (divResult << 16);

    camera->orbitAngle = atan2Fixed(camera->cameraX, camera->cameraZ);

    x = camera->cameraX;
    z = camera->cameraZ;
    camera->orbitRadius = isqrt64((s64)x * x + (s64)z * z);

    matrixPtr = (Transform3D *)&camera->orientMatrix;
    createYRotationMatrix(matrixPtr, camera->orbitAngle);
    func_8006B084_6BC84(camera, matrixPtr, sp10);

    state->unk3EC = camera->cameraX;
    state->unk3F0 = camera->cameraZ;
    memcpy(&state->unk3B0, sp10, 0x20);
    state->unk3F4 = camera->orbitAngle;
    state->unk3F8 = camera->orbitRadius;
    state->unk3FC = camera->viewAngle & 0x1FFF;

    mode = getStoryMapCameraMode();
    if (mode == 2) {
        if (camera->orbitAngle < 0x8D1) {
            setCallback(func_800175E0_181E0);
        }
    } else if (camera->orbitAngle >= 0x1730) {
        setCallback(func_800175E0_181E0);
    }
}

void func_800187DC_193DC(void);

void func_80018798_19398(void) {
    GameState *state = (GameState *)getCurrentAllocation();

    if (func_8006FE10_70A10(0) == 0) {
        state->unk427 = 0xFF;
        setCallbackWithContinue(&func_800187DC_193DC);
    }
}

void func_800187DC_193DC(void) {
}

s32 abs(s32 n) {
    if (n < 0) {
        n = -n;
    }
    return n;
}
