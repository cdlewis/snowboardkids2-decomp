#pragma once

#include "include_asm.h"
#include "ultra64.h"

void *memcpy(void *, const void *, size_t);

#define ABS(d) ((d) > 0) ? (d) : -(d)

#define STICK_RIGHT 0x40000
#define STICK_LEFT 0x80000
#define STICK_UP 0x10000
#define STICK_DOWN 0x20000