#pragma once

#include "common.h"

extern u8 gRandTable[256];
extern s32 gRandIndexA;
extern s32 gRandIndexB;

void initRand(void);
u8 randA(void);
u8 randB(void);
u8 getRand(s32 index);
