#pragma once

#include "common.h"
#include "geometry.h"

typedef struct {
    /* 0x00 */ void *model;
    /* 0x04 */ Transform3D matrix;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ u16 targetRotation;
    /* 0x32 */ u8 pad32[0x96];
    /* 0xC8 */ void *returnCallback;
    /* 0xCC */ u8 padCC[0x7];
    /* 0xD3 */ u8 dialogueState;
} StoryMapDialogueState;

void func_8002BCF0_2C8F0(void *arg0);

void updateStoryMapNpcDialogue(StoryMapDialogueState *arg0);
