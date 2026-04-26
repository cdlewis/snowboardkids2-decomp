#pragma once

#include "common.h"
#include "graphics/displaylist.h"
#include "math/geometry.h"

// Start gate display object structure (3 parts: main gate, left door, right door)
// Each part is a DisplayListObject with transform, display lists, and segment pointers
typedef struct {
    /* Main gate structure (offset 0x00) */
    Transform3D rotationMatrix;         // 0x00-0x1F, with position at 0x14-0x1F
    DisplayLists *mainGateDisplayLists; // 0x20
    void *mainGateSegment1;             // 0x24 - uncompressed asset
    void *mainGateSegment2;             // 0x28 - compressed asset
    s32 mainGateSegment3;               // 0x2C
    u8 pad[0xC];
    /* Left door structure (offset 0x3C) */
    Transform3D leftDoorTransform;      // 0x3C
    DisplayLists *leftDoorDisplayLists; // 0x5C
    void *leftDoorSegment1;             // 0x60
    void *leftDoorSegment2;             // 0x64
    s32 leftDoorSegment3;               // 0x68
    u8 pad2[0xC];
    /* Right door structure (offset 0x78) */
    Transform3D rightDoorTransform;      // 0x78
    DisplayLists *rightDoorDisplayLists; // 0x98
    void *rightDoorSegment1;             // 0x9C
    void *rightDoorSegment2;             // 0xA0
    s32 rightDoorSegment3;               // 0xA4
    u8 pad3[0xC];
    s16 gateRotation;   // 0xB4
    s16 animationState; // 0xB6
    s16 pauseTimer;     // 0xB8
} StartGate;

void spawnPushStartPrompt(s32, s16, u8, u8, s16, s16);
void initStartGate(StartGate *gate);
